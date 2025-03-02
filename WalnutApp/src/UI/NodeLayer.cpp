#include "NodeLayer.h"


NodeLayer::NodeLayer()
{
    tracks.clear();
    for (int i = 0; i < 6; i++) {
        enums::EGemType type = static_cast<enums::EGemType>(i);
        Track track;
        track.gemType = type;
        track.currentPower = 0;
            
        // Create 6 nodes for each track
        for (int j = 0; j < 12; j++) {
            Node node;
            node.level = j + 1;
            node.powerRequired = GetRequiredPowerToNextNode(j + 1);
            node.isActive = false;
            track.nodes.push_back(node);
        }
            
        tracks[type] = track;
    }
}

NodeLayer::~NodeLayer()
{
}

void NodeLayer::addGemPower(enums::EGemType type, float amount)
{
    if (tracks.find(type) != tracks.end()) {
        tracks[type].addPower(amount);
    }
}

bool NodeLayer::isNodeActive(enums::EGemType type, int nodeIndex)
{
    if (tracks.find(type) != tracks.end() && nodeIndex >= 0 && nodeIndex < tracks[type].nodes.size()) {
        return tracks[type].nodes[nodeIndex].isActive;
    }
    return false;
}

int NodeLayer::getActiveNodes(enums::EGemType type)
{
    int total = 0;
    if (tracks.find(type) != tracks.end()) {
        for (auto& node : tracks[type].nodes) {
            if (node.isActive) {
                total++;
            }
        }
    }
    return total;
}

void NodeLayer::UpdateNodes(vector<tuple<enums::EGemSize, enums::EGemType, float, std::string>>& assignedPieces)
{

    ResetTracks();
    
    for (const auto& Piece : assignedPieces)
    {
        enums::EGemSize gemSize = std::get<0>(Piece);
        enums::EGemType gemType = std::get<1>(Piece);
        float addPer = std::get<2>(Piece);
        std::string gearName = std::get<3>(Piece);

        float baseAmount = (gemSize + 1) * 120;
        

        addGemPower(gemType, baseAmount * addPer);
    }
    
}

void NodeLayer::ResetTracks()
{
    for (int i = 0; i < 6; i++)
    {
        enums::EGemType type = static_cast<enums::EGemType>(i);
        tracks.find(type)->second.currentPower = 0;
        tracks.find(type)->second.updateNodeActivation();
    }


}

int NodeLayer::GetRequiredPowerToNextNode(int index) const
{
    if (index <= 2) return 120;          // Level 1: 120
    if (index <= 6) return 240;        // Levels 3-5: 240
    return 360;                       // Levels 6+: 360
}

const std::unordered_map<enums::EGemType, NodeLayer::Track>& NodeLayer::getTracks()
{
    return tracks;
}

void NodeLayer::showNodeLayer()
{
    
    ImGui::Begin("Tracks");
    
    // For each gem type
    for (int gemIndex = 0; gemIndex < 6; gemIndex++) {
        enums::EGemType type = static_cast<enums::EGemType>(gemIndex);
        
        // Header
        ImGui::PushStyleColor(ImGuiCol_Text, gemColors[gemIndex]);
        ImGui::Text("%s Track - Tier: %d", gemNames[gemIndex], getActiveNodes(type));
        ImGui::PopStyleColor();
        
        // colored line
        ImGui::PushStyleColor(ImGuiCol_Separator, gemColors[gemIndex]);
        ImGui::Separator();
        ImGui::PopStyleColor();
        
        // power meter
        int currentPower = tracks.find(type)->second.currentPower;
        ImGui::Text("Power: %d", currentPower);



        //  Amount until next node
        // Amount until next node
        int powerUsed = 0;
        int nextNodeCost = 0;
        int lastNodeCost = 0;
        int currentNodeCost = 0;  // New variable for current node cost
        int totalPowerRequiredForNext = 0;  // Total power needed to reach the next node

        for (int i = 0; i < 12; i++) {
            auto& node = tracks.find(type)->second.nodes[i];
            if (node.isActive) {
                powerUsed += node.powerRequired;
                lastNodeCost = node.powerRequired;
            } else {
                nextNodeCost = node.powerRequired;
                currentNodeCost = lastNodeCost;  // Set the current node's power required (last active node)
                totalPowerRequiredForNext = nextNodeCost;
                break;
            }
        }

        
        // Leeway amount
        int currentProgress = (currentNodeCost > 0) ? (currentPower - powerUsed) : 1;  // Exact amount of progress
        SameLine();
        TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "  -%d", currentProgress > 0 ? currentProgress : 0);

        if (IsItemHovered())
        {
            BeginTooltip();
            SetTooltip("Green: next node (threshold).\nRed: previous node (leeway)");
            EndTooltip();
        }

        // Amount till next node
        int powerNeeded = nextNodeCost - (currentPower - powerUsed);
        SameLine();
        TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "  -%d", powerNeeded > 0 ? powerNeeded : 0);

        if (IsItemHovered())
        {
            BeginTooltip();
            SetTooltip("Green: next node (threshold).\nRed: previous node (leeway)");
            EndTooltip();
        }

        //  Progress bar
        int totalPowerRequired = nextNodeCost - powerUsed;
        float progress = static_cast<float>(currentPower - powerUsed) / totalPowerRequired;
        if (progress > 0)
        {
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, gemColors[gemIndex]);
            ImGui::ProgressBar(progress, ImVec2(-1, 10), "");
            ImGui::PopStyleColor();
        } else
        {
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, gemColors[gemIndex]);
            ImGui::ProgressBar(0.0f, ImVec2(-1, 10), "");
            ImGui::PopStyleColor();
        }

        
        // Node display - horizontal layout
        ImGui::BeginGroup();
        float windowWidth = ImGui::GetWindowContentRegionWidth();
        float nodeWidth = ((windowWidth - 50) / 12) - 5;
        
        for (int i = 0; i < 12; i++) {
            if (i > 0) ImGui::SameLine();
            
            bool isActive = isNodeActive(type, i);
            int threshold = GetRequiredPowerToNextNode(i);
            
            if (isActive) {
                ImGui::PushStyleColor(ImGuiCol_Button, gemColors[gemIndex]);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, gemColors[gemIndex]);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, gemColors[gemIndex]);
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            }
            
            ImGui::PushID(i);
            char buf[32];
            sprintf_s(buf, "%d", i+1);
            
            Button(buf, ImVec2(nodeWidth, 40));
            
            if (IsItemHovered()) {
                BeginTooltip();
                if (IsKeyDown(ImGuiKey_LeftShift)) ImGui::Text("Level: %d", i);
                ImGui::Text("Power required: %d",
                            NodePowerRequirements.find(getTracks().find(type)->second.nodes[i].level)->second);
                ImGui::Text("Threshold: %d", threshold);
                ImGui::EndTooltip();
            }
            
            ImGui::PopID();
            ImGui::PopStyleColor(3);
        }
        ImGui::EndGroup();
        
        ImGui::Spacing();
        ImGui::Spacing();
    }
    
    ImGui::End();
}

void NodeLayer::OnUIRender()
{
    BaseLayer::OnUIRender();
    
    if (showLayer)
    {
        showNodeLayer();
    }
}
