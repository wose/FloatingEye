#include "EmptyWidget.h"
#include "Grid.h"
#include "VideoStream.h"

#include "IrrIMGUI/IrrIMGUI.h"
#include "IrrIMGUI/IncludeIrrlicht.h"

#include <ctime>

using namespace irr;
using namespace IrrIMGUI;

void calculateTerminator(ImVec2& worldMapStart);

int main (int argc, char *argv[]) {
    // Irrlicht OpenGL Settings
    SIrrlichtCreationParameters IrrlichtParams;
    IrrlichtParams.DriverType    = video::EDT_OPENGL;
    IrrlichtParams.WindowSize    = core::dimension2d<u32>(1024, 800);
    IrrlichtParams.Bits          = 32;
    IrrlichtParams.Fullscreen    = false;
    IrrlichtParams.Stencilbuffer = true;
    IrrlichtParams.AntiAlias     = 16;
    IrrlichtParams.Vsync         = false;

    // Create standard event receiver for the IrrIMGUI
    CIMGUIEventReceiver EventReceiver;
    IrrlichtParams.EventReceiver = &EventReceiver;

    // Create Irrlicht device
    IrrlichtDevice * const pDevice = createDeviceEx(IrrlichtParams);

    // Create GUI object
    IIMGUIHandle * const pGUI = createIMGUI(pDevice, &EventReceiver);

    auto pSceneManager = pDevice->getSceneManager();
    auto pDriver = pDevice->getVideoDriver();

    //VideoStream videoStream("", pDriver, pGUI);

    // add camera to the scene
    pSceneManager->addCameraSceneNode(0, core::vector3df(0, 0, 0), core::vector3df(0,0,5));

    Grid layout("ISS", pDriver, pGUI, 12, 5);

    // Start main loop
    while(pDevice->run())
    {
        pDriver->beginScene(true, true, irr::video::SColor(255, 80, 80, 80));

        // create the GUI elements
        pGUI->startGUI();
        layout.draw();

        // render your scene
        pSceneManager->drawAll();

        // render the GUI
        pGUI->drawAll();

        pDriver->endScene();
    }

    // free up memory
    pGUI->drop();
    pDevice->drop();
}
