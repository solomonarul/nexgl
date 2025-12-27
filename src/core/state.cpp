#include "core/state.hpp"

using namespace NEX::Core;

void StateManager::draw(int w, int h)
{
    for (auto& state : states) state->draw(w, h);
}

void StateManager::update(double dt)
{
    for (auto& state : states) state->update(dt);
}

void StateManager::send(SDL_Event& event)
{
    for (auto& state : states) state->event(event);
}
