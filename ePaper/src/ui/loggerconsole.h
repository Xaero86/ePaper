#ifndef LOGGERCONSOLE
#define LOGGERCONSOLE

#include <QtWidgets>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>

using namespace std;

class TextEditSteamBuf;
class TeeStreamBuf;

class LoggerConsole : public QTextEdit
{
	Q_OBJECT
public:
	LoggerConsole(QWidget *parent = nullptr);
	virtual ~LoggerConsole();

	QSize sizeHint() const;

public slots:
	void connectLogger();
	void disconnectLogger();
	void appendRed(QString str);
	void saveToFile();

private:
	static bool isStandardOutputConnected;

	bool _isTheOneConnected;
	TextEditSteamBuf *_textEditBufOut;
	TextEditSteamBuf *_textEditBufErr;
	TeeStreamBuf *_teeBufCout;
	TeeStreamBuf *_teeBufCerr;
};

class TextEditSteamBuf : public QObject, public std::streambuf
{
	Q_OBJECT
public:
	TextEditSteamBuf() {}
	~TextEditSteamBuf() {}

signals:
	void overflow(QString str);

protected:
	virtual int_type overflow(int_type c);

private:
	std::stringstream _internalBuffer;
};

class TeeStreamBuf : public std::streambuf
{
public:
	TeeStreamBuf(std::ostream &mainStream, std::streambuf *otherBuf)
		: _mainStream(mainStream), _otherBuf(otherBuf)
	{
		_mainBuf = mainStream.rdbuf();
		_mainStream.rdbuf(this);
	}
	~TeeStreamBuf() {_mainStream.rdbuf(_mainBuf);}

protected:
	virtual int overflow(int c);
	virtual int sync();

private:
	std::ostream &_mainStream;
	std::streambuf *_mainBuf;
	std::streambuf *_otherBuf;
};

#endif // LOGGERCONSOLE

