#ifndef DRAWABLEELEMENT_H
#define DRAWABLEELEMENT_H

#include "definition.h"
#include "displaymanager.h"

#include <QString>
#include <QtXml>

class DrawableElement
{
public:
	DrawableElement(Color color);
	virtual ~DrawableElement();

	virtual void display(DisplayManager *displayManager) = 0;
	virtual void displayMoving(DisplayManager *displayManager, int deltaX, int deltaY) = 0;
	virtual QString toString() = 0;
	virtual QString parametersToString() = 0;
	virtual void toXml(QDomElement* root) = 0;

	virtual void move(int deltaX, int deltaY) = 0;

	Color getColor() {return _color;}
	void setColor(Color color) {_color = color;}

protected:
	Color   _color;
};

#endif // DRAWABLEELEMENT_H
