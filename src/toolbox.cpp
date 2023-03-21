#include <toolbox.h>
#include <imgui.h>


ToolBox::ToolBox() 
{
    buttons[toolPointer] = ButtonSprite("assets/icons/809452_arrow_click_miscellaneous_pointer_select_icon.png");
    buttons[toolAddBox] = ButtonSprite("assets/icons/351984_crop_square_icon.png");
    buttons[toolAddCircle] = ButtonSprite("assets/icons/326565_blank_check_circle_icon.png");
    buttons[toolAddPoly] = ButtonSprite("assets/icons/9025719_line_segments_icon.png");
    buttons[toolAddHexagon] = ButtonSprite("assets/icons/9025624_hexagon_icon.png");
    buttons[toolJoint] = ButtonSprite("assets/icons/5288409_location_map_navigation_pin_point_icon.png");
}

void ToolBox::draw() {
    ImGui::Begin("Tools");
    for (auto& button : buttons) {
        if(button.second.draw(currentTool == button.first))
        {
            currentTool = button.first;
        }
    }
    ImGui::End();
}
