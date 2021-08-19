#include <iostream>
#include <vector>


// tested x64, c++17

using namespace std;

class Unit
{
public:
    int unitId() { return 1; }

    void jump() { cout << "jump" << endl; }
    void swapWeapon() { cout << "swapWeapon" << endl; }
    void useSkill(int skillId) { cout << "useSkill: " << skillId << endl; }
    void moveTo(int x, int y) { cout << "moveTo: " << x << ", " << y << endl; }
};

struct InputNub
{
    InputNub(Unit& unit) : unit(unit) {}
    InputNub(Unit& unit, int x, int y) : unit(unit), posX(x), posY(y){}
    InputNub(Unit& unit, int skill) : unit(unit), skillId(skill) {}
    InputNub(Unit& unit, int x, int y, int skill) 
        : unit(unit), posX(x), posY(y), skillId(skill) {}

    Unit& unit;
    int posX = 0;
    int posY = 0;
    int skillId = 0;
};


class Command
{
public:
    virtual ~Command() = default;
    virtual void execute(const InputNub& ) const = 0;
    virtual void undo() const = 0;
};

class NullCommand : public Command
{
public:
    void execute(const InputNub& ) const override {
        cout << "null command" << endl;
    }

    void undo() const override {}
};

class JumpCommand : public Command
{
public:
    void execute(const InputNub& nub) const override {
        nub.unit.jump();
    }

    void undo() const override {}
};

class SwapWeaponCommand : public Command
{
public:
    void execute(const InputNub& nub) const override {
        nub.unit.swapWeapon();
    }

    void undo() const override {}
};

class UseSkillCommand : public Command
{
public:
    void execute(const InputNub& nub) const override {
        nub.unit.useSkill(nub.skillId);
    }

    void undo() const override {}
};

class MoveToCommand : public Command
{
public:
    void execute(const InputNub& nub) const override {
        nub.unit.moveTo(nub.posX, nub.posY);
    }

    void undo() const override {}
};

class MacroCommand : public Command
{
public:
    MacroCommand() { bindMacro(); }

    void execute(const InputNub& nub) const override {
        cout << "macro command begin" << endl;

        for (const auto& m : macros)
            m->execute(nub);

        cout << "macro command end" << endl;
    }

    void undo() const override {}

    void bindMacro();

private:
    vector<unique_ptr<Command>> macros;
};

void MacroCommand::bindMacro()
{
    macros.clear();

    macros.push_back(make_unique<SwapWeaponCommand>());
    macros.push_back(make_unique<UseSkillCommand>());
    macros.push_back(make_unique<JumpCommand>());
}


class InputHandler
{
public:
    InputHandler() { bindCommand(); }

    bool isValidSlot(int slot) { return true; }
    void bindCommand();
    void executeCommand(int slot, InputNub& nub);
 
private:
    vector<unique_ptr<Command>> commands;

};

void InputHandler::bindCommand()
{
    commands.clear();

    commands.push_back(make_unique<NullCommand>());
    commands.push_back(make_unique<JumpCommand>());
    commands.push_back(make_unique<SwapWeaponCommand>());
    commands.push_back(make_unique<UseSkillCommand>());
    commands.push_back(make_unique<MoveToCommand>());
    commands.push_back(make_unique<MacroCommand>());
}

void InputHandler::executeCommand(int slot, InputNub& nub)
{
    if (!isValidSlot(slot))
        return;

    commands[slot]->execute(nub);
}


int main()
{
    InputHandler inputHandler;
    Unit selectedUnit;
    InputNub nub(selectedUnit, 12, 34, 5);

    for (int i = 0; i < 6; ++i)
        inputHandler.executeCommand(i, nub);

    return 0;
}

