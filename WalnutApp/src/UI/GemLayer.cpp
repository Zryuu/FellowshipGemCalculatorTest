#include "GemLayer.h"

int GemLayer::GetCurrentGems(std::vector<std::tuple<enums::EGemType, int, int, int, int>>& Gems) const
{
    int total = 0;
    for (auto& [Type, sm, l, sp, f] : Gems)
    {
        total += sm + l + sp + f;
    }

    return total;
}

void GemLayer::ResetGems(std::vector<std::tuple<enums::EGemType, int, int, int, int>>& Gems)
{
    for (auto& Gem : Gems)
    {
        get<1>(Gem) = 0;
        get<2>(Gem) = 0;
        get<3>(Gem) = 0;
        get<4>(Gem) = 0;
    }
}

GemLayer::GemLayer()
{
}

GemLayer::~GemLayer()
{
    totalGemSizes = {};
    gemTotals = {};
}

std::string GemLayer::GetGemType(int Index)
{
    switch (Index)
    {
        case 0: return "Ruby";
        case 1: return "Amethyst";
        case 2: return "Topaz";
        case 3: return "Emerald";
        case 4: return "Sapphire";
        case 5: return "Diamond";
        default: return "shit";
    }
}

void GemLayer::DrawInventoryWindow()
{
    
    Begin("Gems", &showLayer, ImGuiWindowFlags_AlwaysAutoResize);

    if (BeginTable("Gem Inventory", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable))
    {
        TableSetupColumn("Gems",     ImGuiTableColumnFlags_WidthFixed, 100.0f);
        TableSetupColumn("Small",    ImGuiTableColumnFlags_WidthFixed, 100.0f);
        TableSetupColumn("Large",    ImGuiTableColumnFlags_WidthFixed, 100.0f);
        TableSetupColumn("Splendid", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        TableSetupColumn("Flawless", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        TableHeadersRow();
        
        for (auto& [Type, small, large, splendid, flawless] : Gems)
        {
            TableNextRow();

            TableNextColumn();
            Text(GetGemTypeName(Type).c_str());
            
            // Small Amount:
            TableNextColumn();
            
            InputInt(("##small" + GetGemTypeName(Type)).c_str(), &small, 0);

            if (IsItemDeactivatedAfterEdit())
            {
                SetTotalGemPower();
            }
            
            TableNextColumn();
            InputInt(("##large" + GetGemTypeName(Type)).c_str(), &large, 0);

            if (IsItemDeactivatedAfterEdit())
            {
                SetTotalGemPower();
            }
            
            TableNextColumn();
            InputInt(("##splendid" + GetGemTypeName(Type)).c_str(), &splendid, 0);

            if (IsItemDeactivatedAfterEdit())
            {
                SetTotalGemPower();
            }
            
            TableNextColumn();
            InputInt(("##flawless" + GetGemTypeName(Type)).c_str(), &flawless, 0);
            
        }
        
        EndTable();
    }

    if (IsItemHovered() && IsKeyDown(ImGuiKey_LeftShift))
    {
        BeginTooltip();
        Text("Gem Amounts:\nSmall:      120 | 150 | 200*\nLarge:      240 | 300 | 400*\nSplendid: 360 | 450 | 600*\nFlawless: 480 | 600 | 800*");
        EndTooltip();
    }
		
    
    Text("Total: " "%d", GetCurrentGems(Gems));
    Indent();
    SameLine();
    if (Button("Reset Gems"))
    {
        ResetGems(Gems);
    }
    
    End();
}

bool GemLayer::SetTotalGemPower()
{
    gemTotals.clear();
    totalGemSizes = FGemAmounts();

    for (const auto& [type, small, large, splendid, flawless] : Gems) {
        gemTotals[type].smallAmount += small;
        gemTotals[type].largeAmount += large;
        gemTotals[type].splendidAmount += splendid;
        gemTotals[type].flawlessAmount += flawless;

        totalGemSizes.smallAmount += small;
        totalGemSizes.largeAmount += large;
        totalGemSizes.splendidAmount += splendid;
        totalGemSizes.flawlessAmount += flawless;
    }

    return true;
}

int GemLayer::GetTotalGemPower()
{
    float total = 0.0f;
    for (auto& [Type, Amounts] : gemTotals)
    {
        total += Amounts.smallAmount * 120;
        total += Amounts.largeAmount * 240;
        total += Amounts.splendidAmount * 360;
        total += Amounts.flawlessAmount * 480;
    }
    return std::floor(total);
}

std::string GemLayer::GetGemTypeName(enums::EGemType Type)
{
    switch (Type)
    {
    case enums::EGemType::ruby: return "Ruby";
    case enums::EGemType::amethyst: return "Amethyst";
    case enums::EGemType::topaz: return "Topaz";
    case enums::EGemType::emerald: return "Emerald";
    case enums::EGemType::sapphire: return "Sapphire";
    case enums::EGemType::diamond: return "Diamond";
    }
}

std::string GemLayer::GetGemSizeTypeName(enums::EGemSize Size)
{
    switch (Size)
    {
    case enums::EGemSize::small: return "Small";
    case enums::EGemSize::large: return "Large";
    case enums::EGemSize::splendid: return "Splendid";
    case enums::EGemSize::flawless: return "Flawless";
    }
}


void GemLayer::OnUIRender()
{
    BaseLayer::OnUIRender();
    
    if (showLayer)
    {
        DrawInventoryWindow();
    }
}

std::vector<std::tuple<enums::EGemType, int, int, int, int>> GemLayer::GetTotalGemAmounts()
{
    return Gems;
}
