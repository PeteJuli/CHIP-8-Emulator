#include "Window.hpp"
#include <stdexcept>

SDL::Window::Window()
{
    // SDL Window
    SDL_Window *rawWindow = SDL_CreateWindow("CHIP-8-Emulator", 800, 600, 0);
    if (!rawWindow)
    {
        throw std::runtime_error("SDL Window Error!");
    }
    else
    {
        window.reset(rawWindow);
    }

    // SDL Renderer
    SDL_Renderer *rawRenderer = SDL_CreateRenderer(window.get(), nullptr);
    if (!rawRenderer)
    {
        throw std::runtime_error("SDL Renderer Error!");
    }
    else
    {
        renderer.reset(rawRenderer);
    }
}

bool SDL::Window::eventHandling()
{
    SDL_Event event;

    // OS-Events
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            return false;
        }
    }

    return true;
}

void SDL::Window::render()
{
    // Backgroundcolor
    SDL_SetRenderDrawColor(renderer.get(), 10, 92, 172, 255);
    SDL_RenderClear(renderer.get());

    // Present
    SDL_RenderPresent(renderer.get());
}