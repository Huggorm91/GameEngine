#include "MuninGraph.pch.h"
#include "Blueprint.h"
#include "ScriptGraphNode.h"
#include "Nodes/SGNode_Variable.h"
#include "Json/nlohmannJson/json.hpp"

Blueprint::Blueprint(const std::string& aJson)
{
	// From: ScriptGraphSchema::DeserializeScriptGraph(std::shared_ptr<ScriptGraph>& outGraph, const std::string& inData)
	// ScriptGraphSchema.cpp : 213
	outGraph = CreateScriptGraphInternal(true);
	std::unique_ptr<ScriptGraphSchema> outGraphSchema = outGraph->GetGraphSchema();

	using namespace nlohmann;
	json graphJson = json::parse(inData);

	outGraph->myEdges.clear();
	outGraph->myPins.clear();
	outGraph->myEntryPoints.clear();
	outGraph->myNodeUIDToEntryHandle.clear();
	outGraph->myNodes.clear();
	outGraph->myVariables.clear();

	// Load all the variables
	for (const auto& it : graphJson["variables"])
	{
		const std::string varName = it.at("name");
		const std::string varType = it.at("type");
		const std::vector<uint8_t> varData = it.at("value");

		const std::type_index varTypeId = ScriptGraphDataTypeRegistry::GetTypeFromString(varType);

		std::shared_ptr<ScriptGraphVariable> newVariable = std::make_shared<ScriptGraphVariable>();
		newVariable->Data = ScriptGraphDataTypeRegistry::GetDataObjectOfType(varTypeId);
		newVariable->Name = varName;
		newVariable->DefaultData = ScriptGraphDataTypeRegistry::GetDataObjectOfType(varTypeId);

		ScriptGraphDataTypeRegistry::Deserialize(varTypeId, newVariable->DefaultData.Ptr, varData);
		newVariable->ResetVariable();

		outGraph->myVariables.insert({ varName, newVariable });
	}

	std::unordered_map<size_t, std::shared_ptr<ScriptGraphNode>> fileUIDToNode;

	// Create all the nodes.
	for (const auto& it : graphJson["nodes"])
	{
		const std::string nodeType = it.at("type");
		const auto typeData = MyNodeTypesMap().find(nodeType);
		std::shared_ptr<ScriptGraphNode> newNode = typeData->second.New();

		if (std::shared_ptr<VariableNodeBase> varNode = std::dynamic_pointer_cast<VariableNodeBase>(newNode))
		{
			const std::string nodeVarName = it.at("variable");
			std::shared_ptr<ScriptGraphVariable> nodeVar = outGraph->myVariables.find(nodeVarName)->second;
			varNode->SetNodeVariable(nodeVar);

			for (auto& [pinUID, pin] : newNode->myPins)
			{
				if (pin.GetType() == ScriptGraphPinType::Variable)
				{
					pin.InitVariableBlock(nodeVar->Data.TypeData->GetType());
				}
			}
		}

		for (const auto& pinIt : it.at("pins"))
		{
			const std::string pinName = pinIt.at("name");
			const std::vector<uint8_t> varData = pinIt.at("value");
			if (auto pinLbl = newNode->myPinLabelToUID.find(pinName); pinLbl != newNode->myPinLabelToUID.end())
			{
				if (auto pinObj = newNode->myPins.find(pinLbl->second); pinObj != newNode->myPins.end())
				{
					ScriptGraphDataTypeRegistry::Deserialize(pinObj->second.GetDataType(), pinObj->second.myData.Ptr, varData);
					//memcpy_s(pinObj->second.myData.Ptr, pinObj->second.myData.TypeData->GetTypeSize(), varData.data(), varData.size());
				}
			}
		}

		const auto uuidAwareBase = AsGUIDAwareSharedPtr(newNode);

		const float x = it.at("x");
		const float y = it.at("y");
		const float z = it.at("z");
		newNode->myPosition[0] = x;
		newNode->myPosition[1] = y;
		newNode->myPosition[2] = z;

		// Here we should read all the pins and restore their values if we can.
		// This gets fun if we have a lot of custom types to account for :).
		// TODO: read constant pin data. Needs to be saved too. Won't work for pointers of course
		// since they're just an integer.

		if (!outGraphSchema->RegisterNode(newNode))
		{
			assert(false && "The graph has invalid numbers of a node!");
		}

		if (newNode->IsEntryNode())
		{
			outGraphSchema->RegisterEntryPointNode(newNode, it.at("entryHandle"));
		}

		fileUIDToNode.insert({ it.at("UID"), newNode });
	}

	// And all the edges
	for (const auto& it : graphJson["edges"])
	{
		const size_t sourceUID = it.at("sourceUID");
		const std::string sourcePinLabel = it.at("sourcePin");
		const size_t targetUID = it.at("targetUID");
		const std::string targetPinLabel = it.at("targetPin");

		// Find the source node owning the source pin.
		if (auto srcIt = fileUIDToNode.find(sourceUID); srcIt != fileUIDToNode.end())
		{
			// And find the target node owning the target pin.
			if (auto tgtIt = fileUIDToNode.find(targetUID); srcIt != fileUIDToNode.end())
			{
				const std::shared_ptr<ScriptGraphNode> sourceNode = srcIt->second;
				const std::shared_ptr<ScriptGraphNode> targetNode = tgtIt->second;

				// Get the actual pins from the nodes.
				const ScriptGraphPin& srcPin = sourceNode->GetPin(sourcePinLabel);
				const ScriptGraphPin& tgtPin = targetNode->GetPin(targetPinLabel);

				// And create a link.
				outGraphSchema->CreateEdge(srcPin.GetUID(), tgtPin.GetUID());
			}
		}
	}
}


void Blueprint::Tick(float aDeltaTime)
{
	if (const auto it = myEntryPoints.find("Tick"); it != myEntryPoints.end())
	{
		ScriptGraphNodePayload tickPayload;
		tickPayload.SetVariable("Delta Time", aDeltaTime);
		RunWithPayload("Tick", tickPayload);
	}
}

void Blueprint::BindErrorHandler(ScriptGraphErrorHandlerSignature&& aErrorHandler)
{
	myErrorDelegate = aErrorHandler;
}

void Blueprint::UnbindErrorHandler()
{
	myErrorDelegate = nullptr;
}

bool Blueprint::Run(const std::string& anEntryPointHandle)
{
	if (const auto it = myEntryPoints.find(anEntryPointHandle); it != myEntryPoints.end())
	{
		myLastExecutedPath.clear();
		return it->second->Exec(0) != 0;
	}

	return false;
}

bool Blueprint::RunWithPayload(const std::string& anEntryPointHandle, const ScriptGraphNodePayload& aPayload)
{
	if (const auto it = myEntryPoints.find(anEntryPointHandle); it != myEntryPoints.end())
	{
		it->second->DeliverPayload(aPayload);
		return Run(anEntryPointHandle);
	}

	return false;
}

std::string Blueprint::GetPath() const
{
	if (myPath)
	{
		return *myPath;
	}
	return std::string();
}

const ScriptGraphPin& Blueprint::GetDataSourcePin(size_t aPinUID, bool& outErrored) const
{
	typedef NodeGraph<ScriptGraphNode, ScriptGraphPin, NodeGraphEdge, ScriptGraphSchema> Super;
	const ScriptGraphPin& dataPin = Super::GetDataSourcePin(aPinUID, outErrored);
	if (dataPin.GetUID() != aPinUID && !dataPin.GetOwner()->IsExecNode())
	{
		dataPin.GetOwner()->DoOperation();
		outErrored = dataPin.GetOwner()->HasError();
	}

	return dataPin;
}

void Blueprint::ReportEdgeFlow(size_t anEdgeUID)
{
	myLastExecutedPath.push_back(anEdgeUID);
}

void Blueprint::ReportFlowError(size_t aNodeUID, const std::string& anErrorMessage) const
{
	if (myErrorDelegate)
	{
		myErrorDelegate(*dynamic_cast<const ScriptGraph*>(this), aNodeUID, anErrorMessage);
	}
}