#ifndef TOOLADDELEMENT_H
#define TOOLADDELEMENT_H

#include "definition.h"

#include <QMoveEvent>
#include "displaymanager.h"
#include "drawableelement.h"

class ToolAddElement
{
public:
	ToolAddElement(Color color) : _color(color) {}
	virtual DrawableElement *manageClickEvent(QMouseEvent *e) = 0;
	virtual bool manageMoveEvent(QMouseEvent *e) = 0;
	virtual void reset() = 0;
	virtual void paintGhostDraw(DisplayManager *displayManager) = 0;

	void setColor(Color col) {_color = col;}

protected:
	Color      _color;
};

#endif // TOOLADDELEMENT_H
