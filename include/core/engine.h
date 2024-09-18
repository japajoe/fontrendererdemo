#ifndef ENGINE_H
#define ENGINE_H

#include "application.h"
#include "screen.h"
#include "../rendering/font.h"
#include "../rendering/fontrenderer.h"
#include "../rendering/shaperenderer.h"

class Engine {
public:
    Engine();
    void run();
private:
    Application application;
    Screen screen;
    FontRenderer fontRenderer;
    ShapeRenderer shapeRenderer;
    Font font1;
    Font font2;
    Font font3;
};

#endif