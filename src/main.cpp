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
        pDriver->beginScene(true, true, irr::video::SColor(255, 80, 80, 80));

        // create the GUI elements
        pGUI->startGUI();
        ImGui::Begin("FloatingEye");

        ImVec2 worldMapStart = ImGui::GetCursorScreenPos();
        ImGui::Image(worldTex, ImVec2(1080.0f, 540.0f));

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        const int pixelPer10Deg = 30;

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


        calculateTerminator(worldMapStart);

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

/*
  based on a matlab script from Mattia Rossi:
  http://caia.swin.edu.au/mapping/gametraffic/anim100309A/plotdaynightterminator.m
*/
void calculateTerminator(ImVec2& worldMapStart)
{
    time_t rawtime{};
    time(&rawtime);

    auto ptm = gmtime(&rawtime);
    const double pi = 3.14159265359;

    double utcHour = ptm->tm_hour + ptm->tm_min / 60 + ptm->tm_sec / 3600;

    if(ptm->tm_mon <= 2) ptm->tm_mon += 12;

    ptm->tm_year--;

    double julianDate = (365.25 * ptm->tm_year) + (30.6001 * (ptm->tm_mon + 1))
        - 15 + 1720996.5 + ptm->tm_mday + utcHour / 24;
    double K = pi / 180;
    double T = (julianDate - 2451545.0 ) / 36525;
    double L = 280.46645 + 36000.76983 * T + 0.0003032 * T * T;
    L = fmod(L, 360);
    if(L < 0) L += 360;

    double M = 357.52910 + 35999.05030 * T - 0.0001559 * T * T - 0.00000048 * T * T * T;
    M = fmod(M, 360);
    if(M < 0) M += 360;

    double C = (1.914600 - 0.004817 * T - 0.000014 * T * T) * sin(K * M);
    C += (0.019993 - 0.000101 * T) * sin(K * 2 * M);
    C += 0.000290 * sin(K * 3 * M);
    double theta = L + C;
    double LS = L;
    double LM = 218.3165 + 481267.8813 * T;
    double eps0 =  23.0 + 26.0/60.0 + 21.448/3600.0
        - (46.8150 * T + 0.00059 * T * T - 0.001813 * T * T * T) / 3600;
    double omega = 125.04452 - 1934.136261 * T + 0.0020708 * T * T + T * T * T / 450000;
    double deltaEps = (9.20 * cos(K * omega) + 0.57 * cos(K * 2 * LS)
        + 0.10 * cos(K * 2 * LM) - 0.09 * cos(K * 2 * omega)) / 3600;
    double eps = eps0 + deltaEps + 0.00256 * cos(K * (125.04 - 1934.136 * T));
    double lambda = theta - 0.00569 - 0.00478 * sin(K * (125.04 - 1934.136 * T));
    double delta = asin(sin(K * eps) * sin(K * lambda));
    double dec = delta / K;
    double tau = utcHour * 15;

    auto draw_list = ImGui::GetWindowDrawList();
    double base = worldMapStart.y + 540;
    //    draw_list->PathClear();

    for(double lon = -180; lon <= 180; ++lon) {
        double lat = atan(cos((lon + tau) * K) / tan(dec * K)) / K;
        ImGui::Text("Lon: %.2f Lat: %.2f", lon, lat);
        draw_list->PathLineTo(ImVec2(worldMapStart.x + 540 + lon * 3,
                                     worldMapStart.y + 270 - lat * 3));

        /* since we can only draw convex polygons we need to split the curve in quads */
        if(lon > -180) {
            draw_list->PathLineTo(ImVec2(worldMapStart.x + 540 + lon * 3, base));
            draw_list->PathLineTo(ImVec2(worldMapStart.x + 540 + (lon - 1) * 3, base));
            draw_list->PathFill(ImColor(0, 0, 0, 80));
            draw_list->PathClear();
            draw_list->PathLineTo(ImVec2(worldMapStart.x + 540 + lon * 3,
                                         worldMapStart.y + 270 - lat * 3));
        }
    }
}
