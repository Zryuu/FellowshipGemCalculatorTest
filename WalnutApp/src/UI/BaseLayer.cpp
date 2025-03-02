#include "BaseLayer.h"

#include "imgui_internal.h"

BaseLayer::BaseLayer()
{
}

BaseLayer::~BaseLayer()
{
}

void BaseLayer::OnUIRender()
{
    Layer::OnUIRender();
    
    static bool firstTime = true;
    if (firstTime)
    {
        SetupDockSpace();
        firstTime = false;
    }
}

void BaseLayer::ResetLayerPosition()
{
    ImGuiViewport* viewport = GetMainViewport();

    ImVec2 centerPos = ImVec2(viewport->Pos.x + (viewport->Size.x * 0.5f),
                                viewport->Pos.y + (viewport->Size.y * 0.5f));

    SetNextWindowPos(centerPos);
}

void BaseLayer::SetupDockSpace()
{
    static bool firstTime = true;
    if (!firstTime)
        return;

    firstTime = false;

    // Reset the current ImGui window/docking state
    ImGui::DockBuilderRemoveNode(ImGui::GetID("DockSpace"));
    ImGui::DockBuilderAddNode(ImGui::GetID("DockSpace"), ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(ImGui::GetID("DockSpace"), ImGui::GetMainViewport()->Size);

    // Create your docking layout
    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGuiID dock_main_id = dockspace_id;

    // Example layout - customize to your needs:
    // Split the dockspace into left and right parts
    ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.3f, nullptr, &dock_main_id);
    ImGuiID dock_id_right = dock_main_id;

    // Further split the right part into top and bottom
    ImGuiID dock_id_right_top = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Up, 0.6f, nullptr, &dock_id_right);
    ImGuiID dock_id_right_bottom = dock_id_right;

    // Dock your windows
    ImGui::DockBuilderDockWindow("Calculate Window", dock_id_left);
    ImGui::DockBuilderDockWindow("Gear Window", dock_id_right_top);
    ImGui::DockBuilderDockWindow("Gem Window", dock_id_right_bottom);

    // Finish the docking setup
    ImGui::DockBuilderFinish(dockspace_id);
}
