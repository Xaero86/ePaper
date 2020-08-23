#ifndef TOOLADDLINE_H
#define TOOLADDLINE_H

#include "tooladdelement.h"

class ToolAddLine : virtual public ToolAddElement
{
public:
	ToolAddLine(Color color);

	DrawableElement *manageClickEvent(QMouseEvent *e);
	bool manageMoveEvent(QMouseEvent *e);
	void reset();
	void paintGhostDraw(DisplayManager *displayManager);

private:
	bool         _valid0;
	unsigned int _x0;
	unsigned int _y0;
	unsigned int _xCurrent;
	unsigned int _yCurrent;
};

#endif // TOOLADDLINE_H
