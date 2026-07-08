#pragma once

#include <memory>

//Forward Declarations
struct SDL_Window;
struct SDL_Renderer;

namespace SDL
{
    //Forward Declarations
    class Context;
    
    class Window
    {
      public:
        explicit Window(const Context& context);

        void render();
        bool eventHandling();
        [[nodiscard]] bool isCreated() const;
        
      private:
        //Type Alias
        using WindowPtr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
        using RendererPtr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
        
        WindowPtr m_window{nullptr, nullptr};
        RendererPtr m_renderer{nullptr, nullptr};
        bool m_created{false};
    };
} //namespace SDL