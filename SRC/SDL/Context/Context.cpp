#include "Context.hpp"
#include <stdexcept>

SDL::Context::Context()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        throw std::runtime_error("SDL_Init Error!");
    }

    created = true;
}

SDL::Context::~Context()
{
    if (created)
    {
        SDL_Quit();
    }
}
