#pragma once
#include <memory>
#include <stack>

#include "BaseLayer.h"
#include "GearLayer.h"
#include "GemLayer.h"
#include "NodeLayer.h"

class CalculateLayer : public BaseLayer
{
public:

    stack<int> gemStack;
    
    CalculateLayer(shared_ptr<GearLayer> gearLayer, std::shared_ptr<GemLayer> gemLayer, std::shared_ptr<NodeLayer> 
    nodeLayer);
    ~CalculateLayer() override;

    void ShowCalculateLayer();

    std::unordered_map<std::string, int> GearOrder =
    {
        {"Head", 0},
        {"Shoulder", 1},
        {"Cape", 2},
        {"Chest", 3},
        {"Hands", 4},
        {"Legs", 5},
        {"Feet", 6},
        {"Necklace", 7},
        {"Wrist", 8},
        {"RingFirst", 9},
        {"RingSecond", 10}
        
    };
    
    std::vector<std::tuple<std::string, enums::ESocketType>> SortedGearPieces;
    std::vector<std::tuple<enums::EGemType, int, int, int, int>> Gems = {
        std::make_tuple(enums::EGemType::ruby, 0, 0, 0, 0),
        std::make_tuple(enums::EGemType::amethyst, 0, 0, 0, 0),
        std::make_tuple(enums::EGemType::topaz, 0, 0, 0, 0),
        std::make_tuple(enums::EGemType::emerald, 0, 0, 0, 0),
        std::make_tuple(enums::EGemType::sapphire, 0, 0, 0, 0),
        std::make_tuple(enums::EGemType::diamond, 0, 0, 0, 0),
    };
    
    std::vector<std::tuple<std::string, enums::ESocketType>> GetGearPieces();
    std::vector<std::tuple<enums::EGemType, int, int, int, int>> GetGemPieces();

    vector<tuple<enums::EGemSize, enums::EGemType, float, string>> assignedPieces;

    void SortGearPiecesByAddPerDecendingOrder(std::vector<std::tuple<std::string, enums::ESocketType>>& InGear);
    
    void assignPieces();
    
    void StartCalculation();

    void OnUIRender() override;

    bool calculate = false;

    float GetAddPerBySocket(enums::ESocketType& Type);
    
private:
    std::shared_ptr<GearLayer> _gearLayer;
    std::shared_ptr<GemLayer> _gemLayer;
    std::shared_ptr<NodeLayer> _nodeLayer;
};


