cmake_minimum_required(VERSION 4.0)

# Libraries
#find_package(glfw3 3.4 REQUIRED)
#find_package(Vulkan REQUIRED)
#set(ModelViewer_LIBRARIES "glfw;Vulkan::Vulkan")

set(ModelViewer_LOCAL_SOURCES
        ModelViewer.pch.cpp
        Main.cpp
        ModelViewer.cpp
        Commands/EditCmd_AddGameObject.cpp
        Commands/EditCmd_AddGameObjects.cpp
        Commands/EditCmd_ChangeGameObjectName.cpp
        Commands/EditCmd_ChangeMultipleGameObjects.cpp
        Commands/EditCmd_ChangeTransform.cpp
        Source/ModelViewer/Commands/EditCmd_ChangeValue.hpp
        Commands/EditCmd_RemoveGameObject.cpp
        Commands/EditCmd_RemoveGameObjects.cpp
        Commands/EditCommand.cpp
        Core/ApplicationState.cpp
        Core/AssetTypes.cpp
        Editor/ImguiManager.cpp
        Editor/SkeletonEditor.cpp
        Windows/SplashWindow.cpp
)

set(ModelViewer_PUBLIC_HEADERS
        ModelViewer.h
        Commands/EditCmd_AddComponent.h
        Commands/EditCmd_AddGameObject.h
        Commands/EditCmd_AddGameObjects.h
        Commands/EditCmd_ChangeComponent.h
        Commands/EditCmd_ChangeGameObjectName.h
        Commands/EditCmd_ChangeMultipleGameObjects.h
        Commands/EditCmd_ChangeTransform.h
        Commands/EditCmd_RemoveComponent.h
        Commands/EditCmd_RemoveGameObject.h
        Commands/EditCmd_RemoveGameObjects.h
        Commands/EditCommand.h
        Core/ApplicationState.h
        Core/AssetTypes.h
        Editor/ImguiManager.h
        Editor/SkeletonEditor.h
        Windows/SplashWindow.h
)

set(ModelViewer_PRIVATE_HEADERS
        ModelViewer.pch.h
        resource.h
        targetver.h
)

# Dear ImGui
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/ThirdParty/ImGui)
set(IMGUI_INCLUDES ${IMGUI_DIR} ${IMGUI_DIR}/backends)
set(IMGUI_SOURCES
#        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
#        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
)
include_directories(${IMGUI_INCLUDES})

# Linux
#find_package(X11 REQUIRED)
#include_directories(${X11_INCLUDE_DIR})
#link_directories(${X11_LIBRARIES})

# TODO: Check how to add executable icon to build
add_executable(ModelViewer
        ${ModelViewer_LOCAL_SOURCES}
        ${IMGUI_SOURCES}
)

#target_link_libraries(ModelViewer
 #       ${ModelViewer_LIBRARIES}
#)
# Linux
#target_link_libraries(ModelViewer ${X11_LIBRARIES})