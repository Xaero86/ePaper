#ifndef TOOLADDCIRCLE_H
#define TOOLADDCIRCLE_H

#include "tooladdelement.h"

class ToolAddCircle : virtual public ToolAddElement
{
public:
	ToolAddCircle(Color color);

	DrawableElement *manageClickEvent(QMouseEvent *e);
	bool manageMoveEvent(QMouseEvent *e);
	void reset();
	void setFilled(bool filled) {_filled = filled;}
	void paintGhostDraw(DisplayManager *displayManager);

private:
	bool         _valid;
	unsigned int _x;
	unsigned int _y;
	bool         _filled;
	unsigned int _xCurrent;
	unsigned int _yCurrent;
};

#endif // TOOLADDCIRCLE_H
