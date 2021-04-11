#include "epapercontroller.h"

#include "properties.h"

#include <iostream>

EPaperController::EPaperController()
	: _session(Properties::defaultDirection(), Properties::defaultBackgroundColor()),
	  _everythingOK(true), _previousStatus(-2)
{
	_myThread.start();
	moveToThread(&_myThread);
	
	// CONNECTION
	connect(&_session, SIGNAL(statusChanged(int)),           this, SLOT(handleStatus(int)));
	connect(&_session, SIGNAL(errorLoading(QString)),        this, SLOT(handleError(QString)), Qt::DirectConnection);
	connect(&_session, SIGNAL(newWriteStatus(int)),          this, SLOT(updateWriteStatus(int)));
}

EPaperController::~EPaperController()
{
	_myThread.quit();
	_myThread.wait();
}

int EPaperController::display(const QString &path, bool sendVeille, bool sendRefresh)
{
	if (!path.isEmpty())
	{
		QUrl imageFile = QUrl::fromLocalFile(path);
		_session.loadImage(imageFile);
		
		if (!_everythingOK)
		{
			return 1;
		}
	}

	_mutexStatus.lock();
	_session.changeSerialPort(Properties::defaultSerialPort());
	_waitStatusCondition.wait(&_mutexStatus);
	_mutexStatus.unlock();
	
	if (!_everythingOK)
	{
		return 1;
	}
	
	_previousStatus = -1;

	if (!path.isEmpty())
	{
		if (sendRefresh)
		{
			_session.drawSerial(false);
		}
		else
		{
			_session.sendData(false);
		}
		_session.waitReady();

		if (!_everythingOK)
		{
			return 1;
		}

		if (sendVeille)
		{
			QThread::sleep(2);
		}
	}

	if (sendVeille)
	{
		_mutexWrite.lock();
		_session.turnIdleSerial();
		_waitWriteCondition.wait(&_mutexWrite);
		_mutexWrite.unlock();

		if (!_everythingOK)
		{
			return 1;
		}
	}
	return 0;
}

int EPaperController::refresh(bool sendVeille)
{
	_mutexStatus.lock();
	_session.changeSerialPort(Properties::defaultSerialPort());
	_waitStatusCondition.wait(&_mutexStatus);
	_mutexStatus.unlock();

	if (!_everythingOK)
	{
		return 1;
	}

	_previousStatus = -1;

	_session.refresh(false);
	_session.waitReady();

	if (!_everythingOK)
	{
		return 1;
	}

	if (sendVeille)
	{
		QThread::sleep(2);
		_mutexWrite.lock();
		_session.turnIdleSerial();
		_waitWriteCondition.wait(&_mutexWrite);
		_mutexWrite.unlock();

		if (!_everythingOK)
		{
			return 1;
		}
	}
	return 0;
}

void EPaperController::handleStatus(int status)
{
	if (status == NOT_CONNECTED)
	{
		_everythingOK = false;
	}
	
	if (status != CONNECTING)
	{
		_mutexStatus.lock();
		_waitStatusCondition.wakeOne();
		_mutexStatus.unlock();
	}
}

void EPaperController::handleError(QString errorMsg)
{
	_everythingOK = false;
	std::cerr << "Erreur : " << errorMsg.toStdString() << std::endl;
}

void EPaperController::updateWriteStatus(int percent)
{
	if (_previousStatus == -2)
	{
		return;
	}
	
	if (percent - _previousStatus > 9)
	{
		std::cout << percent << "%" << std::endl;
		_previousStatus = percent;
	}
	
	if (percent == -1)
	{
		_previousStatus = -1;
		std::cout << "Envoi fini" << std::endl;
		_mutexWrite.lock();
		_waitWriteCondition.wakeOne();
		_mutexWrite.unlock();
	}
}
