#include "screen.h"

Screen *Screen::instance = nullptr;

Screen::Screen() {
    width = 512;
    height = 512;
    
    if(instance == nullptr) {
        instance = this;
    }
}

void Screen::setWindowSize(uint32_t width, uint32_t height) {
    this->width = width;
    this->height = height;
}