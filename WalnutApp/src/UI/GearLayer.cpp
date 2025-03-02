#include "imgui.h"
#include <string>
#include "GearLayer.h"

#include "imgui_internal.h"

#pragma once

using namespace ImGui;
using namespace std;

GearLayer::GearLayer()
{
}

GearLayer::~GearLayer()
{
}

std::vector<std::tuple<std::string, enums::ESocketType>> GearLayer::
GetGearPieces()
{
	return GearPieces;
}

void GearLayer::EquipmentStats()
{
	Begin("Gear", &showLayer, ImGuiWindowFlags_AlwaysAutoResize);

	if (BeginTable("GearTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | 
	ImGuiTableFlags_Reorderable))
	{
		TableSetupColumn("Gear",	ImGuiTableColumnFlags_WidthFixed, 100.0f);
		TableSetupColumn("Socket", ImGuiTableColumnFlags_WidthFixed, 50.0f);
		TableHeadersRow();
		

		TableNextRow();
		int index = 0;
		for (auto& GearTuple : GearPieces)
		{
			std::string Gear = std::get<0>(GearTuple);
			enums::ESocketType& Socket = std::get<1>(GearTuple);
			
			
			std::string buttonID = "##" + Gear;
			TableNextRow();

			TableNextColumn();
			Text(Gear.c_str());
			
			TableNextColumn();
			DrawButton(EColumnType::Socket, Gear, Socket, index);

			index++;
			
		}
		EndTable();
	}

	if (Button("None"))
	{
		SetAllGearToSocket(GearPieces, enums::ESocketType::NoSocket);
	}
	SameLine();
	if (Button("No Add"))
	{
		SetAllGearToSocket(GearPieces, enums::ESocketType::NoAdd);
	}
	SameLine();
	if (Button("25%"))
	{
		SetAllGearToSocket(GearPieces, enums::ESocketType::Percentage25);
	}
	SameLine();
	if (Button("66%"))
	{
		SetAllGearToSocket(GearPieces, enums::ESocketType::Percentage66);
	}
	

	End();
}

int GearLayer::GetTotalGemPower()
{
	float total = 0;
	for (auto& GearTuple : GearPieces)
	{
		std::string Gear = std::get<0>(GearTuple);
		enums::ESocketType& Socket = std::get<1>(GearTuple);

		float addPer = 1.0f;
		
		switch (Socket)
		{
		case enums::NoSocket: addPer = 0.0f; break;
		case enums::NoAdd: addPer = 1.0f; break;
		case enums::Percentage25: addPer = 1.25f; break;
		case enums::Percentage66: addPer = 1.66f; break;
		}
	}
	return ceil(total);
}

void GearLayer::SetAllGearToSocket(std::vector<std::tuple<std::string, enums::ESocketType>>& GearPieces, 
								   enums::ESocketType NewSocket)
{
	for (auto& GearTuple : GearPieces)
	{
		get<1>(GearTuple) = NewSocket;
	}
}


void GearLayer::DrawButton(EColumnType type, std::string& Gear, 
                                   enums::ESocketType& Socket, int index)
{
	std::string popupID;
    std::string buttonLabel = "##" + Gear ;
	
    switch (type)
    {
        case EColumnType::Socket:
            popupID = GetSocketTypeName(Socket) + "##" + Gear + std::to_string(index);
            buttonLabel = GetSocketTypeName(Socket) + "##" + Gear + std::to_string(index);
            break;
    }
	
	
	// Button
	for (int i = 0; i < 4; ++i) {
		enums::ESocketType currentType = static_cast<enums::ESocketType>(i);

		// Change button color based on whether it matches the selected Socket
		if (Socket == currentType)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.02f, 0.2f, 0.2f, 1.0f));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
		}

		if (ImGui::Button((GetSocketTypeName(currentType) + "##" + buttonLabel).c_str())) {
			Socket = currentType;
		}

		ImGui::PopStyleColor(1); // Pop the last pushed color

		if (i < 3) ImGui::SameLine(); // Avoid SameLine after the last button
	}
	

}


std::string GearLayer::GetSocketTypeName(enums::ESocketType SocketType)
{
	switch (SocketType)
	{
		case enums::NoSocket: return "None";
		case enums::NoAdd: return "NoAdd";
		case enums::Percentage25: return "25%";
		case enums::Percentage66: return "66%";
	}
}

std::string GearLayer::GetColorTypeName(enums::EGemType Socket)
{
	switch (Socket)
	{
	case enums::EGemType::ruby: return "Ruby";
	case enums::EGemType::amethyst: return "Amethyst";
	case enums::EGemType::topaz: return "Topaz";
	case enums::EGemType::emerald: return "Emerald";
	case enums::EGemType::sapphire: return "Sapphire";
	case enums::EGemType::diamond: return "Diamond";
	}
}

std::string GearLayer::GetSizeTypeName(enums::EGemSize Socket)
{
	switch (Socket)
	{
	case enums::small: return "Small";
	case enums::large: return "Large";
	case enums::splendid: return "Splendid";
	case enums::flawless: return "Flawless";
	}
}

void ::GearLayer::OnUIRender()
{
	BaseLayer::OnUIRender();
	if (showLayer)
	{
		EquipmentStats();
		
	}
}
