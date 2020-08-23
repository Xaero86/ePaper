#ifndef DRAWABLEPOINT_H
#define DRAWABLEPOINT_H

#include "drawableelement.h"

class DrawablePoint : public DrawableElement
{
public:
	DrawablePoint(short x, short y, Color color);

	void display(DisplayManager *displayManager);
	void displayMoving(DisplayManager *displayManager, int deltaX, int deltaY);
	QString toString() {return QString("Point");}
	QString parametersToString();

	static DrawablePoint* createPoint(QDomElement &element);
	void toXml(QDomElement* root);

	void move(int deltaX, int deltaY);

	short x() {return _x;}
	short y() {return _y;}

	void setX(short x) {_x=x;}
	void setY(short y) {_y=y;}

private:
	short _x;
	short _y;
};

#endif // DRAWABLEPOINT_H
