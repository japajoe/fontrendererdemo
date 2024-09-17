#include "engine.h"
#include "../external/glad.h"
#include "../rendering/font.h"
#include <iostream>

std::string demoText =
R"(Font rendering is such a pain in the ass -_-
That said, big thanks to https://github.com/shreyaspranav/stb-truetype-example)";

Engine::Engine() {
    size_t width = 512;
    size_t height = 512;
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
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        fontRenderer.addText(&font2, demoText, glm::vec2(10.0f, 22.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), 32.0f);
        fontRenderer.addText(&font1, "This works", glm::vec2(10.0f, 72.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 22.0f);
        fontRenderer.addText(&font3, "Noice", glm::vec2(10.0f, 122.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 64.0f);
        
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