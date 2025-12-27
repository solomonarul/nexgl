#pragma once

#include <memory>
#include <vector>

#include "defines.hpp"

namespace NEX::Core
{
    struct StateManager;

    struct IState
    {
        IState(StateManager& sm) : state_manager(sm) {}

        StateManager& state_manager;
        virtual ~IState() = default;
        virtual void draw(int, int) = 0;
        virtual void update(double) = 0;
        virtual void event(SDL_Event&) = 0;
    };

    struct StateManager
    {
        void draw(int, int);
        void update(double);
        void send(SDL_Event&);

        std::vector<std::shared_ptr<IState>> states;
    };
}; // namespace NEX::Core

#define PUSH_STATE(sm, state, ...) sm.states.push_back(std::make_shared<state>(sm, ##__VA_ARGS__))
