#ifndef DRAWABLELINE_H
#define DRAWABLELINE_H

#include "drawableelement.h"

class DrawableLine : public DrawableElement
{
public:
	DrawableLine(short x0, short y0, short x1, short y1, Color color);

	void display(DisplayManager *displayManager);
	void displayMoving(DisplayManager *displayManager, int deltaX, int deltaY);
	QString toString() {return QString("Ligne");}
	QString parametersToString();

	static DrawableLine* createLine(QDomElement &element);
	void toXml(QDomElement* root);

	void move(int deltaX, int deltaY);

	short x0() {return _x0;}
	short y0() {return _y0;}
	short x1() {return _x1;}
	short y1() {return _y1;}

	void setX0(short x0) {_x0=x0;}
	void setY0(short y0) {_y0=y0;}
	void setX1(short x1) {_x1=x1;}
	void setY1(short y1) {_y1=y1;}

private:
	short _x0;
	short _y0;
	short _x1;
	short _y1;
};

#endif // DRAWABLELINE_H
