#include <iostream>
#include <vector>
#include <string_view>


// tested x64, c++17

using namespace std;

class Graphics {};
class World {};

class Player;


class Component
{
public:
    virtual ~Component() = default;
    virtual void receive(string_view msg) = 0;
};


class InputComponent : public Component
{
public:
    virtual ~InputComponent() = default;
    virtual void update(Player& player) = 0;
};

class DemoInputComponent : public InputComponent
{
public:
    virtual ~DemoInputComponent() = default;

    virtual void receive(string_view msg) override {
        cout << "DemoInputComponent receive: " << msg << endl;
    }

    virtual void update(Player& player) override {
        cout << "DemoInputComponent update" << endl;
    }
};

class GameInputComponent : public InputComponent
{
public:
    virtual ~GameInputComponent() = default;

    virtual void receive(string_view msg) override {
        cout << "GameInputComponent receive: " << msg << endl;
    }

    virtual void update(Player& player) override {
        cout << "GameInputComponent update" << endl;
    }
};


class PhysicsComponent : public Component
{
public:
    virtual ~PhysicsComponent() = default;
    virtual void update(Player& player, World& world) = 0;
};

class ZeroGPhysicsComponent : public PhysicsComponent
{
public:
    virtual ~ZeroGPhysicsComponent() = default;

    virtual void receive(string_view msg) override {
        cout << "ZeroGPhysicsComponent receive: " << msg << endl;
    }

    virtual void update(Player& player, World& world) override {
        cout << "ZeroGPhysicsComponent update" << endl;
    }
};

class NormalPhysicsComponent : public PhysicsComponent
{
public:
    virtual ~NormalPhysicsComponent() = default;

    virtual void receive(string_view msg) override {
        cout << "NormalPhysicsComponent receive: " << msg << endl;
    }

    virtual void update(Player& player, World& world) override {
        cout << "NormalPhysicsComponent update" << endl;
    }
};


class GraphicsComponent : public Component
{
public:
    virtual ~GraphicsComponent() = default;
    virtual void update(Player& player, Graphics& graphics) = 0;
};

class ToonGraphicsComponent : public GraphicsComponent
{
public:
    virtual ~ToonGraphicsComponent() = default;

    virtual void receive(string_view msg) override {
        cout << "ToonGraphicsComponent receive: " << msg << endl;
    }

    virtual void update(Player& player, Graphics& graphics) override {
        cout << "ToonGraphicsComponent update" << endl;
    }
};

class RealGraphicsComponent : public GraphicsComponent
{
public:
    virtual ~RealGraphicsComponent() = default;

    virtual void receive(string_view msg) override {
        cout << "RealGraphicsComponent receive: " << msg << endl;
    }

    virtual void update(Player& player, Graphics& graphics) override {
        cout << "RealGraphicsComponent update" << endl;
    }
};


class Player
{
public:
    Player() { create(); }
    virtual ~Player() = default;

    void create();
    void update(World& world, Graphics& graphics);
    void sendMessage(string_view msg);

public:
    int posX, posY;
    int velocity;

private:
    shared_ptr<InputComponent> inputC;
    shared_ptr<PhysicsComponent> physicsC;
    shared_ptr<GraphicsComponent> graphicsC;

    vector<shared_ptr<Component>> components;
};

void Player::create()
{
    posX = 100;
    posY = 130;
    velocity = 20;

    inputC = make_shared<GameInputComponent>();
    physicsC = make_shared<NormalPhysicsComponent>();
    graphicsC = make_shared<RealGraphicsComponent>();

    components.push_back(inputC);
    components.push_back(physicsC);
    components.push_back(graphicsC);
}

void Player::update(World& world, Graphics& graphics)
{
    inputC->update(*this);
    physicsC->update(*this, world);
    graphicsC->update(*this, graphics);
}

void Player::sendMessage(string_view msg)
{
    for (auto& com : components)
        com->receive(msg);
}


int main()
{
    World world;
    Graphics graphics;
    Player player;

    player.update(world, graphics);
    player.sendMessage("player msg");

    return 0;
}

