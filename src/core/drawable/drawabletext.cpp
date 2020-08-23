#include "drawabletext.h"

#include "properties.h"
#include "tools.h"

DrawableText::DrawableText(short x, short y, const QString &text, unsigned short fontSize, Color color)
	: DrawableElement(color), _x(x), _y(y), _text(text), _fontSize(fontSize)
{

}

void DrawableText::display(DisplayManager *displayManager)
{
	displayManager->displayText(_x, _y, _text, _fontSize, _color);
}

void DrawableText::displayMoving(DisplayManager *displayManager, int deltaX, int deltaY)
{
	displayManager->displayText(_x+deltaX, _y+deltaY, _text, _fontSize, Color::ghostMove);
}

QString DrawableText::parametersToString()
{
	return "(" % QString::number(_x) % ", " % QString::number(_y) %
			"), \"" % _text % "\" Taille=" % QString::number(_fontSize);
}

DrawableText* DrawableText::createText(QDomElement &element)
{
	Color color = Tools::qStringToColor(element.attribute(QString("color"), QString("black")), Properties::defaultForegroundColor());
	bool okx, oky, okf;
	int x = element.attribute("x", "1000").toInt(&okx);
	int y = element.attribute("y", "1000").toInt(&oky);
	QString text = element.attribute("text", "");
	int fontSize = element.attribute("fontSize", "1").toInt(&okf);

	if (okx && oky && okf && (0<=x) && (x<=800) && (0<=y) && (y<=800) && (1<=fontSize) && (fontSize<=3))
	{
		return new DrawableText(x, y, text, fontSize, color);
	}
	else
	{
		return nullptr;
	}
}

void DrawableText::toXml(QDomElement* root)
{
	QDomElement element = root->ownerDocument().createElement("text");
	element.setAttribute("color", Tools::colorToQString(_color));
	element.setAttribute("x", _x);
	element.setAttribute("y", _y);
	element.setAttribute("text", _text);
	element.setAttribute("fontSize", _fontSize);
	root->appendChild(element);
}

void DrawableText::move(int deltaX, int deltaY)
{
	_x += deltaX;
	_y += deltaY;
}
