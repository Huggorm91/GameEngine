# MuninGraph II
*by Daniel Borgshammar @ TGA*

## Blueprint-style Visual Scripting system

### Contents:
**MuninGraph** - Generic Graph system with implemented ScriptGraph. This project handles all the workings of the NodeGraph and ScriptGraph systems.  
**MuninGraphEditor** - ImGui-based visual editor for NodeGraphs including a ScriptGraph editor.  
**ScriptGraphNodes** - Separate project for implementing nodes not integral to the ScriptGraph system (such as user nodes).

**MuninGraph Editor depends on:**  
ImGui 1.90.1
ImGuiNodeEditor 0.9.3

## Usage:

* Add all the projects to your solution and build
* Set up your artifact project (exe or dll):

### For Runtime:

* Link: muningraph.lib, scriptgraphnodes.lib
* Include MuninGraph.h and ScriptGraphNodes.h
* Call `MuninGraph::Get().Initialize();` and you are ready to go.

### For Development:

* Link: imgui.lib, imguinodeeditor.lib, muningraph.lib, scriptgraphnodes.lib, muningrapheditor.lib
* Include MuninGraph.h, ScriptGraphNodes.h and ScriptGraphEditor.h
* Call `MuninGraph::Get().Initialize();` to initialize the system.
* Create a `ScriptGraph` with `ScriptGraph myGraph = new ScriptGraph();`
* Create Editor Settings and State objects:  
```
ScriptGraphEditorSettings editorSettings(GraphicsEngine::GetRHI()->GetDevice());
ScriptGraphEditorState editorState;
```
* Finally instantiate the `ScriptGraphEditor`  
`ScriptGraphEditor myScriptEditor = new ScriptGraphEditor(&editorSettings, &editorState, myGraph);`
* Call Render in some handy location. `myScriptEditor->Render();`

### TGE:
If you would like to use this in TGE you can add the various projects using the `externalproject` command in premake5. Keep in mind that TGE already has a version of ImGui in its External library but the version might not be compatible with this project! You will also need to edit the include paths for ScriptGraphEditor to look for ImGui in the correct place.
You will need to read the documentation for `externalproject` here https://premake.github.io/docs/externalproject/ and you will need UUIDs for the various projects in this package which are provided below:

MuninGraph : cab443dd-87c5-4989-a385-f45201559698  
MuninGraphEditor : 3996b64c-9a2b-41f5-9d0e-1a1525dae621  
ScriptGraphNodes : 1b06105b-c006-49ec-8500-983773169cb7  
ImGuiNodeEditor : a4912cdb-df6b-46c5-8b09-4d3ddb85f985  

## ScriptGraph User Manual

## ScriptGraphs

A ScriptGraph in C++ is a simple object. It has facilities for De/Serialization and Execution of the graph. De/Serialization is made to and from a vector of bytes (uint8_t) which you can store somewhere handy for your project. 

> The sample editor functionality for Loading and Saving is done to a memory array which will not persist between executions!

Executing a graph takes in a Handle referring to which node to start. See Event Nodes for more information on how these are defined.

## Nodes

There are several places where you can extend the ScriptGraph system depending what you want to do. The most common usage case is to create new Nodes but this manual also covers adding new data types to the system.

> TLDR:  Put ```DECLARE_GRAPH_NODE(NodeClass, NodeBase)``` in your header file, and put ```IMPLEMENT_GRAPH_NODE(NodeClass, NodeBase);``` in your source file. Override GetNodeTitle and GetNodeCategory to provide information. Ctor creates Pins. Override DoOperation to provide node functionality

To create a new Node for the ScriptGraph system you need to use the following macro in a header file:  
```DECLARE_GRAPH_NODE(NodeClass, NodeBase)```  
Example:  
```DECLARE_GRAPH_NODE(SGNode_MathAdd, ScriptGraphNode)```  
This will set up a node Class called SGNode_MathAdd which inherits from ScriptGraphNode.

Similarly there needs to be a macro in the source file to provide the rest of the boilerplate code for the node:  
```IMPLEMENT_GRAPH_NODE(SGNode_MathAdd, ScriptGraphNode);```  
This ensures that everything will function correctly. Without these two macros your code will not compile.

> All nodes created in this manner in the ScriptGraphNodes project will automatically be discovered and registered when you compile the project as long as they are in subfolder under the ScriptGraphNodes project (see example nodes).

### Node Functionality

All nodes need to override GetNodeTitle and GetNodeCategory to provide information about themselves. If we again look at the declaration of our sample node it looks like this:
```
DECLARE_GRAPH_NODE(SGNode_MathAdd, ScriptGraphNode)
{
public:
	SGNode_MathAdd();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Add"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Math"; }
	NodeResult DoOperation() override;
};
```
The primary functionality of each node is defined in its constructor and in the DoOperation function. The constructor is responsible for creating the pins that will appear on the node and DoOperation will define exactly what it is that the node does. In this case it defines the code required to perform addition of two float numbers.  

Looking at the source file we have the following code:
```
IMPLEMENT_GRAPH_NODE(SGNode_MathAdd, ScriptGraphNode);

SGNode_MathAdd::SGNode_MathAdd()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("A", PinDirection::Input);
	CreateDataPin<float>("B", PinDirection::Input);
	CreateDataPin<float>("Result", PinDirection::Output);
}

NodeResult SGNode_MathAdd::DoOperation()
{
	float inA = 0;
	float inB = 0;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		const float result = inA + inB;
		SetPinData("Result", result);
		return ExecPin("Out");
	}

	return Error("Something went wrong adding up A and B!");
}
```

The constructor creates five pins. The first pins are Exec pins that control the flow of execution through the node. The majority of nodes will have two Exec pins; one input pin and one output pin. Further the node defines three data pins; two input values called A and B and an output pin called Result. This shows the user that we expect to receive two float values as input and that we will provide one float value as output.

The magic itself happens in DoOperation where we retrieve the values of both input pins, add them together and then write the result to our Result pin. Finally we also tell the owning graph system that we want to resume execution on the Output Exec pin.

### Node Types

The default node type is a Function node. This means that it's some sort of node that performs a function, usually with input and with a result. They are, however, incapable of being executed on their own and require a start point in the form of an Event Node. Event Nodes can be thought of as function calls in C++. They do not have an input execution pin and therefore cannot be linked in the middle of a node graph but they do provide a handy method of passing data from C++ to the Script Graph system.

Event Nodes are derived from `ScriptGraphEventNode` in a similar way to normal nodes. You declare them using the same macro: `DECLARE_GRAPH_NODE(SGNode_BeginPlay, ScriptGraphEventNode)`. However, for Event nodes you do not want to override DoOperation since this is handled automatically by the parent class. Instead you just define pins as normal. Keep in mind that an Event Node cannot have any input pins! All event nodes will automatically appear in the Events category if you do not override GetNodeCategory.

```
SGNode_BeginPlay::SGNode_BeginPlay()
{
	// Event Nodes already have an Exec Pin called "Out" which gets called automatically.
	// You can add more pins here if you wish but it can only ever have ONE Exec Pin and
	// no Input Pins! Events are considered start nodes.
}
```

You can, however, provide output data pins that appear on the node.  

Event nodes can be executed by calling them from C++ via the ScriptGraph's `Execute` functions. Both functions take an Entry Point Handle. This is a string referring to which Event node to execute. By default the Event Node Handle is the name of the node class minus the prefix. In the case of SGNode_BeginPlay the handle is BeginPlay. You can override the handle of a node by overriding the GetEntryHandle function of the Event Node. 

Event nodes can receive data from C++ invocation and are an excellent means of providing data for the graph to work with. `ExecuteWithPayload` is used instead of simply calling `Execute` to provide a payload with data for the event node. Here is an example of using a payload:

First, define a pin with an appropriate name. For this example we have a Tick node that provides Delta Time to the ScriptGraph:
```
SGNode_Tick::SGNode_Tick()
{
	CreateDataPin<float>("Delta Time", PinDirection::Output);
}
```

Next, to execute this node and provide Delta Time to the graph we must create a Payload. Payloads allocate memory and it's a good idea to reuse them if it's possible to do so to avoid the allocation overhead. Then we set the value of the pin "Delta Time" on the payload. It is important that this pin name is exactly the same as the name defined in the nodes constructor!
```
void ScriptGraph::Tick(float aDeltaTime)
{
	myLastExecutedPath.clear();
	static ScriptGraphPayload thisPayload;
	thisPayload.SetPinValue("Delta Time", aDeltaTime);
	ExecuteWithPayload("Tick", thisPayload);
}
```

## Data Types

By default the ScriptGraph only knows about two Data Types: Float and String. You can easily extend this to include any type you wish but care must be taken to handle types that are not trivially copyable (i.e. those requiring a deep copy). Data types can be declared anywhere but it's recommended to keep them somewhere handy and easy to locate. The two that are registered by default reside in `ScriptGraphTypes.h` in the `ScriptGraphNodes` project.  

To declare a type you use the `DECLARE_REGISTERED_TYPE(T)` macro in a header file, for example `DECLARE_REGISTERED_TYPE(float);`. Similarly to declaring and implementing nodes you must also implement the data for the type you are declaring. This is done using the `IMPLEMENT_REGISTERED_TYPE(T, N)` macro. Using the float type as example: `IMPLEMENT_REGISTERED_TYPE(float, Float);`. This macro tells the type system what datatype the type is derived from and the "friendly name" of the type, the name to be used in the UI and in other places where we need to identify this type.

For types requiring Deep Copy you need to do more work than simply using the normal implement macro. Instead you must use the `BEGIN_IMPLEMENT_REGISTERED_TYPE(T, N)` macro, fill in the required functionality and finally finish with the `END_IMPLEMENT_REGISTERED_TYPE()` macro. Required funcionality includes providing a Copy Function, a Serialize Function and a Deserialize Function. You can look in ScriptGraphTypes.cpp for an example under the String definition.

### Data Types and the Editor
Simply defining a Data Type allows the ScriptGraph to use it, but to use it in the Editor you must provide some additional functionality. To keep the Graph Runtime disconnected from the editor this cannot be done in the `ScriptGraphTypes.h` file and must instead be done separately. The example implementations for Float and String types are defined in `ScriptGraphEditorTypes.h` in the `MuninGraphEditor`` project.

Editor functionality for a type is declared using the `BEGIN_DECLARE_EDITOR_TYPE(T, N, C, E)` and `END_DECLARE_EDITOR_TYPE()` macros. The parameters are as follows:
* T - The Type we want to use
* N - The Friendly Name of the type
* C - The color used to identify this type in the Editor
* E - If this type is inline-editable or not (can be edited directly on nodes in the editor).

You must provide GUI functionality for editing this type which can be done by overriding `RenderEditWidget` of your declared editor type. It is also recommended to override ToString to allow an easy string representation of your value. This can be handy for debugging to a log file or console, but also to display a numerical value in a text field in your user interface.  

Here is an example declaration for the Float editor type:
```
BEGIN_DECLARE_EDITOR_TYPE(float, Float, GraphColor(156, 246, 60, 255), true);
	void RenderEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const override;
	std::string ToString(const TypedDataContainer& aDataContainer) const override;
END_DECLARE_EDITOR_TYPE();
```
and the corresponding implementation:
```
IMPLEMENT_EDITOR_TYPE(float, Float)

void ScriptGraphEditorType_Float::RenderEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const
{
	const float y = ImGui::GetCursorPosY();
	ImGui::SetCursorPosY(y - 2);
	const ImVec2 inputSize = ImGui::CalcTextSize("0.0000");
	ImGui::SetNextItemWidth(inputSize.x);
	ImGui::InputFloat(aUniqueName.data(), static_cast<float*>(*aDataContainer), 0, 0, "%.1f");
}

std::string ScriptGraphEditorType_Float::ToString(const TypedDataContainer& aDataContainer) const
{
	float f = 0;
	aDataContainer.TryGet(f);
	return std::to_string(f);
};
```

## Variables

ScriptGraph can also utilize in-graph variables. These can be edited directly in the sample editor and nodes will automatically be provided that can Get or Set these variables accordingly.

## Schemas

To modify a ScriptGraph (or any NodeGraph) you must first create a Schema. The Schema contains logic for how the underlying graph may be modified and enforces rules for how a graph should look and function. This is generally not required in runtime since a pre-created graph should already have been sanity checked by the schema used to create it. To create a schema you simply need to instantiate `ScriptGraphSchema` and provide its constructor with a graph to modify. All editing of a graph should go through its schema (see the example editor for a demonstation).