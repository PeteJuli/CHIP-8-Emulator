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
    SDL_Window *rawWindow = SDL_CreateWindow("CHIP-8-Emulator", 800, 600, 0);//Hardcoded window size...
    if (!rawWindow)
    {
        std::cerr << "[SDL::Window] -> Window Creation Failed!" << std::endl;

        return;
    }

    m_window.reset(rawWindow); 

    //SDL Renderer
    SDL_Renderer *rawRenderer = SDL_CreateRenderer(m_window.get(), nullptr);
    if (!rawRenderer)
    {
        std::cerr << "[SDL::Window] -> Renderer Creation Failed!" << std::endl;

        return;
    }
    
    m_renderer.reset(rawRenderer);

    m_created = true;
}

void SDL::Window::render(const std::array<uint8_t, 64 * 32>& buffer)
{
    if (!m_created) return;

    //Backgroundcolor
    SDL_SetRenderDrawColor(m_renderer.get(), 10, 92, 172, 255);
    SDL_RenderClear(m_renderer.get());

    //calc CHIP-8 Pixel on 800x600 Window
    constexpr float pixel_width = 800.0f / 64.0f;
    constexpr float pixel_height = 600.0f / 32.0f;

    // draw pixel
    SDL_SetRenderDrawColor(m_renderer.get(), 255, 255, 255, 255);

    for (int y = 0; y < 32; ++y)
    {
        for (int x = 0; x < 64; ++x)
        {
            if (buffer[x + (y * 64)] == 1)
            {
                SDL_FRect rect{
                    static_cast<float>(x) * pixel_width,
                    static_cast<float>(y) * pixel_height,
                    pixel_width,
                    pixel_height
                };
                SDL_RenderFillRect(m_renderer.get(), &rect);
            }
        }
    }

    //Present
    SDL_RenderPresent(m_renderer.get());
}

bool SDL::Window::isCreated() const
{
    return m_created;
}