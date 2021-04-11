#ifndef TOOLS_H
#define TOOLS_H

#include "definition.h"
#include <QString>
#include <QColor>

class Tools
{
public:
	static Color qStringToColor(const QString str, const Color defaultCol = Color::unknown);
	static QString colorToQString(const Color &color);
	static QString colorToQStringIHM(const Color &color);

	static Direction qStringToDirection(const QString str);
	static QString directionToQString(const Direction &direction);

	static Color qColorToColor(const QColor &qcolor);
	static QColor colorToQColor(const Color &color);
};

#endif // TOOLS_H
