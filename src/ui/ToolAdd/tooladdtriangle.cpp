#include "tooladdtriangle.h"

#include "drawabletriangle.h"

ToolAddTriangle::ToolAddTriangle(Color color)
	: ToolAddElement(color), _valid0(false), _x0(0), _y0(0), _valid1(false), _x1(0), _y1(0), _filled(false)
{

}

DrawableElement *ToolAddTriangle::manageClickEvent(QMouseEvent *e)
{
	if (!_valid0)
	{
		_x0 = e->x();
		_y0 = e->y();
		_valid0 = true;
		return nullptr;
	}
	else if (!_valid1)
	{
		_x1 = e->x();
		_y1 = e->y();
		_valid1 = true;
		return nullptr;
	}
	else
	{
		DrawableTriangle *newTriangle = new DrawableTriangle(_x0, _y0, _x1, _y1, e->x(), e->y(), _filled, _color);
		reset();
		return newTriangle;
	}
}

bool ToolAddTriangle::manageMoveEvent(QMouseEvent *e)
{
	_xCurrent = e->x();
	_yCurrent = e->y();
	return _valid0;
}

void ToolAddTriangle::reset()
{
	_valid0 = false;
	_x0 = 0;
	_y0 = 0;
	_valid1 = false;
	_x1 = 0;
	_y1 = 0;
}

void ToolAddTriangle::paintGhostDraw(DisplayManager *displayManager)
{
	if (!_valid0)
	{
		return;
	}

	if (!_valid1)
	{
		displayManager->displayLine(_x0,_y0,_xCurrent,_yCurrent, Color::ghostAdd);
	}
	else
	{
		displayManager->displayTriangle(_x0,_y0,_x1, _y1,_xCurrent,_yCurrent,_filled, Color::ghostAdd);
	}
}
