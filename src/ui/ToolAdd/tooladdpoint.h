#ifndef TOOLADDPOINT_H
#define TOOLADDPOINT_H

#include "tooladdelement.h"

class ToolAddPoint : virtual public ToolAddElement
{
public:
	ToolAddPoint(Color color);

	DrawableElement *manageClickEvent(QMouseEvent *e);
	bool manageMoveEvent(QMouseEvent */*e*/) {return false;}
	void reset() {}
	void paintGhostDraw(DisplayManager* /*displayManager*/) {}
};

#endif // TOOLADDPOINT_H
