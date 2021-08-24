#include <iostream>
#include <string>
#include <string_view>


// tested x64, c++17

using namespace std;

enum Input
{
    PRESS_UP,
    PRESS_DOWN,
    RELEASE_UP,
    RELEASE_DOWN,
};

class Hero;
class StandingState;
class JumpingState;
class DuckingState;


class IState
{
public:
    IState(string_view str) : name(str) {}
    virtual ~IState() = default;

    virtual void enter(Hero& hero) = 0;
    virtual void handleInput(Hero& hero, Input input) = 0;
    virtual void update(Hero& hero) = 0;
    virtual void exit(Hero& hero) = 0;

protected:
    string name = "state";
};

class StandingState : public IState
{
public:
    StandingState() : IState("standing") {}
    virtual ~StandingState() = default;

    virtual void enter(Hero& hero) override;
    virtual void handleInput(Hero& hero, Input input) override;
    virtual void update(Hero& hero) override {}
    virtual void exit(Hero& hero) override;
};

class JumpingState : public IState
{
public:
    JumpingState() : IState("jumping") {}
    virtual ~JumpingState() = default;

    virtual void enter(Hero& hero) override;
    virtual void handleInput(Hero& hero, Input input) override;
    virtual void update(Hero& hero) override {}
    virtual void exit(Hero& hero) override;
};

class DuckingState : public IState
{
public:
    DuckingState() : IState("ducking"), chargeCount(0) {}
    virtual ~DuckingState() = default;

    virtual void enter(Hero& hero) override;
    virtual void handleInput(Hero& hero, Input input) override;
    virtual void update(Hero& hero) override;
    virtual void exit(Hero& hero) override;

private:
    int chargeCount = 0;
};


class HeroState
{
public:
    static inline StandingState standing;
    static inline JumpingState jumping;
    static inline DuckingState ducking;
};


class Hero
{
public:
    Hero() {
        changeState(&HeroState::standing);
    }

    virtual void changeState(IState* change) {
        if (state)
            state->exit(*this);

        state = change;
        state->enter(*this);
    }

    virtual void handleInput(Input input) {
        state->handleInput(*this, input);
    }

    virtual void update() {
        state->update(*this);
    }

    void setAnimation(string_view animate) {
        cout << "set animation: " << animate << endl;
    }

    void superBomb() {
        cout << "execute super bomb!!" << endl;
    }

private:
    IState* state;
};


void StandingState::enter(Hero& hero)
{
    cout << "enter: " << name << endl;
    hero.setAnimation(name);
}

void StandingState::handleInput(Hero& hero, Input input)
{
    switch (input) {
    case PRESS_UP:
        hero.changeState(&HeroState::jumping);
        break;
    case PRESS_DOWN:
        hero.changeState(&HeroState::ducking);
        break;
    }
}

void StandingState::exit(Hero& hero)
{
    cout << "exit: " << name << endl;
}


void JumpingState::enter(Hero& hero)
{
    cout << "enter: " << name << endl;
    hero.setAnimation(name);
}

void JumpingState::handleInput(Hero& hero, Input input)
{
    switch (input) {
    case PRESS_DOWN:
        hero.changeState(&HeroState::ducking);
        break;
    case RELEASE_UP:
        hero.changeState(&HeroState::standing);
        break;
    }
}

void JumpingState::exit(Hero& hero)
{
    cout << "exit: " << name << endl;
}


void DuckingState::enter(Hero& hero)
{
    cout << "enter: " << name << endl;
    hero.setAnimation(name);
    chargeCount = 0;
}

void DuckingState::handleInput(Hero& hero, Input input)
{
    switch (input) {
    case PRESS_UP:
        hero.changeState(&HeroState::jumping);
        break;
    case RELEASE_DOWN:
        hero.changeState(&HeroState::standing);
        break;
    }
}

void DuckingState::update(Hero& hero)
{
    chargeCount += 1;

    if (chargeCount > 3) {
        hero.superBomb();
        chargeCount = 0;
    }
}

void DuckingState::exit(Hero& hero)
{
    cout << "exit: " << name << endl;
}


int main()
{
    Hero hero;
    hero.handleInput(PRESS_UP);
    hero.handleInput(RELEASE_UP);
    hero.handleInput(PRESS_DOWN);

    for (int i = 0; i < 11; ++i)
        hero.update();

    hero.handleInput(RELEASE_DOWN);

    return 0;
}

