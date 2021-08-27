#include <iostream>
#include <chrono>
#include <thread>


// tested x64, c++17

using namespace std;
using namespace std::chrono;

class GameLogic
{
public:
    GameLogic() {}
    virtual ~GameLogic() = default;

    void runLoop1();
    void runLoop2();
    void runLoop3();
    void runLoop4();

private:
    void processInput() {}
    void update() {}
    void update(nanoseconds& elapsed) {}
    void render() {}

public:
    static const int FPS = 60;
    static constexpr milliseconds MS_PER_FRAME = 1000ms / FPS;
    static constexpr milliseconds MS_PER_UPDATE = 8ms;
    static constexpr milliseconds LOOP_TIME = 1000ms;
};


void GameLogic::runLoop1()
{
    cout << "fast as possible loop for " << LOOP_TIME << endl;

    int loopCount{ 0 };
    nanoseconds run_time{ 0 };

    steady_clock::time_point begin = steady_clock::now();

    while (run_time < LOOP_TIME) {
        processInput();
        update();
        render();

        ++loopCount;

        steady_clock::time_point end = steady_clock::now();
        run_time = end - begin;
    }

    cout << "loop count: " << loopCount << endl;
}

void GameLogic::runLoop2()
{
    cout << "fixed frame rate loop for " << LOOP_TIME << endl;

    int loopCount{ 0 };
    nanoseconds run_time{ 0 };

    steady_clock::time_point begin = steady_clock::now();

    while (run_time < LOOP_TIME) {
        steady_clock::time_point begin_frame = steady_clock::now();

        processInput();
        update();
        render();

        ++loopCount;

        steady_clock::time_point end_frame = steady_clock::now();

        nanoseconds remain_frame = MS_PER_FRAME - (end_frame - begin_frame);
        this_thread::sleep_for(remain_frame);

        steady_clock::time_point end = steady_clock::now();
        run_time = end - begin;
    }

    cout << "loop count: " << loopCount << endl;

}

void GameLogic::runLoop3()
{
    cout << "fluid frame rate loop for " << LOOP_TIME << endl;

    int loopCount{ 0 };
    nanoseconds run_time{ 0 };

    steady_clock::time_point begin = steady_clock::now();
    steady_clock::time_point previous = steady_clock::now();

    while (run_time < LOOP_TIME) {
        steady_clock::time_point current = steady_clock::now();

        auto elapsed = current - previous;
        previous = current;

        processInput();
        update(elapsed);
        render();

        ++loopCount;

        steady_clock::time_point end = steady_clock::now();
        run_time = end - begin;
    }

    cout << "loop count: " << loopCount << endl;
}


void GameLogic::runLoop4()
{
    cout << "fixed update frame rate loop for " << LOOP_TIME << endl;

    int loopCount{ 0 };
    int updateCount{ 0 };

    nanoseconds run_time{ 0 };
    nanoseconds lag{ 0 };

    steady_clock::time_point begin = steady_clock::now();
    steady_clock::time_point previous = steady_clock::now();

    while (run_time < LOOP_TIME) {
        steady_clock::time_point current = steady_clock::now();

        auto elapsed = current - previous;
        previous = current;

        lag += elapsed;

        processInput();

        while (lag >= MS_PER_UPDATE) {
            update();

            ++updateCount;
            lag -= MS_PER_UPDATE;
        }

        render();

        ++loopCount;

        steady_clock::time_point end = steady_clock::now();
        run_time = end - begin;
    }

    cout << "loop count: " << loopCount << endl;
    cout << "update count: " << updateCount << endl;
}


int main()
{
    GameLogic game;

    game.runLoop1();
    game.runLoop2();
    game.runLoop3();
    game.runLoop4();

    return 0;
}

