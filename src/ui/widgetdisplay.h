#ifndef WIDGETDISPLAY_H
#define WIDGETDISPLAY_H

#include "displaymanager.h"

#include <QWidget>
#include <QPainter>

#include "session.h"
#include "tooladdelement.h"
#include "toolmoveelement.h"

class WidgetDisplay : public QWidget, virtual public DisplayManager
{
	Q_OBJECT
public:
	WidgetDisplay(QWidget *parent, Session *session, Direction direction, Color backgroundColor, ToolMoveElement *movingTool);

	void setBackgroundColor(const Color backgroundColor);
	void setDirection(const Direction direction);

	void displayPoint(short x, short y, const Color color);
	void displayLine(short x0, short y0, short x1, short y1, const Color color);
	void displayRectangle(short x0, short y0, short x1, short y1, bool filled, const Color color);
	void displayCircle(short x, short y, unsigned short radius, bool filled, const Color color);
	void displayTriangle(short x0, short y0, short x1, short y1, short x2, short y2, bool filled, const Color color);
	void displayText(short x, short y, const QString &text, unsigned short fontSize, const Color color);

	void setToolAddElement(ToolAddElement* currentTools) {_currentTools = currentTools;}

signals:
	void elementMoved();

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	Session*  _session;
	Direction _direction;
	Color     _backgroundColor;

	QPainter* _myPainter;

	ToolAddElement*  _currentTools;
	ToolMoveElement* _movingTool;
};

#endif // WIDGETDISPLAY_H
