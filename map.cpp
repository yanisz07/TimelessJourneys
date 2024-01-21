#include "map.hpp"
#include "game.hpp"
#include <fstream>
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"

Map::Map(std::string tID, int ms, int ts, Manager *man) : texID(tID), mapScale(ms), tileSize(ts)
{
    scaledSize = ms*ts;
    manager = man;
}

Map::~Map()
{
}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
    char c;
    std::fstream mapFile;
    mapFile.open(path);

    int srcX, srcY;

    if (!mapFile.is_open())
    {
        std::cerr << "Error opening file: " << path << std::endl;
        return;
    }

    for (int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            mapFile.get(c); //get y value of tile
            //std::cout << 'c = ' << c << std::endl;
            srcY = atoi(&c) * tileSize;
            //std::cout << 'srcY = ' << srcY << std::endl;
            mapFile.get(c); //get x value of tile
            //std::cout << 'c = ' << c << std::endl;
            srcX = atoi(&c) * tileSize;
            //std::cout << 'srcX = ' << srcX << std::endl;
            AddTile(srcX, srcY, x* scaledSize, y* scaledSize);
            mapFile.ignore(); //","
        }
    }

    mapFile.ignore();

    for (int y=0; y < sizeY; y++)
    {
        for (int x =0; x < sizeX; x++)
        {
            mapFile.get(c);
            if ( c == '1' )
            {
                auto& tcol(manager->addEntity());
                tcol.addComponent<TransformComponent>(x*scaledSize,y*scaledSize,tileSize,tileSize,mapScale);
                tcol.addComponent<ColliderComponent>("terrain");
                tcol.addGroup(Game::groupMapColliders);
            }
            mapFile.ignore(); //','
        }
    }

    mapFile.close();
}

void Map::LoadMap2(std::string path, int sizeX, int sizeY,std::map<int,Entity*> &map)
{
    char c;
    std::fstream mapFile;
    mapFile.open(path);

    int srcX, srcY;

    if (!mapFile.is_open())
    {
        std::cerr << "Error opening file: " << path << std::endl;
        return;
    }

    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            mapFile.get(c); //get y value of tile
            //std::cout << 'c = ' << c << std::endl;
            srcY = atoi(&c) * tileSize;
            //std::cout << 'srcY = ' << srcY << std::endl;
            mapFile.get(c); //get x value of tile
            //std::cout << 'c = ' << c << std::endl;
            srcX = atoi(&c) * tileSize;
            //std::cout << 'srcX = ' << srcX << std::endl;
            auto& tile(manager->addEntity());
            tile.addComponent<TileComponent>(srcX, srcY, x* scaledSize, y* scaledSize, tileSize, mapScale, texID);
            tile.addGroup(Game::groupMap);
            map[100*x+y] = &tile;
            mapFile.ignore(); //","
        }
    }

    mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
    auto& tile(manager->addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID);
    tile.addGroup(Game::groupMap);
}

