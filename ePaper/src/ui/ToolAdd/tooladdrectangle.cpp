#include "tooladdrectangle.h"

#include "drawablerectangle.h"

ToolAddRectangle::ToolAddRectangle( Color color)
	: ToolAddElement(color), _valid0(false), _x0(0), _y0(0), _filled(false)
{

}

DrawableElement *ToolAddRectangle::manageClickEvent(QMouseEvent *e)
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
		DrawableRectangle *newRectangle = new DrawableRectangle(_x0, _y0, e->x(), e->y(), _filled, _color);
		reset();
		return newRectangle;
	}
}

bool ToolAddRectangle::manageMoveEvent(QMouseEvent *e)
{
	_xCurrent = e->x();
	_yCurrent = e->y();
	return _valid0;
}

void ToolAddRectangle::reset()
{
	_valid0 = false;
	_x0 = 0;
	_y0 = 0;
}

void ToolAddRectangle::paintGhostDraw(DisplayManager *displayManager)
{
	if (!_valid0)
	{
		return;
	}
	displayManager->displayRectangle(_x0,_y0, _xCurrent, _yCurrent, _filled, Color::ghostAdd);
}
