#pragma once
#include "Internal/MuninGraphCommon.h"
#include "NodeGraph/NodeGraph.h"
#include "NodeGraph/NodeGraphCommon.h"

class ScriptGraphPayload;
struct ScriptGraphVariable;
class ScriptGraphSchema;
struct ScriptGraphEdge;
class ScriptGraphNode;
class ScriptGraphPin;

class ScriptGraph : public NodeGraph<ScriptGraphNode, ScriptGraphPin, ScriptGraphEdge, ScriptGraphSchema>
{
	friend class ScriptGraphSchema;

public:

	ScriptGraph(void* aOwner = nullptr);

	bool Execute(std::string_view aEntryPointHandle);
	bool ExecuteWithPayload(std::string_view aEntryPointHandle, const ScriptGraphPayload& aPayload);
	void Tick(float aDeltaTime);

	void Stop();

	bool Serialize(std::vector<uint8_t>& outResult) override;
	bool Deserialize(const std::vector<uint8_t>& inData) override;

	FORCEINLINE const std::vector<size_t>& GetLastExecutedPath() const { return myLastExecutedPath; }
	FORCEINLINE size_t GetNumActiveFunctions() const { return myNumActiveFunctions; }

	typedef std::function<void(const ScriptGraph*, size_t, std::string_view)> ScriptGraphErrorHandlerSignature;

	void BindErrorHandler(ScriptGraphErrorHandlerSignature&& aErrorHandler);
	void UnbindErrorHandler();

	FORCEINLINE void* GetOwner() const { return myOwner; }

protected:
	void Reset() override;

private:

	void SetupScriptGraph();

	bool ExecuteInternal(ScriptGraphNode* aNode, size_t aPinId);
	void ReportError(size_t aNodeId, std::string_view aErrorMessage) const;

	bool myShouldStop;
	size_t myNumActiveFunctions;
	void* myOwner;

	std::vector<size_t> myLastExecutedPath;
	std::unordered_map<std::string, size_t, string_hash, std::equal_to<>> myEntryPoints;
	std::unordered_map<size_t, std::string> myNodeIdToEntryPoint;

	std::unordered_map<std::string, ScriptGraphVariable, string_hash, std::equal_to<>> myVariables;

	ScriptGraphErrorHandlerSignature myErrorDelegate;
};

