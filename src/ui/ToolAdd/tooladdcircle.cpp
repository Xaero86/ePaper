#include "tooladdcircle.h"

#include "drawablecircle.h"

ToolAddCircle::ToolAddCircle(Color color)
	: ToolAddElement(color), _valid(false), _x(0), _y(0), _filled(false)
{

}

DrawableElement *ToolAddCircle::manageClickEvent(QMouseEvent *e)
{
	if (!_valid)
	{
		_x = e->x();
		_y = e->y();
		_valid = true;
		return nullptr;
	}
	else
	{
		unsigned int radius = QLineF(_x, _y, e->x(), e->y()).length();
		DrawableCircle *newCircle = new DrawableCircle(_x, _y, radius, _filled, _color);
		reset();
		return newCircle;
	}
}

bool ToolAddCircle::manageMoveEvent(QMouseEvent *e)
{
	_xCurrent = e->x();
	_yCurrent = e->y();
	return _valid;
}

void ToolAddCircle::reset()
{
	_valid = false;
	_x = 0;
	_y = 0;
}

void ToolAddCircle::paintGhostDraw(DisplayManager *displayManager)
{
	if (!_valid)
	{
		return;
	}

	unsigned int radius = QLineF(_x, _y, _xCurrent, _yCurrent).length();
	displayManager->displayCircle(_x, _y, radius, _filled, Color::ghostAdd);
}
