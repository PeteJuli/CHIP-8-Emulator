#pragma once

#include "../SDL/Context/Context.hpp"
#include "../SDL/Window/Window.hpp"
#include "../Chip8/Chip8.hpp"
#include <string_view>

//Forward Declarations
union SDL_Event;

class Emulator
{
  public:
    Emulator();
    [[nodiscard]] bool loadGame(std::string_view path);
    void mainLoop();

  private:
    void inputHandle(const SDL_Event& event);

    //Create SDL Context first cause of deletion order for RAII
    SDL::Context m_context;
    SDL::Window m_window;
    bool m_created{false};

    Chip8 m_chip8;
};
