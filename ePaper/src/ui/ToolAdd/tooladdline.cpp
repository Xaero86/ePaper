#include "tooladdline.h"

#include "drawableline.h"

ToolAddLine::ToolAddLine(Color color)
	: ToolAddElement(color), _valid0(false), _x0(0), _y0(0)
{

}

DrawableElement *ToolAddLine::manageClickEvent(QMouseEvent *e)
{
	if (!_valid0)
	{
		_x0 = e->x();
		_y0 = e->y();
		_valid0 = true;
		return nullptr;
	}
	else
	{
		DrawableLine *newline = new DrawableLine(_x0, _y0, e->x(), e->y(), _color);
		reset();
		return newline;
	}
}

bool ToolAddLine::manageMoveEvent(QMouseEvent *e)
{
	_xCurrent = e->x();
	_yCurrent = e->y();
	return _valid0;
}

void ToolAddLine::reset()
{
	_valid0 = false;
	_x0 = 0;
	_y0 = 0;
}

void ToolAddLine::paintGhostDraw(DisplayManager *displayManager)
{
	if (!_valid0)
	{
		return;
	}
	displayManager->displayLine(_x0,_y0,_xCurrent,_yCurrent, Color::ghostAdd);
}
