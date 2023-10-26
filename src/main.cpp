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
#include "toolbox.h"
#include "SegundoDoc.h"
#include "SegundoVw.h"


int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Box2D - segundo");
    window.setFramerateLimit(60);
    if(!ImGui::SFML::Init(window, true))
    {
        std::cout << "Error initializing ImGui-SFML!" << std::endl;
        return 1;
    }
    ImGuiIO& io = ImGui::GetIO();

    CSegundoDoc doc;
    CSegundoVw view(&doc, window);

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

        window.clear();
        if(ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            view.OnLButtonDown(0, sf::Vector2i(ImGui::GetMousePos().x, ImGui::GetMousePos().y));
        }
        if(ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            view.OnLButtonUp(0, sf::Vector2i(ImGui::GetMousePos().x, ImGui::GetMousePos().y));
        }
        if(ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            view.OnMouseMove(io.KeyShift, sf::Vector2i(ImGui::GetMousePos().x, ImGui::GetMousePos().y));
        }
        if(ImGui::IsMouseDragging(ImGuiMouseButton_Right))
        {
            view.OnMouseMove(io.KeyShift, sf::Vector2i(ImGui::GetMousePos().x, ImGui::GetMousePos().y));
        }
        // TODO: Completar aqui
        // if(ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        // {
        //     view.OnRButtonUp(0, sf::Vector2i(ImGui::GetMousePos().x, ImGui::GetMousePos().y));
        // }
        
        view.OnDraw(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
