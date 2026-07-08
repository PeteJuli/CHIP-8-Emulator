#include "Context.hpp"
#include <SDL3/SDL.h>
#include <iostream>

SDL::Context::Context()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "[SDL::Context] -> Init Failed!" << std::endl;

        return;
    }
    
     m_created = true;
}

SDL::Context::~Context()
{
    if (m_created)//Only CleanUp if its necessary
    {
        SDL_Quit();
    }
}

bool SDL::Context::isCreated() const
{
    return m_created;
}
