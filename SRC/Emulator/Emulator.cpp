#include "Emulator.hpp"
#include "SDL3/SDL.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

Emulator::Emulator() : m_context(), m_window(m_context)
{
    //Check that everything is set up correctly
    if (!m_context.isCreated() || !m_window.isCreated())
    {
        std::cerr << "[Emulator] -> Critical Error SDL initialization failed!" << std::endl;
        return;
    }

    m_created = true;
}

bool Emulator::loadGame(std::string_view path)
{
    //Get size of ROM
    auto size = std::filesystem::file_size(path);
    if (size == NULL) 
    {
        std::cerr << "[Emulator] -> Error: Could not access file Size -> path: " << path << std::endl;
        return false;
    }
    
    //Open ROM file
    std::ifstream file(path.data(), std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "[Emulator] -> Error ROM could not be opened -> path: " << path << std::endl;
        return false;
    }

    //Read the ROM
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
    {
        std::cerr << "[Emulator] -> Error ROM could not be read -> path: " << path << std::endl;
        return false;
    }

    //Send it to Chip8
    return m_chip8.loadROM(std::span(buffer.data(), buffer.size()));
}

void Emulator::inputHandle(const SDL_Event& event)//For German Keyboard
{
    //SDL3 Key-Mapping
    auto map_key = [](SDL_Keycode key) -> int {
        switch (key) {
            case SDLK_1: return 0x1; case SDLK_2: return 0x2; case SDLK_3: return 0x3; case SDLK_4: return 0xC;
            case SDLK_Q: return 0x4; case SDLK_W: return 0x5; case SDLK_E: return 0x6; case SDLK_R: return 0xD;
            case SDLK_A: return 0x7; case SDLK_S: return 0x8; case SDLK_D: return 0x9; case SDLK_F: return 0xE;
            case SDLK_Y: return 0xA; case SDLK_X: return 0x0; case SDLK_C: return 0xB; case SDLK_V: return 0xF;
            default: return -1;
        }
    };

    if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
    {
        int chip8_key = map_key(event.key.key);
        if (chip8_key != -1)
        {
            m_chip8.setKeyState(static_cast<uint8_t>(chip8_key), event.type == SDL_EVENT_KEY_DOWN);
        }
    }
}

void Emulator::mainLoop()
{
    if (!m_created)
    {
        return;
    }

    bool running = true;

    uint64_t lastTimer = SDL_GetTicks();

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            inputHandle(event); //Get Inputs
        }

        //CPU Tick (~500Hz bis 700Hz)
        m_chip8.tick();

        //CHIP-8 Timer decrease with 60Hz (approximately every 16ms)
        uint64_t currentTime = SDL_GetTicks();
        if (currentTime - lastTimer >= 16)
        {
            m_chip8.updateTimers();
            currentTime = currentTime;
        }

        //Update renderer
        m_window.render(m_chip8.getDisplayBuffer());

        SDL_Delay(1); 
    }
}