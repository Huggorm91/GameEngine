#include "Modelviewer.pch.h"
#include "SkeletonEditor.h"
#include "AssetManager/Assets/Components/Camera/PerspectiveCameraComponent.h"
#include "AssetManager/Assets/Components/Camera/EditorCameraControllerComponent.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "Modelviewer.h"

SkeletonEditor::SkeletonEditor():
	myIsActive(false),
	mySkeleton(nullptr)
{}

void SkeletonEditor::Init(float aFoVDegree, float aNearPlane, float aFarPlane, float aCameraSpeed, float aMouseSensitivity)
{
	myCamera.SetPosition({ 0.f, 200.f, -200.f });
	myCamera.AddComponent(PerspectiveCameraComponent(aFoVDegree, aNearPlane, aFarPlane));
	myCamera.AddComponent(EditorCameraControllerComponent(aCameraSpeed, aMouseSensitivity));

	myWindowSize = GraphicsEngine::Get().GetWindowSize();

	mySkeleton = AssetManager::GetAsset<Skeleton*>("SK_C_TGA_Bro.fbx");
}

void SkeletonEditor::Update()
{
	if (!myIsActive)
	{
		return;
	}
	myCamera.Update();

	// Create Editor workarea
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(myWindowSize.x, myWindowSize.y));
	const auto& color = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(color.x, color.y, color.z, 1.f));
	ImGui::Begin("Skeleton_MainWindow", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar);
	ImGui::PopStyleColor();

	CreateMenubar();

	ImGuiID dockspace_id = ImGui::GetID("Skeleton_MainDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_AutoHideTabBar);

	CreateViewport();
	CreateSkeletonHeirarchy();
	CreateAssetBrowser();

	ImGui::End();
}

void SkeletonEditor::SetSkeleton(Skeleton* aSkeleton)
{
	mySkeleton = aSkeleton;
}

void SkeletonEditor::SetActive(bool aState)
{
	myIsActive = aState;
}

void SkeletonEditor::SetCameraSpeed(float aSpeed)
{
	myCamera.GetComponent<EditorCameraControllerComponent>().SetSpeed(aSpeed);
}

void SkeletonEditor::SetMouseSensitivity(float aSensitivity)
{
	myCamera.GetComponent<EditorCameraControllerComponent>().SetMouseSensitivity(aSensitivity);
}

void SkeletonEditor::Activate()
{
	myIsActive = true;
}

void SkeletonEditor::CreateMenubar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::MenuItem("Close"))
		{
			myIsActive = false;
			ModelViewer::Get().ActivateImGuiEditor();
		}
		ImGui::EndMenuBar();
	}
}

void SkeletonEditor::CreateSkeletonHeirarchy()
{

}

void SkeletonEditor::CreateViewport()
{
	if (ImGui::Begin("Skeleton_Viewport", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar))
	{
		const Crimson::Vector2f aspectRatio = { myWindowSize.y / myWindowSize.x, myWindowSize.x / myWindowSize.y };

		auto size = ImGui::GetContentRegionAvail();

		const Crimson::Vector2f regionSize = { size.x, size.y };
		const Crimson::Vector2f regionRatio = { size.y / size.x, size.x / size.y };

		if (regionRatio.x < aspectRatio.x)
		{
			size.x = size.y / aspectRatio.x;
		}
		if (regionRatio.y < aspectRatio.y)
		{
			size.y = size.x / aspectRatio.y;
		}

		const Crimson::Vector2f off = (regionSize - Crimson::Vector2f{ size.x, size.y }) * 0.5f;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off.x);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + off.y);

		ImGui::Image(GraphicsEngine::Get().GetBackBufferCopy()->GetSRV().Get(), size);
	}
	ImGui::End();
}

void SkeletonEditor::CreateAssetBrowser()
{}
