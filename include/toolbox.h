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
		toolVarBox,
		toolQtd
	} TTool;
	static const char* window_id;

private:
    std::map<TTool, ButtonSprite> buttons;
    TTool m_currentTool = toolPointer;

public:
    ToolBox();
    void draw();
	void setCurrentTool(TTool tool);
	TTool getCurrentTool();
};
