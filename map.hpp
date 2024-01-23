#ifndef MAP_H
#define MAP_H

#include "ECS/ECS.hpp"
#include <string>

class Map{
public:

    Map(std::string tID, int ms, int ts, Manager *man); //mfp=map file path, ms = map scale, ts = tile size
    ~Map();

    int size_x;
    int size_y;

    void LoadMap(std::string path, int sizeX, int sizeY);
    void AddTile(int srcX, int srcY, int xpos, int ypos);

private:
    std::string texID;
    int mapScale;
    int tileSize;
    int scaledSize;

    Manager* manager;
};

#endif // MAP_H
