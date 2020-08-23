#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include "definition.h"

#include <QString>

class DisplayManager
{
public:
	virtual void setBackgroundColor(const Color backgroundColor) = 0;
	virtual void setDirection(const Direction direction) = 0;

	virtual void displayPoint(short x, short y, const Color color) = 0;
	virtual void displayLine(short x0, short y0, short x1, short y1, const Color color) = 0;
	virtual void displayRectangle(short x0, short y0, short x1, short y1, bool filled, const Color color) = 0;
	virtual void displayCircle(short x, short y, unsigned short radius, bool filled, const Color color) = 0;
	virtual void displayTriangle(short x0, short y0, short x1, short y1, short x2, short y2, bool filled, const Color color) = 0;
	virtual void displayText(short x, short y, const QString &text, unsigned short fontSize, const Color color) = 0;
};

#endif // DISPLAYMANAGER_H
