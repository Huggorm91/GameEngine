#pragma once

// Defines needed for macro overload
#define CE_EXPAND(...) __VA_ARGS__
#define CE_COUNT_ARGS_N(_0, _1, _2, N, ...) N
#define CE_COUNT_ARGS(...) CE_EXPAND(CE_COUNT_ARGS_N(__VA_ARGS__, _2, _1, _0))
#define CE_SELECT_FUNC_EXPAND(MFunc,NumArgs) MFunc ## NumArgs
#define CE_SELECT_FUNC(MFunc,NumArgs) CE_SELECT_FUNC_EXPAND(MFunc, NumArgs)
#define CE_OVERLOAD_MACRO(MFunc, ...) CE_SELECT_FUNC(MFunc, CE_COUNT_ARGS(, __VA_ARGS__))(__VA_ARGS__)

#define CE_COMPONENT_INTERFACE(Name){\
public:\
	inline const Name* GetTypePointer() const override\
	{ return this; }\
	inline Name* GetTypePointer() override\
	{ return this; }\
	inline void MoveTo(void* aTarget) override\
	{ new (aTarget) Name(std::move(*this)); }\
	inline void CopyTo(void* aTarget) const override\
	{ new (aTarget) Name(*this); }\
	inline size_t SizeOf() const override\
	{ return sizeof(Name); }\
	inline std::type_index TypeId() const override\
	{ return typeid(Name); }\
private:

// Creates necessary signature for a class inheriting from Component
#define BEGIN_COMPONENT_1(Name) class Name : public Component CE_COMPONENT_INTERFACE(Name)

// Creates necessary signature for a class inheriting from class that in turn must inherit from Component
#define BEGIN_COMPONENT_2(Name, Parent) class Name : public Parent CE_COMPONENT_INTERFACE(Name)

// Creates necessary signature for a class inheriting from Component and other classes
#define BEGIN_COMPONENT_MULTI_INHERITANCE(Name, Parent, ...) class Name : public Parent, __VA_ARGS__ CE_COMPONENT_INTERFACE(Name)

// Creates necessary signature for an abstract class that inherits from Component
#define BEGIN_COMPONENT_ABSTRACT(Name) class Name : public Component {

// Creates necessary signature for a class inheriting from Component
#define BEGIN_COMPONENT(...) CE_OVERLOAD_MACRO(BEGIN_COMPONENT, __VA_ARGS__)

#define END_COMPONENT };