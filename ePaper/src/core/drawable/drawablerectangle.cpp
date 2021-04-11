#include "drawablerectangle.h"

#include "properties.h"
#include "tools.h"

DrawableRectangle::DrawableRectangle(short x0, short y0, short x1, short y1, bool filled, Color color)
	: DrawableElement(color), _x0(x0), _y0(y0), _x1(x1), _y1(y1), _filled(filled)
{

}

void DrawableRectangle::display(DisplayManager *displayManager)
{
	displayManager->displayRectangle(_x0, _y0, _x1, _y1, _filled, _color);
}

void DrawableRectangle::displayMoving(DisplayManager *displayManager, int deltaX, int deltaY)
{
	displayManager->displayRectangle(_x0+deltaX, _y0+deltaY, _x1+deltaX, _y1+deltaY, _filled, Color::ghostMove);
}

QString DrawableRectangle::toString()
{
	if (_filled)
	{
		return QString("Rectangle plein");
	}
	else
	{
		return QString("Rectangle");
	}
}

QString DrawableRectangle::parametersToString()
{
	return "(" % QString::number(_x0) % ", " % QString::number(_y0) %
			")-(" % QString::number(_x1) % ", " % QString::number(_y1) % ")";
}

DrawableRectangle* DrawableRectangle::createRectangle(QDomElement &element)
{
	Color color = Tools::qStringToColor(element.attribute("color", "black"), Properties::defaultForegroundColor());
	QString f = element.attribute("filled", "false");
	bool filled = (QString::compare(f, "true", Qt::CaseInsensitive) == 0) ||
			(QString::compare(f, "1", Qt::CaseInsensitive) == 0);
	bool okx0, oky0, okx1, oky1;
	int x0 = element.attribute("x0", "1000").toInt(&okx0);
	int y0 = element.attribute("y0", "1000").toInt(&oky0);
	int x1 = element.attribute("x1", "1000").toInt(&okx1);
	int y1 = element.attribute("y1", "1000").toInt(&oky1);
	if (okx0 && oky0 && (0<=x0) && (x0<=800) && (0<=y0) && (y0<=800) &&
		okx1 && oky1 && (0<=x1) && (x1<=800) && (0<=y1) && (y1<=800))
	{
		return new DrawableRectangle(x0, y0, x1, y1, filled, color);
	}
	else
	{
		return nullptr;
	}
}

void DrawableRectangle::toXml(QDomElement* root)
{
	QDomElement element = root->ownerDocument().createElement("rectangle");
	element.setAttribute("color", Tools::colorToQString(_color));
	element.setAttribute("x0", _x0);
	element.setAttribute("y0", _y0);
	element.setAttribute("x1", _x1);
	element.setAttribute("y1", _y1);
	element.setAttribute("filled", _filled?"true":"false");
	root->appendChild(element);
}

void DrawableRectangle::move(int deltaX, int deltaY)
{
	_x0 += deltaX;
	_y0 += deltaY;
	_x1 += deltaX;
	_y1 += deltaY;
}
