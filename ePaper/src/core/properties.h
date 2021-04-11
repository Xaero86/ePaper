#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "definition.h"

#include <QString>
#include <QMap>

class Properties
{
public:
	static void loadPropertyFile(const QString &path);

	static Direction defaultDirection()   { return G_SingleInstance._defaultDirection; }
	static Direction defaultHorizontal()  { return G_SingleInstance._defaultHorizontal; }
	static Direction defaultVertical()    { return G_SingleInstance._defaultVertical; }

	static Color defaultBackgroundColor() { return G_SingleInstance._defaultBackgroundColor; }
	static Color defaultForegroundColor() { return G_SingleInstance._defaultForegroundColor; }
	
	static unsigned int defautImageColorDepth() { return G_SingleInstance._defautImageColorDepth; }
	static QMap<unsigned int, Color>& defaultColorPalette(unsigned int depth) { return G_SingleInstance._defaultColorPalette[depth]; }

	static QString defaultSerialPort()    { return G_SingleInstance._defaultSerialPort; }
	static unsigned int wakeUpMode()      { return G_SingleInstance._wakeUpMode; }
	static unsigned int wakeUpDelay()     { return G_SingleInstance._wakeUpDelay; }

	static bool debug()                   { return G_SingleInstance._debug; }

private:
	static Properties G_SingleInstance;
	Properties();
	virtual ~Properties() {}

	Direction    _defaultDirection;
	Direction    _defaultHorizontal;
	Direction    _defaultVertical;

	Color        _defaultBackgroundColor;
	Color        _defaultForegroundColor;
	
	unsigned int _defautImageColorDepth;
	QMap<unsigned int, QMap<unsigned int, Color> > _defaultColorPalette;

	QString      _defaultSerialPort;
	unsigned int _wakeUpMode;
	unsigned int _wakeUpDelay;

	bool         _debug;
};

#endif // PROPERTIES_H
