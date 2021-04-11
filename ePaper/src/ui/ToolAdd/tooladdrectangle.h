#ifndef TOOLADDRECTANGLE_H
#define TOOLADDRECTANGLE_H

#include "tooladdelement.h"

class ToolAddRectangle : virtual public ToolAddElement
{
public:
	ToolAddRectangle(Color color);

	DrawableElement *manageClickEvent(QMouseEvent *e);
	bool manageMoveEvent(QMouseEvent *e);
	void reset();
	void setFilled(bool filled) {_filled = filled;}
	void paintGhostDraw(DisplayManager *displayManager);

private:
	bool         _valid0;
	unsigned int _x0;
	unsigned int _y0;
	bool         _filled;
	unsigned int _xCurrent;
	unsigned int _yCurrent;
};

#endif // TOOLADDRECTANGLE_H
