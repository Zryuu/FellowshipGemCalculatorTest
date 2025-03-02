#include "imgui_internal.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Application.h"
#include "UI/CalculateLayer.h"
#include "UI/GearLayer.h"
#include "UI/GemLayer.h"
#include "UI/NodeLayer.h"

using namespace ImGui;


Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "FGC-Test";

	Walnut::Application* app = new Walnut::Application(spec);
	std::shared_ptr<GearLayer> gearLayer = std::make_shared<GearLayer>();
	std::shared_ptr<GemLayer> gemLayer = std::make_shared<GemLayer>();
	std::shared_ptr<NodeLayer> nodeLayer = std::make_shared<NodeLayer>();
	std::shared_ptr<CalculateLayer> calculateLayer = std::make_shared<CalculateLayer>(gearLayer, gemLayer, nodeLayer);
	
	
	app->PushLayer(gearLayer);
	app->PushLayer(gemLayer);
	app->PushLayer(nodeLayer);
	app->PushLayer(calculateLayer);
	app->SetMenubarCallback([app, gemLayer, gearLayer, nodeLayer, calculateLayer]()
	{
		if (BeginMenu("Windows"))
		{
			Button("Gems");
			//	LeftClick
			if (IsItemClicked(ImGuiMouseButton_Left))
			{
				gemLayer->showLayer = true;
			}

			//	RightClick
			if (IsItemClicked(ImGuiMouseButton_Right))
			{
				gemLayer->showLayer = true;
				gemLayer->ResetLayerPosition();
			}

			// Shift+Hovered
			if (IsKeyDown(ImGuiKey_LeftShift) && IsItemHovered())
			{
				BeginTooltip();
				Text("Set your Gems.");
				EndTooltip();
			}

			Button("Gear");
			//	LeftClick
			if (IsItemClicked(ImGuiMouseButton_Left))
			{
				gearLayer->showLayer = true;
			}

			//	RightClick
			if (IsItemClicked(ImGuiMouseButton_Right))
			{
				gearLayer->showLayer = true;
				gearLayer->ResetLayerPosition();
			}

			// Shift+Hovered
			if (IsKeyDown(ImGuiKey_LeftShift) && IsItemHovered())
			{
				BeginTooltip();
				Text("Set your Gear");
				EndTooltip();
			}

			Button("Nodes");
			//	LeftClick
			if (IsItemClicked(ImGuiMouseButton_Left))
			{
				nodeLayer->showLayer = true;
			}

			//	RightClick
			if (IsItemClicked(ImGuiMouseButton_Right))
			{
				nodeLayer->showLayer = true;
				nodeLayer->ResetLayerPosition();
			}

			// Shift+Hovered
			if (IsKeyDown(ImGuiKey_LeftShift) && IsItemHovered())
			{
				BeginTooltip();
				Text("Nodes that are available with the current Gear+Gem configuration");
				EndTooltip();
			}

			Button("Calculate");
			//	LeftClick
			if (IsItemClicked(ImGuiMouseButton_Left))
			{
				calculateLayer->showLayer = true;
			}

			//	RightClick
			if (IsItemClicked(ImGuiMouseButton_Right))
			{
				calculateLayer->showLayer = true;
				calculateLayer->ResetLayerPosition();
			}

			// Shift+Hovered
			if (IsKeyDown(ImGuiKey_LeftShift) && IsItemHovered())
			{
				BeginTooltip();
				Text("It calculates....");
				EndTooltip();
			}
			
			EndMenu();
		}
		});
	
	return app;
}