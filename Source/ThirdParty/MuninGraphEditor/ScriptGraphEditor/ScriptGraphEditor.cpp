﻿#include "pch.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#include "ScriptGraphEditor.h"

#include <ranges>
#include <unordered_set>

#include "imgui.h"
#include "ScriptGraphEditorTypes.h"
#include "DirectX/DDSTextureLoader11.h"
#include "ScriptGraph/Nodes/VariableNodes.h"
#include "Textures/Texture_Event_Icon.h"
#include "Textures/Texture_Function_Icon.h"
#include "Textures/Texture_NodeGradient.h"
#include "Textures/Texture_NodeHeader.h"

constexpr std::string_view ScriptGraphEditor_TriggerEntryPointDialogName(ICON_FA_PLAY "  Events");
constexpr std::string_view ScriptGraphEditor_EditVariablesDialogName( ICON_FA_CODE "  Variables");

ScriptGraphEditorSettings::ScriptGraphEditorSettings(const ComPtr<ID3D11Device>& aDevice)
{
	// Load Node Textures here
	DirectX::CreateDDSTextureFromMemory(aDevice.Get(), BuiltIn_NodeHeader_ByteCode, sizeof(BuiltIn_NodeHeader_ByteCode), nullptr, NodeHeaderTexture.GetAddressOf());
	DirectX::CreateDDSTextureFromMemory(aDevice.Get(), BuiltIn_NodeGradient_ByteCode, sizeof(BuiltIn_NodeGradient_ByteCode), nullptr, NodeGradientTexture.GetAddressOf());

	// Load Icons here
	{
		NodeTypeSettings nodeSetting;
		nodeSetting.Type = EnumAsIntegral(ScriptGraphNodeType::Function);
		nodeSetting.Color = GraphColor(80, 124, 153, 255);
		DirectX::CreateDDSTextureFromMemory(aDevice.Get(), BuiltIn_NodeFunctionIcon_ByteCode, sizeof(BuiltIn_NodeFunctionIcon_ByteCode), nullptr, nodeSetting.Icon.GetAddressOf());
		NodeSettings.emplace(nodeSetting.Type, nodeSetting);
	}

	{
		NodeTypeSettings nodeSetting;
		nodeSetting.Type = EnumAsIntegral(ScriptGraphNodeType::Event);
		nodeSetting.Color = GraphColor(255, 0, 0, 128);
		DirectX::CreateDDSTextureFromMemory(aDevice.Get(), BuiltIn_NodeEventIcon_ByteCode, sizeof(BuiltIn_NodeEventIcon_ByteCode), nullptr, nodeSetting.Icon.GetAddressOf());
		NodeSettings.emplace(nodeSetting.Type, nodeSetting);
	}

	{
		NodeTypeSettings nodeSetting;
		nodeSetting.Type = EnumAsIntegral(ScriptGraphNodeType::Variable);
		nodeSetting.UseTypeColor = true;
		nodeSetting.CenterTitle = true;
		NodeSettings.emplace(nodeSetting.Type, nodeSetting);
	}
}

ScriptGraphEditor::ScriptGraphEditor(ScriptGraphEditorSettings* aSettings, ScriptGraphEditorState* aState, ScriptGraph* aGraph)
	: GraphEditorBase(aSettings, aState, aGraph)
{
	ScriptGraphEditorTypeRegistry::Get().Initialize();
	ScriptGraphEditorState* state = dynamic_cast<ScriptGraphEditorState*>(myEditorState);
	state->VariablesContextCategory.Title = "Variables";

	UpdateVariablesContextMenu();

	const auto& editorTypes = ScriptGraphEditorTypeRegistry::Get().GetEditorTypeNames();
	for(const auto& editorType : editorTypes)
	{
		const ScriptGraphEditorType* type = ScriptGraphEditorTypeRegistry::Get().GetEditorType(editorType);
		if (type->IsInternal())
			continue;

		state->VisibleEditorTypes.emplace_back(editorType);
	}

	myGraph->BindErrorHandler([this](const ScriptGraph* aGraph, size_t aNodeId, std::string_view aErrorMessage)
	{
		ReportError(std::forward<const ScriptGraph*>(aGraph), std::forward<size_t>(aNodeId), std::forward<std::string_view>(aErrorMessage));
	});
	
	for(auto& cat : myEditorState->BackgroundContextMenu.Categories | std::views::values)
	{
		for(auto& item : cat.Items)
		{
			const std::shared_ptr<ScriptGraphNode> CDO = std::static_pointer_cast<ScriptGraphNode>(item.Value->ClassDefaultObject);
			switch(CDO->GetNodeType())
			{
			case ScriptGraphNodeType::Function:
				{
					item.Title = ICON_FA_FLORIN_SIGN + ("  " + item.Title);
				}
				break;
			case ScriptGraphNodeType::Event:
				{
					item.Title = ICON_FA_RIGHT_FROM_BRACKET + ("  " + item.Title);
				}
				break;
			case ScriptGraphNodeType::Variable:
				{
					item.Title = ICON_FA_DATABASE + ("  " + item.Title);
				}
			break;
			}
		}
	}
}

void ScriptGraphEditor::RenderEditorStyle()
{
	ScriptGraphEditorState* state = dynamic_cast<ScriptGraphEditorState*>(myEditorState);
	if(state->ShowFlow && !myGraph->GetLastExecutedPath().empty())
	{
		for(const size_t& edgeId : myGraph->GetLastExecutedPath())
		{
			ImNodeEd::Flow(edgeId, ax::NodeEditor::FlowDirection::Forward);
		}

		state->ShowFlow = false;
	}
}

void ScriptGraphEditor::RenderNode(const std::shared_ptr<ScriptGraphNode>& aNode)
{
	std::vector<const ScriptGraphPin*> inputPins;
	std::vector<const ScriptGraphPin*> outputPins;

	ImGui::PushFont(mySettings->NodeTitleFont);
	const ImVec2 headerTextSize = ImGui::CalcTextSize(aNode->GetNodeTitle().data());
	ImGui::PopFont();

	ImVec2 leftMinSize = { 0, 0 }; // was 64, 0
	ImVec2 rightMinSize = { 64, 0 };

	ImGui::PushFont(mySettings->NodeBodyFont);
	for(const auto& pin : aNode->GetPins() | std::views::values)
	{
		const ImVec2 currentTextSize = ImGui::CalcTextSize(pin.GetLabel().data());

		if (pin.GetPinDirection() == PinDirection::Input)
		{
			if (currentTextSize.x > leftMinSize.x)
				leftMinSize.x = currentTextSize.x;
			if (currentTextSize.y > leftMinSize.y)
				leftMinSize.y = currentTextSize.y;

			inputPins.push_back(&pin);
		}
		else
		{
			if (currentTextSize.x > rightMinSize.x)
				rightMinSize.x = currentTextSize.x;
			if (currentTextSize.y > rightMinSize.y)
				rightMinSize.y = currentTextSize.y;

			outputPins.push_back(&pin);
		}
	}
	ImGui::PopFont();

	if (leftMinSize.x > 0)
		leftMinSize.x += 48;

	if (rightMinSize.x > 0)
		rightMinSize.x += 48;

	std::sort(inputPins.begin(), inputPins.end(), [](const ScriptGraphPin* A, const ScriptGraphPin* B) { return A->GetUID() < B->GetUID(); });
	std::sort(outputPins.begin(), outputPins.end(), [](const ScriptGraphPin* A, const ScriptGraphPin* B) { return A->GetUID() < B->GetUID(); });

	ImNodeEd::PushStyleVar(ImNodeEd::StyleVar_NodeRounding, 3.0f);
	ImNodeEd::PushStyleVar(ImNodeEd::StyleVar_NodePadding, ImVec4(8, 4, 8, 8));

	const auto uidAwareNode = AsObjectUIDSharedPtr(aNode);
	const ImNodeEd::NodeId currentImNodeId = uidAwareNode->GetUID();
	ImNodeEd::BeginNode(currentImNodeId);
	ImGui::PushID(static_cast<int>(uidAwareNode->GetUID()));

	ImVec2 cursorPos = ImGui::GetCursorPos();
	ImGui::SetCursorPos({ cursorPos.x, cursorPos.y + 2 });

	const float bodyMinWidth = leftMinSize.x + rightMinSize.x + 64;
	const ImVec2 nodePinTableSize = { headerTextSize.x > bodyMinWidth ? headerTextSize.x : bodyMinWidth, 0 };

	if(aNode->IsExecNode())
	{
		ImGui::BeginTable("nodeHeader", 2, ImGuiTableFlags_SizingFixedFit, nodePinTableSize);
		ImGui::TableNextColumn();
		RenderNodeHeader(aNode, nodePinTableSize);
		ImGui::EndTable();
	}

	const ImVec2 headerRectMin = ImGui::GetItemRectMin();
	const ImVec2 headerRectMax = ImGui::GetItemRectMax();
	const ImRect nodeHeaderRect(headerRectMin, headerRectMax);

	// Node Pins
	ImGui::BeginTable("body", 2, ImGuiTableFlags_SizingStretchProp, nodePinTableSize);
	ImGui::TableNextRow();
	ImGui::PushFont(mySettings->NodeBodyFont);
	const size_t numRows = inputPins.size() > outputPins.size() ? inputPins.size() : outputPins.size();
	for (size_t row = 0; row < numRows; row++)
	{
		ImGui::TableNextColumn();
		if (row < inputPins.size())
		{
			const ScriptGraphPin* inputPin = inputPins[row];
			RenderPin(*inputPin);
		}
		ImGui::TableNextColumn();
		if (row < outputPins.size())
		{
			const ScriptGraphPin* outputPin = outputPins[row];
			RenderPin(*outputPin);
		}
	}

	ImGui::PopFont();

	if(aNode->HasFlag(GraphNodeFlag_EditableInputPins) || aNode->HasFlag(GraphNodeFlag_EditableOutputPins))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		if(aNode->HasFlag(GraphNodeFlag_EditableInputPins))
		{
			ImGui::Button(ICON_FA_PLUS "  Add");
		}
		else
		{
			ImGui::Dummy({32, 32});
		}
		ImGui::TableNextColumn();
		if(aNode->HasFlag(GraphNodeFlag_EditableOutputPins))
		{
			ImGui::SetCursorPosX(
				ImGui::GetCursorPosX()
				+ ImGui::GetColumnWidth()
				- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x
				- 32.0f
			);

			const float y = ImGui::GetCursorPosY();
			ImGui::SetCursorPosY(y + 6);
			ImVec2 posCache = ImGui::GetCursorPos();
			const ImVec2 textSize = ImGui::CalcTextSize(ICON_FA_PLUS "  Add");

			if(ImGui::InvisibleButton("##btnAddOutputPin", textSize))
			{
				const size_t newPinId = mySchema->CreateDynamicExecPin(aNode.get(), "New Pin", PinDirection::Output);
				aNode->OnUserAddedPin(mySchema, newPinId);
			}
			const bool isButtonHovered = ImGui::IsItemHovered();
			const bool isButtonClicked = ImGui::IsItemActive();
			if(isButtonHovered || isButtonClicked)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(128, 128, 128, 255));	
			}
			if(ImGui::IsItemActive())
			{
				posCache.y += 2;
			}
			ImGui::SetCursorPos(posCache);			
			ImGui::TextUnformatted(ICON_FA_PLUS "  Add");
			if(isButtonHovered || isButtonClicked)
			{
				ImGui::PopStyleColor();
			}
		}
		else
		{
			ImGui::Dummy({2, 2});
		}
	}

	ImGui::EndTable();

	const ImVec2 nodeBodyMin = ImGui::GetItemRectMin();
	const ImVec2 nodeBodyMax = ImGui::GetItemRectMax();
	const ImRect nodeBodyRect(nodeBodyMin, nodeBodyMax);
	ImNodeEd::EndNode();

	// This cannot happen before ImNodeEd::EndNode() since the DrawList won't be available.
	RenderNodeStyle(aNode, nodeHeaderRect, nodeBodyRect);
	
	ImGui::PopID();

	ImNodeEd::PopStyleVar();
	ImNodeEd::PopStyleVar();
}

void ScriptGraphEditor::RenderNodeHeader(const std::shared_ptr<ScriptGraphNode>& aNode, const ImVec2& aNodeHeaderRect)
{
	ScriptGraphEditorSettings* settings = dynamic_cast<ScriptGraphEditorSettings*>(mySettings);
	if(aNode->IsExecNode())
	{
		ImGui::PushFont(mySettings->NodeTitleFont);
		const unsigned nodeTypeU = EnumAsIntegral(aNode->GetNodeType());
		if(settings->NodeSettings.contains(nodeTypeU))
		{
			const auto& nodeSettings = settings->NodeSettings.at(nodeTypeU);
			if(const ImTextureID funcTextureId = nodeSettings.Icon.Get())
			{
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.0f);
				ImGui::Image(funcTextureId, { 16, 16 }, { 0, 0 }, { 1, 1 }, { 255, 255, 255, 255 });
			}
			ImGui::TableNextColumn();
			if(nodeSettings.CenterTitle)
			{
				const ImVec2 nodeTitleSize = ImGui::CalcTextSize(aNode->GetNodeTitle().data());
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.0f);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 2.0f + aNodeHeaderRect.x * 0.5f - nodeTitleSize.x * 0.5f - 8);
			}
			else
			{
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.0f);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 2.0f);
			}
			ImGui::TextUnformatted(aNode->GetNodeTitle().data());
		}
		else
		{
			ImGui::TableNextColumn();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.0f);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 2.0f);
			ImGui::PushFont(mySettings->NodeTitleFont);
			ImGui::TextUnformatted(aNode->GetNodeTitle().data());
		}
		ImGui::PopFont();
	}
}

void ScriptGraphEditor::RenderNodeStyle(const std::shared_ptr<ScriptGraphNode>& aNode, const ImRect& aNodeHeaderRect, const ImRect& aNodeBodyRect)
{
	ScriptGraphEditorSettings* settings = dynamic_cast<ScriptGraphEditorSettings*>(mySettings);
	if(aNode->IsExecNode())
	{
		GraphColor nodeHeaderColor = GraphColor(80, 80, 80, 255);

		// If were a SetVar node we need to hijack the rendering:
		if (const auto& varNode = std::dynamic_pointer_cast<const SGNode_SetVariable>(aNode))
		{
			const ScriptGraphVariable* nodeVar = varNode->GetVariable();
			const ScriptGraphEditorType* sgVarType = ScriptGraphEditorTypeRegistry::Get().GetEditorType(nodeVar->GetType());
			nodeHeaderColor = sgVarType->GetTypeColor();

			ImDrawList* nodeDrawList = ImNodeEd::GetNodeBackgroundDrawList(varNode->GetUID());
			const ImTextureID nodeTextureId = settings->NodeGradientTexture.Get();
			const float halfBorderWidth = ImNodeEd::GetStyle().NodeBorderWidth * 0.5f;
			const ImRect nodeHeader = ImRect(aNodeHeaderRect.Min.x - (8 - halfBorderWidth), aNodeHeaderRect.Min.y - 2 - (4 - halfBorderWidth), aNodeBodyRect.Max.x + (8 - halfBorderWidth), aNodeBodyRect.Min.y - 2);
			nodeDrawList->AddImageRounded(nodeTextureId,
				nodeHeader.Min,
				nodeHeader.Max,
				ImVec2(0.1f, 0.2f), ImVec2(0.8f, 0.8f),
				nodeHeaderColor.AsU32(),
				ImNodeEd::GetStyle().NodeRounding,
				ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight
			);
		}
		else
		{
			const unsigned nodeTypeU = EnumAsIntegral(aNode->GetNodeType());
		
			if(settings->NodeSettings.contains(nodeTypeU))
			{
				const ScriptGraphEditorSettings::NodeTypeSettings& nodeSettings = settings->NodeSettings.at(nodeTypeU);
				nodeHeaderColor = nodeSettings.Color;
			}	

			const float halfBorderWidth = ImNodeEd::GetStyle().NodeBorderWidth * 0.5f;
			const ImRect nodeHeader = ImRect(aNodeHeaderRect.Min.x - (8 - halfBorderWidth), aNodeHeaderRect.Min.y - 2 - (4 - halfBorderWidth), aNodeBodyRect.Max.x + (8 - halfBorderWidth), aNodeBodyRect.Min.y - 2);
			const auto aNodeUID = AsObjectUIDSharedPtr(aNode);
			ImDrawList* nodeDrawList = ImNodeEd::GetNodeBackgroundDrawList(aNodeUID->GetUID());
			const ImTextureID nodeTextureId = settings->NodeHeaderTexture.Get();
			nodeDrawList->AddImageRounded(nodeTextureId,
				nodeHeader.Min,
				nodeHeader.Max,
				ImVec2(0.1f, 0.2f), ImVec2(0.8f, 0.8f),
				nodeHeaderColor.AsU32(),
				ImNodeEd::GetStyle().NodeRounding,
				ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight
				);
		}

		return;
	}

	if(const auto& varNode = std::dynamic_pointer_cast<const SGNode_GetVariable>(aNode))
	{
		ImDrawList* nodeDrawList = ImNodeEd::GetNodeBackgroundDrawList(varNode->GetUID());

		const ScriptGraphVariable* nodeVar = varNode->GetVariable();
		const ScriptGraphEditorType* sgVarType = ScriptGraphEditorTypeRegistry::Get().GetEditorType(nodeVar->GetType());

		const ImTextureID nodeTextureId = settings->NodeGradientTexture.Get();
		nodeDrawList->AddImageRounded(nodeTextureId,
			aNodeBodyRect.Min,
			aNodeBodyRect.Max,
			ImVec2(0.1f, 0.2f), ImVec2(0.8f, 0.8f),
			sgVarType->GetTypeColor().AsU32(),
			ImNodeEd::GetStyle().NodeRounding,
			ImDrawFlags_RoundCornersAll
		);

		return;
	}

	// Compact node style goes here.
	// Nodes have a short name perhaps that is drawn centered and faded, like UE does?
}

void ScriptGraphEditor::RenderPin(const ScriptGraphPin& aPin)
{
	const PinDirection pinDir = aPin.GetPinDirection();
	const float iconSize = 24.0f;
	const ImVec2 iconSizeRect(iconSize, iconSize);

	if(pinDir == PinDirection::Input)
	{
		ImNodeEd::BeginPin(aPin.GetUID(), static_cast<ImNodeEd::PinKind>(pinDir));

		const ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		const ImRect sizeRect(cursorPos, cursorPos + iconSizeRect);
		
		if(ImGui::IsRectVisible(iconSizeRect))
		{
			RenderPinIcon(aPin, sizeRect);
		}

		ImGui::Dummy(iconSizeRect);

		ImNodeEd::EndPin();
		ImGui::SameLine();

		if(aPin.IsLabelVisible())
		{
			const float y = ImGui::GetCursorPosY();
			ImGui::SetCursorPosY(y + 3);
			ImGui::TextUnformatted(aPin.GetLabel().data());
			ImGui::SetCursorPosY(y);
		}

		if(!aPin.IsConnected() && aPin.GetPinType() != ScriptGraphPinType::Exec)
		{
			ImGui::SameLine();
			const float prevY = ImGui::GetCursorPosY();
			const RegisteredType* pinDataType = aPin.GetPinDataType();
			const ScriptGraphEditorType* pinEditorType = ScriptGraphEditorTypeRegistry::Get().GetEditorType(pinDataType);

			const TypedDataContainer& pinData = aPin.GetDataContainer();

			if(pinEditorType->TypeEditWidget(aPin.GetUniqueName(), pinData))
			{
				aPin.GetOwner()->OnUserChangedPinValue(mySchema, aPin.GetUID());
			}
			ImGui::SetCursorPosY(prevY + 3);
			ImGui::Dummy({0, 0});
		}
	}
	else // Output
	{
		if(aPin.IsLabelVisible())
		{
			ImGui::SetCursorPosX(
			ImGui::GetCursorPosX()
				+ ImGui::GetColumnWidth()
				- ImGui::CalcTextSize(aPin.GetLabel().data()).x
				- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x
				- 14.0f
			);

			const float y = ImGui::GetCursorPosY();
			ImGui::SetCursorPosY(y + 3);
			ImGui::TextUnformatted(aPin.GetLabel().data());
			ImGui::SameLine();
			ImGui::SetCursorPosY(y);	
		}
		else
		{
			ImGui::SetCursorPosX(
				ImGui::GetCursorPosX()
				+ ImGui::GetColumnWidth()
				- ImGui::GetScrollX() - ImGui::GetStyle().ItemSpacing.x
				- 14.0f
			);
		}

		ImNodeEd::BeginPin(aPin.GetUID(), static_cast<ImNodeEd::PinKind>(pinDir));

		const ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		const ImRect sizeRect(cursorPos, cursorPos + iconSizeRect);
		
		if(ImGui::IsRectVisible(iconSizeRect))
		{
			RenderPinIcon(aPin, sizeRect);
		}

		ImGui::Dummy(iconSizeRect);
		ImNodeEd::EndPin();
	}
}

void ScriptGraphEditor::RenderPinIcon(const ScriptGraphPin& aPin, const ImRect& aPinRect)
{
	const GraphColor emptyColor(32, 32, 32, 255);
	const bool pinConnected = aPin.IsConnected();
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	const ScriptGraphPinType& pinType = aPin.GetPinType();
	GraphColor pinColor = GraphColor::White;

	const RegisteredType* pinDataType = aPin.GetPinDataType();
	if(pinType != ScriptGraphPinType::Exec && pinDataType)
	{
		const ScriptGraphEditorType* pinEditorType = ScriptGraphEditorTypeRegistry::Get().GetEditorType(pinDataType);	
		if(pinEditorType)
			pinColor = pinEditorType->GetTypeColor();	
	}

	switch(pinType)
	{
	case ScriptGraphPinType::Exec:
		GraphEditorUtils::DrawIcon(drawList, aPinRect, GraphIcon::Exec, pinConnected, GraphColor::White, emptyColor);
		break;
	default:
		GraphEditorUtils::DrawIcon(drawList, aPinRect, GraphIcon::Circle, pinConnected, pinColor, emptyColor);
		break;
	}
}

void ScriptGraphEditor::RenderEdge(const ScriptGraphEdge& anEdge)
{
	if(anEdge.Type == ScriptGraphPinType::Exec)
	{
		return GraphEditorBase::RenderEdge(anEdge);
	}

	const ScriptGraphPin* fromPin = mySchema->GetPin(anEdge.FromId);
	const RegisteredType* fromPinType = fromPin->GetPinDataType();
	const ScriptGraphEditorType* editorType = ScriptGraphEditorTypeRegistry::Get().GetEditorType(fromPinType);
	const GraphColor typeColor = editorType->GetTypeColor().AsNormalized();
	ImNodeEd::Link(anEdge.EdgeId, anEdge.FromId, anEdge.ToId, {typeColor.R, typeColor.G, typeColor.B, typeColor.A}, 1.0f);
}

void ScriptGraphEditor::RenderToolbar()
{
	ImGui::SetNextItemWidth(50);
	if(ImGui::Button(ICON_FA_PLAY "  Events"))
	{
		ImGui::OpenPopup(ScriptGraphEditor_TriggerEntryPointDialogName.data());
	}
	ImGui::SameLine();
	if(ImGui::Button(ICON_FA_CODE "  Variables"))
	{
		ImGui::OpenPopup(ScriptGraphEditor_EditVariablesDialogName.data());
	}
	ImGui::SameLine();
	if(ImGui::Button(ICON_FA_FLOPPY_DISK "  Save"))
	{		
		myGraph->Serialize(TEMP_SAVE_LOAD_dataBlock);
	}
	ImGui::SameLine();
	if(ImGui::Button(ICON_FA_FOLDER_OPEN "  Load"))
	{
		myGraph->Deserialize(TEMP_SAVE_LOAD_dataBlock);
		myEditorState->Layout.RefreshNodePositions = true;
	}

	ScriptGraphEditor_TriggerEntryPointDialog();
	ScriptGraphEditor_EditVariablesDialog();
}

void ScriptGraphEditor::RenderUserBackgroundContextMenu()
{
	const ScriptGraphEditorState* state = dynamic_cast<ScriptGraphEditorState*>(myEditorState);
	if(!state->VariablesContextCategory.Items.empty())
	{
		if (ImGui::TreeNodeEx("Variables", ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			for (const auto& item : state->VariablesContextCategory.Items)
			{
				if (ImGui::TreeNodeEx(item.Title.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth))
				{
					if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					{
						HandleBackgroundContextMenuItemClicked(item);
						myEditorState->ContextSearch.Value.clear();
						myEditorState->ContextSearch.FieldChanged = false;
						ImGui::CloseCurrentPopup();
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
}

void ScriptGraphEditor::HandleUserTransaction()
{
	mySchema->CommitDynamicPinDeletes();
}

std::string_view ScriptGraphEditor::GetEditorTitle() const
{
	return ICON_FA_CROW "  Munin ScriptGraph Editor";
}

std::string_view ScriptGraphEditor::GetEditorToken() const
{
	return "ScriptGraph";
}

std::vector<GraphEditorStateBase::ContextSearchInfo::SearchMenuItem> ScriptGraphEditor::HandleContextMenuSearch(std::string_view aSearchQuery)
{
	std::vector<GraphEditorStateBase::ContextSearchInfo::SearchMenuItem> results = GraphEditorBase::HandleContextMenuSearch(aSearchQuery);
	std::string queryCopy(aSearchQuery);
	std::ranges::transform(queryCopy, queryCopy.begin(), tolower);

	const RegisteredNodeClass& getterNodeClass = MuninGraph::Get().GetNodeClass(typeid(SGNode_GetVariable));
	const RegisteredNodeClass& setterNodeClass = MuninGraph::Get().GetNodeClass(typeid(SGNode_SetVariable));

	bool varGetter = false;
	if(queryCopy.starts_with("get"))
	{
		varGetter = true;
		if(queryCopy.length() > 3)
			queryCopy = queryCopy.substr(3);
	}

	else if(queryCopy.starts_with("set"))
	{
		if(queryCopy.length() > 3)
			queryCopy = queryCopy.substr(3);
	}

	trim_in_place(queryCopy);

	if(!queryCopy.empty())
	{
		for(const auto& varName : mySchema->GetVariables() | std::views::keys)
		{
			const float rank = JaroWinklerDistance(varName, queryCopy);
			GraphEditorContextMenuItem foundItem;
			foundItem.Title = (varGetter ? "Get " : "Set ") + varName;
			foundItem.Value = varGetter ? &getterNodeClass : &setterNodeClass;
			foundItem.Tag = varName;
			results.emplace_back(
				foundItem,
				rank
			);
		}

		std::ranges::sort(results, [](const GraphEditorStateBase::ContextSearchInfo::SearchMenuItem& A, const GraphEditorStateBase::ContextSearchInfo::SearchMenuItem& B)
		{
			return A.Rank > B.Rank;
		});
	}

	for (auto& item : results)
	{
		const std::shared_ptr<ScriptGraphNode>& CDO = std::static_pointer_cast<ScriptGraphNode>(item.Item.Value->ClassDefaultObject);
		switch (CDO->GetNodeType())
		{
		case ScriptGraphNodeType::Function:
		{
			item.Item.Title = ICON_FA_FLORIN_SIGN + ("  " + item.Item.Title);
		}
		break;
		case ScriptGraphNodeType::Event:
		{
			item.Item.Title = ICON_FA_RIGHT_FROM_BRACKET + ("  " + item.Item.Title);
		}
		break;
		case ScriptGraphNodeType::Variable:
		{
			item.Item.Title = ICON_FA_DATABASE + ("  " + item.Item.Title);
		}
		break;
		}
	}

	return results;
}

void ScriptGraphEditor::HandleBackgroundContextMenuItemClicked(const GraphEditorContextMenuItem& aItem)
{
	const ImVec2 mousePos = ImNodeEd::ScreenToCanvas(ImGui::GetMousePos());
	if(const std::shared_ptr<ScriptGraphNode> newNode = mySchema->CreateNode(*aItem.Value))
	{
		const auto uidNewNode = AsObjectUIDSharedPtr(newNode);
		ImNodeEd::SetNodePosition(uidNewNode->GetUID(), mousePos);

		if(const std::shared_ptr<ScriptGraphVariableNode> varNode = std::dynamic_pointer_cast<ScriptGraphVariableNode>(newNode))
		{
			mySchema->SetNodeVariable(varNode.get(), aItem.Tag);
		}
	}
}

void ScriptGraphEditor::ReportError([[maybe_unused]] const ScriptGraph* aGraph, size_t aNodeId, std::string_view aErrorMessage)
{
	ScriptGraphEditorState* state = dynamic_cast<ScriptGraphEditorState*>(myEditorState);
	if (!state->GraphError.HasError)
	{
		state->GraphError.HasError = true;
		myGraph->Stop();

		state->GraphError.Message = aErrorMessage;
		state->GraphError.Node = aNodeId;

		ImNodeEd::SelectNode(state->GraphError.Node);
		ImNodeEd::NavigateToSelection();
		ImNodeEd::DeselectNode(state->GraphError.Node);
	}
}

void ScriptGraphEditor::UpdateVariablesContextMenu() const
{
	ScriptGraphEditorState* state = dynamic_cast<ScriptGraphEditorState*>(myEditorState);
	state->VariablesContextCategory.Items.clear();
	state->VariablesContextCategory.Items.reserve(mySchema->GetVariables().size() * 2);

	const RegisteredNodeClass& getterNodeClass = MuninGraph::Get().GetNodeClass(typeid(SGNode_GetVariable));
	const RegisteredNodeClass& setterNodeClass = MuninGraph::Get().GetNodeClass(typeid(SGNode_SetVariable));

	for (const auto& [varName, var] : mySchema->GetVariables())
	{
		GraphEditorContextMenuItem getterItem;
		getterItem.Title = ICON_FA_DATABASE + ("Get " + varName);
		getterItem.Value = &getterNodeClass;
		getterItem.Tag = varName;

		state->VariablesContextCategory.Items.emplace_back(getterItem);

		if(!var.HasFlag(ScriptGraphVariableFlag_ReadOnly))
		{
			GraphEditorContextMenuItem setterItem;
			setterItem.Title = ICON_FA_DATABASE + ("Set " + varName);
			setterItem.Value = &setterNodeClass;
			setterItem.Tag = varName;

			state->VariablesContextCategory.Items.emplace_back(setterItem);
		}
	}

	std::ranges::sort(state->VariablesContextCategory.Items, [](const GraphEditorContextMenuItem& A, const GraphEditorContextMenuItem& B)
		{
			return A.Tag < B.Tag;
		});
}

void ScriptGraphEditor::ScriptGraphEditor_TriggerEntryPointDialog() const
{
	const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	ImGui::SetNextWindowSize({512, 256});
	if(ImGui::BeginPopupModal(ScriptGraphEditor_TriggerEntryPointDialogName.data()))
	{
		const auto& entryPointList = mySchema->GetEntryPoints();
		ImGui::BeginTable("entryPointBody", 2, ImGuiTableFlags_SizingStretchProp);
		for(const auto& label : entryPointList | std::views::keys)
		{
			ImGui::TableNextColumn();
			if(ImGui::Button(label.c_str()))
			{
				ImGui::CloseCurrentPopup();
				ScriptGraphEditorState* state = dynamic_cast<ScriptGraphEditorState*>(myEditorState);
				state->ShowFlow = true;
				myGraph->Execute(label);
			}
		}
		ImGui::EndTable();

		if(ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
}

void ScriptGraphEditor::ScriptGraphEditor_EditVariablesDialog() const
{
	ScriptGraphEditorState* state = dynamic_cast<ScriptGraphEditorState*>(myEditorState);
	const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	ImGui::SetNextWindowSize({ 1024, 512 });
	if (ImGui::BeginPopupModal(ScriptGraphEditor_EditVariablesDialogName.data(), 0, ImGuiWindowFlags_NoResize))
	{
		const auto& graphVariables = mySchema->GetVariables();
		ImGui::BeginTable("body", 4, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg);
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Type");
		ImGui::TableSetupColumn("Default");
		ImGui::TableSetupColumn("Actions");
		ImGui::TableHeadersRow();
		int varIdx = 0;
		for(const auto& [ varName, variable ] : graphVariables)
		{
			if(variable.HasFlag(ScriptGraphVariableFlag_Internal))
			{
				continue;
			}

			ImGui::TableNextColumn();
			const RegisteredType* varType = variable.GetType();
			const ScriptGraphEditorType* editorType = ScriptGraphEditorTypeRegistry::Get().GetEditorType(varType);
			const GraphColor typeColor = editorType->GetTypeColor().AsNormalized();
			ImGui::TextColored({ typeColor.R, typeColor.G, typeColor.B, typeColor.A }, varName.c_str());
			ImGui::TableNextColumn();
			ImGui::TextUnformatted(varType->GetFriendlyName().data());
			ImGui::TableNextColumn();

			if (editorType->IsInlineEditable())
			{
				ImGui::BeginDisabled(state->VariableEditor.CurrentVariable != varIdx);
				const float y = ImGui::GetCursorPosY(); ImGui::SetCursorPosY(y + 2);
				editorType->TypeEditWidget("##_NewVariable_Type", variable.Default);
				ImGui::EndDisabled();
				ImGui::SetCursorPosY(y);
				ImGui::SameLine();

			}
			else
			{
				ImGui::Dummy({ 1, 1 });
			}

			ImGui::TableNextColumn();
			ImGui::PushID(varName.c_str());
			if(state->VariableEditor.CurrentVariable == varIdx)
			{
				if(ImGui::Button(ICON_FA_FLOPPY_DISK))
				{
					variable.Reset();
					state->VariableEditor.CurrentVariable = -1;
				}
			}
			else
			{
				if(ImGui::Button(ICON_FA_PEN))
				{
					state->VariableEditor.CurrentVariable = varIdx;
				}
				ImGui::SameLine();
				if(ImGui::Button(ICON_FA_XMARK))
				{
					state->VariableEditor.VarToDelete = varName;
				}
			}
			ImGui::PopID();
			varIdx++;
		}
		ImGui::EndTable();
		ImGui::Separator();
		ImGui::TextUnformatted("Create New Variable");
		ImGui::PushItemWidth(200);
		ImGui::InputText("##newVar", &state->VariableEditor.NewVarName);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushItemWidth(150);
		{
			if(ImGui::BeginCombo("##newVarType", state->VariableEditor.NewVarTypeIndex>= 0 ? state->VisibleEditorTypes[state->VariableEditor.NewVarTypeIndex].data() : nullptr, 0))
			{
				for(size_t i = 0; i < state->VisibleEditorTypes.size(); i++)
				{
					const ScriptGraphEditorType* editorType = ScriptGraphEditorTypeRegistry::Get().GetEditorType(state->VisibleEditorTypes[i]);
					if (editorType->IsInternal())
						continue;

					const bool isSelected = (state->VariableEditor.NewVarTypeIndex == static_cast<int>(i));
					if (ImGui::Selectable(state->VisibleEditorTypes[i].data(), isSelected))
					{
						state->VariableEditor.NewVarTypeIndex = static_cast<int>(i);
					}
				}
				ImGui::EndCombo();
			}
		}
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::BeginDisabled(state->VariableEditor.NewVarName.empty() || state->VariableEditor.NewVarTypeIndex < 0);

		ImGui::TextUnformatted("Default Value:");
		ImGui::SameLine();
		if(state->VariableEditor.NewVarTypeIndex >= 0)
		{
			const auto& editorTypes = ScriptGraphEditorTypeRegistry::Get().GetEditorTypeNames();
			const RegisteredType* regType = TypeRegistry::Get().Resolve(editorTypes.at(state->VariableEditor.NewVarTypeIndex));
			const ScriptGraphEditorType* editorType = ScriptGraphEditorTypeRegistry::Get().GetEditorType(regType);
			if(editorType->IsInlineEditable())
			{
				if(state->VariableEditor.NewVariableDefault.GetType() != regType)
				{
					state->VariableEditor.NewVariableDefault = TypedDataContainer::Create(regType);
				}

				const float y = ImGui::GetCursorPosY(); ImGui::SetCursorPosY(y + 2);
				editorType->TypeEditWidget("##_NewVariable_Type", state->VariableEditor.NewVariableDefault);
				ImGui::SetCursorPosY(y);
				ImGui::SameLine();
			}
		}
		//if(ScriptGraphDataTypeRegistry::IsTypeInPlaceConstructible(*myState.varNewVarValue.TypeData))
		//{
		//	const float y = ImGui::GetCursorPosY(); ImGui::SetCursorPosY(y + 2);
		//	ScriptGraphDataTypeRegistry::RenderEditInPlaceWidget(*myState.varNewVarValue.TypeData, myState.varNewVarValue.GetUUID(), myState.varNewVarValue.Ptr);
		//	ImGui::SetCursorPosY(y);
		//	ImGui::SameLine();
		//}

		if(ImGui::Button(ICON_FA_PLUS "  Create", ImVec2(100, 0)))
		{
			mySchema->AddVariable(state->VariableEditor.NewVarName, state->VariableEditor.NewVariableDefault);
			state->VariableEditor.NewVarName.clear();
			state->VariableEditor.NewVariableDefault = TypedDataContainer();
			state->VariableEditor.NewVarTypeIndex = -1;
		}
		ImGui::EndDisabled();

		const float x = ImGui::GetCursorPosX();
		ImGuiWindow* window = GImGui->CurrentWindow;
		;
		ImGui::SetCursorPosX(window->ContentRegionRect.GetWidth() - 100);
		if (ImGui::Button("Close", ImVec2(100, 0)))
		{
			ImGui::CloseCurrentPopup();
			UpdateVariablesContextMenu();
		}

		ImGui::EndPopup();
	}

	ImGui::PopStyleVar();

	if(!state->VariableEditor.VarToDelete.empty())
	{
		mySchema->RemoveVariable(state->VariableEditor.VarToDelete);
		state->VariableEditor.VarToDelete.clear();
	}
}
