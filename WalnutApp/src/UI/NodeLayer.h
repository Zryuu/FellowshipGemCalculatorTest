#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "BaseLayer.h"
#include "../enums.h"

using namespace ImGui;
using namespace std;

class NodeLayer : public BaseLayer
{
public:
    
    struct Node {
        int level;
        int powerRequired;
        bool isActive;
    
        // Calculate bonus based on node level
        int calculateBonus() const {
            if (level == 1) return 120;                      // Level 1: 120
            if (level == 2) return 240;                      // Level 2: 240
            if (level <= 5) return 240 * (level - 2);        // Levels 3-6: 240*(level-2)
            return 1200 + 360 * (level - 6);                 // Levels 6+: 1200 + 360*(level-6)
        }

        void ResetIsActive()
        {
            isActive = false;
        }
    };

    struct Track {
        enums::EGemType gemType;
        std::vector<Node> nodes;
        int currentPower;
        
        void addPower(float amount) {
            currentPower += amount;
            updateNodeActivation();
        }

        void resetPower()
        {
            currentPower = 0;
            updateNodeActivation();
        }
        
        void updateNodeActivation() {
            int remainingPower = currentPower; // Start with the total current power

            for (auto& node : nodes) {
                if (remainingPower >= node.powerRequired) {
                    node.isActive = true;
                    remainingPower -= node.powerRequired; // Subtract the power used
                } else {
                    node.isActive = false;
                }
            }
        }
    };

    // Gem colors for ImGui
    const ImVec4 gemColors[6] = {
        ImVec4(0.9f, 0.2f, 0.2f, 1.0f), // RED
        ImVec4(0.6f, 0.2f, 0.8f, 1.0f), // PURPLE
        ImVec4(0.9f, 0.9f, 0.2f, 1.0f), // YELLOW
        ImVec4(0.2f, 0.8f, 0.2f, 1.0f), // GREEN
        ImVec4(0.2f, 0.4f, 0.9f, 1.0f), // BLUE
        ImVec4(1.0f, 1.0f, 1.0f, 0.8f)  // WHITE
    };
    
    // Gem type names
    const char* gemNames[6] = {
        "Ruby", "Amethyst", "Topaz", "Emerald", "Sapphire", "Diamond"
    };
    
    NodeLayer();
    ~NodeLayer() override;

    unordered_map<enums::EGemType, Track> tracks;
    unordered_map<int, int> NodePowerRequirements =
        {
        {0, 1},
        {1, 120},
        {2, 240},
        {3, 480},
        {4, 720},
        {5, 960},
        {6, 1200},
        {7, 1560},
        {8, 1920},
        {9, 2280},
        {10, 2640},
        {11, 3000},
        {12, 3360}
        };
    
    void addGemPower(enums::EGemType type, float amount);
    bool isNodeActive(enums::EGemType type, int nodeIndex);
    int getActiveNodes(enums::EGemType type);
    void UpdateNodes(vector<tuple<enums::EGemSize, enums::EGemType, float, std::string>>& assignedPieces);
    
    void ResetTracks();
    int GetRequiredPowerToNextNode(int index) const;

    const std::unordered_map<enums::EGemType, Track>& getTracks();

    void showNodeLayer();

    virtual void OnUIRender() override;

};
