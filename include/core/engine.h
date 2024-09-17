#ifndef ENGINE_H
#define ENGINE_H

#include "application.h"
#include "screen.h"
#include "../rendering/fontrenderer.h"
#include "../rendering/font.h"

class Engine {
public:
    Engine();
    void run();
private:
    Application application;
    Screen screen;
    FontRenderer fontRenderer;
    Font font;
};

#endif