#include <iostream>
#include <string>


// tested x64, c++17

using namespace std;

class Monster
{
public:
    virtual ~Monster() = default;
    virtual unique_ptr<Monster> clone() = 0;
};

class Ghost : public Monster
{
public:
    Ghost(int health, int mana, string route = "new")
        : health(health), mana(mana), route(route) {
        cout << route << " ghost: " << health << ", " << mana << endl;
    }

    unique_ptr<Monster> clone() override {
        return make_unique<Ghost>(health, mana, "clone");
    }

private:
    string route;
    int health;
    int mana;
};

class Beast : public Monster
{
public:
    Beast(int health, int mana, string route = "new")
        : health(health), mana(mana), route(route) {
        cout << route << " beast: " << health << ", " << mana << endl;
    }

    unique_ptr<Monster> clone() override {
        return make_unique<Beast>(health, mana, "clone");
    }

private:
    string route;
    int health;
    int mana;
};


class Spawner
{
public:
    Spawner(Monster* prototype) {
        setSpawner(prototype);
    }

    void setSpawner(Monster* prototype) {
        this->prototype = prototype;
    }

    unique_ptr<Monster> spawnMonster() {
        return prototype->clone();
    }

private:
    Monster* prototype;
};


template <typename T>
unique_ptr<Monster> spawnMonster(int health, int mana)
{
    return make_unique<T>(health, mana);
}


int main()
{
    unique_ptr<Monster> proto_ghost = make_unique<Ghost>(10, 15);
    unique_ptr<Monster> proto_beast = make_unique<Beast>(20, 3);

    Spawner spawner{ proto_ghost.get() };
    unique_ptr<Monster> clone_ghost1 = spawner.spawnMonster();
    unique_ptr<Monster> clone_ghost2 = spawner.spawnMonster();
    unique_ptr<Monster> clone_ghost3 = spawner.spawnMonster();

    spawner.setSpawner(proto_beast.get());
    unique_ptr<Monster> clone_beast1 = spawner.spawnMonster();
    unique_ptr<Monster> clone_beast2 = spawner.spawnMonster();
    unique_ptr<Monster> clone_beast3 = spawner.spawnMonster();

    unique_ptr<Monster> create_ghost1 = spawnMonster<Ghost>(22, 38);
    unique_ptr<Monster> create_ghost2 = spawnMonster<Ghost>(23, 37);
    unique_ptr<Monster> create_ghost3 = spawnMonster<Ghost>(24, 36);

    unique_ptr<Monster> create_beast1 = spawnMonster<Beast>(31, 7);
    unique_ptr<Monster> create_beast2 = spawnMonster<Beast>(32, 6);
    unique_ptr<Monster> create_beast3 = spawnMonster<Beast>(33, 5);

    return 0;
}

/* prototype for data modeling, json format
{
    "name": "goblin warrior",
    "normal_health": 30,
    "max_health": 40,
    "immunity": ["cold", "poision"],
    "weakness": ["fire", "light"]
}

{
    "name": "goblin wizard",
    "prototype": "goblin warrior",
    "skill": ["fireball", "lightning arrow"]
}

{
    "name": "goblin archer",
    "prototype" : "goblin warrior",
    "attack" : ["short bow"]
}
 */

