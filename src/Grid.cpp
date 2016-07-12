#include "EmptyWidget.h"
#include "Grid.h"
#include "IWidget.h"

#include "IrrIMGUI/IrrIMGUI.h"

#include <memory>

Grid::Grid(const std::string& name, int cols /* =1 */, int rows /* = 1 */) :
    name_{name},
    cols_{cols},
    rows_{rows}
{
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Empty1")));
    cells_.emplace_back(std::make_unique<Cell>(std::make_shared<EmptyWidget>("Empty2"), 2, 2, 2, 2));
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
    ImGui::SetNextWindowSize(ImVec2(colSpan_ * cellWidth, rowSpan_ * cellHeight));
    widget_->draw();
}
