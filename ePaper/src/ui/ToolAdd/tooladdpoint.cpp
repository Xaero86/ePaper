#include "tooladdpoint.h"

#include "drawablepoint.h"

ToolAddPoint::ToolAddPoint(Color color)
	: ToolAddElement(color)
{

}

DrawableElement *ToolAddPoint::manageClickEvent(QMouseEvent *e)
{
	return new DrawablePoint(e->x(), e->y(), _color);
}
