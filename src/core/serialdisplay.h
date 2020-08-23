#ifndef SERIALDISPLAY_H
#define SERIALDISPLAY_H

#include "displaymanager.h"
#include "serialdriver.h"

#include <QRunnable>

class SerialDisplay : public QObject, virtual public DisplayManager
{
	Q_OBJECT
public:
	SerialDisplay();
	virtual ~SerialDisplay();

	void changePort(const QString &port);
	void connectSerial();
	void disconnectSerial();

	void setBackgroundColor(const Color backgroundColor);
	void setDirection(const Direction direction);

	void displayPoint(short x, short y, const Color color);
	void displayLine(short x0, short y0, short x1, short y1, const Color color);
	void displayRectangle(short x0, short y0, short x1, short y1, bool filled, const Color color);
	void displayCircle(short x, short y, unsigned short radius, bool filled, const Color color);
	void displayTriangle(short x0, short y0, short x1, short y1, short x2, short y2, bool filled, const Color color);
	void displayText(short x, short y, const QString &text, unsigned short fontSize, const Color color);
	void clearScreen();

	void turnIdle();
	void refresh();

	void cancelOperation();

signals:
	void statusChanged(int status);
	void newWriteStatus(int percent);
	void commandNotAcked();

public slots:
	void manageTimeoutInWrite();
	void manageTimeoutInRead();
	void updateWriteStatus(int percent);

private:
	SerialDriver  _serialDriver;

	bool          _deviceReady;
	Direction     _memDirection;
	Color         _memForegroundColor;
	Color         _memBackgroundColor;
	int           _memFontSize;

	void initSerial();

	void setColor(const Color foregroundColor);
	void setFontSize(int size);
	bool wakeUp(int nbTry);

	bool checkReady();
	void setBaudRate(unsigned int);
	int getBaudRate();
	void getDirection();
	void sendDirection();
	void sendColor();
	void getColor();
	void getFontSize();
	void sendFontSize();

	void checkResponse(QByteArray &response);
	void newStatus(int status);

	class ConnexionRunnable : public QRunnable
	{
	public:
		ConnexionRunnable(SerialDisplay* serialDisplay) : _serialDisplay(serialDisplay) {}
		void run();

	private:
		SerialDisplay* _serialDisplay;
	};

	ConnexionRunnable* _connexionRunnable;
};

#endif // SERIALDISPLAY_H
