#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include "BaseLayer.h"
#include "../enums.h"
#include "imgui.h"

using namespace ImGui;
using namespace std;

class GearLayer : public BaseLayer
{
public:
    GearLayer();
    ~GearLayer() override;

    virtual void OnUIRender() override;
    
    enum EColumnType
    {
        Socket = 0,
        Size = 1,
        Color = 2,
        COLUMN_ENUM_LAST = 3
    };
    enum EGearSlotType
    {
        Head,
        Shoulders,
        Cape,
        Chest,
        Hands,
        Legs,
        Feets,
        Necklace,
        Wrist,
        RingFirst,
        RingSecond
    };

    struct FPopupState
    {
        enums::ESocketType addPercentage = enums::NoSocket;
        enums::EGemSize size = enums::small;
        enums::EGemType color = enums::EGemType::ruby;
    };

    std::vector<std::tuple<std::string, enums::ESocketType>> GearPieces = {
        std::make_tuple("Head", enums::NoSocket),
        std::make_tuple("Shoulder", enums::NoSocket),
        std::make_tuple("Cape", enums::NoSocket),
        std::make_tuple("Chest", enums::NoSocket),
        std::make_tuple("Hands", enums::NoSocket),
        std::make_tuple("Legs", enums::NoSocket),
        std::make_tuple("Feet", enums::NoSocket),
        std::make_tuple("Necklace", enums::NoSocket),
        std::make_tuple("Wrist", enums::NoSocket),
        std::make_tuple("RingFirst", enums::NoSocket),
        std::make_tuple("RingSecond", enums::NoSocket)
    };

    std::vector<std::tuple<std::string, enums::ESocketType>> GetGearPieces();
    
    void EquipmentStats();
    int GetTotalGemPower();
    
    void SetAllGearToSocket(std::vector<std::tuple<std::string, enums::ESocketType>>& GearPieces,
                            enums::ESocketType NewSocket);

    std::string GetSocketTypeName(enums::ESocketType Socket);
    std::string GetSizeTypeName(enums::EGemSize Socket);
    std::string GetColorTypeName(enums::EGemType Socket);

    void DrawButton(EColumnType type, std::string& Gear, enums::ESocketType& Socket, int index);

    FPopupState GemPowerWithEquipmentBonus_Popup;
};




