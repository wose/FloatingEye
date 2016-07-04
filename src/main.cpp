#include "IrrIMGUI/IrrIMGUI.h"
#include "IrrIMGUI/IncludeIrrlicht.h"

using namespace irr;
using namespace IrrIMGUI;

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

    scene::ISceneManager * const pSceneManager = pDevice->getSceneManager();
    video::IVideoDriver * const pDriver = pDevice->getVideoDriver();

    auto worldImage = pDriver->createImageFromFile("GRAY_50M_SR_W.png");
    auto worldTex = pGUI->createTexture(worldImage);
    worldImage->drop();

    // add camera to the scene
    pSceneManager->addCameraSceneNode(0, core::vector3df(0, 0, 0), core::vector3df(0,0,5));

    // Start main loop
    while(pDevice->run())
    {
        pDriver->beginScene(true, true, irr::video::SColor(255,100,101,140));

        // create the GUI elements
        pGUI->startGUI();
        ImGui::Begin("FloatingEye");

        ImVec2 worldMapStart = ImGui::GetCursorScreenPos();
        ImGui::Image(worldTex, ImVec2(1080.0f, 540.0f));

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        const int pixelPer10Deg = 15;

        // draw grid X
        for(int position = pixelPer10Deg; position < 1080; position += pixelPer10Deg) {
            draw_list->AddLine(ImVec2(worldMapStart.x + position, worldMapStart.y),
                           ImVec2(worldMapStart.x + position, worldMapStart.y + 540),
                           ImColor(180, 180, 180, 80), 1.0f);
        }

        for(int position = pixelPer10Deg; position < 540; position += pixelPer10Deg) {
            draw_list->AddLine(ImVec2(worldMapStart.x, worldMapStart.y + position),
                           ImVec2(worldMapStart.x + 1080, worldMapStart.y + position),
                           ImColor(180, 180, 180, 80), 1.0f);
        }

        ImGui::Separator();
        if (ImGui::Button("Exit", ImVec2(40, 20)))
        {
            pDevice->closeDevice();
        }
        ImGui::End();

        // render your scene
        pSceneManager->drawAll();

        // render the GUI
        pGUI->drawAll();

        pDriver->endScene();
    }

    // free up memory
    pGUI->deleteTexture(worldTex);
    pGUI->drop();
    pDevice->drop();
}
