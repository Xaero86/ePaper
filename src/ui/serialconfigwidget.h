#ifndef SERIALCONFIGWIDGET_H
#define SERIALCONFIGWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>

class SerialPortCombo : public QComboBox
{
	Q_OBJECT
public:
	SerialPortCombo(QWidget *parent);

	void showPopup();
	void hidePopup();
	void setSerialPort(const QString &port);

	QString getPreviousPort() {return _previousPort;}

private:
	QStringList _serialPortList;
	QString     _previousPort;

	void updateList();
};

class SerialConfigWidget : public QWidget
{
	Q_OBJECT
public:
	SerialConfigWidget(QWidget *parent);

	void setSerialPort(const QString &port);

signals:
	void changeSerialPort(const QString &port);
	void commandDraw();
	void commandIdle();

protected slots:
	void internChangeSerialPort(const QString &port);
	void internCommandDraw();
	void internCommandIdle();

private:
	SerialPortCombo* _serialPortCombo;
	QPushButton*     _buttonDisplay;
	QPushButton*     _buttonIdle;

	void updatePortList();
};

#endif // SERIALCONFIGWIDGET_H
