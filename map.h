#ifndef MAP_H
#define MAP_H

#include <string>

class Map{
public:

    Map(std::string tID, int ms, int ts); //mfp=map file path, ms = map scale, ts = tile size
    ~Map();

    void LoadMap(std::string path, int sizeX, int sizeY);
    void AddTile(int srcX, int srcY, int xpos, int ypos);

private:
    std::string texID;
    int mapScale;
    int tileSize;
    int scaledSize;
};

#endif // MAP_H
