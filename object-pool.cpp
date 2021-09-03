#include <iostream>
#include <variant>
#include <array>


// tested x64, c++17

using namespace std;

struct ParticleNub
{
    float x = 0.f;
    float y = 0.f;
    float xVel = 0.f;
    float yVel = 0.f;
};

class Particle
{
public:
    void init(int lifeTime, ParticleNub& nub) {
        frameOn = lifeTime;
        unionNub = nub;
    }

    bool inUse() const {
        return frameOn > 0;
    }

    Particle* getNext() const {
        return get<0>(unionNub);
    }

    void setNext(Particle* next) {
        unionNub = next;
    }

    void viewStatus() const {
        cout << (inUse() ? "in use: " : "not use: ") << frameOn << endl;
    }

    bool update();

private:
    int frameOn;
    variant<Particle*, ParticleNub> unionNub;
};

bool Particle::update()
{
    if (!inUse())
        return false;

    frameOn -= 1;
    ParticleNub& nub = get<1>(unionNub);
    nub.x += nub.xVel;
    nub.y += nub.yVel;

    return !inUse(); // return update end
}


class ParticlePool
{
public:
    ParticlePool();
    virtual ~ParticlePool() = default;

    void create(int lifeTime, ParticleNub& nub);
    void update();
    void viewStatus();

private:
    static const int POOL_SIZE = 8;
    array<Particle, POOL_SIZE> particles;

    Particle* usableHead;
};

ParticlePool::ParticlePool()
{
    usableHead = &particles[0];

    for (int i = 0; i < POOL_SIZE - 1; ++i)
        particles[i].setNext(&particles[i + 1]);

    particles.back().setNext(nullptr);
}

void ParticlePool::create(int lifeTime, ParticleNub& nub)
{
    if (usableHead == nullptr)
        return;

    // free list process
    Particle* newParticle = usableHead;
    usableHead = newParticle->getNext();

    newParticle->init(lifeTime, nub);
}

void ParticlePool::update()
{
    for (int i = 0; i < POOL_SIZE; i++) {
        if (!particles[i].update())
            continue;

        // update end, release 
        particles[i].setNext(usableHead);
        usableHead = &particles[i];
    }
}

void ParticlePool::viewStatus()
{
    int inUseCount = 0;

    for (auto& p : particles) {
        p.viewStatus();
        inUseCount += (p.inUse() ? 1 : 0);
    }

    cout << "in use count: " << inUseCount << endl;
}


int main()
{
    ParticleNub nub{ 1.f, 2.f, 0.1f, 0.2f };

    ParticlePool pool;
    pool.create(3, nub);
    pool.create(2, nub);
    pool.create(5, nub);
    pool.create(5, nub);
    pool.viewStatus();

    for (int i = 0; i < 3; ++i)
        pool.update();
    pool.viewStatus();

    pool.create(2, nub);
    pool.create(7, nub);
    pool.create(8, nub);
    pool.viewStatus();

    for (int i = 0; i < 3; ++i)
        pool.update();
    pool.viewStatus();

    return 0;
}

