#ifndef TOOLADDTRIANGLE_H
#define TOOLADDTRIANGLE_H

#include "tooladdelement.h"

class ToolAddTriangle : virtual public ToolAddElement
{
public:
	ToolAddTriangle(Color color);

	DrawableElement *manageClickEvent(QMouseEvent *e);
	bool manageMoveEvent(QMouseEvent *e);
	void reset();
	void setFilled(bool filled) {_filled = filled;}
	void paintGhostDraw(DisplayManager *displayManager);

private:
	bool         _valid0;
	unsigned int _x0;
	unsigned int _y0;
	bool         _valid1;
	unsigned int _x1;
	unsigned int _y1;
	bool         _filled;
	unsigned int _xCurrent;
	unsigned int _yCurrent;
};

#endif // TOOLADDTRIANGLE_H
