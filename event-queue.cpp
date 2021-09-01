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
    static Audio& instance() {
        static Audio instance;
        return instance;
    }

    void playSound(int soundId, int volume);
    void update();

private:
    Audio() = default;
    ~Audio() = default;

    static const int MAX_SIZE = 16;
    array<SoundMessage, MAX_SIZE> queue;

    atomic<int> head = 0;
    atomic<int> tail = 0;
};

void Audio::playSound(int soundId, int volume)
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

void Audio::update()
{
    int soundId = 0;

    while (soundId != -1) {
        this_thread::sleep_for(100ms);

        if (head == tail)
            continue;

        // ResourceId resource = loadSound(queue[head].soundId);
        // int channel = findOpenChannel();
        // if (channel == -1) return;
        // startSound(resource, channel, queue[head].volume);

        soundId = queue[head].soundId;
        int volume = queue[head].volume;

        cout << "start sound: " << soundId << ", " << volume << endl;

        head = (head + 1) % MAX_SIZE;
    }
}


int main()
{
    for (int i = 1; i < 10; ++i)
        Audio::instance().playSound(i, i + 1);

    thread t1{ &Audio::update, ref(Audio::instance()) };

    this_thread::sleep_for(600ms);

    for (int i = 100; i < 105; ++i)
        Audio::instance().playSound(i, i + 1);

    Audio::instance().playSound(-1, 0); // for end

    t1.join();

    return 0;
}

