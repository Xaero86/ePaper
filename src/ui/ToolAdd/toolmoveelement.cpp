#include "toolmoveelement.h"

ToolMoveElement::ToolMoveElement()
	: _active(true), _validOrigin(false), _xOrigin(0), _yOrigin(0), _xCurrent(0), _yCurrent(0), _selection(nullptr)
{

}

void ToolMoveElement::setActive(bool active)
{
	_active = active;
	_validOrigin = false;
	_xOrigin = 0;
	_yOrigin = 0;
	_xCurrent = 0;
	_yCurrent = 0;
}

void ToolMoveElement::managePressEvent(QMouseEvent *event)
{
	if (_active && event->button() == Qt::LeftButton)
	{
		_validOrigin = true;
		_xOrigin = event->pos().x();
		_yOrigin = event->pos().y();
	}
	else if (_active && event->button() == Qt::RightButton)
	{
		// deplacement annule par clic droit
		_validOrigin = false;
	}
}

void ToolMoveElement::manageReleaseEvent(QMouseEvent *event, QList<DrawableElement*>* elements)
{
	if (_active && _validOrigin && event->button() == Qt::LeftButton)
	{
		for (int i = 0; i < _selection->selectedRows().count(); i++)
		{
			elements->at(_selection->selectedRows()[i].row())->move(event->x()-_xOrigin, event->y()-_yOrigin);
		}
		_validOrigin = false;
	}
}

bool ToolMoveElement::manageMoveEvent(QMouseEvent *event)
{
	if (_active && _validOrigin)
	{
		_xCurrent = event->x();
		_yCurrent = event->y();
		return true;
	}
	return false;
}

bool ToolMoveElement::displayMovingElement(QList<DrawableElement*>* elements, int index, DisplayManager *display)
{
	if (!_active || !_validOrigin)
	{
		return false;
	}
	bool selected = false;
	for (int i = 0; i < _selection->selectedRows().count(); i++)
	{
		if (_selection->selectedRows().at(i).row() == index)
		{
			selected = true;
		}
	}
	if (selected)
	{
		elements->at(index)->displayMoving(display, _xCurrent-_xOrigin, _yCurrent-_yOrigin);
		return true;
	}
	else
	{
		return false;
	}
}
