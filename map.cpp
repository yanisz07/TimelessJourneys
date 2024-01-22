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

void Map::LoadMap2(std::string path, int sizeX, int sizeY)
{
    char c1, c2;
    std::fstream mapFile;
    mapFile.open(path);
    char buffer[3]; // Buffer to hold the two-digit number and a null terminator

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
            // Read the two digit of the X value
            mapFile.get(c1);
            mapFile.get(c2);

            buffer[0] = c1;
            buffer[1] = c2;
            buffer[2] = '\0';
            srcX = atoi(buffer) * tileSize;
            //std::cout << 'c = ' << c << std::endl;
            //std::cout << 'srcX = ' << srcX << std::endl;
            mapFile.ignore(); //"-"
            // Read the two digit of the X value
            mapFile.get(c1);
            mapFile.get(c2);

            buffer[0] = c1;
            buffer[1] = c2;
            buffer[2] = '\0';

            //std::cout << 'c = ' << c << std::endl;
            srcY = atoi(buffer) * tileSize;
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
            mapFile.get(c1);
            if ( c1 == '1' )
            {
                auto& tcol(manager->addEntity());
                tcol.addComponent<ColliderComponent>("terrain", x*scaledSize , y*scaledSize, scaledSize);
                tcol.addGroup(Game::groupMapColliders);
            }
            mapFile.ignore(); //','
        }
    }

    mapFile.close();
}

void Map::LoadMap3(std::string path, int sizeX, int sizeY)
{
    char c1;
    char c2;
    int k1;
    int k2;
    int k3;
    std::fstream mapFile;
    mapFile.open(path);

    int srcX, srcY;

    if (!mapFile.is_open())
    {
        std::cerr << "Error opening file: " << path << std::endl;
        return;
    }

    for (int y=0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            mapFile.get(c1);
            mapFile.get(c2);
            if (c2 == ',')
            {
                k3 = c1 - '0';
                if (k3!=0)
                {
                    srcY = k3/15 * tileSize;
                    srcX = k3%15 * tileSize;
                    std::cout << srcY << std::endl;
                    std::cout << srcX << std::endl;
                    AddTile(srcX, srcY, x* scaledSize, y* scaledSize);
                }
            }
            else
            {
                k1 = c1 - '0';
                k2 = c2 - '0';
                k3 = k1*10+k2;
                std::cout << c1 << " " << c2 << std::endl;
                std::cout << k3 << std::endl;
                srcY = k3/15 * tileSize;
                srcX = k3%15 * tileSize;
                std::cout << srcY << std::endl;
                std::cout << srcX << std::endl;
                AddTile(srcX, srcY, x* scaledSize, y* scaledSize);
                mapFile.ignore(); //","
            }
        }
        mapFile.ignore();
    }

    mapFile.ignore();

    for (int y=0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            mapFile.get(c1);
            mapFile.get(c2);
            if (c2 == ',')
            {
                k3 = c1 - '0';
                if (k3!=0)
                {
                    srcY = k3/15 * tileSize;
                    srcX = k3%15 * tileSize;
                    std::cout << srcY << std::endl;
                    std::cout << srcX << std::endl;
                    AddTile(srcX, srcY, x* scaledSize, y* scaledSize);
                }
            }
            else
            {
                k1 = c1 - '0';
                k2 = c2 - '0';
                k3 = k1*10+k2;
                std::cout << c1 << " " << c2 << std::endl;
                std::cout << k3 << std::endl;
                srcY = k3/15 * tileSize;
                srcX = k3%15 * tileSize;
                std::cout << srcY << std::endl;
                std::cout << srcX << std::endl;
                AddTile(srcX, srcY, x* scaledSize, y* scaledSize);
                mapFile.ignore(); //","
            }
        }
        mapFile.ignore();
    }

    mapFile.ignore();

    for (int y=0; y < sizeY; y++)
    {
        for (int x =0; x < sizeX; x++)
        {
            mapFile.get(c1);
            if ( c1 == '1' )
            {
                auto& tcol(manager->addEntity());
                tcol.addComponent<TransformComponent>(x*scaledSize,y*scaledSize,tileSize,tileSize,mapScale);
                tcol.addComponent<ColliderComponent>("terrain");
                tcol.addGroup(Game::groupMapColliders);
            }
            mapFile.ignore(); //','
        }
        mapFile.ignore();
    }
    mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
    auto& tile(manager->addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID);
    tile.addGroup(Game::groupMap);
}

