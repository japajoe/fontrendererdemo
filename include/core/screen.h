#ifndef SCREEN_H
#define SCREEN_H

#include <cstdint>

class Screen {
friend class Engine;
public:
    Screen();
    inline static Screen *getInstance() { return instance; }
    inline uint32_t getWidth() const { return width; }
    inline uint32_t getHeight() const { return height; }
private:
    uint32_t width;
    uint32_t height;
    static Screen *instance;
    void setWindowSize(uint32_t width, uint32_t height);
};

#endif