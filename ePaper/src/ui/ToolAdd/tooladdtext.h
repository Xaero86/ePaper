#ifndef TOOLADDTEXT_H
#define TOOLADDTEXT_H

#include "tooladdelement.h"

class ToolAddText : virtual public ToolAddElement
{
public:
	ToolAddText(QWidget *parent, Color color);

	DrawableElement *manageClickEvent(QMouseEvent *e);
	bool manageMoveEvent(QMouseEvent */*e*/) {return false;}
	void reset() {}
	void paintGhostDraw(DisplayManager* /*displayManager*/) {}

private:
	QWidget* _parent;
};

#endif // TOOLADDTEXT_H
