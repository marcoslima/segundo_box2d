#include <toolbox.h>
#include <imgui.h>

const char* ToolBox::window_id = "Tools";

ToolBox::ToolBox() 
{
    buttons[toolPointer] = ButtonSprite("assets/icons/809452_arrow_click_miscellaneous_pointer_select_icon.png");
    buttons[toolAddBox] = ButtonSprite("assets/icons/351984_crop_square_icon.png");
    buttons[toolAddCircle] = ButtonSprite("assets/icons/326565_blank_check_circle_icon.png");
    buttons[toolAddPoly] = ButtonSprite("assets/icons/9025719_line_segments_icon.png");
    buttons[toolAddHexagon] = ButtonSprite("assets/icons/9025624_hexagon_icon.png");
    buttons[toolJoint] = ButtonSprite("assets/icons/5288409_location_map_navigation_pin_point_icon.png");
    buttons[toolVarBox] = ButtonSprite("assets/icons/rect1.png");
}

void ToolBox::draw() 
{
    ImGuiWindowFlags window_flags =   ImGuiWindowFlags_NoResize
                                    | ImGuiWindowFlags_NoMove
                                    | ImGuiWindowFlags_NoCollapse
                                    | ImGuiWindowFlags_NoTitleBar
                                    | ImGuiWindowFlags_NoScrollbar
                                    | ImGuiWindowFlags_NoScrollWithMouse
                                    | ImGuiWindowFlags_AlwaysAutoResize
                                    ;
    ImVec2 window_size = ImVec2(64*ToolBox::TTool::toolQtd, 64);
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::BeginChild(ToolBox::window_id, window_size, false, window_flags);
    for (auto& button : buttons) 
    {
        if(button.second.draw(m_currentTool == button.first))
        {
            m_currentTool = button.first;
        }
    }
    ImGui::EndChild();
}

void ToolBox::setCurrentTool(TTool tool) 
{
    m_currentTool = tool;
}

ToolBox::TTool ToolBox::getCurrentTool()
{
    return m_currentTool;
}
