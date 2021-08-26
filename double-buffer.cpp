#include <iostream>
#include <thread>


// tested x64, c++17

using namespace std;

class Framebuffer
{
public:
    static const int HEIGHT = 10;
    static const int WIDTH = 10;
    static constexpr int SIZE = HEIGHT * WIDTH;

    Framebuffer() { clear(); }
    virtual ~Framebuffer() = default;

    void clear() {
        fill(&pixels[0], &pixels[0] + SIZE, '0');
    }

    void drawAll(char ch) {
        fill(&pixels[0], &pixels[0] + SIZE, ch);
    }

    void draw(int x, int y, char ch) {
        pixels[(HEIGHT * x) + y] = ch;
    }

    const char* getPixels() {
        return pixels;
    }

private:
    char pixels[SIZE];
};


class Scene
{
public:
    Scene() : current(&buffers[0]), next(&buffers[1]) {}
    virtual ~Scene() = default;

    void make(char ch);
    void render();

    Framebuffer& getBuffer() { return *current; }

private:
    Framebuffer  buffers[2];
    Framebuffer* current;
    Framebuffer* next;
};

void Scene::make(char ch)
{
    next->clear();
    next->drawAll(ch);

    char ch_1 = static_cast<char>(ch + 1);

    for (int x = 0; x < Framebuffer::HEIGHT; ++x) {
        next->draw(x, 4, ch_1);
        next->draw(x, 5, ch_1);
    }

    for (int y = 0; y < Framebuffer::WIDTH; ++y) {
        next->draw(4, y, ch_1);
        next->draw(5, y, ch_1);
    }

    swap(current, next);
}

void Scene::render()
{
    int loopCount = 0;

    while (loopCount < 7) {
        const char* pixels = getBuffer().getPixels();

        for (int x = 0; x < Framebuffer::HEIGHT; ++x) {
            for (int y = 0; y < Framebuffer::WIDTH; ++y)
                cout << pixels[(Framebuffer::HEIGHT * x) + y] << ' ';

            cout << endl;
        }

        ++loopCount;
        cout << " scene render end" << loopCount << endl;

        this_thread::sleep_for(70ms);
    }
}


int main()
{
    Scene scene;

    thread t1{ &Scene::render, &scene };

    char sceneCh = '0';

    // ascii, '0' ~ 'z', 48 ~ 122
    for (int i = 0; i <= 74; ++i) {
        sceneCh = static_cast<char>(sceneCh + i);
        scene.make(sceneCh);

        this_thread::sleep_for(60ms);
    }

    t1.join();

    return 0;
}

