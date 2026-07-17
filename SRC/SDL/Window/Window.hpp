#pragma once

#include <memory>
#include <cstdint>

//Forward Declarations
struct SDL_Window;
struct SDL_Renderer;

extern "C" void SDL_DestroyWindow(SDL_Window* window);
extern "C" void SDL_DestroyRenderer(SDL_Renderer* renderer);

namespace SDL
{
    //Forward Declarations
    class Context;
    
    class Window
    {
      public:
        explicit Window(const Context& context);

        void render(const std::array<uint8_t, 64 * 32>& buffer);
        [[nodiscard]] bool isCreated() const;
        
      private:
        //Type Alias
        using WindowPtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
        using RendererPtr = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
        
        WindowPtr m_window{nullptr, SDL_DestroyWindow};
        RendererPtr m_renderer{nullptr, SDL_DestroyRenderer};
        bool m_created{false};
    };
} //namespace SDL