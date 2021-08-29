#include <iostream>
#include <vector>
#include <string>
#include <string_view>


// tested x64, c++17

using namespace std;

class AlarmSystem {};
class UISystem {};
class SoundSystem {};
class ParticleSystem {};

AlarmSystem alarmSys;
UISystem uiSys;
SoundSystem soundSys;
ParticleSystem particleSys;


class Effect
{
public:
    Effect() { init(); }
    virtual ~Effect() = default;

    virtual void activate() = 0;

protected:
    void alarmText(string_view type, string_view content) {
        cout << type << ": " << content << endl; // alarm->notice();
    }

    void showUI(string_view type, string_view content) {
        cout << type << ": " << content << endl; // ui->show();
    }

    void playSound(string_view type, string_view content) {
        cout << type << ": " << content << endl; // sound->play();
    }

    void spawnParticle(string_view type, string_view content) {
        cout << type << ": " << content << endl; // particle->spawn();
    }

private:
    void init() {
        alarm = &alarmSys;
        ui = &uiSys;
        sound = &soundSys;
        particle = &particleSys;
    }

private:
    AlarmSystem* alarm;
    UISystem* ui;
    SoundSystem* sound;
    ParticleSystem* particle;
};


class FireEffect : public Effect
{
public:
    FireEffect(string_view type) : type(type) {}
    virtual ~FireEffect() = default;

protected:
    virtual void activate() override;

private:
    string type;
};

void FireEffect::activate()
{
    if (type == "fire ball") {
        playSound(type, "casting sound");
        spawnParticle(type, "spark ember");
    }
    else if (type == "fire wall") {
        playSound(type, "flame sound");
        spawnParticle(type, "fire stream");
        spawnParticle(type, "fire particle");
    }
    else if (type == "fire hand") {
        alarmText(type, "notice status");
        showUI(type, "caution icon");
        playSound(type, "flame sound");
    }
}


class IceEffect : public Effect
{
public:
    IceEffect(string_view type) : type(type) {}
    virtual ~IceEffect() = default;

protected:
    virtual void activate() override;

private:
    string type;
};

void IceEffect::activate()
{
    if (type == "ice storm") {
        showUI(type, "show ice wind");
        playSound(type, "storm sound 1");
        playSound(type, "storm sound 2");
        spawnParticle(type, "snow");
    }
    else if (type == "ice arrow") {
        playSound(type, "arrow sound");
        spawnParticle(type, "ice particle");
    }
    else if (type == "ice shield") {
        alarmText(type, "status change");
        showUI(type, "shield icon");
        playSound(type, "equip shield");
    }
}


int main()
{
    vector<unique_ptr<Effect>> effects;

    effects.push_back(make_unique<FireEffect>("fire ball"));
    effects.push_back(make_unique<FireEffect>("fire wall"));
    effects.push_back(make_unique<FireEffect>("fire hand"));
    effects.push_back(make_unique<IceEffect>("ice storm"));
    effects.push_back(make_unique<IceEffect>("ice arrow"));
    effects.push_back(make_unique<IceEffect>("ice shield"));

    for (auto& eff : effects)
        eff->activate();

    return 0;
}

