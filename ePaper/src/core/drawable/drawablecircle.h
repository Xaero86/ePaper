#ifndef DRAWABLECIRCLE_H
#define DRAWABLECIRCLE_H

#include "drawableelement.h"

class DrawableCircle : public DrawableElement
{
public:
	DrawableCircle(short x, short y, unsigned short radius, bool filled, Color color);

	void display(DisplayManager *displayManager);
	void displayMoving(DisplayManager *displayManager, int deltaX, int deltaY);
	QString toString();
	QString parametersToString();

	static DrawableCircle* createCircle(QDomElement &element);
	void toXml(QDomElement* root);

	void move(int deltaX, int deltaY);

	short x() {return _x;}
	short y() {return _y;}
	unsigned short radius() {return _radius;}
	bool filled() {return _filled;}

	void setX(short x) {_x=x;}
	void setY(short y) {_y=y;}
	void setRadius(short radius) {_radius=radius;}
	void setFilled(bool filled) {_filled=filled;}

private:
	short _x;
	short _y;
	unsigned short _radius;
	bool  _filled;
};

#endif // DRAWABLECIRCLE_H
