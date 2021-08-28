#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <string_view>


// tested x64, c++17

using namespace std;
using namespace std::chrono;

class Entity
{
public:
    Entity(string_view n, double x, double y)
        : name(n), posX(x), posY(y) {}

    virtual ~Entity() = default;

    virtual void update(double) = 0;

protected:
    string name;
    double posX;
    double posY;
};

class Skeleton : public Entity
{
public:
    Skeleton(string_view n, int x, int y)
        : Entity(n, x, y), patroll(false) {}

    virtual ~Skeleton() = default;

    virtual void update(double elapsed) override {
        posX += static_cast<int>(elapsed);
        posY += static_cast<int>(elapsed);
        cout << name << " : position " << posX << ", " << posY << endl;
    }

private:
    bool patroll;
};

class Beast : public Entity
{
public:
    Beast(string_view n, int x, int y)
        : Entity(n, x, y), sleep(false) {}

    virtual ~Beast() = default;

    virtual void update(double elapsed) override {
        posX -= static_cast<int>(elapsed);
        posY -= static_cast<int>(elapsed);
        cout << name << " : position " << posX << ", " << posY << endl;
    }

private:
    bool sleep;
};


class GameLogic
{
public:
    GameLogic() { init(); }
    virtual ~GameLogic() = default;

    void runLoop();

private:
    void init();
    void processInput() {}
    void update(double elapsed);
    void render() {}

private:
    vector<unique_ptr<Entity>> entities;
};

void GameLogic::init()
{
    entities.push_back(make_unique<Skeleton>("skel1", 100, 100));
    entities.push_back(make_unique<Skeleton>("skel2", 200, 150));
    entities.push_back(make_unique<Skeleton>("skel3", 400, 330));
    entities.push_back(make_unique<Skeleton>("skel4", 210, 350));
    entities.push_back(make_unique<Beast>("beast1", 1000, 1200));
    entities.push_back(make_unique<Beast>("beast2", 1170, 2100));
    entities.push_back(make_unique<Beast>("beast3", 1500, 1990));
}

void GameLogic::runLoop()
{
    nanoseconds run_time{ 0 };

    steady_clock::time_point begin = steady_clock::now();
    steady_clock::time_point previous = steady_clock::now();

    while (run_time < 100ms) {
        steady_clock::time_point current = steady_clock::now();

        duration<double, milli> elapsed = current - previous;
        previous = current;

        processInput();
        update(elapsed.count());
        render();

        steady_clock::time_point end = steady_clock::now();
        run_time = end - begin;
    }
}

void GameLogic::update(double elapsed)
{
    for (auto& entity : entities)
        entity->update(elapsed);

    cout << "update complete" << endl;
}


int main()
{
    GameLogic game;
    game.runLoop();

    return 0;
}

