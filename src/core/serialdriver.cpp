#include "serialdriver.h"

#include "definition.h"
#include "properties.h"

#include <QtSerialPort/QSerialPortInfo>
#include <QProcess>
#include <iostream>

SerialDriver::SerialDriver()
	: QObject(), _myThread(), _serialThread(), _mayBeReady(false),
	_outputFifo(), _mutexFifo(),
	_mutexAckOutput(),_waitSendAck(),
	_bytesToWrite(0), _bytesWritten(0),
	_mutexReadData(), _waitReadData(),
	_bytesToRead(0), _bytesRead(nullptr),
	_totalBytesToWrite(0), _totalBytesWritten(0), _previousWriteStatus(-1)
{
	_myThread.start();
	moveToThread(&_myThread);

	connect(this, SIGNAL(newDataInFifo()),            this, SLOT(sendData()));

	connect(&_serialThread, SIGNAL(handleBytesWrittenO(qint64)), this, SLOT(handleBytesWritten(qint64)), Qt::DirectConnection);
	connect(&_serialThread, SIGNAL(handleBytesReadO()),          this, SLOT(handleBytesRead()), Qt::DirectConnection);

	connect(this, SIGNAL(requestOpen(QMutex*, QWaitCondition*)),  &_serialThread, SLOT(open(QMutex*, QWaitCondition*)));
	connect(this, SIGNAL(requestClose(QMutex*, QWaitCondition*)), &_serialThread, SLOT(close(QMutex*, QWaitCondition*)));
	connect(this, SIGNAL(requestSetPortName(QString)),            &_serialThread, SLOT(setPortName(QString)));
	connect(this, SIGNAL(requestWakeUp()),                        &_serialThread, SLOT(wakeUp()), Qt::DirectConnection);
	connect(this, SIGNAL(byteToWrite(QByteArray)),                &_serialThread, SLOT(write(QByteArray)));
}

SerialDriver::~SerialDriver()
{
	close(true);
	clearFifo();
	_myThread.quit();
	_myThread.wait();
}

void SerialDriver::open(bool lock)
{
	if (lock)
	{
		QMutex openMutex;
		QWaitCondition waitCondition;
		
		openMutex.lock();
		emit requestOpen(&openMutex, &waitCondition);
		waitCondition.wait(&openMutex);
		openMutex.unlock();
	}
	else
	{
		emit requestOpen();
	}
	_mayBeReady = true;
}

void SerialDriver::close(bool lock)
{
	if (lock)
	{
		QMutex openMutex;
		QWaitCondition waitCondition;
		
		openMutex.lock();
		emit requestClose(&openMutex, &waitCondition);
		waitCondition.wait(&openMutex);
		openMutex.unlock();
	}
	else
	{
		emit requestClose();
	}
	_mayBeReady = false;
}

void SerialDriver::setPortName(const QString &port)
{
	emit requestSetPortName(port);
}

void SerialDriver::wakeUp()
{
	emit requestWakeUp();
}

void SerialDriver::resetTotalBytesWritten()
{
	_totalBytesToWrite = 0;
	_totalBytesWritten = 0;
}

void SerialDriver::updateWriteStatus()
{
	int writeStatus;
	if (_totalBytesToWrite == 0)
	{
		writeStatus = -1;
	}
	else
	{
		writeStatus = (100*_totalBytesWritten)/_totalBytesToWrite;
	}
	if ((writeStatus == -1) || (writeStatus - _previousWriteStatus > 2))
	{
		emit newWriteStatus(writeStatus);
		_previousWriteStatus = writeStatus;
	}
}

void SerialDriver::sendData(QByteArray &data)
{
	prepareData(data);
	if (_mayBeReady)
	{
		addToFifo(data);
	}
}

QByteArray SerialDriver::sendDataAndWait(QByteArray &data, unsigned int replySize)
{
	QMutex ackMutex;
	QWaitCondition waitCondition;
	QByteArray reply;

	prepareData(data);

	if (_mayBeReady)
	{
		ackMutex.lock();
		addToFifo(data, &ackMutex, &waitCondition, &reply, replySize);
		waitCondition.wait(&ackMutex);
		ackMutex.unlock();
	}

	return reply;
}

void SerialDriver::prepareData(QByteArray &data)
{
	int totalSize = data.length();
	data[0] = 0xa5; // header
	data[1] = (totalSize & 0xff00) >> 8;
	data[2] =  totalSize & 0x00ff;

	data[totalSize - 5] = 0xcc; // footer
	data[totalSize - 4] = 0x33; // footer
	data[totalSize - 3] = 0xc3; // footer
	data[totalSize - 2] = 0x3c; // footer

	char parity = data[0];
	for (int i = 1; i < totalSize - 1; i++)
	{
		parity ^= data[i];
	}
	data[totalSize - 1] = parity;
}

void SerialDriver::addToFifo(QByteArray &data, QMutex *mutex, QWaitCondition *waitCond, QByteArray *reply, unsigned int replySize)
{
	t_comEvent event;
	event.data = data;
	event.mutex = mutex;
	event.waitCond = waitCond;
	event.reply = reply;
	event.replySize = replySize;

	_totalBytesToWrite += data.length();

	_mutexFifo.lock();
	_outputFifo.enqueue(event);
	emit newDataInFifo();
	_mutexFifo.unlock();
}

void SerialDriver::clearFifo()
{
	_mutexFifo.lock();
	while (!_outputFifo.isEmpty())
	{
		t_comEvent event = _outputFifo.dequeue();
		if ((event.mutex != nullptr) && (event.waitCond != nullptr))
		{
			// reveil du thread en attente de la reponse
			event.mutex->lock();
			event.waitCond->wakeOne();
			event.mutex->unlock();
		}
	}
	resetTotalBytesWritten();
	updateWriteStatus();
	_mutexFifo.unlock();

	// on debloque le thread de fifo pour ne pas avoir un autre timeout
	_mutexAckOutput.lock();
	_waitSendAck.wakeOne();
	_mutexAckOutput.unlock();

	// on debloque le thread de fifo pour ne pas avoir un autre timeout
	_mutexReadData.lock();
	_waitReadData.wakeOne();
	_mutexReadData.unlock();
}

void SerialDriver::sendData()
{
	bool dataRead;
	int nbTryWrite = 0;
	bool dataSent;

	_mutexFifo.lock();
	if (_outputFifo.isEmpty())
	{
		_mutexFifo.unlock();
		return;
	}
	t_comEvent event = _outputFifo.dequeue();
	_mutexFifo.unlock();

	QByteArray data = event.data;
	_bytesToWrite = data.size();
	_bytesWritten = 0;

	if (event.replySize > 0)
	{
		// on vide le buffer input
		do {
			_mutexReadData.lock();
			dataRead = _waitReadData.wait(&_mutexReadData, 200);
			_mutexReadData.unlock();
		} while (dataRead);
		_bytesToRead = event.replySize;
		_bytesRead = event.reply;
	}

	do {
		// envoi du message
		emit byteToWrite(data);
		nbTryWrite++;

		// attente de la confirmation de l'envoi
		_mutexAckOutput.lock();
		dataSent = _waitSendAck.wait(&_mutexAckOutput, TIMEOUT_WRITE);
		_mutexAckOutput.unlock();
		
		if (!dataSent)
		{
			std::cerr << "Warning: Timeout writing data. Retry (" << nbTryWrite << ")" << std::endl;
		}
	} while (!dataSent && nbTryWrite < NB_TRY_WRITE_MAX);

	if (!dataSent)
	{
		// probleme d'emission confirme
		emit timeoutInWrite();

		// on libere le thread en attente de la reponse
		if (event.mutex != nullptr && event.waitCond != nullptr)
		{
			event.mutex->lock();
			event.waitCond->wakeOne();
			event.mutex->unlock();
		}
		return;
	}

	if (event.mutex != nullptr && event.waitCond != nullptr && event.reply != nullptr && event.replySize > 0)
	{
		// attente de la reponse
		_mutexReadData.lock();
		dataRead = _waitReadData.wait(&_mutexReadData, TIMEOUT_READ);
		_mutexReadData.unlock();

		if (!dataRead)
		{
			emit timeoutInRead();
		}
		_bytesRead = nullptr;

		// reveil du thread en attente de la reponse
		event.mutex->lock();
		event.waitCond->wakeOne();
		event.mutex->unlock();
	}
	
	_mutexFifo.lock();
	if (_outputFifo.isEmpty())
	{
		resetTotalBytesWritten();
	}
	_mutexFifo.unlock();
	updateWriteStatus();
}

void SerialDriver::handleBytesWritten(qint64 bytes)
{
	_bytesWritten += bytes;
	if (_bytesWritten == _bytesToWrite)
	{
		// suffisamment de bytes ecrits, deblocage du thread d'envoi
		_bytesWritten = 0;
		_mutexAckOutput.lock();
		_waitSendAck.wakeOne();
		_mutexAckOutput.unlock();
	}
	_totalBytesWritten += bytes;
}

void SerialDriver::handleBytesRead()
{
	if (_bytesRead != nullptr)
	{
		_bytesRead->append(_serialThread.readAll());
		if (_bytesRead->size() >= _bytesToRead)
		{
			// suffisamment de bytes lus, deblocage du thread de lecture
			_mutexReadData.lock();
			_waitReadData.wakeOne();
			_mutexReadData.unlock();
		}
	}
	else
	{
		_bytesReadNullifier.append(_serialThread.readAll());
		_bytesReadNullifier.clear();
		_mutexReadData.lock();
		_waitReadData.wakeOne();
		_mutexReadData.unlock();
	}
}

SerialThread::SerialThread()
	: QObject(), _myThread(), _serialPort(), _isConfigured(false)
{
	_myThread.start();
	this->moveToThread(&_myThread);
	_serialPort.moveToThread(&_myThread);
	// option deprecated !
	_serialPort.setSettingsRestoredOnClose(false);
	connect(&_serialPort, SIGNAL(bytesWritten(qint64)), this, SLOT(handleBytesWrittenI(qint64)));
	connect(&_serialPort, SIGNAL(readyRead()), this, SLOT(handleBytesReadI()));
}

SerialThread::~SerialThread()
{
	_myThread.quit();
	_myThread.wait();
}

void SerialThread::open(QMutex* mutex, QWaitCondition* condition)
{
	if (_isConfigured)
	{
		_serialPort.open(QIODevice::ReadWrite);
	}
	
	if ((mutex != nullptr) && (condition != nullptr))
	{
		mutex->lock();
		condition->wakeOne();
		mutex->unlock();
	}
}

void SerialThread::close(QMutex* mutex, QWaitCondition* condition)
{
	checkPortValidity();
	if (_serialPort.isOpen())
	{
		_serialPort.clear(QSerialPort::AllDirections);
		_serialPort.close();
	}
	
	if ((mutex != nullptr) && (condition != nullptr))
	{
		mutex->lock();
		condition->wakeOne();
		mutex->unlock();
	}
}

bool SerialThread::isConfigured()
{
	return _isConfigured;
}

bool SerialThread::isOpen()
{
	return _serialPort.isOpen();
}

bool SerialThread::isPortValid(const QString &port)
{
	QList<QSerialPortInfo> listPort = QSerialPortInfo::availablePorts();

	for (int i = 0; i < listPort.length(); i++)
	{
		if (QString::compare(listPort.at(i).portName(), &port) == 0)
		{
			return true;
		}
	}
	return false;
}

void SerialThread::checkPortValidity()
{
	if (!isPortValid(_serialPort.portName()))
	{
		_isConfigured = false;
	}
}

void SerialThread::setPortName(const QString &port)
{
	_isConfigured = false;
	if (isPortValid(port))
	{
		_serialPort.setPortName(port);

		bool configResult = true;
		configResult &= _serialPort.setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
		configResult &= _serialPort.setDataBits(QSerialPort::Data8);
		configResult &= _serialPort.setFlowControl(QSerialPort::NoFlowControl);
		configResult &= _serialPort.setParity(QSerialPort::NoParity);
		configResult &= _serialPort.setStopBits(QSerialPort::OneStop);
		if (!configResult)
		{
			std::cerr << "Error configuring port" << std::endl;
			emit errorInitPort();
		}
		else
		{
			_isConfigured = true;
			if (Properties::wakeUpMode() == WAKE_UP_DTR)
			{
				_serialPort.setDataTerminalReady(true);
			}
			else if (Properties::wakeUpMode() == WAKE_UP_RTS)
			{
				_serialPort.setRequestToSend(true);
			}
		}
	}
}

void SerialThread::wakeUp()
{
	if (_serialPort.isOpen())
	{
		if (Properties::wakeUpMode() == WAKE_UP_DTR)
		{
			_serialPort.setDataTerminalReady(false);
			QThread::msleep(Properties::wakeUpDelay());
			_serialPort.setDataTerminalReady(true);
		}
		else if (Properties::wakeUpMode() == WAKE_UP_RTS)
		{
			_serialPort.setRequestToSend(false);
			QThread::msleep(Properties::wakeUpDelay());
			_serialPort.setRequestToSend(true);
		}
		else if (Properties::wakeUpMode() == WAKE_UP_DTR_INV)
		{
			_serialPort.setDataTerminalReady(true);
			QThread::msleep(Properties::wakeUpDelay());
			_serialPort.setDataTerminalReady(false);
		}
		else if (Properties::wakeUpMode() == WAKE_UP_RTS_INV)
		{
			_serialPort.setRequestToSend(true);
			QThread::msleep(Properties::wakeUpDelay());
			_serialPort.setRequestToSend(false);
		}
		else if (Properties::wakeUpMode() == WAKE_UP_SCRIPT)
		{
			QProcess process;
			process.start(Properties::wakeUpScript());
			process.waitForFinished(Properties::wakeUpDelay());
		}
	}
}

QByteArray SerialThread::readAll()
{
	if (_serialPort.isOpen())
	{
		return _serialPort.readAll();
	}
	else
	{
		return QByteArray();
	}
}

void SerialThread::write(const QByteArray &data)
{
	if (_serialPort.isOpen())
	{
		_serialPort.write(data);
	}
}

void SerialThread::handleBytesWrittenI(qint64 bytes)
{
	emit handleBytesWrittenO(bytes);
}

void SerialThread::handleBytesReadI()
{
	emit handleBytesReadO();
}

