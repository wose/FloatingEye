#include "EnumSensorWidget.h"
#include "EmptyWidget.h"
#include "Grid.h"
#include "IWidget.h"
#include "MapWidget.h"
#include "AnalogSensorWidget.h"
#include "IntSensorWidget.h"
#include "SensorWidget.h"

#include "IrrIMGUI/IrrIMGUI.h"

#include <memory>

Grid::Grid(const std::string& name, irr::video::IVideoDriver* const irrDriver,
           IrrIMGUI::IIMGUIHandle* const imGuiHandle, int cols /* =1 */, int rows /* = 1 */) :
    name_{name},
    cols_{cols},
    rows_{rows}
{
    // row 1
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Video 1"), 0, 0, 4, 2));
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Video 2"), 4, 0, 4, 2));
    cells_.emplace_back(std::make_unique<Cell>
                        (std::make_shared<MapWidget>("Map", irrDriver, imGuiHandle), 8, 0, 4, 2));

    // row 2
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Blueprint"), 0, 2, 8, 2));
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Stats"), 8, 2, 4, 2));

    // row 3
    auto sensorWidget = std::make_shared<SensorWidget>("Channel 1A");
    auto sensor = std::make_shared<Sensor<double>>("Voltage", "V");
    sensorWidget->addSensor(std::make_shared<AnalogSensorWidget>(iss_.c1AVoltage_));
    sensorWidget->addSensor(std::make_shared<AnalogSensorWidget>
                            (std::make_shared<Sensor<double>>("Current", "A")));
    sensorWidget->addSensor(std::make_shared<AnalogSensorWidget>
                            (std::make_shared<Sensor<double>>("Position", "deg"), SWStyle::Rotary));
    cells_.emplace_back(std::make_unique<Cell>(std::move(sensorWidget), 0, 4, 1, 1));

    sensorWidget = std::make_shared<SensorWidget>("Hi P O2 Supply");
    sensorWidget->addSensor(std::make_shared<EnumSensorWidget>
                            (std::make_shared<Sensor<int>>("State", "")));
    cells_.emplace_back(std::make_unique<Cell>(std::move(sensorWidget), 1, 4, 1, 1));
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Sensor 3"), 2, 4, 1, 1));
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Sensor 4"), 3, 4, 1, 1));
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Sensor 5"), 4, 4, 1, 1));
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Sensor 6"), 5, 4, 1, 1));
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Sensor 7"), 6, 4, 1, 1));
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Sensor 8"), 7, 4, 1, 1));
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Sensor 9"), 8, 4, 1, 1));
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Sensor 10"), 9, 4, 1, 1));
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Sensor 11"), 10, 4, 1, 1));
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Sensor 12"), 11, 4, 1, 1));
}

void Grid::draw() const
{
    for(const auto& cell : cells_)
    {
        cell->draw(*this);
    }
}

void Grid::setName(const std::string& name)
{
    name_ = name;
}

std::string Grid::getName() const
{
    return name_;
}

float Grid::getCellWidth() const
{
    return ImGui::GetIO().DisplaySize.x / cols_;
}

float Grid::getCellHeight() const
{
    return ImGui::GetIO().DisplaySize.y / rows_;
}

Grid::Cell::Cell(std::shared_ptr<IWidget> widget, int col, int row, int colSpan, int rowSpan) :
    widget_{widget},
    col_{col},
    row_{row},
    colSpan_{colSpan},
    rowSpan_{rowSpan}
{
}

void Grid::Cell::draw(const Grid& grid) const
{
    float cellWidth = grid.getCellWidth();
    float cellHeight = grid.getCellHeight();

    ImGui::SetNextWindowPos(ImVec2(col_ * cellWidth, row_ * cellHeight));
    auto size = ImVec2(col_ + colSpan_ == grid.cols_ ? colSpan_ * cellWidth : colSpan_ * cellWidth - 5.0,
                       row_ + rowSpan_ == grid.rows_ ? rowSpan_ * cellHeight : rowSpan_ * cellHeight - 5.0);
    ImGui::SetNextWindowSize(size);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);

    widget_->draw();

    ImGui::PopStyleVar();
}
