#include <imgui.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <map>


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

void ShiftButtonColor(ImGuiCol color)
{
    ImGui::PushStyleColor(color, ImVec4(1, 1, 1, 1));
}

void ShiftButtonHoveredColor(ImGuiCol color)
{
    ImGui::PushStyleColor(color, ImVec4(0.8f, 0.8f, 0.8f, 1));
}

void ShiftButtonActiveColor(ImGuiCol color)
{
    ImGui::PushStyleColor(color, ImVec4(0.6f, 0.6f, 0.6f, 1));
}

void ShiftButtonColors()
{
    ShiftButtonColor(ImGuiCol_Button);
    ShiftButtonHoveredColor(ImGuiCol_ButtonHovered);
    ShiftButtonActiveColor(ImGuiCol_ButtonActive);
}

bool ButtonSprite::draw(bool selected) {
    if (selected) ShiftButtonColors();

    bool result = ImGui::ImageButton(sprite);
    ImGui::SameLine();

    if (selected) ImGui::PopStyleColor(3);

    return result;
}
