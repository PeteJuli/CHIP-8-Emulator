#pragma once

#include <SDL3/SDL.h>
#include <memory>

namespace SDL
{
    class Window
    {
      public:
        Window();

        bool eventHandling();
        void render();

      private:
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{nullptr, SDL_DestroyWindow};
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer{nullptr, SDL_DestroyRenderer};
    };
} // namespace SDL