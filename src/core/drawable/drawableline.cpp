#include "drawableline.h"

#include "properties.h"
#include "tools.h"

DrawableLine::DrawableLine(short x0, short y0, short x1, short y1, Color color)
	: DrawableElement(color), _x0(x0), _y0(y0), _x1(x1), _y1(y1)
{

}

void DrawableLine::display(DisplayManager *displayManager)
{
	displayManager->displayLine(_x0, _y0, _x1, _y1, _color);
}

void DrawableLine::displayMoving(DisplayManager *displayManager, int deltaX, int deltaY)
{
	displayManager->displayLine(_x0+deltaX, _y0+deltaY, _x1+deltaX, _y1+deltaY, Color::ghostMove);
}

QString DrawableLine::parametersToString()
{
	return "(" % QString::number(_x0) % ", " % QString::number(_y0) %
			")-(" % QString::number(_x1) % ", " % QString::number(_y1) % ")";
}

DrawableLine* DrawableLine::createLine(QDomElement &element)
{
	Color color = Tools::qStringToColor(element.attribute("color", "black"), Properties::defaultForegroundColor());
	bool okx0, oky0, okx1, oky1;
	int x0 = element.attribute("x0", "1000").toInt(&okx0);
	int y0 = element.attribute("y0", "1000").toInt(&oky0);
	int x1 = element.attribute("x1", "1000").toInt(&okx1);
	int y1 = element.attribute("y1", "1000").toInt(&oky1);
	if (okx0 && oky0 && (0<=x0) && (x0<=800) && (0<=y0) && (y0<=800) &&
		okx1 && oky1 && (0<=x1) && (x1<=800) && (0<=y1) && (y1<=800))
	{
		return new DrawableLine(x0, y0, x1, y1, color);
	}
	else
	{
		return nullptr;
	}
}

void DrawableLine::toXml(QDomElement* root)
{
	QDomElement element = root->ownerDocument().createElement("line");
	element.setAttribute("color", Tools::colorToQString(_color));
	element.setAttribute("x0", _x0);
	element.setAttribute("y0", _y0);
	element.setAttribute("x1", _x1);
	element.setAttribute("y1", _y1);
	root->appendChild(element);
}

void DrawableLine::move(int deltaX, int deltaY)
{
	_x0 += deltaX;
	_y0 += deltaY;
	_x1 += deltaX;
	_y1 += deltaY;
}
