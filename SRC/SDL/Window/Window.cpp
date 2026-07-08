#include "Window.hpp"
#include "../Context/Context.hpp"
#include <SDL3/SDL.h>
#include <iostream>

SDL::Window::Window(const Context& context)
{
    if (!context.isCreated()) 
    {
        std::cerr << "[SDL::Window] -> Cannot create Window, SDL Context is invalid!" << std::endl;
        return;
    }

    //SDL Window
    SDL_Window *rawWindow = SDL_CreateWindow("CHIP-8-Emulator", 800, 600, 0);
    if (!rawWindow)
    {
        std::cerr << "[SDL::Window] -> Window Creation Failed!" << std::endl;

        return;
    }

    m_window = WindowPtr(rawWindow, SDL_DestroyWindow);

    //SDL Renderer
    SDL_Renderer *rawRenderer = SDL_CreateRenderer(m_window.get(), nullptr);
    if (!rawRenderer)
    {
        std::cerr << "[SDL::Window] -> Renderer Creation Failed!" << std::endl;

        return;
    }
    
    m_renderer = RendererPtr(rawRenderer, SDL_DestroyRenderer);

    m_created = true;
}

void SDL::Window::render()
{
    if (!m_created) return;

    //Backgroundcolor
    SDL_SetRenderDrawColor(m_renderer.get(), 10, 92, 172, 255);
    SDL_RenderClear(m_renderer.get());

    //Present
    SDL_RenderPresent(m_renderer.get());
}

bool SDL::Window::eventHandling()
{
    SDL_Event event;

    //OS-Events
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            return false;
        }
    }

    return true;
}

bool SDL::Window::isCreated() const
{
    return m_created;
}