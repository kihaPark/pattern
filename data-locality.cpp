#include <iostream>
#include <vector>
#include <chrono>


// tested x64, c++17

using namespace std;
using namespace std::chrono;

class Graphics {};
class World {};

class Unit;


class Component
{
public:
    virtual ~Component() = default;
    virtual void receive(string_view msg) = 0;
};

class AIComponent : public Component
{
public:
    AIComponent() : unitId(0) {}
    AIComponent(int unitId) : unitId(unitId) {}

    virtual ~AIComponent() = default;

    virtual void receive(string_view msg) override {}

    void setUnitId(int id) { unitId = id; }

    void update(World& world) {}
    void update(Unit& unit, World& world) {}

private:
    int unitId;
};

class PhysicsComponent : public Component
{
public:
    PhysicsComponent() : unitId(0) {}
    PhysicsComponent(int unitId) : unitId(unitId) {}

    virtual ~PhysicsComponent() = default;

    virtual void receive(string_view msg) override {}

    void setUnitId(int id) { unitId = id; }

    void update(World& world) {}
    void update(Unit& unit, World& world) {}

private:
    int unitId;
};

class RenderComponent : public Component
{
public:
    RenderComponent() : unitId(0) {}
    RenderComponent(int unitId) : unitId(unitId) {}

    virtual ~RenderComponent() = default;

    virtual void receive(string_view msg) override {}

    void setUnitId(int id) { unitId = id; }

    void render(Graphics& graphics) {}
    void render(Unit& unit, Graphics& graphics) {}

private:
    int unitId;
};


class Unit
{
public:
    Unit() { create(); }
    virtual ~Unit() = default;

    void create();
    void update(World& world, Graphics& graphics);
    void sendMessage(string_view msg);

public:
    int id;
    int posX, posY;
    int velocity;

private:
    shared_ptr<AIComponent> aiC;
    shared_ptr<PhysicsComponent> physicsC;
    shared_ptr<RenderComponent> renderC;

    vector<shared_ptr<Component>> components;
};

void Unit::create()
{
    static int genId = 1;

    id = genId;
    genId++;

    posX = 100;
    posY = 130;
    velocity = 20;

    aiC = make_shared<AIComponent>(id);
    physicsC = make_shared<PhysicsComponent>(id);
    renderC = make_shared<RenderComponent>(id);

    components.push_back(aiC);
    components.push_back(physicsC);
    components.push_back(renderC);
}

void Unit::update(World& world, Graphics& graphics)
{
    aiC->update(*this, world);
    physicsC->update(*this, world);
    renderC->render(*this, graphics);
}

void Unit::sendMessage(string_view msg)
{
    for (auto& com : components)
        com->receive(msg);
}


const int MAX_UNIT_COUNT = 10000;
const int MAX_LOOP_COUNT = 1000;

void TestDataLocality1()
{
    World world;
    Graphics graphics;

    vector<unique_ptr<Unit>> units;

    for (int i = 0; i < MAX_UNIT_COUNT; ++i)
        units.push_back(make_unique<Unit>());

    int loopCount = 0;
    steady_clock::time_point begin = steady_clock::now();

    while (loopCount < MAX_LOOP_COUNT) {
        for (auto& unit : units)
            unit->update(world, graphics);

        ++loopCount;
    }

    steady_clock::time_point end = steady_clock::now();
    nanoseconds elapsed = end - begin;
    milliseconds elapsed2 = duration_cast<milliseconds>(elapsed);

    cout << "test_1 elapsed time: " << elapsed << ", " << elapsed2 << endl;
}

void TestDataLocality2()
{
    World world;
    Graphics graphics;

    vector<Unit> units;

    for (int i = 0; i < MAX_UNIT_COUNT; ++i)
        units.push_back(Unit());

    int loopCount = 0;
    steady_clock::time_point begin = steady_clock::now();

    while (loopCount < MAX_LOOP_COUNT) {
        for (auto& unit : units)
            unit.update(world, graphics);

        ++loopCount;
    }

    steady_clock::time_point end = steady_clock::now();
    nanoseconds elapsed = end - begin;
    milliseconds elapsed2 = duration_cast<milliseconds>(elapsed);

    cout << "test_2 elapsed time: " << elapsed << ", " << elapsed2 << endl;
}

void TestDataLocality3()
{
    World world;
    Graphics graphics;

    vector<unique_ptr<AIComponent>> aiComps;
    vector<unique_ptr<PhysicsComponent>> physicsComps;
    vector<unique_ptr<RenderComponent>> renderComps;

    for (int i = 1; i <= MAX_UNIT_COUNT; ++i) {
        aiComps.push_back(make_unique<AIComponent>(i));
        physicsComps.push_back(make_unique<PhysicsComponent>(i));
        renderComps.push_back(make_unique<RenderComponent>(i));
    }

    int loopCount = 0;
    steady_clock::time_point begin = steady_clock::now();

    while (loopCount < MAX_LOOP_COUNT) {
        for (auto& ac : aiComps)
            ac->update(world);

        for (auto& pc : physicsComps)
            pc->update(world);

        for (auto& rc : renderComps)
            rc->render(graphics);

        ++loopCount;
    }

    steady_clock::time_point end = steady_clock::now();
    nanoseconds elapsed = end - begin;
    milliseconds elapsed2 = duration_cast<milliseconds>(elapsed);

    cout << "test_3 elapsed time: " << elapsed << ", " << elapsed2 << endl;
}

void TestDataLocality4()
{
    World world;
    Graphics graphics;

    vector<AIComponent> aiComps;
    vector<PhysicsComponent> physicsComps;
    vector<RenderComponent> renderComps;

    for (int i = 1; i <= MAX_UNIT_COUNT; ++i) {
        aiComps.push_back(AIComponent(i));
        physicsComps.push_back(PhysicsComponent(i));
        renderComps.push_back(RenderComponent(i));
    }

    int loopCount = 0;
    steady_clock::time_point begin = steady_clock::now();

    while (loopCount < MAX_LOOP_COUNT) {
        for (auto& ac : aiComps)
            ac.update(world);

        for (auto& pc : physicsComps)
            pc.update(world);

        for (auto& rc : renderComps)
            rc.render(graphics);

        ++loopCount;
    }

    steady_clock::time_point end = steady_clock::now();
    nanoseconds elapsed = end - begin;
    milliseconds elapsed2 = duration_cast<milliseconds>(elapsed);

    cout << "test_4 elapsed time: " << elapsed << ", " << elapsed2 << endl;
}

void TestDataLocality5()
{
    World world;
    Graphics graphics;

    AIComponent aiComps[MAX_UNIT_COUNT];
    PhysicsComponent physicsComps[MAX_UNIT_COUNT];
    RenderComponent renderComps[MAX_UNIT_COUNT];

    for (int i = 0; i < MAX_UNIT_COUNT; ++i) {
        aiComps[i].setUnitId(i + 1);
        physicsComps[i].setUnitId(i + 1);
        renderComps[i].setUnitId(i + 1);
    }

    int loopCount = 0;
    steady_clock::time_point begin = steady_clock::now();

    while (loopCount < MAX_LOOP_COUNT) {
        for (int i = 0; i < MAX_UNIT_COUNT; ++i)
            aiComps[i].update(world);

        for (int i = 0; i < MAX_UNIT_COUNT; ++i)
            physicsComps[i].update(world);

        for (int i = 0; i < MAX_UNIT_COUNT; ++i)
            renderComps[i].render(graphics);

        ++loopCount;
    }

    steady_clock::time_point end = steady_clock::now();
    nanoseconds elapsed = end - begin;
    milliseconds elapsed2 = duration_cast<milliseconds>(elapsed);

    cout << "test_5 elapsed time: " << elapsed << ", " << elapsed2 << endl;
}


int main()
{
    cout << "test environment" << endl;
    cout << "unit count: " << MAX_UNIT_COUNT << endl;
    cout << "loop count: " << MAX_LOOP_COUNT << endl;

    TestDataLocality1();
    TestDataLocality2();
    TestDataLocality3();
    TestDataLocality4();
    TestDataLocality5();

    return 0;
}

