#pragma once
#include "Walnut/Layer.h"

using namespace std;
using namespace ImGui;

class BaseLayer : public Walnut::Layer
{
public:
    BaseLayer();
    ~BaseLayer();
    void OnUIRender() override;

    void ResetLayerPosition();

    bool showLayer = true;

    void SetupDockSpace();
    
};
