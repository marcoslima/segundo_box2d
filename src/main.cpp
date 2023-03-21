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

class ToolBox
{
public:
	typedef enum eTools
	{
		toolPointer,
		toolAddBox,
		toolAddCircle,
		toolAddPoly,
		toolAddHexagon,
		toolJoint,
		toolQtd
	} TTool;

private:
    std::map<TTool, ButtonSprite> buttons;
    TTool currentTool = toolPointer;

public:
    ToolBox() {
        buttons[toolPointer] = ButtonSprite("assets/icons/809452_arrow_click_miscellaneous_pointer_select_icon.png");
        buttons[toolAddBox] = ButtonSprite("assets/icons/351984_crop_square_icon.png");
        buttons[toolAddCircle] = ButtonSprite("assets/icons/326565_blank_check_circle_icon.png");
        buttons[toolAddPoly] = ButtonSprite("assets/icons/9025719_line_segments_icon.png");
        buttons[toolAddHexagon] = ButtonSprite("assets/icons/9025624_hexagon_icon.png");
        buttons[toolJoint] = ButtonSprite("assets/icons/5288409_location_map_navigation_pin_point_icon.png");
    }

    void draw() {
        ImGui::Begin("Tools");
        for (auto& button : buttons) {
            if(button.second.draw(currentTool == button.first))
            {
                currentTool = button.first;
            }
        }
        ImGui::End();
    }
};


int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Box2D - segundo");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    ToolBox toolbox;

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
