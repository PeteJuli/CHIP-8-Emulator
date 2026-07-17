#include "Chip8.hpp"
#include <algorithm>
#include <iostream>

//CHIP-8 Fontset (Pixelimages for 0 to F)
static constexpr std::array<uint8_t, 80> FONTSET = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8()
{ 
    //Copy Fontset at the begining of RAM (0x000 to 0x050)
    std::copy(FONTSET.begin(), FONTSET.end(), m_memory.begin());
}

bool Chip8::loadROM(std::span<const uint8_t> data)
{
    const size_t maxSize = RAM_SIZE - START_ADDRESS;

    if (data.size() > maxSize)
    {
        std::cerr << "[Chip8] -> ROM is to big!" << std::endl;
        return false;
    }

    //Copy Rom in memory
    std::copy(data.begin(), data.end(), m_memory.begin() + START_ADDRESS);
    
    return true;
}

void Chip8::tick()
{
    // 1. FETCH: Read opcode from RAM (combine 2 bytes)
    uint16_t opcode = (m_memory[m_pc] << 8) | m_memory[m_pc + 1];
    
    // Increment Program Counter by 2 bytes
    m_pc += 2;

    // Helper variables for decoding (Bitmasks)
    uint8_t  x   = (opcode & 0x0F00) >> 8;  // Register Vx index
    uint8_t  y   = (opcode & 0x00F0) >> 4;  // Register Vy index
    uint8_t  kk  = (opcode & 0x00FF);       // 8-bit constant
    uint16_t nnn = (opcode & 0x0FFF);       // 12-bit memory address

    // 2. DECODE & EXECUTE
    switch (opcode & 0xF000)//Bitmask to get the operation
    {
        case 0x0000:
            if (opcode == 0x00E0) // 0x00E0: CLS (Clear screen)
            {
                m_display.fill(0);
            }
            else if (opcode == 0x00EE) // 0x00EE: RET (Return from subroutine)
            {
                if (m_sp > 0)
                {
                    --m_sp;
                    m_pc = m_stack[m_sp];
                }
            }
            break;

        case 0x1000: // 1NNN: JP address (Jump to address NNN)
            m_pc = nnn;
            break;

        case 0x2000: // 2NNN: CALL address (Call subroutine)
            if (m_sp < m_stack.size())
            {
                m_stack[m_sp] = m_pc; // Save current PC to the stack
                ++m_sp;
                m_pc = nnn;           // Jump to subroutine
            }
            break;

        case 0x3000: // 3XKK: SE Vx, byte (Skip next instruction if Vx == KK)
            if (m_v[x] == kk) m_pc += 2;
            break;

        case 0x4000: // 4XKK: SNE Vx, byte (Skip next instruction if Vx != KK)
            if (m_v[x] != kk) m_pc += 2;
            break;

        case 0x5000: // 5XY0: SE Vx, Vy (Skip next instruction if Vx == Vy)
            if (m_v[x] == m_v[y]) m_pc += 2;
            break;

        case 0x6000: // 6XKK: LD Vx, byte (Set Vx = KK)
            m_v[x] = kk;
            break;

        case 0x7000: // 7XKK: ADD Vx, byte (Set Vx = Vx + KK, does not affect Carry-Flag)
            m_v[x] += kk;
            break;

        case 0x8000: // Arithmetic and Logical Operations
            switch (opcode & 0x000F)
            {
                case 0x0000: // 8XY0: LD Vx, Vy (Vx = Vy)
                    m_v[x] = m_v[y];
                    break;
                case 0x0001: // 8XY1: OR Vx, Vy (Bitwise OR)
                    m_v[x] |= m_v[y];
                    break;
                case 0x0002: // 8XY2: AND Vx, Vy (Bitwise UND)
                    m_v[x] &= m_v[y];
                    break;
                case 0x0003: // 8XY3: XOR Vx, Vy (Bitwise Exclusive OR)
                    m_v[x] ^= m_v[y];
                    break;
                case 0x0004: // 8XY4: ADD Vx, Vy (Addition with Carry-Flag in VF)
                    {
                        uint16_t sum = m_v[x] + m_v[y];
                        m_v[0xF] = (sum > 255) ? 1 : 0; // Set Carry-Flag
                        m_v[x] = static_cast<uint8_t>(sum & 0xFF);
                    }
                    break;
                case 0x0005: // 8XY5: SUB Vx, Vy (Vx = Vx - Vy, VF is set to 1 if no underflow)
                    {
                        uint8_t flag = (m_v[x] >= m_v[y]) ? 1 : 0;
                        m_v[x] -= m_v[y];
                        m_v[0xF] = flag; // Set flag AFTER operation to avoid overriding if x == 0xF
                    }
                    break;
                case 0x0006: // 8XY6: SHR Vx {, Vy} (Shift Right)
                    { 
                        uint8_t lsb = m_v[x] & 0x01;
                        m_v[x] >>= 1;
                        m_v[0xF] = lsb;
                    }
                    break;
                case 0x0007: // 8XY7: SUBN Vx, Vy (Vx = Vy - Vx, VF is set to 1 if no underflow)
                    {
                        uint8_t flag = (m_v[y] >= m_v[x]) ? 1 : 0;
                        m_v[x] = m_v[y] - m_v[x];
                        m_v[0xF] = flag;
                    }
                    break;
                case 0x000E: // 8XYE: SHL Vx {, Vy} (Shift Left)
                    {
                        uint8_t msb = (m_v[x] & 0x80) >> 7;
                        m_v[x] <<= 1;
                        m_v[0xF] = msb;
                    }
                    break;
            }
            break;

        case 0x9000: // 9XY0: SNE Vx, Vy (Skip next instruction if Vx != Vy)
            if (m_v[x] != m_v[y]) m_pc += 2;
            break;

        case 0xA000: // ANNN: LD I, address (Set index register I = NNN)
            m_i = nnn;
            break;

        case 0xB000: // BNNN: JP V0, address (Jump to address NNN + V0)
            m_pc = nnn + m_v[0];
            break;

        case 0xC000: // CXKK: RND Vx, byte (Vx = Random Byte AND KK)
            // Using modern random masking. 
            m_v[x] = static_cast<uint8_t>(rand() % 256) & kk;
            break;

        case 0xD000: // DXYN: DRW Vx, Vy, n(Draw sprite to screen)
            {
                uint8_t height = opcode & 0x000F;
                
                // Original behavior wraps coordinates if they start outside the display boundaries
                uint8_t start_x = m_v[x] % DISPLAY_WIDTH;
                uint8_t start_y = m_v[y] % DISPLAY_HEIGHT;

                m_v[0xF] = 0; // Clear collision flag by default

                for (unsigned int row = 0; row < height; ++row)
                {
                    // Stop drawing vertical pixels if reaching the bottom of the screen (Clipping)
                    if (start_y + row >= DISPLAY_HEIGHT) break; 

                    uint8_t spriteByte = m_memory[m_i + row];

                    for (unsigned int col = 0; col < 8; ++col)
                    {
                        // Stop drawing horizontal pixels if reaching the right edge (Clipping)
                        if (start_x + col >= DISPLAY_WIDTH) break; 

                        // Check if the current bit in the sprite byte is set to 1
                        if ((spriteByte & (0x80 >> col)) != 0)
                        {
                            size_t screenIndex = (start_x + col) + ((start_y + row) * DISPLAY_WIDTH);
                            
                            // CHIP-8 draws using XOR. If a pixel is overwritten, a collision occurs
                            if (m_display[screenIndex] == 1)
                            {
                                m_v[0xF] = 1; // Collision detected!
                            }
                            
                            m_display[screenIndex] ^= 1; // Flip the pixel using XOR
                        }
                    }
                }
            }
            break;

        case 0xE000:
            if (kk == 0x009E) // EX9E: SKP Vx (Skip next instruction if key in Vx is pressed)
            {
                if (m_v[x] < 16 && m_keys[m_v[x]]) m_pc += 2;
            }
            else if (kk == 0x00A1) // EXA1: SKNP Vx (Skip next instruction if key in Vx is NOT pressed)
            {
                if (m_v[x] < 16 && !m_keys[m_v[x]]) m_pc += 2;
            }
            break;

        case 0xF000:
            switch (kk)
            {
                case 0x0007: // FX07: LD Vx, DT (Set Vx = Current Delay Timer)
                    m_v[x] = m_delayTimer;
                    break;

                case 0x000A: // FX0A: LD Vx, K (Wait for key press, halting CPU execution)
                    {
                        bool key_pressed = false;
                        for (uint8_t i = 0; i < 16; ++i)
                        {
                            if (m_keys[i])
                            {
                                m_v[x] = i;
                                key_pressed = true;
                                break;
                            }
                        }
                        // Execution trick: Push PC back by 2 bytes to infinitely repeat this instruction until a key is pressed
                        if (!key_pressed) m_pc -= 2; 
                    }
                    break;

                case 0x0015: // FX15: LD DT, Vx (Set Delay Timer = Vx)
                    m_delayTimer = m_v[x];
                    break;

                case 0x0018: // FX18: LD ST, Vx (Set Sound Timer = Vx)
                    m_soundTimer = m_v[x];
                    break;

                case 0x001E: // FX1E: ADD I, Vx (Set I = I + Vx)
                    m_i += m_v[x];
                    break;

                case 0x0029: // FX29: LD F, Vx (Set I to the memory location of the hex sprite stored in Vx)
                    // Each font character is exactly 5 bytes large
                    m_i = m_v[x] * 5;
                    break;

                case 0x0033: // FX33: LD B, Vx (Store Binary-Coded Decimal (BCD) representation of Vx in memory)
                    // Breaks down a number (e.g. 254) into Hundreds, Tens, and Ones places
                    m_memory[m_i]     = m_v[x] / 100;
                    m_memory[m_i + 1] = (m_v[x] / 10) % 10;
                    m_memory[m_i + 2] = m_v[x] % 10;
                    break;

                case 0x0055: // FX55: LD [I], Vx (Store registers V0 through Vx in memory starting at location I)
                    for (uint8_t i = 0; i <= x; ++i)
                    {
                        m_memory[m_i + i] = m_v[i];
                    }
                    break;

                case 0x0065: // FX65: LD Vx, [I] (Read registers V0 through Vx from memory starting at location I)
                    for (uint8_t i = 0; i <= x; ++i)
                    {
                        m_v[i] = m_memory[m_i + i];
                    }
                    break;
            }
            break;

        default:
            std::cerr << "[Chip8] -> Unknown Opcode: 0x" << std::hex << opcode << std::dec << std::endl;
            break;
    }
}

void Chip8::updateTimers()
{
    if (m_delayTimer > 0)
        --m_delayTimer;

    //Need to be implemented
    if (m_soundTimer > 0)
        --m_soundTimer;
}

void Chip8::setKeyState(uint8_t key, bool pressed)
{
    if (key < 16)
    {
        m_keys[key] = pressed;
    }
}

const std::array<uint8_t, Chip8::DISPLAY_WIDTH * Chip8::DISPLAY_HEIGHT> &Chip8::getDisplayBuffer() const
{
    return m_display;
}