#include "drawablepoint.h"

#include "properties.h"
#include "tools.h"

DrawablePoint::DrawablePoint(short x, short y, Color color)
	: DrawableElement(color), _x(x), _y(y)
{

}

void DrawablePoint::display(DisplayManager *displayManager)
{
	displayManager->displayPoint(_x, _y, _color);
}

void DrawablePoint::displayMoving(DisplayManager *displayManager, int deltaX, int deltaY)
{
	displayManager->displayPoint(_x+deltaX, _y+deltaY, Color::ghostMove);
}

QString DrawablePoint::parametersToString()
{
	return "(" % QString::number(_x) % ", " % QString::number(_y) % ")";
}

DrawablePoint* DrawablePoint::createPoint(QDomElement &element)
{
	Color color = Tools::qStringToColor(element.attribute("color", "black"), Properties::defaultForegroundColor());
	bool okx, oky;
	int x = element.attribute("x", "1000").toInt(&okx);
	int y = element.attribute("y", "1000").toInt(&oky);
	if (okx && oky && (0<=x) && (x<=800) && (0<=y) && (y<=800))
	{
		return new DrawablePoint(x, y, color);
	}
	else
	{
		return nullptr;
	}
}

void DrawablePoint::toXml(QDomElement* root)
{
	QDomElement element = root->ownerDocument().createElement("point");
	element.setAttribute("color", Tools::colorToQString(_color));
	element.setAttribute("x", _x);
	element.setAttribute("y", _y);
	root->appendChild(element);
}

void DrawablePoint::move(int deltaX, int deltaY)
{
	_x += deltaX;
	_y += deltaY;
}
