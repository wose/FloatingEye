#ifndef FLOATING_EYE_BODYWIDGET_H
#define FLOATING_EYE_BODYWIDGET_H

#include "IWidget.h"

#include "IrrIMGUI/IrrIMGUI.h"

#include <string>
#include <vector>

namespace irr {
class IrrlichtDevice;
}

namespace IrrIMGUI {
class IGUITexture;
class IIMGUIHandle;
}

class BodyWidget :
    public IWidget
{
public:
    BodyWidget(const std::string& name, irr::IrrlichtDevice* const irrDevice,
               IrrIMGUI::IIMGUIHandle* const imGuiHandle);
    virtual ~BodyWidget();

    virtual void render();
    virtual void draw() override;

private:
    bool showTexture_ = true;
    bool showPointCloud_ = false;

    IrrIMGUI::IIMGUIHandle* imGuiHandle_;
    IrrIMGUI::IGUITexture* renderTargetID_;
    irr::IrrlichtDevice* irrDevice_;

    irr::scene::ICameraSceneNode* cameraNode_;
    irr::scene::IMeshSceneNode* meshNode_;
    irr::scene::ISceneNode* sceneNode_;
    irr::video::ITexture* meshTexture_;
    irr::video::ITexture* renderTarget_;
    irr::video::ITexture* edgeTexture_;
    irr::video::SMaterial markerMaterial_;

    std::vector<irr::scene::IMeshBuffer*> meshBuffers_;
    std::string name_;
};

#endif // FLOATING_EYE_BODYWIDGET_H
