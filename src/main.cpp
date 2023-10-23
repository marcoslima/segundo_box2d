#include <iostream>
#include <map>
#include <imgui.h> // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h

#include <imgui-SFML.h> // for ImGui::SFML::* functions and SFML-specific overloads

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <toolbox.h>
#include "SegundoDoc.h"


int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Box2D - segundo");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    ToolBox toolbox;
    CSegundoDoc doc;

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::ShowDemoWindow();

        toolbox.draw();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
