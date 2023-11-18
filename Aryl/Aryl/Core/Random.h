#pragma once
#include <random>

namespace Aryl
{
    class Random
    {
    public:
        static void SetSeed(unsigned int seed);
        static int GetRandomInt(int min, int max);
        static float GetRandomFloat(float min, float max);

    private:
        static std::random_device myRandomDevice;
        static std::mt19937 myGenerator;
    };
}
