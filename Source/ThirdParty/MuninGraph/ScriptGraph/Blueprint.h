#pragma once
#include <functional>
#include <memory>

struct ScriptGraphNodePayload;
struct ScriptGraphVariable;
class ScriptGraphPin;
class ScriptGraphNode;
struct NodeGraphEdge;
class ScriptGraphSchema;
class ScriptGraph;

class Blueprint : public NodeGraph<ScriptGraphNode, ScriptGraphPin, NodeGraphEdge, ScriptGraphSchema>
{
public:
	typedef std::function<void(const class ScriptGraph&, size_t, const std::string&)> ScriptGraphErrorHandlerSignature;

	Blueprint() = default;
	Blueprint(const std::string& aJson);
	Blueprint(const Blueprint& anOther);
	Blueprint operator=(const Blueprint& anOther);

	const std::vector<size_t>& GetLastExecutedPath() const { return myLastExecutedPath; }
	void ResetLastExecutedPath() { myLastExecutedPath.clear(); }

	void Tick(float aDeltaTime);

	void BindErrorHandler(ScriptGraphErrorHandlerSignature&& aErrorHandler);
	void UnbindErrorHandler();

	bool Run(const std::string& anEntryPointHandle);
	bool RunWithPayload(const std::string& anEntryPointHandle, const ScriptGraphNodePayload& aPayload);

	std::string GetPath() const;

private:
	friend class ScriptManager;

	std::unordered_map<std::string, std::shared_ptr<ScriptGraphNode>> myEntryPoints;
	std::unordered_map<size_t, std::string> myNodeUIDToEntryHandle;

	std::unordered_map<std::string, std::shared_ptr<ScriptGraphVariable>> myVariables;

	std::vector<size_t> myLastExecutedPath;

	ScriptGraphErrorHandlerSignature myErrorDelegate;

	const std::string* myPath;

	virtual const ScriptGraphPin& GetDataSourcePin(size_t aPinUID, bool& outErrored) const override;

	void ReportEdgeFlow(size_t anEdgeUID);
	void ReportFlowError(size_t aNodeUID, const std::string& anErrorMessage) const;
};