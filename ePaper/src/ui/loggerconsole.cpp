#include "loggerconsole.h"

#include <QString>

bool LoggerConsole::isStandardOutputConnected = false;

LoggerConsole::LoggerConsole(QWidget *parent) : QTextEdit(parent)
{
	setReadOnly(true);
	setAcceptRichText(true);

	_isTheOneConnected = false;
	connectLogger();
}

LoggerConsole::~LoggerConsole()
{
	disconnectLogger();
}

QSize LoggerConsole::sizeHint() const
{
	return QSize(0, 70);
}

void LoggerConsole::connectLogger()
{
	if (!isStandardOutputConnected)
	{
		isStandardOutputConnected = true;
		_isTheOneConnected = true;

		_textEditBufOut = new TextEditSteamBuf();
		_textEditBufErr = new TextEditSteamBuf();

		_teeBufCout = new TeeStreamBuf(std::cout, _textEditBufOut);
		_teeBufCerr = new TeeStreamBuf(std::cerr, _textEditBufErr);

		connect(_textEditBufOut, SIGNAL(overflow(QString)), this, SLOT(append(QString)));
		connect(_textEditBufErr, SIGNAL(overflow(QString)), this, SLOT(appendRed(QString)));
	}
}

void LoggerConsole::disconnectLogger()
{
	if (_isTheOneConnected)
	{
		delete _teeBufCout;
		delete _teeBufCerr;
		delete _textEditBufOut;
		delete _textEditBufErr;

		isStandardOutputConnected = false;
		_isTheOneConnected = false;
	}
}

void LoggerConsole::appendRed(QString str)
{
	append("<font color=\"Red\">" + str + "</font>");
}

void LoggerConsole::saveToFile()
{
	QString contents = toPlainText();

	if (!contents.isEmpty())
	{
		QDateTime dateTime = dateTime.currentDateTime();
		QString dateTimeString = dateTime.toString("yyyy_MM_dd_hh_mm_ss");

		QFile file(dateTimeString + ".log");

		if (file.open(QIODevice::WriteOnly))
		{
			QTextStream stream(&file);
			stream << contents;
			file.flush();
			file.close();
		}
	}
}

TextEditSteamBuf::int_type TextEditSteamBuf::overflow(int_type c)
{
	if ((c == EOF) || (c == '\n'))
	{
		QString str(_internalBuffer.str().c_str());
		emit overflow(str);
		_internalBuffer.str("");
	}
	else
	{
		_internalBuffer << (char) c;
	}
	return c;
}

int TeeStreamBuf::overflow(int c)
{
	if (c == EOF)
	{
		return !EOF;
	}
	else
	{
		int const r1 = _mainBuf->sputc(c);
		int const r2 = _otherBuf->sputc(c);
		return r1 == EOF || r2 == EOF ? EOF : c;
	}
}

int TeeStreamBuf::sync()
{
	int const r1 = _mainBuf->pubsync();
	int const r2 = _otherBuf->pubsync();
	return r1 == 0 && r2 == 0 ? 0 : -1;
}
