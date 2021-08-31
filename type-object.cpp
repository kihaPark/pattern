#include <iostream>
#include <vector>
#include <string>
#include <string_view>


// tested x64, c++17

using namespace std;

class Monster;

class Tribe
{
public:
    Tribe(Tribe* parent, string_view name, int health, string_view attack);
    virtual ~Tribe() = default;

    unique_ptr<Monster> newMonster() {
        return make_unique<Monster>(*this);
    }

    string_view getName() const {
        return name;
    }

    int getHealth() const {
        return health;
    }

    string_view getAttack() const {
        return attack;
    }

private:
    Tribe* parent;
    string name;
    int health;
    string attack;
};

Tribe::Tribe(Tribe* parent, string_view name, int health, string_view attack)
    : parent(parent), name(name), health(health), attack(attack)
{
    // copy down delegation
    if (parent != nullptr) {
        if (this->health == 0)
            this->health = parent->getHealth();

        if (this->attack == "")
            this->attack = parent->getAttack();
    }
}


class Monster
{
public:
    Monster(Tribe& tribe)
        : health(tribe.getHealth()), tribe(tribe) {}
    virtual ~Monster() = default;

    string_view getAttack() {
        return (health < 10) ? "howling wind" : tribe.getAttack();
    }

    void viewStatus() {
        cout << "monster status: " << tribe.getName()
            << ", health: " << health
            << ", attack: " << tribe.getAttack() << endl;
    }

private:
    int health;
    Tribe& tribe;
};


int main()
{
    Tribe tribe1{ nullptr, "troll", 25, "troll attack!!" };
    Tribe tribe2{ &tribe1, "troll archer", 0, "troll archer shot!!" };
    Tribe tribe3{ &tribe1, "troll wizard", 0, "troll wizard spell!!" };

    vector<unique_ptr<Monster>> monsters;

    monsters.push_back(tribe1.newMonster());
    monsters.push_back(tribe2.newMonster());
    monsters.push_back(tribe3.newMonster());

    for (auto& mon : monsters)
        mon->viewStatus();

    return 0;
}

/* type object for data modeling, json format
{
    "troll": {
      "health": 25,
      "attack": "troll attack!!"
    },
    "troll archer": {
      "parent": "troll",
      "health": 0,
      "attack": "troll archer shot!!"
    },
    "troll wizard": {
      "parent": "troll",
      "health": 0,
      "attack": "troll wizard spell!!"
    }
}
*/

