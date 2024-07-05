#pragma once
#include "imgui.h"
#include "ScriptGraphEditorTypeRegistry.h"
#include "Math/Vector3.hpp"

#pragma comment(linker, "/include:__MuninGraph_AutoRegEditorTypes")

BEGIN_DECLARE_EDITOR_TYPE(bool, Bool, GraphColor(255, 0, 0, 255), true);
bool TypeEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const override;
std::string ToString(const TypedDataContainer& aDataContainer) const override;
END_DECLARE_EDITOR_TYPE();

BEGIN_DECLARE_EDITOR_TYPE(int, Int, GraphColor(0, 255, 255, 255), true);
bool TypeEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const override;
std::string ToString(const TypedDataContainer& aDataContainer) const override;
END_DECLARE_EDITOR_TYPE();

BEGIN_DECLARE_EDITOR_TYPE(unsigned, GameObjectID, GraphColor(0, 255, 0, 255), true);
bool TypeEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const override;
std::string ToString(const TypedDataContainer& aDataContainer) const override;
END_DECLARE_EDITOR_TYPE();

BEGIN_DECLARE_EDITOR_TYPE(float, Float, GraphColor(156, 246, 60, 255), true);
	bool TypeEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const override;
	std::string ToString(const TypedDataContainer& aDataContainer) const override;
END_DECLARE_EDITOR_TYPE();

BEGIN_DECLARE_EDITOR_TYPE(std::string, String, GraphColor(250, 0, 208, 255), true);
	bool TypeEditWidget(std::string_view aUniqueName, const TypedDataContainer& aDataContainer) const override;
	std::string ToString(const TypedDataContainer& aDataContainer) const override;
END_DECLARE_EDITOR_TYPE();

BEGIN_DECLARE_EDITOR_TYPE(Crimson::Vector3f, Vector3, GraphColor(255, 255, 0, 255), true);
std::string ToString(const TypedDataContainer& aDataContainer) const override;
END_DECLARE_EDITOR_TYPE();

BEGIN_DECLARE_EDITOR_TYPE(void*, Internal_VoidPtr, GraphColor(0, 168, 241, 255), false);
	FORCEINLINE bool IsInternal() const override { return true; }
END_DECLARE_EDITOR_TYPE();