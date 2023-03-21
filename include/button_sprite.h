#pragma once
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>

class ButtonSprite 
{
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    ButtonSprite();
    ButtonSprite(const std::string& path);
    ButtonSprite(const ButtonSprite& other);
    ButtonSprite& operator=(const ButtonSprite& other);
    bool draw(bool selected = false);
};
