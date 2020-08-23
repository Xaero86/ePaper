#ifndef DRAWABLETRIANGLE_H
#define DRAWABLETRIANGLE_H

#include "drawableelement.h"

class DrawableTriangle : public DrawableElement
{
public:
	DrawableTriangle(short x0, short y0, short x1, short y1, short x2, short y2, bool filled, Color color);

	void display(DisplayManager *displayManager);
	void displayMoving(DisplayManager *displayManager, int deltaX, int deltaY);
	QString toString();
	QString parametersToString();

	static DrawableTriangle* createTriangle(QDomElement &element);
	void toXml(QDomElement* root);

	void move(int deltaX, int deltaY);

	short x0() {return _x0;}
	short y0() {return _y0;}
	short x1() {return _x1;}
	short y1() {return _y1;}
	short x2() {return _x2;}
	short y2() {return _y2;}
	bool filled() {return _filled;}

	void setX0(short x0) {_x0=x0;}
	void setY0(short y0) {_y0=y0;}
	void setX1(short x1) {_x1=x1;}
	void setY1(short y1) {_y1=y1;}
	void setX2(short x2) {_x2=x2;}
	void setY2(short y2) {_y2=y2;}
	void setFilled(bool filled) {_filled=filled;}

private:
	short _x0;
	short _y0;
	short _x1;
	short _y1;
	short _x2;
	short _y2;
	bool  _filled;
};

#endif // DRAWABLETRIANGLE_H
