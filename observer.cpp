#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


// tested x64, c++17

using namespace std;

class Unit;


class Observer
{
public:
    virtual ~Observer() = default;
    virtual void onNotify(const Unit&, const string&) = 0;
};

class Quests : public Observer
{
public:
    void onNotify(const Unit& unit, const string& event) override {
        cout << "quest noti: " << event << endl;
    }
};

class Achievements : public Observer
{
public:
    void onNotify(const Unit& unit, const string& event) override {
        cout << "achievement noti: " << event << endl;
    }
};


class Observable
{
public:
    virtual ~Observable() = default;

    void addObserver(Observer* obs);
    void removeObserver(Observer* obs);
    void notify(const Unit& unit, const string& event);

private:
    vector<Observer*> observers;
};

void Observable::addObserver(Observer* obs)
{
    auto it = find(observers.begin(), observers.end(), obs);
    if (it != observers.end())
        return;

    observers.push_back(obs);
    cout << "add observer" << endl;
}

void Observable::removeObserver(Observer* obs)
{
    auto it = remove(observers.begin(), observers.end(), obs);
    observers.erase(it, observers.end());
    cout << "remove observer" << endl;
}

void Observable::notify(const Unit& unit, const string& event)
{
    for (auto* obs : observers)
        obs->onNotify(unit, event);
}


class Unit : public Observable
{
public:
    int unitId() { return 1; }

    void jump();
    void swapWeapon();
    void useSkill(int skillId);
    void moveTo(int x, int y);
};

void Unit::jump()
{
    cout << "unit jump" << endl;
    notify(*this, "jump");
}

void Unit::swapWeapon()
{
    cout << "unit swapWeapon" << endl;
    notify(*this, "swapWeapon");
}

void Unit::useSkill(int skillId)
{
    cout << "unit useSkill: " << skillId << endl;
    notify(*this, "useSkill");
}

void Unit::moveTo(int x, int y)
{
    cout << "unit moveTo: " << x << ", " << y << endl;
    notify(*this, "moveTo");
}


void TestObserverPattern()
{
    Unit unit;
    Quests quest;
    Achievements achievement;

    unit.addObserver(&quest);
    unit.addObserver(&achievement);

    unit.jump();
    unit.removeObserver(&quest);
    unit.swapWeapon();
    unit.useSkill(2);
    unit.removeObserver(&achievement);
    unit.moveTo(1002, 2345);
}



class Unit2;


template<typename T>
class Observer2
{
public:
    virtual ~Observer2() = default;
    virtual void onNotify(const T&, const string&) = 0;
};

class Quests2 : public Observer2<Unit2>
{
public:
    void onNotify(const Unit2& unit, const string& event) override {
        cout << "quest noti: " << event << endl;
    }
};

class Achievements2 : public Observer2<Unit2>
{
public:
    void onNotify(const Unit2& unit, const string& event) override {
        cout << "achievement noti: " << event << endl;
    }
};


template<typename T>
class Observable2
{
public:
    virtual ~Observable2() = default;

    void addObserver(Observer2<T>* obs);
    void removeObserver(Observer2<T>* obs);
    void notify(const T& obj, const string& event);

private:
    vector <Observer2<T>*> observers;
};

template<typename T>
void Observable2<T>::addObserver(Observer2<T>* obs)
{
    auto it = find(observers.begin(), observers.end(), obs);
    if (it != observers.end())
        return;

    observers.push_back(obs);
    cout << "add observer" << endl;
}

template<typename T>
void Observable2<T>::removeObserver(Observer2<T>* obs)
{
    auto it = remove(observers.begin(), observers.end(), obs);
    observers.erase(it, observers.end());
    cout << "remove observer" << endl;
}

template<typename T>
void Observable2<T>::notify(const T& obj, const string& event)
{
    for (auto* obs : observers)
        obs->onNotify(obj, event);
}


class Unit2 : public Observable2<Unit2>
{
public:
    int unitId() { return 1; }

    void jump();
    void swapWeapon();
    void useSkill(int skillId);
    void moveTo(int x, int y);
};

void Unit2::jump()
{
    cout << "unit jump" << endl;
    notify(*this, "jump");
}

void Unit2::swapWeapon()
{
    cout << "unit swapWeapon" << endl;
    notify(*this, "swapWeapon");
}

void Unit2::useSkill(int skillId)
{
    cout << "unit useSkill: " << skillId << endl;
    notify(*this, "useSkill");
}

void Unit2::moveTo(int x, int y)
{
    cout << "unit moveTo: " << x << ", " << y << endl;
    notify(*this, "moveTo");
}


void TestObserverTemplate()
{
    Unit2 unit;
    Quests2 quest;
    Achievements2 achievement;

    unit.addObserver(&quest);
    unit.addObserver(&achievement);

    unit.jump();
    unit.removeObserver(&quest);
    unit.swapWeapon();
    unit.useSkill(2);
    unit.removeObserver(&achievement);
    unit.moveTo(1002, 2345);
}



int main()
{
    TestObserverPattern();

    cout << endl << "using template =======" << endl;

    TestObserverTemplate();

    return 0;
}

