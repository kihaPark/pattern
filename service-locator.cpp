#include <iostream>
#include <array>
#include <thread>
#include <atomic>


// tested x64, c++17

using namespace std;

struct SoundMessage
{
    int soundId = 0;
    int volume = 0;
};


class Audio
{
public:
    virtual ~Audio() = default;

    virtual void playSound(int soundId, int volume) = 0;
    virtual void update() = 0;
};

class NullAudio : public Audio
{
public:
    virtual ~NullAudio() = default;

    virtual void playSound(int soundId, int volume) override {
        cout << "null audio play sound: " << soundId << ", " << volume << endl;
    }

    virtual void update() override {
        cout << "null audio update" << endl;
    }
};

class ConsoleAudio : public Audio
{
public:
    virtual ~ConsoleAudio() = default;

    virtual void playSound(int soundId, int volume) override;
    virtual void update() override;

private:
    static const int MAX_SIZE = 16;
    array<SoundMessage, MAX_SIZE> queue;

    atomic<int> head = 0;
    atomic<int> tail = 0;
};

void ConsoleAudio::playSound(int soundId, int volume)
{
    for (int i = head; i != tail; i = (i + 1) % MAX_SIZE) {
        if (queue[i].soundId != soundId)
            continue;

        // skip same sound, use the larger of the two volumes
        queue[i].volume = max(volume, queue[i].volume);
        return;
    }

    cout << "queueing sound: " << soundId << ", " << volume << endl;

    queue[tail].soundId = soundId;
    queue[tail].volume = volume;

    tail = (tail + 1) % MAX_SIZE;
}

void ConsoleAudio::update()
{
    int soundId = 0;

    while (soundId != -1) {
        this_thread::sleep_for(100ms);

        if (head == tail)
            continue;

        soundId = queue[head].soundId;
        int volume = queue[head].volume;

        cout << "start sound: " << soundId << ", " << volume << endl;

        head = (head + 1) % MAX_SIZE;
    }
}

// decorator pattern
class LoggedAudio : public Audio
{
public:
    LoggedAudio(Audio& wrapped) : wrapped(wrapped) {}
    virtual ~LoggedAudio() = default;

    virtual void playSound(int soundId, int volume) override {
        cout << "log audio play sound: " << soundId << ", " << volume << endl;
        wrapped.playSound(soundId, volume);
    }

    virtual void update() override {
        cout << "log audio update begin" << endl;
        wrapped.update();
        cout << "log audio update end" << endl;
    }

private:
    Audio& wrapped;
};


class Locator
{
public:
    static Audio& getAudio() {
        return *service;
    }

    static void provide(Audio* audio) {
        service = (audio == nullptr) ? &nullService : audio;
    }

private:
    static inline NullAudio nullService;
    static inline Audio* service = &nullService;
};


int main()
{
    ConsoleAudio csAudio;
    Locator::provide(&csAudio);

    LoggedAudio logAudio(Locator::getAudio());
    Locator::provide(&logAudio);

    for (int i = 1; i < 10; ++i)
        Locator::getAudio().playSound(i, i + 1);

    thread t1{ &Audio::update, ref(Locator::getAudio()) };

    this_thread::sleep_for(600ms);

    for (int i = 100; i < 105; ++i)
        Locator::getAudio().playSound(i, i + 1);

    Locator::getAudio().playSound(-1, 0); // for end

    t1.join();

    return 0;
}

