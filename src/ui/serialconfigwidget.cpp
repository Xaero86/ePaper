#include "serialconfigwidget.h"

#include "definition.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtSerialPort/QSerialPortInfo>

SerialConfigWidget::SerialConfigWidget(QWidget *parent)
	: QWidget(parent)
{
	_serialPortCombo = new SerialPortCombo(this);
	_buttonDisplay = new QPushButton("Afficher", this);
	_buttonIdle = new QPushButton("Veille", this);

	QVBoxLayout* vbl = new QVBoxLayout;
	vbl->addWidget(_serialPortCombo);
	vbl->addWidget(_buttonDisplay);
	vbl->addWidget(_buttonIdle);
	vbl->addStretch(1);

	QHBoxLayout* hbl = new QHBoxLayout;
	hbl->addStretch();
	hbl->addLayout(vbl);
	hbl->addStretch();

	setLayout(hbl);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

	connect(_serialPortCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(internChangeSerialPort(QString)));
	connect(_buttonDisplay, SIGNAL(released()), this, SLOT(internCommandDraw()));
	connect(_buttonIdle, SIGNAL(released()), this, SLOT(internCommandIdle()));
}

void SerialConfigWidget::setSerialPort(const QString &port)
{
	_serialPortCombo->setSerialPort(port);
}

void SerialConfigWidget::internChangeSerialPort(const QString &port)
{
	if (QString::compare(port, _serialPortCombo->getPreviousPort(), Qt::CaseInsensitive) != 0)
	{
		emit changeSerialPort(port);
	}
}

void SerialConfigWidget::internCommandDraw()
{
	emit commandDraw();
}

void SerialConfigWidget::internCommandIdle()
{
	emit commandIdle();
}

SerialPortCombo::SerialPortCombo(QWidget *parent)
	: QComboBox(parent), _serialPortList(), _previousPort(NONE_PORT)
{
	updateList();
}

void SerialPortCombo::showPopup()
{
	_previousPort = currentText();
	updateList();
	QComboBox::showPopup();
}

void SerialPortCombo::hidePopup()
{
	QComboBox::hidePopup();
}

void SerialPortCombo::setSerialPort(const QString &port)
{
	_previousPort = currentText();
	updateList();
	int newIndex = findText(port, Qt::MatchFixedString);
	if (newIndex > -1)
	{
		setCurrentIndex(newIndex);
	}
}

void SerialPortCombo::updateList()
{
	QStringList newList(QString(NONE_PORT));
	QList<QSerialPortInfo> listPort = QSerialPortInfo::availablePorts();

	for (int i = 0; i < listPort.length(); i++)
	{
		newList.append(listPort.at(i).portName());
	}

	bool needChange = false;
	if (_serialPortList.count() != newList.length())
	{
		needChange = true;
	}
	else
	{
		for (int i = 0; i < listPort.length() && !needChange; i++)
		{
			if (QString::compare(_serialPortList[i], newList[i], Qt::CaseInsensitive) != 0)
			{
				needChange = true;
			}
		}
	}

	if (needChange)
	{
		while(count()>0)
		{
			removeItem(0);
		}
		addItems(newList);
		_serialPortList = newList;
	}
}

