#ifndef EPAPERCONTROLLER_H
#define EPAPERCONTROLLER_H

#include "definition.h"
#include "session.h"

#include <QObject>

class EPaperController : public QObject
{
	Q_OBJECT

public:
	EPaperController();
	~EPaperController();
	
	int display(const QString &path, bool sendVeille, bool sendRefresh);
	int refresh(bool sendVeille);

public slots:
	void handleStatus(int status);
	void updateWriteStatus(int percent);
	void handleError(QString errorMsg);

private:
	Session        _session;
	
	bool           _everythingOK;
	int            _previousStatus;
	
	QThread        _myThread;
	QMutex         _mutexStatus;
	QWaitCondition _waitStatusCondition;
	QMutex         _mutexWrite;
	QWaitCondition _waitWriteCondition;
};

#endif // EPAPERCONTROLLER_H
