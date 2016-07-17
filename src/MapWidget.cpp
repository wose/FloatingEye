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
    updateTerminator();

    ImGui::Begin(name_.c_str(), NULL,
                 ImGuiWindowFlags_NoTitleBar
                 | ImGuiWindowFlags_NoResize
                 | ImGuiWindowFlags_NoMove
                 | ImGuiWindowFlags_NoBringToFrontOnFocus
                 | ImGuiWindowFlags_NoScrollbar);

    ImVec2 worldMapStart = ImGui::GetCursorScreenPos();

    ImVec2 region = ImGui::GetContentRegionAvail();
    ImGui::Image(mapTexture_, ImVec2(region.x, region.y), ImVec2(0,0), ImVec2(1,1),
                 ImVec4(0.25f, 0.75f, 0.75f, 1.00f), ImVec4(0.25f, 0.75f, 0.75f, 1.00f));

    drawGrid(worldMapStart, region);
    drawTerminator(worldMapStart, region);

    ImGui::End();
}

void MapWidget::drawGrid(const ImVec2& contentLT, const ImVec2& contentSize)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const double pixelPerDegX = (contentSize.x / textureWidth_) * textureWidth_ / 360.0;
    const double pixelPerDegY = (contentSize.y / textureHeight_) * textureHeight_ / 180.0;

    for(double position = pixelPerDegX * 10; position < contentSize.x; position += pixelPerDegX * 10) {
        draw_list->AddLine(ImVec2(contentLT.x + position, contentLT.y),
                           ImVec2(contentLT.x + position, contentLT.y + contentSize.y),
                           ImColor(180, 180, 180, 80), 1.0f);
    }

    for(double position = pixelPerDegY * 10; position < contentSize.y; position += pixelPerDegY * 10) {
        draw_list->AddLine(ImVec2(contentLT.x, contentLT.y + position),
                           ImVec2(contentLT.x + contentSize.x, contentLT.y + position),
                           ImColor(180, 180, 180, 80), 1.0f);
    }
}

void MapWidget::drawTerminator(const ImVec2& contentLT, const ImVec2& contentSize)
{
    auto draw_list = ImGui::GetWindowDrawList();
    const double pixelPerDegX = (contentSize.x / textureWidth_) * textureWidth_ / 360.0;
    const double pixelPerDegY = (contentSize.y / textureHeight_) * textureHeight_ / 180.0;
    double base = contentLT.y + contentSize.y;
    draw_list->PathClear();

    double lastX;
    bool first = true;
    for(const auto& position : terminatorPoints_) {
        draw_list->PathLineTo(ImVec2(contentLT.x + contentSize.x / 2 + position.x * pixelPerDegX,
                                     contentLT.y + contentSize.y / 2 - position.y * pixelPerDegY));

        if(first) {
            lastX = position.x;
        }

        if((position.x - lastX) * pixelPerDegX >= 3) {
            // since we can only draw convex polygons we need to split the curve in quads
            if(!first) {
                draw_list->PathLineTo(ImVec2(contentLT.x + contentSize.x / 2 + position.x * pixelPerDegX, base));
                draw_list->PathLineTo(ImVec2(contentLT.x + contentSize.x / 2 + lastX * pixelPerDegX, base));
                draw_list->PathFill(ImColor(0, 0, 0, 80));
                draw_list->PathClear();
                draw_list->PathLineTo(ImVec2(contentLT.x + contentSize.x / 2 + position.x * pixelPerDegX,
                                             contentLT.y + contentSize.y / 2 - position.y * pixelPerDegY));
            }
            lastX = position.x;
        }
        first = false;
    }
}

/*
  based on a matlab script from Mattia Rossi:
  http://caia.swin.edu.au/mapping/gametraffic/anim100309A/plotdaynightterminator.m
*/
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

    for(double lon = -180; lon <= 180; ++lon) {
        double lat = atan(cos((lon + tau) * K) / tan(dec * K)) / K;
        terminatorPoints_.push_back(ImVec2(lon, lat));
    }
}
