#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include "BaseLayer.h"
#include "../enums.h"
#include "Walnut/Layer.h"

using namespace ImGui;

class GemLayer  : public BaseLayer
{
public:
    struct FGemAmounts
    {
        int smallAmount;
        int largeAmount;
        int splendidAmount;
        int flawlessAmount;
    };
    
    std::vector<std::tuple<enums::EGemType, int, int, int, int>> Gems = {
        std::make_tuple(enums::EGemType::ruby, 0, 0, 0, 0),
        std::make_tuple(enums::EGemType::amethyst, 0, 0, 0, 0),
        std::make_tuple(enums::EGemType::topaz, 0, 0, 0, 0),
        std::make_tuple(enums::EGemType::emerald, 0, 0, 0, 0),
        std::make_tuple(enums::EGemType::sapphire, 0, 0, 0, 0),
        std::make_tuple(enums::EGemType::diamond, 0, 0, 0, 0),
    };

    std::map<enums::EGemType, FGemAmounts> gemTotals;
    FGemAmounts totalGemSizes;
    int CurrentGems;

    int GetCurrentGems(std::vector<std::tuple<enums::EGemType, int, int, int, int>>& Gems) const;
    void ResetGems(std::vector<std::tuple<enums::EGemType, int, int, int, int>>& Gems);

    GemLayer();
    ~GemLayer() override;
    void OnUIRender() override;
    
    std::vector<std::tuple<enums::EGemType, int, int, int, int>> GetTotalGemAmounts();
    
    std::string GetGemSizeTypeName(enums::EGemSize Size);
    std::string GetGemTypeName(enums::EGemType Type);
    
    std::string GetGemType(int Index);
    
    bool SetTotalGemPower();
    
    void DrawInventoryWindow();
    
    int GetTotalGemPower();

};
