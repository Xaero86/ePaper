#include "properties.h"

#include "tools.h"

#include <QtXml>

static QString getAttribute(QDomElement &elem, const QString &name, QString defValue = QString());

Properties Properties::G_SingleInstance = Properties();

Properties::Properties()
{
	_defaultDirection            = Direction::vertical3;
	_defaultHorizontal           = Direction::horizontal2;
	_defaultVertical             = Direction::vertical3;
	_defaultBackgroundColor      = Color::white;
	_defaultForegroundColor      = Color::black;
	_defautImageColorDepth       = 2;
	_defaultColorPalette         = {{2, {{0, Color::white}, {1, Color::black}}},
									{3, {{0, Color::white}, {1, Color::lightGray}, {2, Color::darkGray}}},
									{4, {{0, Color::white}, {1, Color::lightGray}, {2, Color::darkGray}, {3, Color::black}}}};
	_defaultSerialPort           = NONE_PORT;
	_wakeUpMode                  = WAKE_UP_NONE;
	_wakeUpDelay                 = 1000;
	_debug                       = false;
}

void Properties::loadPropertyFile(const QString &path)
{
	QFile file(path);
	if (file.open(QIODevice::ReadOnly))
	{
		QDomDocument doc("Properties");
		if (doc.setContent(&file))
		{
			QDomElement root = doc.documentElement();
 			if (QString::compare(root.nodeName(), "properties", Qt::CaseInsensitive) == 0)
			{
				QString defaultHorizontal = getAttribute(root, "defaultHorizontal");
				Direction defH = Tools::qStringToDirection(defaultHorizontal);
				if ((defH == Direction::horizontal0) || (defH == Direction::horizontal2))
				{
					G_SingleInstance._defaultHorizontal = defH;
				}
				QString defaultVertical   = getAttribute(root, "defaultVertical");
				Direction defV = Tools::qStringToDirection(defaultVertical);
				if ((defV == Direction::vertical1) || (defV == Direction::vertical3))
				{
					G_SingleInstance._defaultVertical   = defV;
				}
				QString defaultDirection  = getAttribute(root, "defaultDirection");
				Direction defD = Tools::qStringToDirection(defaultDirection);
				if (defD != Direction::unknown)
				{
					G_SingleInstance._defaultDirection  = defD;
				}

				QString defaultBackgroundColor = getAttribute(root, "defaultBackgroundColor");
				G_SingleInstance._defaultBackgroundColor = Tools::qStringToColor(defaultBackgroundColor, G_SingleInstance._defaultBackgroundColor);
				QString defaultForegroundColor = getAttribute(root, "defaultForgroundColor");
				Color defaultForeCol = Tools::qStringToColor(defaultForegroundColor, G_SingleInstance._defaultForegroundColor);
				if (defaultForeCol != G_SingleInstance._defaultBackgroundColor)
				{
					G_SingleInstance._defaultForegroundColor = defaultForeCol;
				}
				else if (G_SingleInstance._defaultForegroundColor == G_SingleInstance._defaultBackgroundColor)
				{
					// si le background default a ete defini en black:
					G_SingleInstance._defaultForegroundColor = Color::lightGray;
				}

				QString defautImageColorDepth = getAttribute(root, "defautImageColorDepth");
				bool okd;
				int newDepth = defautImageColorDepth.toInt(&okd);
				if (okd && (newDepth >= 2) && (newDepth <= 4))
				{
					G_SingleInstance._defautImageColorDepth = newDepth;
				}

				G_SingleInstance._defaultSerialPort = getAttribute(root, "defaultSerialPort", G_SingleInstance._defaultSerialPort);
				
				QString wakeUpMode = getAttribute(root, "wakeUpMode");
				QDomNode wakeUpNode = root.namedItem("wakeUpMode");
		
				if (QString::compare(wakeUpMode, "none", Qt::CaseInsensitive) == 0)
				{
					G_SingleInstance._wakeUpMode = WAKE_UP_NONE;
				}
				else if (QString::compare(wakeUpMode, "dtr", Qt::CaseInsensitive) == 0)
				{
					G_SingleInstance._wakeUpMode = WAKE_UP_DTR;
				}
				else if (QString::compare(wakeUpMode, "rts", Qt::CaseInsensitive) == 0)
				{
					G_SingleInstance._wakeUpMode = WAKE_UP_RTS;
				}
				else if (QString::compare(wakeUpMode, "dtr_inv", Qt::CaseInsensitive) == 0)
				{
					G_SingleInstance._wakeUpMode = WAKE_UP_DTR_INV;
				}
				else if (QString::compare(wakeUpMode, "rts_inv", Qt::CaseInsensitive) == 0)
				{
					G_SingleInstance._wakeUpMode = WAKE_UP_RTS_INV;
				}
				if (!wakeUpNode.isNull())
				{
					QString wakeUpDel = wakeUpNode.toElement().attribute("delay");
					bool okw;
					int wakeUpDelay = wakeUpDel.toInt(&okw);
					if (okw && (wakeUpDelay >= 100))
					{
						G_SingleInstance._wakeUpDelay = wakeUpDelay;
					}
				}

				QString debug = getAttribute(root, "debug");
				G_SingleInstance._debug = ((QString::compare(debug, "true", Qt::CaseInsensitive) == 0) ||
								(QString::compare(debug, "vrai", Qt::CaseInsensitive) == 0));
				
				QDomNode nodePalette = root.firstChild();

				while(!nodePalette.isNull())
				{
					QDomElement elementPalette = nodePalette.toElement();
					if (QString::compare(elementPalette.nodeName(), "defaultPalette", Qt::CaseInsensitive) == 0)
					{
						QString paletteD = elementPalette.attribute("depth");
						bool okp;
						int paletteDepth = paletteD.toInt(&okp);
						if (okp && (paletteDepth >= 2) && (paletteDepth <= 4))
						{
							QDomNode nodeColor = nodePalette.firstChild();
							
							while(!nodeColor.isNull())
							{
								QDomElement elementColor = nodeColor.toElement();
								if (QString::compare(elementColor.nodeName(), "color", Qt::CaseInsensitive) == 0)
								{
									QString colorI = elementColor.attribute("index");
									bool okc;
									int colorIndex = colorI.toInt(&okc);
									
									if (okc && (colorIndex >= 0) && (colorIndex < paletteDepth))
									{
										QString paletteC = elementColor.attribute("color");
										Color paletteColor = Tools::qStringToColor(paletteC, G_SingleInstance._defaultColorPalette[paletteDepth][colorIndex]);
										G_SingleInstance._defaultColorPalette[paletteDepth][colorIndex] = paletteColor;
									}
								}
								nodeColor = nodeColor.nextSibling();
							}
							
						}
					}
					nodePalette = nodePalette.nextSibling();
				}
			}
		}
	}
}

QString getAttribute(QDomElement &elem, const QString &name, QString defValue)
{
	QString result;
	result = elem.attribute(name);
	
	if (result.isEmpty())
	{
		QDomNode childNode = elem.namedItem(name);
		
		if (!childNode.isNull())
		{
			result = childNode.toElement().attribute("value");
		}
	}
	if (result.isEmpty())
	{
		result = defValue;
	}
	return result;
}
