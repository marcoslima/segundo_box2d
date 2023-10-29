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

int main() 
{
    uint64_t screenWidth = 1920;
    uint64_t screenHeight = 1080;
    double aspectRatio = (double)screenWidth / (double)screenHeight;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Box2D - segundo");
    window.setFramerateLimit(60);
    if(!ImGui::SFML::Init(window, true))
    {
        std::cout << "Error initializing ImGui-SFML!" << std::endl;
        return 1;
    }
    ImGuiIO& io = ImGui::GetIO();

    CSegundoDoc doc(aspectRatio);
    CSegundoVw view(&doc, window);

    sf::Clock deltaClock;
    bool bMouseDown = false;
    sf::Vector2i ptMouse, lastPtMouse;
    while (window.isOpen()) 
    {

        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear();

        ptMouse = sf::Vector2i(ImGui::GetMousePos().x, ImGui::GetMousePos().y);

        if(ptMouse != lastPtMouse)
        {
            view.OnMouseMove(io.KeyShift, ptMouse);
        }
        lastPtMouse = ptMouse;

        if(! bMouseDown && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            bMouseDown = true;
            view.OnLButtonDown(0, ptMouse);
        }
        if(bMouseDown && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            bMouseDown = false;
            view.OnLButtonUp(0, ptMouse);
        }

        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            view.OnLButtonClicked(0, ptMouse);
        }

        view.OnDraw(window);
        ImGui::SFML::Render(window);
        window.display();

    }

    ImGui::SFML::Shutdown();

    return 0;
}
