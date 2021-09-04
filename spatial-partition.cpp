#include <iostream>
#include <list>
#include <array>
#include <unordered_map>
#include <algorithm>


// tested x64, c++17

using namespace std;

class Grid;

class Unit
{
public:
    Unit() : grid(nullptr) { create(); }
    Unit(Grid* grd) : grid(grd) { create(); }

    virtual ~Unit() = default;

    void create();
    void move(double x, double y);

public:
    int id;
    double posX;
    double posY;

private:
    Grid* grid;
};


using unit_map = unordered_map<int, Unit>;
using id_list = list<int>;

class Grid
{
public:
    virtual ~Grid() = default;

    void add(Unit* unit);
    void move(Unit* unit, double x, double y);

    void updateBattle(unit_map& units);
    void handleCell(id_list& ids, unit_map& units);

    void dump();

private:
    static const int CELL_SIZE = 20;
    static const int NUM_CELLS = 10;
    static constexpr int MAX_CELLS = NUM_CELLS * NUM_CELLS;

    array<id_list, MAX_CELLS> cells;
};


void Unit::create()
{
    static int genId = 1;

    id = genId;
    genId++;

    posX = 1.;
    posY = 1.;

    if (grid)
        grid->add(this);
}

void Unit::move(double x, double y)
{
    if (grid) {
        grid->move(this, x, y);
    }
    else {
        posX = x;
        posY = y;
    }
}


void Grid::add(Unit* unit)
{
    int cellX = (int)(unit->posX / CELL_SIZE);
    int cellY = (int)(unit->posY / CELL_SIZE);

    int cellId = NUM_CELLS * cellX + cellY;

    cells[cellId].push_back(unit->id);
    int a = 10;
}

void Grid::move(Unit* unit, double x, double y)
{
    int oldCellX = (int)(unit->posX / CELL_SIZE);
    int oldCellY = (int)(unit->posY / CELL_SIZE);

    int cellX = (int)(x / CELL_SIZE);
    int cellY = (int)(y / CELL_SIZE);

    unit->posX = x;
    unit->posY = y;

    if (oldCellX == cellX && oldCellY == cellY)
        return;

    int cellId = NUM_CELLS * oldCellX + oldCellY;
    auto& unitIds = cells[cellId];

    auto it = find(unitIds.begin(), unitIds.end(), unit->id);
    if (it != unitIds.end())
        unitIds.erase(it);

    add(unit);
}

void Grid::updateBattle(unit_map& units)
{
    cout << endl << "grid battle log" << endl;

    for (int cellId = 0; cellId < MAX_CELLS; ++cellId) {
        handleCell(cells[cellId], units);

        if (cells[cellId].size() < 2)
            continue;

        int x = cellId / NUM_CELLS;
        int y = cellId % NUM_CELLS;

        cout << "cell (" << x << ", " << y << ") battle end " << endl;
    }
}

void Grid::handleCell(id_list& ids, unit_map& units)
{
    for (auto& unitId : ids) {
        for (auto& otherId : ids) {
            if (unitId == otherId)
                continue;

            // check distance, attack other, self heal...
            unit_map::iterator unitIter = units.find(unitId);
            if (unitIter == units.end())
                continue;

            unit_map::iterator otherIter = units.find(otherId);
            if (otherIter == units.end())
                continue;

            cout << "attack: " << unitIter->first
                << " -> " << otherIter->first << endl;
        }
    }
}

void Grid::dump()
{
    cout << endl << "grid unit ids info" << endl;

    for (int cellId = 0; cellId < MAX_CELLS; ++cellId) {
        int x = cellId / NUM_CELLS;
        int y = cellId % NUM_CELLS;

        auto& unitIds = cells[cellId];

        if (unitIds.empty())
            continue;

        for (auto& unitId : unitIds)
            cout << "unit id: " << unitId << endl;

        cout << "cell (" << x << ", " << y << ") unit count: "
            << unitIds.size() << endl;
    }
}


int main()
{
    Grid grid;
    unordered_map<int, Unit> units;

    for (int i = 0; i < 10; ++i) {
        Unit unit(&grid);
        units.insert_or_assign(unit.id, unit);
    }

    grid.dump();

    int pos = 3;
    for (auto& unit : units) {
        unit.second.move(pos, pos);
        pos += 7;
    }

    grid.dump();

    grid.updateBattle(units);

    return 0;
}

