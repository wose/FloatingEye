#include "MapWidget.h"

#include "IrrIMGUI/IrrIMGUI.h"

MapWidget::MapWidget(const std::string& name, irr::video::IVideoDriver* const irrDriver,
                     IrrIMGUI::IIMGUIHandle* const imGuiHandle) :
    name_{name},
    irrDriver_{irrDriver},
    imGuiHandle_{imGuiHandle}
{
    irrDriver_->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);

    auto mapImage = irrDriver_->createImageFromFile("GRAY_50M_SR_W.png");
    auto size = mapImage->getDimension();
    textureWidth_ = size.Width;
    textureHeight_ = size.Height;
    mapTexture_ = imGuiHandle->createTexture(mapImage);
    mapImage->drop();

    updateTerminator();
}

MapWidget::~MapWidget()
{
    imGuiHandle_->deleteTexture(mapTexture_);
}

void MapWidget::draw()
{
    ImGui::Begin(name_.c_str());

    ImVec2 worldMapStart = ImGui::GetCursorScreenPos();

    ImVec2 region = ImGui::GetContentRegionMax();
    ImGui::Image(mapTexture_, ImVec2(region.x, region.y));

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const double pixelPerDeg = textureWidth_ / 360.0;

    // draw grid X
    for(double position = pixelPerDeg * 10; position < textureWidth_; position += pixelPerDeg * 10) {
        draw_list->AddLine(ImVec2(worldMapStart.x + position, worldMapStart.y),
                           ImVec2(worldMapStart.x + position, worldMapStart.y + textureHeight_),
                           ImColor(180, 180, 180, 80), 1.0f);
    }

    for(int position = pixelPerDeg * 10; position < textureHeight_; position += pixelPerDeg * 10) {
        draw_list->AddLine(ImVec2(worldMapStart.x, worldMapStart.y + position),
                           ImVec2(worldMapStart.x + 1080, worldMapStart.y + position),
                           ImColor(180, 180, 180, 80), 1.0f);
    }
}

void MapWidget::drawGrid()
{
}

void MapWidget::drawTerminator(const ImVec2& worldMapStart)
{
    auto draw_list = ImGui::GetWindowDrawList();
    double base = worldMapStart.y + textureHeight_;
    //    draw_list->PathClear();

    /*
    for(double lon = -180; lon <= 180; ++lon) {
        double lat = atan(cos((lon + tau) * K) / tan(dec * K)) / K;
        ImGui::Text("Lon: %.2f Lat: %.2f", lon, lat);
        draw_list->PathLineTo(ImVec2(worldMapStart.x + 540 + lon * 3,
                                     worldMapStart.y + 270 - lat * 3));

        // since we can only draw convex polygons we need to split the curve in quads
        if(lon > -180) {
            draw_list->PathLineTo(ImVec2(worldMapStart.x + 540 + lon * 3, base));
            draw_list->PathLineTo(ImVec2(worldMapStart.x + 540 + (lon - 1) * 3, base));
            draw_list->PathFill(ImColor(0, 0, 0, 80));
            draw_list->PathClear();
            draw_list->PathLineTo(ImVec2(worldMapStart.x + 540 + lon * 3,
                                         worldMapStart.y + 270 - lat * 3));
        }
    }
    */
}

void MapWidget::updateTerminator()
{
    terminatorPoints_.clear();

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

}
