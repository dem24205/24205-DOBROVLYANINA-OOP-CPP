#ifndef GRID_H
#define GRID_H

#include <vector>

class Grid {
private:
    std::vector<std::vector<bool>> grid;
    int width, height;
public:
    explicit Grid(int newWidth, int newHeight);

    void setCell(int x, int y, bool value = true);

    bool getCell(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    std::vector<std::vector<bool>> getGrid() const;

    void setSnowflakePattern();
    void setPulsarPattern();
    void setGliderPattern();
    void setBlinkerPattern();
    void setPentominoPattern();
    void setDiehardPattern();
    void set101Pattern();
};

#endif