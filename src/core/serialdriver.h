#ifndef SERIALDRIVER_H
#define SERIALDRIVER_H

#include <QtSerialPort/QSerialPort>
#include <QQueue>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

/**
 * Classe pour encapsuler un QSerialPort et l'executer dans un thread unique
 */
class SerialThread : public QObject
{
	Q_OBJECT
public:
	SerialThread();
	virtual ~SerialThread();

	bool isConfigured();
	bool isOpen();
	QByteArray readAll();

	void checkPortValidity();

signals:
	// Intern
	void handleBytesWrittenO(qint64 bytes);
	void handleBytesReadO();
	// Extern
	void errorInitPort();

protected slots:
	void open(QMutex* mutex = nullptr, QWaitCondition* condition = nullptr);
	void close(QMutex* mutex = nullptr, QWaitCondition* condition = nullptr);
	void setPortName(const QString &port);
	void wakeUp();
	void write(const QByteArray &data);
	void handleBytesWrittenI(qint64 bytes);
	void handleBytesReadI();

private:
	QThread       _myThread;
	QSerialPort   _serialPort;
	bool          _isConfigured;

	static bool isPortValid(const QString &port);
};

class SerialDriver : public QObject
{
	Q_OBJECT
public:
	SerialDriver();
	virtual ~SerialDriver();

	void open(bool lock);
	void close(bool lock);
	bool isOpen() {return _serialThread.isOpen();}
	void setPortName(const QString &port);
	void wakeUp();
	bool isConfigured() {return _serialThread.isConfigured();}
	void updateWriteStatus();

	void sendData(QByteArray &data);
	QByteArray sendDataAndWait(QByteArray &data, unsigned int replySize);

	void resetTotalBytesWritten();
	void clearFifo();

signals:
	// Extern
	void timeoutInWrite();
	void timeoutInRead();
	void newWriteStatus(int percent);
	// Intern
	void requestOpen(QMutex* mutex = nullptr, QWaitCondition* condition = nullptr);
	void requestClose(QMutex* mutex = nullptr, QWaitCondition* condition = nullptr);
	void requestSetPortName(const QString &port);
	void requestWakeUp();
	void newDataInFifo();
	void byteToWrite(const QByteArray &data);

protected slots:
	void sendData();
	void handleBytesWritten(qint64 bytes);
	void handleBytesRead();

private:
	QThread            _myThread;
	SerialThread       _serialThread;
	bool               _mayBeReady;

	struct t_comEvent {
		QByteArray      data;      // les donnees a envoyer
		QMutex*         mutex;     // si attente reponse
		QWaitCondition* waitCond;  // si attente reponse
		QByteArray*     reply;     // si attente reponse: buffer contenant la reponse
		unsigned int    replySize; // si attente reponse: taille du message de reponse attendu
	};
	// la fifo et son mutex
	QQueue<t_comEvent> _outputFifo;
	QMutex             _mutexFifo;

	// permet de se mettre en attente de la confirmation de l'envoi du message
	QMutex             _mutexAckOutput;
	QWaitCondition     _waitSendAck;

	// controle de l'envoi d'un message
	unsigned int       _bytesToWrite;
	unsigned int       _bytesWritten;

	// permet de se mettre en attente de la reponse a un message
	QMutex             _mutexReadData;
	QWaitCondition     _waitReadData;
	int                _bytesToRead;
	QByteArray*        _bytesRead;

	// poubelle
	QByteArray         _bytesReadNullifier;

	// etat de la fifo depuis un clear screen
	long               _totalBytesToWrite;
	long               _totalBytesWritten;
	int                _previousWriteStatus;

	void prepareData(QByteArray &data);
	void addToFifo(QByteArray &data, QMutex *mutex = nullptr, QWaitCondition *waitCond = nullptr, QByteArray *reply = nullptr, unsigned int replySize = 0);
};

#endif // SERIALDRIVER_H
