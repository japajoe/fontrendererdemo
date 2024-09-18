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
        shapeRenderer.initialize();
        font1 = Font("../data/BarlowCondensed-Regular.ttf", 64);
        font2 = Font("../data/sui.ttf", 64);
        font3 = Font("../data/Handjet-Regular.ttf", 64);
    };

    application.close += [this] () {
        fontRenderer.deinitialize();
        shapeRenderer.deinitialize();
        font1.destroy();
        font2.destroy();
        font3.destroy();
    };

    uint64_t counter = 0;

    application.newFrame += [&] () {
        auto getColor = [] (int r, int g, int b, int a) {
            glm::vec4 col;
            col.x = r > 0 ? ((float)r / 255) : 0;
            col.y = g > 0 ? ((float)g / 255) : 0;
            col.z = b > 0 ? ((float)b / 255) : 0;
            col.w = a > 0 ? ((float)a / 255) : 0;
            return col;
        };

        glm::vec4 bg = getColor(37, 36, 34, 255);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float alpha = (sinf(counter * 0.03) + 1.0f) * 0.5f;

        fontRenderer.addText(&font3, demoText1, glm::vec2(5.0f, 0.0f), getColor(124, 181, 24, alpha * 255), 64.0f);
        fontRenderer.addText(&font2, demoText2, glm::vec2(5.0f, 65.0f), getColor(255, 178, 56, 255), 22.0f);
        fontRenderer.addText(&font1, demoText3, glm::vec2(5.0f, 85.0f), getColor(128, 164, 237, 255), 18.0f);

        shapeRenderer.addRectangle(glm::vec2(5, 5), glm::vec2(430, 60), getColor(158, 158, 158, alpha * 64));

        shapeRenderer.newFrame();
        fontRenderer.newFrame();

        counter++;
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