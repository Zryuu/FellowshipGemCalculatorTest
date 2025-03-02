#include "CalculateLayer.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>


CalculateLayer::CalculateLayer(shared_ptr<GearLayer> gearLayer, std::shared_ptr<GemLayer> gemLayer,
                               std::shared_ptr<NodeLayer> nodeLayer) : _gearLayer(gearLayer), _gemLayer(gemLayer), _nodeLayer(nodeLayer) {}

CalculateLayer::~CalculateLayer()
{
}

void CalculateLayer::ShowCalculateLayer()
{
    Begin("##CalculateLayer", &showLayer, ImGuiWindowFlags_NoTitleBar);

    
    Dummy(ImVec2((GetWindowSize().x / 2) - 50, 0));
    SameLine();
    if (Button("Calculate"))
    {
        StartCalculation();
        calculate = true;
    }

    if (calculate)
    {
        int index = 0;
        // First, determine the maximum width needed for each column
        size_t maxGearNameWidth = 0;
        size_t maxGemTypeWidth = 0;

        for (const auto& piece : assignedPieces) {
            enums::EGemSize gemSize = std::get<0>(piece);
            enums::EGemType gemType = std::get<1>(piece);
            std::string gearName = std::get<3>(piece);
    
            std::string gemTypeName = _gemLayer->GetGemTypeName(gemType);
    
            maxGearNameWidth = std::max(maxGearNameWidth, gearName.length());
            maxGemTypeWidth = std::max(maxGemTypeWidth, gemTypeName.length());
        }

        // Now print with consistent spacing
        for (const auto& piece : assignedPieces) {
            enums::EGemSize gemSize = std::get<0>(piece);
            enums::EGemType gemType = std::get<1>(piece);
            std::string gearName = std::get<3>(piece);
    
            std::string gemTypeName = _gemLayer->GetGemTypeName(gemType);
            std::string gemSizeName = _gemLayer->GetGemSizeTypeName(gemSize);
    
            // Use std::setw to create columns of fixed width
            std::ostringstream oss;
            oss << std::left << std::setw(3) << index << ": " 
                << std::setw(maxGearNameWidth + 3) << (gearName + ", ")
                << std::setw(maxGemTypeWidth + 3) << (gemTypeName + ", ")
                << gemSizeName;
    
            Text("%s", oss.str().c_str());
    
            index++;
        }
    }
    
    End();

}

std::vector<std::tuple<std::string, enums::ESocketType>> CalculateLayer::
GetGearPieces()
{
    
    return _gearLayer->GetGearPieces();
}

std::vector<std::tuple<enums::EGemType, int, int, int, int>> CalculateLayer::GetGemPieces()
{
    return _gemLayer->GetTotalGemAmounts();
}


void CalculateLayer::SortGearPiecesByAddPerDecendingOrder(
    std::vector<std::tuple<std::string, enums::ESocketType>>& InGear)
{
    std::sort(InGear.begin(), InGear.end(), [](const std::tuple<std::string, enums::ESocketType> &a, const std::tuple<std::string, enums::ESocketType> &b)
    {
        return std::get<1>(a) > std::get<1>(b);
    });
}

void CalculateLayer::assignPieces()
{
    //  Copy of Gems so we can sort
    auto AvailableGems = Gems;
    
        for (const auto& gear : SortedGearPieces) {
        std::string gearName = std::get<0>(gear);
        enums::ESocketType SocketType = std::get<1>(gear);

        if (SocketType == enums::NoSocket) {
            continue; 
        }

        float addPer = GetAddPerBySocket(SocketType);
        
        bool gemAssigned = false;
        
        // Trying to assign by gemsize first. Largest > smallest
        
        //  Flawless
        for (auto& gem : AvailableGems) {
            enums::EGemType gemType = std::get<0>(gem);
            int& flawlessAmount = std::get<4>(gem);
            
            if (flawlessAmount > 0) {
                assignedPieces.push_back({enums::EGemSize::flawless, gemType, addPer, gearName});
                flawlessAmount--;
                gemAssigned = true;
                break;
            }
        }
        
        if (gemAssigned) continue;
        
        // Splendid
        for (auto& gem : AvailableGems) {
            enums::EGemType gemType = std::get<0>(gem);
            int& splendidAmount = std::get<3>(gem);
            
            if (splendidAmount > 0) {
                assignedPieces.push_back({enums::EGemSize::splendid, gemType, addPer, gearName});
                splendidAmount--;
                gemAssigned = true;
                break;
            }
        }
        
        if (gemAssigned) continue;
        
        // Large
        for (auto& gem : AvailableGems) {
            enums::EGemType gemType = std::get<0>(gem);
            int& largeAmount = std::get<2>(gem);
            
            if (largeAmount > 0) {
                assignedPieces.push_back({enums::EGemSize::large, gemType, addPer, gearName});
                largeAmount--;
                gemAssigned = true;
                break;
            }
        }
        
        if (gemAssigned) continue;
        
        // Small
        for (auto& gem : AvailableGems) {
            enums::EGemType gemType = std::get<0>(gem);
            int& smallAmount = std::get<1>(gem);
            
            if (smallAmount > 0) {
                assignedPieces.push_back({enums::EGemSize::small, gemType, addPer, gearName});
                smallAmount--;
                gemAssigned = true;
                break;
            }
        }
        
        // If no gem was assigned for this gear piece, we've run out of gems
        if (!gemAssigned) {
            break;
        }
    }
}

void CalculateLayer::StartCalculation()
{
    assignedPieces.clear();
    _nodeLayer->ResetTracks();
    
    SortedGearPieces = GetGearPieces();
    SortGearPiecesByAddPerDecendingOrder(SortedGearPieces);

    Gems = GetGemPieces();

    assignPieces();

    _nodeLayer->UpdateNodes(assignedPieces);
    
    //  Sorts assignedPieces for Printing.
    if (!assignedPieces.empty())
    {
        std::sort(assignedPieces.begin(), assignedPieces.end(), [&](const auto& a, const auto& b) {
        std::string gearNameA = std::get<3>(a); //  Gear name A
        std::string gearNameB = std::get<3>(b); //  Gear name B

        int indexA = (GearOrder.find(gearNameA) != GearOrder.end()) ? GearOrder[gearNameA] : INT_MAX;
        int indexB = (GearOrder.find(gearNameB) != GearOrder.end()) ? GearOrder[gearNameB] : INT_MAX;

        return indexA < indexB;
    });
    }
}

void CalculateLayer::OnUIRender()
{
    BaseLayer::OnUIRender();
    
    if (showLayer)
    {
        ShowCalculateLayer();
    }
}

float CalculateLayer::GetAddPerBySocket(enums::ESocketType& Type)
{
    switch (Type)
    {
    case enums::NoSocket: return 0.0f;
    case enums::NoAdd: return 1.0f;
    case enums::Percentage25:  return 1.25f;
    case enums::Percentage66:  return 1.66f;
    }
    return 100.0f;
}
