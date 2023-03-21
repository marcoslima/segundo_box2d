#pragma once
#include <map>
#include <button_sprite.h>

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
    ToolBox();
    void draw();
};
