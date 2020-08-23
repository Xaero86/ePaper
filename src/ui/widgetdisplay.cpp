#include "widgetdisplay.h"

#include "tools.h"

WidgetDisplay::WidgetDisplay(QWidget *parent, Session *session, Direction direction, Color backgroundColor, ToolMoveElement *movingTool)
	: QWidget(parent), _session(session), _direction(direction),  _backgroundColor(backgroundColor),
	  _myPainter(nullptr), _currentTools(nullptr), _movingTool(movingTool)
{
	setDirection(direction);
	setMouseTracking(true);
}

void WidgetDisplay::setBackgroundColor(const Color backgroundColor)
{
	_backgroundColor = backgroundColor;
}

void WidgetDisplay::setDirection(const Direction direction)
{
	switch (direction) {
	case Direction::horizontal0:
	case Direction::horizontal2:
		_direction = direction;
		setFixedSize(800, 600);
		break;
	case Direction::vertical1:
	case Direction::vertical3:
		_direction = direction;
		setFixedSize(600, 800);
		break;
	case Direction::unknown:
		break;
	}
}

void WidgetDisplay::displayPoint(short x, short y, const Color color)
{
	if (_myPainter != nullptr)
	{
		_myPainter->setPen(Tools::colorToQColor(color));
		_myPainter->drawPoint(x,y);
	}
}

void WidgetDisplay::displayLine(short x0, short y0, short x1, short y1, const Color color)
{
	if (_myPainter != nullptr)
	{
		_myPainter->setPen(Tools::colorToQColor(color));
		_myPainter->drawLine(x0,y0,x1,y1);
	}
}

void WidgetDisplay::displayRectangle(short x0, short y0, short x1, short y1, bool filled, const Color color)
{
	if (_myPainter != nullptr)
	{
		_myPainter->setPen(Tools::colorToQColor(color));
		if (filled)
		{
			_myPainter->setBrush(Qt::SolidPattern);
			_myPainter->fillRect(x0,y0,x1-x0,y1-y0,Tools::colorToQColor(color));
		}
		else
		{
			_myPainter->setBrush(Qt::NoBrush);
			_myPainter->drawRect(x0,y0,x1-x0,y1-y0);
		}
	}
}

void WidgetDisplay::displayCircle(short x, short y, unsigned short radius, bool filled, const Color color)
{
	if (_myPainter != nullptr)
	{
		_myPainter->setPen(Tools::colorToQColor(color));
		if (filled)
		{
			_myPainter->setBrush(QBrush(Tools::colorToQColor(color), Qt::SolidPattern));
			_myPainter->drawEllipse(x-radius,y-radius,2*radius,2*radius);
		}
		else
		{
			_myPainter->setBrush(Qt::NoBrush);
			_myPainter->drawArc(x-radius,y-radius,2*radius,2*radius,0,16*360);
		}
	}
}

void WidgetDisplay::displayTriangle(short x0, short y0, short x1, short y1, short x2, short y2, bool filled, const Color color)
{
	if (_myPainter != nullptr)
	{
		_myPainter->setPen(Tools::colorToQColor(color));
		if (filled)
		{
			QPoint points[3] = {
				QPoint(x0, y0),
				QPoint(x1, y1),
				QPoint(x2, y2)
			};
			_myPainter->setBrush(QBrush(Tools::colorToQColor(color), Qt::SolidPattern));
			_myPainter->drawPolygon(points, 3);
		}
		else
		{
			QPoint points[4] = {
				QPoint(x0, y0),
				QPoint(x1, y1),
				QPoint(x2, y2),
				QPoint(x0, y0)
			};
			_myPainter->setBrush(Qt::NoBrush);
			_myPainter->drawPolyline(points, 4);
		}
	}
}

void WidgetDisplay::displayText(short x, short y, const QString &text, unsigned short fontSize, const Color color)
{
	if (_myPainter != nullptr)
	{
		QFont font = _myPainter->font();
		font.setPointSize(14*(fontSize+1));
		_myPainter->setFont(font);
		_myPainter->setPen(Tools::colorToQColor(color));
		_myPainter->drawText(x, y+15*(fontSize+1), text);
	}
}

void WidgetDisplay::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	painter.fillRect(rect(), Tools::colorToQColor(_backgroundColor));

	painter.save();
	_myPainter = &painter;

	for (int i = 0; i < _session->getElements()->size(); i++)
	{
		DrawableElement* element = _session->getElements()->at(i);
		if (!_movingTool->displayMovingElement(_session->getElements(), i, this))
		{
			element->display(this);
		}
	}

	if (_currentTools != nullptr)
	{
		_currentTools->paintGhostDraw(this);
	}
	_myPainter = nullptr;
	painter.restore();
}

void WidgetDisplay::mousePressEvent(QMouseEvent *event)
{
	_movingTool->managePressEvent(event);
}

void WidgetDisplay::mouseReleaseEvent(QMouseEvent *event)
{
	if (_currentTools != nullptr)
	{
		if (event->button() == Qt::LeftButton)
		{
			DrawableElement *newElement = _currentTools->manageClickEvent(event);
			if (newElement != nullptr)
			{
				_session->addElement(newElement);
			}
		}
		else if (event->button() == Qt::RightButton)
		{
			_currentTools->reset();
			update();
		}
	}
	else
	{
		_movingTool->manageReleaseEvent(event, _session->getElements());
		emit elementMoved();
	}
}

void WidgetDisplay::mouseMoveEvent(QMouseEvent *event)
{
	if ((_currentTools != nullptr) && _currentTools->manageMoveEvent(event))
	{
		update();
	}
	else if	(_movingTool->manageMoveEvent(event))
	{
		emit elementMoved();
	}

}
