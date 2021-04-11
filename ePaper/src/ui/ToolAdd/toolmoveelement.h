#ifndef TOOLMOVEELEMENT_H
#define TOOLMOVEELEMENT_H

#include "drawableelement.h"

#include <QMouseEvent>
#include <QItemSelectionModel>

class ToolMoveElement
{
public:
	ToolMoveElement();

	void setElementsList(QItemSelectionModel *selection) {_selection = selection;}

	void setActive(bool active);

	void managePressEvent(QMouseEvent *event);
	void manageReleaseEvent(QMouseEvent *event, QList<DrawableElement *> *elements);
	bool manageMoveEvent(QMouseEvent *event);

	bool displayMovingElement(QList<DrawableElement *> *elements, int index, DisplayManager *display);

private:
	bool  _active;
	bool  _validOrigin;
	int   _xOrigin;
	int   _yOrigin;
	int   _xCurrent;
	int   _yCurrent;

	QItemSelectionModel *_selection;
};

#endif // TOOLMOVEELEMENT_H
