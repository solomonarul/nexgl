#include "prefabs/timer.hpp"

#include <cmath>

using namespace NEX::Prefabs;

Timer::Timer(double rate) : rate(rate) {}

float Timer::progress(void)
{
    return this->value / this->rate;
}

void Timer::update(double dt)
{
    this->value += dt;
    if (this->value > this->rate)
    {
        this->ok = true;
        this->value = std::fmod(this->value, this->rate);
    }
}

bool Timer::has_ticked(void)
{
    if (!this->ok)
        return false;
    this->ok = false;
    return true;
}
