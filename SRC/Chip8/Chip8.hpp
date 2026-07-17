#pragma once

#include <array>
#include <cstdint>
#include <span>

class Chip8
{
  public:
    // Const
    static constexpr uint16_t START_ADDRESS = 0x200;
    static constexpr size_t RAM_SIZE = 4096;
    static constexpr size_t DISPLAY_WIDTH = 64;
    static constexpr size_t DISPLAY_HEIGHT = 32;

    Chip8();
    //~Chip8();

    //Loading ROM
    [[nodiscard]] bool loadROM(std::span<const uint8_t> data);

    //Executes exactly ONE CPU clock tick (Fetch, Decode, Execute)
    void tick();

    // Timer herunterzählen (muss mit 60Hz aufgerufen werden)
    void updateTimers();

    //Key Input(0x0 to 0xF)
    void setKeyState(uint8_t key, bool pressed);

    //Getter for SDL Window
    [[nodiscard]] const std::array<uint8_t, DISPLAY_WIDTH * DISPLAY_HEIGHT> &getDisplayBuffer() const;

  private:
    // Hardware Registers and Memory
    std::array<uint8_t, 4096> m_memory{}; // 4KB Main Memory
    std::array<uint8_t, 16> m_v{};        // 16 General-purpose 8-bit registers (V0 to VF)
    uint16_t m_i{};                       // 16-bit Index register (I) for memory addresses
    uint16_t m_pc{START_ADDRESS};         // Program counter (PC) - points to the current instruction

    // Stack
    std::array<uint16_t, 16> m_stack{};   // Stack for subroutines (function calls)
    uint8_t m_sp{};                       // Stack pointer

    //Timers count down at 60 Hz when greater than 0
    uint8_t m_delayTimer;
    uint8_t m_soundTimer;

    //Display Buffer
    std::array<uint8_t, DISPLAY_WIDTH * DISPLAY_HEIGHT> m_display{};

    //State of the 16 keys (true = pressed)
    std::array<bool, 16> m_keys{};
};