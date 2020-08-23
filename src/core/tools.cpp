#include "tools.h"

#include "properties.h"

Color Tools::qStringToColor(const QString str, const Color defaultCol)
{
	if ((QString::compare(str, "black", Qt::CaseInsensitive) == 0) ||
		(QString::compare(str, "noir", Qt::CaseInsensitive) == 0))
	{
		return Color::black;
	}
	else if ((QString::compare(str, "darkGray", Qt::CaseInsensitive) == 0) ||
			 (QString::compare(str, "grisClair", Qt::CaseInsensitive) == 0))
	{
		return Color::darkGray;
	}
	else if ((QString::compare(str, "lightGray", Qt::CaseInsensitive) == 0) ||
			 (QString::compare(str, "grisFonce", Qt::CaseInsensitive) == 0))
	{
		return Color::lightGray;
	}
	else if ((QString::compare(str, "white", Qt::CaseInsensitive) == 0) ||
			 (QString::compare(str, "blanc", Qt::CaseInsensitive) == 0))
	{
		return Color::white;
	}
	else
	{
		return defaultCol;
	}
}

QString Tools::colorToQString(const Color &color)
{
	switch (color)
	{
	case Color::black:     return "black";     break;
	case Color::darkGray:  return "darkGray";  break;
	case Color::lightGray: return "lightGray"; break;
	case Color::white:     return "white";     break;
	default:               return "black";     break;
	}
}

QString Tools::colorToQStringIHM(const Color &color)
{
	switch (color)
	{
	case Color::black:     return "Noir";     break;
	case Color::darkGray:  return "Gris fonce";  break;
	case Color::lightGray: return "Gris clair"; break;
	case Color::white:     return "Blanc";     break;
	default:               return "Noir";     break;
	}
}

Direction Tools::qStringToDirection(const QString str)
{
	if ((QString::compare(str, "landscape", Qt::CaseInsensitive) == 0) ||
		(QString::compare(str, "paysage", Qt::CaseInsensitive) == 0) ||
		(QString::compare(str, "horizontal", Qt::CaseInsensitive) == 0))
	{
		return Properties::defaultHorizontal();
	}
	else if ((QString::compare(str, "portrait", Qt::CaseInsensitive) == 0) ||
		(QString::compare(str, "vertical", Qt::CaseInsensitive) == 0))
	{
		return Properties::defaultVertical();
	}
	else if ((QString::compare(str, "landscape0", Qt::CaseInsensitive) == 0) ||
		(QString::compare(str, "paysage0", Qt::CaseInsensitive) == 0) ||
		(QString::compare(str, "horizontal0", Qt::CaseInsensitive) == 0))
	{
		return Direction::horizontal0;
	}
	else if ((QString::compare(str, "portrait1", Qt::CaseInsensitive) == 0) ||
		(QString::compare(str, "vertical1", Qt::CaseInsensitive) == 0))
	{
		return Direction::vertical1;
	}
	else if ((QString::compare(str, "landscape2", Qt::CaseInsensitive) == 0) ||
		(QString::compare(str, "paysage2", Qt::CaseInsensitive) == 0) ||
		(QString::compare(str, "horizontal2", Qt::CaseInsensitive) == 0))
	{
		return Direction::horizontal2;
	}
	else if ((QString::compare(str, "portrait3", Qt::CaseInsensitive) == 0) ||
		(QString::compare(str, "vertical3", Qt::CaseInsensitive) == 0))
	{
		return Direction::vertical3;
	}
	else if (QString::compare(str, "unknown", Qt::CaseInsensitive) == 0)
	{
		return Direction::unknown;
	}
	else
	{
		return Properties::defaultDirection();
	}
}

QString Tools::directionToQString(const Direction &direction)
{
	switch (direction)
	{
	case Direction::horizontal0: return "horizontal0"; break;
	case Direction::vertical1:   return "vertical1";   break;
	case Direction::horizontal2: return "horizontal2"; break;
	case Direction::vertical3:   return "vertical3";   break;
	default:                     return "";            break;
	}
}

Color Tools::qColorToColor(const QColor &qcolor)
{
	if (qcolor == Qt::black)     return Color::black;
	if (qcolor == Qt::darkGray)  return Color::darkGray;
	if (qcolor == Qt::lightGray) return Color::lightGray;
	if (qcolor == Qt::white)     return Color::white;
	return Color::unknown;
}

QColor Tools::colorToQColor(const Color &color)
{
	switch (color)
	{
	case Color::black:     return Qt::black; break;
	case Color::darkGray:  return Qt::darkGray; break;
	case Color::lightGray: return Qt::lightGray; break;
	case Color::white:     return Qt::white; break;
	case Color::ghostAdd:  return Qt::red; break;
	case Color::ghostMove: return Qt::blue; break;
	default:               return Qt::white; break;
	}
}
