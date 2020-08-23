#ifndef SESSION_H
#define SESSION_H

#include <QList>

#include "displaymanager.h"
#include "serialdisplay.h"
#include "drawableelement.h"

class Session : public QObject
{
	Q_OBJECT
public:
	Session(Direction direction, Color backgroundColor);
	virtual ~Session();

	QList<DrawableElement*>* getElements() {return &_elements;}

	void addElement(DrawableElement *element);
	void removeElement(int index);
	void moveElement(int from, int to);
	void moveElements(QList<int> &fromList, int to);
	void clearElements();

	void setDirection(Direction direction);
	void setBackgroundColor(Color backgroundColor);

	Direction getDirection() {return _direction;}
	Color getBackgroundColor() {return _backgroundColor;}

	void loadImage(QUrl &path);
	void saveImage(QUrl &path);

signals:
	void elementsAdded(int row, int count);
	void elementsRemoved(int row, int count);
	void elementsMoved(int dest, int count);
	void directionChanged(bool fromIHM);
	void backgroundColorChanged(bool fromIHM);
	void errorLoading(QString errorMsg);
	void errorSaving(QString errorMsg);
	void statusChanged(int status);
	void newWriteStatus(int percent);

public slots:
	void handleStatus(int status);
	void changeSerialPort(const QString &port);
	void drawSerial();
	void cancelSerial();
	void turnIdleSerial();
	void updateWriteStatus(int percent);

private:
	SerialDisplay              _serialDisplay;

	QList<DrawableElement*>    _elements;
	Direction                  _direction;
	Color                      _backgroundColor;

	class SerialDrawRunnable : public QRunnable
	{
	public:
		SerialDrawRunnable(Session* session) : _session(session) {}
		void run();

	private:
		Session* _session;
	};
};

#endif // SESSION_H
