#include "BodyWidget.h"
#include "EmptyWidget.h"
#include "Grid.h"
#include "VideoStream.h"

#include "IrrIMGUI/IrrIMGUI.h"
#include "IrrIMGUI/IncludeIrrlicht.h"

#include <ctime>

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

    auto pSceneManager = pDevice->getSceneManager();
    auto pDriver = pDevice->getVideoDriver();

    //VideoStream videoStream("", pDriver, pGUI);

    scene::ISceneNode * const pMainScreen = pSceneManager->addEmptySceneNode();

    //    auto phobosTexture = pDriver->getTexture("res/Phobos.jpg");
    //    pMoon->setMaterialTexture(0, phobosTexture);
    //    pMoon->setMaterialFlag(video::EMF_LIGHTING, false);

    // Add camera object
    const auto pMainCam = pSceneManager->addCameraSceneNode(pMainScreen,
                                                            core::vector3df(0, 0, 0),
                                                            core::vector3df(0, 0, 5));
    //   auto filledPolygonTexture = pDriver->addTexture(core::dimension2d<u32>(1,1),
    //                                               "filledPolygonColor",
    //                                               video::ECF_A8R8G8B8);
   //   p = (s32*)filledPolygonTexture->lock();
   //   p[0] = video::SColor(58, 0, 255, 255).color;
   //   filledPolygonTexture->unlock();

   //   irr::video::SMaterial mat2;
   //   mat2.Lighting = false;
   //   mat2.setTexture(0, filledPolygonTexture);
   //   mat2.setFlag(video::EMF_COLOR_MASK, true);
   //   mat2.BlendOperation = video::EBO_ADD;
   //   mat.setFlag(video::EMF_WIREFRAME, true);
   // add camera to the scene
   //    pSceneManager->addCameraSceneNode(0, core::vector3df(0, 0, 0), core::vector3df(0,0,5));

    {
        BodyWidget body("TestBody", pDevice, pGUI);
        Grid layout("ISS", pDriver, pGUI, 12, 5);

        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Text]                  = ImVec4(0.83f, 0.95f, 0.95f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.39f, 0.80f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        style.Colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
        style.Colors[ImGuiCol_Border]                = ImVec4(0.25f, 0.75f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.18f, 0.55f, 0.55f, 0.67f);
        style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.25f, 0.75f, 0.75f, 0.67f);
        style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.16f, 0.49f, 0.49f, 0.45f);
        style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.16f, 0.49f, 0.49f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.16f, 0.49f, 0.49f, 0.60f);
        style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.18f, 0.56f, 0.56f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.16f, 0.47f, 0.47f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.11f, 0.33f, 0.33f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.14f, 0.42f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.33f, 0.78f, 0.78f, 0.67f);
        style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.33f, 0.78f, 0.78f, 1.00f);
        style.Colors[ImGuiCol_ComboBg]               = ImVec4(0.18f, 0.55f, 0.55f, 0.99f);
        style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.44f, 0.81f, 0.81f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.33f, 0.78f, 0.78f, 0.60f);
        style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.33f, 0.78f, 0.78f, 1.00f);
        style.Colors[ImGuiCol_Button]                = ImVec4(0.18f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.33f, 0.78f, 0.78f, 0.67f);
        style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.33f, 0.78f, 0.78f, 1.00f);
        style.Colors[ImGuiCol_Header]                = ImVec4(0.15f, 0.44f, 0.44f, 1.00f);
        style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.33f, 0.78f, 0.78f, 0.67f);
        style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.33f, 0.78f, 0.78f, 1.00f);
        style.Colors[ImGuiCol_Column]                = ImVec4(0.25f, 0.74f, 0.74f, 0.40f);
        style.Colors[ImGuiCol_ColumnHovered]         = ImVec4(0.25f, 0.74f, 0.74f, 0.60f);
        style.Colors[ImGuiCol_ColumnActive]          = ImVec4(0.25f, 0.74f, 0.74f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.20f, 0.61f, 0.61f, 1.00f);
        style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.53f, 0.84f, 0.84f, 0.67f);
        style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.53f, 0.84f, 0.84f, 1.00f);
        style.Colors[ImGuiCol_CloseButton]           = ImVec4(0.22f, 0.67f, 0.67f, 1.00f);
        style.Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(0.53f, 0.84f, 0.84f, 0.40f);
        style.Colors[ImGuiCol_CloseButtonActive]     = ImVec4(0.53f, 0.84f, 0.84f, 1.00f);
        style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.53f, 0.84f, 0.84f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.00f, 0.84f, 0.84f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.53f, 0.84f, 0.84f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.00f, 0.84f, 0.84f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.13f, 0.40f, 0.40f, 1.00f);
        style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.09f, 0.27f, 0.27f, 0.67f);

        // Start main loop
        while(pDevice->run())
        {
            pDriver->beginScene(true, true, irr::video::SColor(255, 80, 80, 80));
            pMainScreen->setVisible(false);

            body.render();

            pDriver->setRenderTarget(0, true, true, video::SColor(255, 80, 80, 80));
            pMainScreen->setVisible(true);
            pSceneManager->setActiveCamera(pMainCam);

            // create the GUI elements
            pGUI->startGUI();
            layout.draw();

            body.draw();
            auto mousePosition = ImGui::GetIO().MousePos;
            if(mousePosition.y < 20) {
                if(ImGui::BeginMainMenuBar()) {
                    if(ImGui::Button("Exit")) {
                        pDevice->closeDevice();
                    }
                    // ImGui::SameLine();
                    ImGui::EndMainMenuBar();
                }
            }

            pSceneManager->drawAll();
            pGUI->drawAll();

            pDriver->endScene();
        }
    }
    // free up memory
    pGUI->drop();
    pDevice->drop();
}
