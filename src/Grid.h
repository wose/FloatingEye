#ifndef FLOATING_EYE_GRID_H
#define FLOATING_EYE_GRID_H

#include <memory>
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

class IWidget;

class Grid
{
public:
    Grid(const std::string& name, irr::video::IVideoDriver* const irrDriver,
         IrrIMGUI::IIMGUIHandle* const imGuiHandle, int cols = 1, int rows = 1);
    virtual ~Grid() = default;

    virtual void draw() const;

    void setName(const std::string& name);
    std::string getName() const;

private:
    float getCellWidth() const;
    float getCellHeight() const;

    class Cell {
    public:
        Cell(std::shared_ptr<IWidget> widget, int col = 0, int row = 0, int colSpan = 1, int rowSpan = 1);

        void draw(const Grid& grid) const;

    private:
        int row_;
        int col_;
        int rowSpan_;
        int colSpan_;
        std::shared_ptr<IWidget> widget_;
    };

    int cols_;
    int rows_;
    std::string name_;

    std::vector<std::unique_ptr<Cell>> cells_;
};

#endif // FLOATING_EYE_GRID_H
