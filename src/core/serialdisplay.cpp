#include "serialdisplay.h"

#include <QThreadPool>
#include <iostream>

#include "properties.h"

static Color charToColor(char val);
static char colorToChar(Color col);
static Direction charToDirection(char val);
static char directionToChar(Direction dir);
static void setShort(QByteArray &data, unsigned short val, int pos);

SerialDisplay::SerialDisplay()
	: _serialDriver(), _deviceReady(false),
	  _memDirection(Direction::unknown), _memForegroundColor(Color::unknown),
	  _memBackgroundColor(Color::unknown), _memFontSize(0), _connexionRunnable(nullptr)
{
	connect(&_serialDriver, SIGNAL(timeoutInWrite()),    this, SLOT(manageTimeoutInWrite()));
	connect(&_serialDriver, SIGNAL(timeoutInRead()),     this, SLOT(manageTimeoutInRead()));
	
	connect(&_serialDriver, SIGNAL(newWriteStatus(int)), this, SLOT(updateWriteStatus(int)), Qt::DirectConnection);
}

SerialDisplay::~SerialDisplay()
{
	disconnectSerial();
}

void SerialDisplay::manageTimeoutInWrite()
{
	std::cerr << "Timeout writing data" << std::endl;
	disconnectSerial();
}

void SerialDisplay::manageTimeoutInRead()
{
	std::cerr << "Timeout reading data" << std::endl;
	_deviceReady = false;
}

void SerialDisplay::disconnectSerial()
{
	if (_deviceReady)
	{
		std::cout << "Disconnected" << std::endl;
		newStatus(NOT_CONNECTED);
	}
	_deviceReady = false;
	_serialDriver.close(false);
	_serialDriver.clearFifo();
}

void SerialDisplay::cancelOperation()
{
	_serialDriver.clearFifo();
}

void SerialDisplay::changePort(const QString &port)
{
	if (_serialDriver.isOpen())
	{
		disconnectSerial();
	}
	_serialDriver.setPortName(port);
	connectSerial();
}

void SerialDisplay::connectSerial()
{
	if (_connexionRunnable == nullptr)
	{
		newStatus(CONNECTING);
		std::cout << "Connecting..." << std::endl;
		_connexionRunnable = new ConnexionRunnable(this);
		QThreadPool::globalInstance()->start(_connexionRunnable);
	}
}

void SerialDisplay::ConnexionRunnable::run()
{
	_serialDisplay->initSerial();
	if (_serialDisplay->_deviceReady)
	{
		_serialDisplay->newStatus(CONNECTED);
		std::cout << "Connected" << std::endl;
	}
	else
	{
		_serialDisplay->newStatus(NOT_CONNECTED);
		std::cout << "Not connected" << std::endl;
	}
	_serialDisplay->_connexionRunnable = nullptr;
}

void SerialDisplay::newStatus(int status)
{
	emit statusChanged(status);
}

void SerialDisplay::updateWriteStatus(int percent)
{
	emit newWriteStatus(percent);
}

void SerialDisplay::initSerial()
{
	_serialDriver.open(true);
	if (!_serialDriver.isOpen())
	{
		return;
	}
	if (Properties::wakeUpMode() == WAKE_UP_NONE)
	{
		_deviceReady = wakeUp(0);
	}
	else
	{
		_deviceReady = wakeUp(5);
	}
	_memDirection = Direction::unknown;
	_memForegroundColor = Color::unknown;
	_memBackgroundColor = Color::unknown;
	_memFontSize = 0;
	getDirection();
	getColor();
	getFontSize();
}

void SerialDisplay::setBackgroundColor(const Color backgroundColor)
{
	if (backgroundColor != _memBackgroundColor)
	{
		_memBackgroundColor = backgroundColor;
		sendColor();
	}
}

void SerialDisplay::setColor(const Color foregroundColor)
{
	if (foregroundColor != _memForegroundColor)
	{
		_memForegroundColor = foregroundColor;
		sendColor();
	}
}

void SerialDisplay::setDirection(const Direction direction)
{
	if (direction != _memDirection)
	{
		_memDirection = direction;
		sendDirection();
	}
}

void SerialDisplay::setFontSize(int size)
{
	if (size != _memFontSize)
	{
		_memFontSize = size;
		sendFontSize();
	}
}

bool SerialDisplay::wakeUp(int nbTry)
{
	bool ready;
	int tryPerformed = 0;
	do {
		ready = checkReady();
		if (!ready)
		{
			_serialDriver.wakeUp();
			tryPerformed++;
		}
	} while (!ready && tryPerformed < nbTry && _serialDriver.isOpen());
	return ready;
}

bool SerialDisplay::checkReady()
{
	QByteArray data(0x0009, 0x00);
	data[3] = 0x00;
	QByteArray result = _serialDriver.sendDataAndWait(data, 2);

	if (result.size() == 2)
	{
		return (result[0] == 'O') && (result[1] == 'K');
	}
	return false;
}

void SerialDisplay::setBaudRate(unsigned int)
{
	if (!_deviceReady) return;
	// NE PAS IMPLEMENTE
	return;
}

int SerialDisplay::getBaudRate()
{
	if (!_deviceReady) return 0;
	// NE PAS IMPLEMENTE
	return 0;
}

void SerialDisplay::turnIdle()
{
	if (!_deviceReady) return;
	QByteArray data(0x0009, 0x00);
	data[3] = 0x08;
	_serialDriver.sendData(data);
}

void SerialDisplay::refresh()
{
	if (!_deviceReady) return;
	QByteArray data(0x0009, 0x00);
	data[3] = 0x0a;
	_serialDriver.sendData(data);
}

void SerialDisplay::getDirection()
{
	if (!_deviceReady) return;
	if (_memDirection != Direction::unknown)
	{
		return;
	}
	QByteArray data(0x0009, 0x00);
	data[3] = 0x0c;
	QByteArray result = _serialDriver.sendDataAndWait(data, 1);

	if (result.size() == 1)
	{
		_memDirection = charToDirection(result[0]);
	}
}

void SerialDisplay::sendDirection()
{
	if (!_deviceReady) return;
	QByteArray data(0x000a, 0x00);
	data[3] = 0x0d;
	data[4] = directionToChar(_memDirection);
	_serialDriver.sendData(data);
}

void SerialDisplay::sendColor()
{
	if (!_deviceReady) return;
	QByteArray data(0x000b, 0x00);
	data[3] = 0x10;
	data[4] = colorToChar(_memForegroundColor);
	data[5] = colorToChar(_memBackgroundColor);
	_serialDriver.sendData(data);
}

void SerialDisplay::getColor()
{
	if (!_deviceReady) return;
	if ((_memForegroundColor != Color::unknown) && (_memBackgroundColor != Color::unknown))
	{
		return;
	}
	QByteArray data(0x0009, 0x00);
	data[3] = 0x11;
	QByteArray result = _serialDriver.sendDataAndWait(data, 2);

	if (result.size() == 2)
	{
		_memForegroundColor = charToColor(result[0]);
		_memBackgroundColor = charToColor(result[1]);
	}
}

void SerialDisplay::getFontSize()
{
	if (!_deviceReady) return;
	if (_memFontSize != 0)
	{
		return;
	}
	QByteArray data(0x0009, 0x00);
	data[3] = 0x1d;
	QByteArray result = _serialDriver.sendDataAndWait(data, 1);

	if (result.size() == 1)
	{
		switch (result[0]) {  // /!\ retour est en ascii
		case '1': _memFontSize = 1; break;
		case '2': _memFontSize = 2; break;
		case '3': _memFontSize = 3; break;
		default:  _memFontSize = 0; break;
		}
	}
}

void SerialDisplay::sendFontSize()
{
	if (!_deviceReady) return;
	QByteArray data(0x000a, 0x00);
	data[3] = 0x1f;
	data[4] = _memFontSize;
	_serialDriver.sendData(data);
}

void SerialDisplay::displayPoint(short x, short y, const Color color)
{
	if (!_deviceReady) return;
	if ((x<0) || (800<x) || (y<0) || (800<y)) return;
	setColor(color);
	QByteArray data(0x000d, 0x00);
	data[3] = 0x20;
	setShort(data, x, 4);
	setShort(data, y, 6);
	_serialDriver.sendData(data);
}

void SerialDisplay::displayLine(short x0, short y0, short x1, short y1, const Color color)
{
	if (!_deviceReady) return;
	if ((x0<0) || (800<x0) || (y0<0) || (800<y0) ||
		(x1<0) || (800<x1) || (y1<0) || (800<y1)) return;
	setColor(color);
	QByteArray data(0x0011, 0x00);
	data[3] = 0x22;
	setShort(data, x0,  4);
	setShort(data, y0,  6);
	setShort(data, x1,  8);
	setShort(data, y1, 10);
	_serialDriver.sendData(data);
}

void SerialDisplay::displayRectangle(short x0, short y0, short x1, short y1, bool filled, const Color color)
{
	if (!_deviceReady) return;
	if ((x0<0) || (800<x0) || (y0<0) || (800<y0) ||
		(x1<0) || (800<x1) || (y1<0) || (800<y1)) return;
	setColor(color);
	QByteArray data(0x0011, 0x00);
	if (filled)
	{
		data[3] = 0x24;
	}
	else
	{
		data[3] = 0x25;
	}
	setShort(data, x0,  4);
	setShort(data, y0,  6);
	setShort(data, x1,  8);
	setShort(data, y1, 10);
	_serialDriver.sendData(data);
}

void SerialDisplay::displayCircle(short x, short y, unsigned short radius, bool filled, const Color color)
{
	if (!_deviceReady) return;
	if ((x<0) || (800<x) || (y<0) || (800<y)) return;
	setColor(color);
	QByteArray data(0x000f, 0x00);
	if (filled)
	{
		data[3] = 0x27;
	}
	else
	{
		data[3] = 0x26;
	}
	setShort(data, x,      4);
	setShort(data, y,      6);
	setShort(data, radius, 8);
	_serialDriver.sendData(data);
}

void SerialDisplay::displayTriangle(short x0, short y0, short x1, short y1, short x2, short y2, bool filled, const Color color)
{
	if (!_deviceReady) return;
	if ((x0<0) || (800<x0) || (y0<0) || (800<y0) ||
		(x1<0) || (800<x1) || (y1<0) || (800<y1) ||
		(x2<0) || (800<x2) || (y2<0) || (800<y2)) return;
	setColor(color);
	QByteArray data(0x0015, 0x00);
	if (filled)
	{
		data[3] = 0x29;
	}
	else
	{
		data[3] = 0x28;
	}
	setShort(data, x0,  4);
	setShort(data, y0,  6);
	setShort(data, x1,  8);
	setShort(data, y1, 10);
	setShort(data, x2, 12);
	setShort(data, y2, 14);
	_serialDriver.sendData(data);
}

void SerialDisplay::clearScreen()
{
	if (!_deviceReady) return;
	QByteArray data(0x0009, 0x00);
	data[3] = 0x2e;
	QByteArray result = _serialDriver.sendDataAndWait(data, 2);
	checkResponse(result);
	_serialDriver.resetTotalBytesWritten();
}

void SerialDisplay::displayText(short x, short y, const QString &text, unsigned short fontSize, const Color color)
{
	if (!_deviceReady) return;
	if ((x<0) || (800<x) || (y<0) || (800<y)) return;
	setColor(color);
	setFontSize(fontSize);
	QByteArray data(8, 0x00);
	data[3] = 0x30;
	setShort(data, x,  4);
	setShort(data, y,  6);
	for (int i = 0; i < text.length() && i < 1019; i++)
	{
		// remove no ascii char
		if (text[i].unicode() <= 127)
			data.append(text[i]);
	}
	data.append('\0'); // add '\0' end of string
	data.append('\0'); // add 5 bytes for footer
	data.append('\0');
	data.append('\0');
	data.append('\0');
	data.append('\0');

	_serialDriver.sendData(data);
}

void SerialDisplay::checkResponse(QByteArray &response)
{
	if (response.size() != 2 || response[0] != 'O' || response[1] != '1')
	{
		emit commandNotAcked();
	}
}

/*----------------- OUTILS ------------------ */
static Color charToColor(char val)
{
	switch (val) { // /!\ retour est en ascii
	case '0': return Color::black; break;
	case '1': return Color::darkGray; break;
	case '2': return Color::lightGray; break;
	case '3': return Color::white; break;
	}
	return Color::unknown;
}

static char colorToChar(Color col)
{
	switch (col)
	{
	case Color::black:     return 0x00; break;
	case Color::darkGray:  return 0x01; break;
	case Color::lightGray: return 0x02; break;
	case Color::white:     return 0x03; break;
	default:               return 0x00; break;
	}
}

static Direction charToDirection(char val)
{
	switch (val) {  // /!\ retour est en ascii
	case '0': return Direction::horizontal0; break;
	case '1': return Direction::vertical1;   break;
	case '2': return Direction::horizontal2; break;
	case '3': return Direction::vertical3;   break;
	}
	return Direction::unknown;
}

static char directionToChar(Direction dir)
{
	switch (dir)
	{
	case Direction::horizontal0: return 0x00; break;
	case Direction::vertical1:   return 0x01; break;
	case Direction::horizontal2: return 0x02; break;
	case Direction::vertical3:   return 0x03; break;
	case Direction::unknown:     return 0x02; break;
	}
	return 0x02;
}

static void setShort(QByteArray &data, unsigned short val, int pos)
{
	data[pos  ] = (val & 0xff00) >> 8;
	data[pos+1] =  val & 0x00ff;
}
