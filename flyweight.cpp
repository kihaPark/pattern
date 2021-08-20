#include <iostream>
#include <string>
#include <random>


// tested x64, c++17

using namespace std;

class Mesh {};
class Texture {};
class Color {};
class Vector {};


class TreeModel
{
private:
    Mesh mesh;
    Texture bark;
    Texture leaves;
};

class Tree1
{
private:
    TreeModel* model;

    double scale;
    Vector position;
    Color barkTint;
    Color leafTint;
};


class Terrain
{
public:
    Terrain() = default;
    Terrain(string type, int moveCost, bool water, Texture texture) {
        load(type, moveCost, water, texture);
    }

    void load(string type, int moveCost, bool water, Texture texture);

    void printType() const { cout << type << " "; }
    int getMoveCost() const { return moveCost; }
    bool isWater() const { return water; }
    const Texture& getTexture() const { return texture; }

private:
    string type;
    int moveCost = 0;
    bool water = false;
    Texture texture;
};

void Terrain::load(string type, int moveCost, bool water, Texture texture)
{
    this->type = type;
    this->moveCost = moveCost;
    this->water = water;
    this->texture = texture;
}


class World
{
public:
    World();

    static const int WIDTH = 10;
    static const int HEIGHT = 10;

    const Terrain& getTile(int x, int y) const;

private:
    Terrain* tiles[WIDTH][HEIGHT];

    Terrain road;
    Terrain grass;
    Terrain hill;
    Terrain river;

    void loadTerrain();
    void generateTerrain();
};

World::World()
{
    loadTerrain();
    generateTerrain();
}

void World::loadTerrain()
{
    road.load("road", 1, false, Texture());
    grass.load("grass", 2, false, Texture());
    hill.load("hill", 3, false, Texture());
    river.load("river", 4, true, Texture());
}

void World::generateTerrain()
{
    random_device rd;
    default_random_engine eng(rd());
    uniform_int_distribution<int> dist(0, WIDTH - 1);

    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            int type = dist(eng) % 3;

            if (type == 0)
                tiles[x][y] = &road;
            else if (type == 1)
                tiles[x][y] = &grass;
            else
                tiles[x][y] = &hill;
        }
    }

    int x = dist(eng);
    for (int y = 0; y < HEIGHT; ++y)
        tiles[x][y] = &river;
}

const Terrain& World::getTile(int x, int y) const
{
    return *tiles[x][y];
}


int main()
{
    World world;
 
    for (int  x = 0; x < World::WIDTH; ++x) {
        for (int y = 0; y < World::HEIGHT; y++)
            world.getTile(x, y).printType();

        cout << endl;
    }

    return 0;
}

