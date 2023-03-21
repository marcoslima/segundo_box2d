#include <imgui.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <map>
#include <toolbox.h>

#include <button_sprite.h>

sf::Texture loadTexture(const std::string& path)
{
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        std::cout << "Error loading texture: " << path << std::endl;
    }
    return texture;
}

ButtonSprite::ButtonSprite() {}
ButtonSprite::ButtonSprite(const std::string& path) {
    texture = loadTexture(path);
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f);
}

ButtonSprite::ButtonSprite(const ButtonSprite& other) {
    texture = other.texture;
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f);
}
ButtonSprite& ButtonSprite::operator=(const ButtonSprite& other) {
    texture = other.texture;
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f);
    return *this;
}

bool ButtonSprite::draw(bool selected) {
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
