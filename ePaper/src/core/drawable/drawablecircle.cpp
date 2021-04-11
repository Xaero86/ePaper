#include "drawablecircle.h"

#include "properties.h"
#include "tools.h"

DrawableCircle::DrawableCircle(short x, short y, unsigned short radius, bool filled, Color color)
	: DrawableElement(color), _x(x), _y(y), _radius(radius), _filled(filled)
{

}

void DrawableCircle::display(DisplayManager *displayManager)
{
	displayManager->displayCircle(_x, _y, _radius, _filled, _color);
}

void DrawableCircle::displayMoving(DisplayManager *displayManager, int deltaX, int deltaY)
{
		displayManager->displayCircle(_x+deltaX, _y+deltaY, _radius, _filled, Color::ghostMove);
}

QString DrawableCircle::toString()
{
	if (_filled)
	{
		return QString("Cercle plein");
	}
	else
	{
		return QString("Cercle");
	}
}

QString DrawableCircle::parametersToString()
{
	return "(" % QString::number(_x) % ", " % QString::number(_y) %
			"), rayon=" % QString::number(_radius);
}

DrawableCircle* DrawableCircle::createCircle(QDomElement &element)
{
	Color color = Tools::qStringToColor(element.attribute(QString("color"), QString("black")), Properties::defaultForegroundColor());
	QString f = element.attribute("filled", "false");
	bool filled = (QString::compare(f, "true", Qt::CaseInsensitive) == 0) ||
			(QString::compare(f, "1", Qt::CaseInsensitive) == 0);
	bool okx, oky, okr;
	int x = element.attribute("x", "1000").toInt(&okx);
	int y = element.attribute("y", "1000").toInt(&oky);
	int r = element.attribute("radius", "1000").toInt(&okr);

	if (okx && oky && okr && (0<=x) && (x<=800) && (0<=y) && (y<=800) && (0<=r) && (r<=800))
	{
		return new DrawableCircle(x, y, r, filled, color);
	}
	else
	{
		return nullptr;
	}
}

void DrawableCircle::toXml(QDomElement* root)
{
	QDomElement element = root->ownerDocument().createElement("circle");
	element.setAttribute("color", Tools::colorToQString(_color));
	element.setAttribute("x", _x);
	element.setAttribute("y", _y);
	element.setAttribute("radius", _radius);
	element.setAttribute("filled", _filled?"true":"false");
	root->appendChild(element);
}

void DrawableCircle::move(int deltaX, int deltaY)
{
	_x += deltaX;
	_y += deltaY;
}
