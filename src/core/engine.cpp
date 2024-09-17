#include "engine.h"
#include "../external/glad.h"
#include "../rendering/font.h"
#include <iostream>

std::string demoText1 = "OpenGL Font Rendering";

std::string demoText2 = "This implementation is based on https://github.com/shreyaspranav/stb-truetype-example";

std::string demoText3 =
R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. 
Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. 
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. 
Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.)";

Engine::Engine() {
    size_t width = 800;
    size_t height = 600;
    application = Application(width, height, "OpenGL Font Rendering");
    screen.setWindowSize(width, height);
}

void Engine::run() {
    if(application.getWindow()) {
        std::cout << "Window is already created!\n";
        return;
    }
    
    application.initialize += [this] () {
        fontRenderer.initialize();
        font1 = Font("../data/BarlowCondensed-Regular.ttf", 64);
        font2 = Font("../data/sui.ttf", 64);
        font3 = Font("../data/Handjet-Regular.ttf", 64);
    };

    application.close += [this] () {
        fontRenderer.deinitialize();
        font1.destroy();
        font2.destroy();
        font3.destroy();
    };

    application.newFrame += [this] () {
        auto convertColor = [] (float r, float g, float b) {
            if(r > 1.0f)
                r /= 255.0f;
            if(g > 1.0f)
                g /= 255.0f;
            if(b > 1.0f)
                b /= 255.0f;
            return glm::vec4(r, g, b, 1.0f);
        };

        glm::vec4 bg = convertColor(37, 36, 34);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        fontRenderer.addText(&font3, demoText1, glm::vec2(5.0f, 0.0f), convertColor(124, 181, 24), 64.0f);
        fontRenderer.addText(&font2, demoText2, glm::vec2(5.0f, 65.0f), convertColor(255, 178, 56), 22.0f);
        fontRenderer.addText(&font1, demoText3, glm::vec2(5.0f, 85.0f), convertColor(128, 164, 237), 18.0f);

        fontRenderer.newFrame();
    };

    application.resize += [this] (int32_t width, int32_t height) {
        glViewport(0, 0, width, height);
        screen.setWindowSize(width, height);
    };

    application.keypress += [this] (int32_t key, int32_t scancode, int32_t action, int32_t mods) {

    };

    application.buttonPress += [this] (int32_t button, int32_t action, int32_t mods) {

    };

    application.mouseMove += [this] (double x, double y) {

    };

    application.mouseScroll += [this] (double x, double y) {

    };
    
    application.run();
}