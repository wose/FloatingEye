#include "EmptyWidget.h"

#include "IrrIMGUI/IrrIMGUI.h"

EmptyWidget::EmptyWidget(const std::string& name) :
    name_{name}
{
}

void EmptyWidget::draw()
{
    ImGui::Begin(name_.c_str(), NULL,
                 ImGuiWindowFlags_NoTitleBar
                 | ImGuiWindowFlags_NoResize
                 | ImGuiWindowFlags_NoMove
                 | ImGuiWindowFlags_NoBringToFrontOnFocus
                 | ImGuiWindowFlags_NoScrollbar);

    ImGui::TextUnformatted("Empty");
    ImGui::End();
}
