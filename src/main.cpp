#include <imgui.h> // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h

#include <imgui-SFML.h> // for ImGui::SFML::* functions and SFML-specific overloads

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

sf::Texture loadTexture(const std::string& path)
{
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        std::cout << "Error loading texture: " << path << std::endl;
    }
    return texture;
}

class ButtonSprite 
{
private:
    sf::Texture texture;
    sf::Sprite sprite;

private:


public:
    ButtonSprite() {}
    ButtonSprite(const std::string& path) {
        texture = loadTexture(path);
        sprite.setTexture(texture);
        sprite.setScale(0.5f, 0.5f);
    }

    ButtonSprite(const ButtonSprite& other) {
        texture = other.texture;
        sprite.setTexture(texture);
        sprite.setScale(0.5f, 0.5f);
    }
    ButtonSprite& operator=(const ButtonSprite& other) {
        texture = other.texture;
        sprite.setTexture(texture);
        sprite.setScale(0.5f, 0.5f);
        return *this;
    }

    bool draw(bool selected = false) {
        if (selected) {
            float h, s, v;
            ImVec4 bc = ImGui::GetStyleColorVec4(ImGuiCol_Button);
            ImGui::ColorConvertRGBtoHSV(bc.x, bc.y, bc.z, h, s, v);
            ImGui::PushStyleColor(ImGuiCol_Button,        (ImVec4)ImColor::HSV(h, s, v));

            bc = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
            ImGui::ColorConvertRGBtoHSV(bc.x, bc.y, bc.z, h, s, v);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(h, s, v));

            bc = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
            ImGui::ColorConvertRGBtoHSV(bc.x, bc.y, bc.z, h, s, v);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,  (ImVec4)ImColor::HSV(h, s, v));
        }

        bool result = ImGui::ImageButton(sprite);
        ImGui::SameLine();

        if (selected) {
            ImGui::PopStyleColor(3);
        }
        return result;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Box2D - segundo");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

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

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
