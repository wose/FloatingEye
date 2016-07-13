#ifndef FLOATING_EYE_MAPWIDGET_H
#define FLOATING_EYE_MAPWIDGET_H

#include "IWidget.h"

#include <string>
#include <vector>

namespace irr { namespace video {
class IVideoDriver;
}
}

namespace IrrIMGUI {
class IGUITexture;
class IIMGUIHandle;
}
class ImVec2;

class MapWidget :
    public IWidget
{
public:
    MapWidget(const std::string& name, irr::video::IVideoDriver* const irrDriver,
              IrrIMGUI::IIMGUIHandle* const imGuiHandle);
    virtual ~MapWidget();

    virtual void draw() override;

private:
    void drawGrid();
    void drawTerminator(const ImVec2& worldMapStart);
    void updateTerminator();

    int textureWidth_;
    int textureHeight_;
    IrrIMGUI::IIMGUIHandle* const imGuiHandle_;
    irr::video::IVideoDriver* const irrDriver_;
    IrrIMGUI::IGUITexture* mapTexture_;
    std::string name_;
    std::vector<ImVec2> terminatorPoints_;
};

#endif // FLOATING_EYE_MAPWIDGET_H
