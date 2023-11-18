#include "Random.h"

namespace Aryl
{
    std::random_device Random::myRandomDevice;
    std::mt19937 Random::myGenerator(myRandomDevice());

    void Random::SetSeed(unsigned int seed)
    {
        myGenerator.seed(seed);
    }

    int Random::GetRandomInt(int min, int max)
    {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(myGenerator);
    }

    float Random::GetRandomFloat(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(myGenerator);
    }
}
