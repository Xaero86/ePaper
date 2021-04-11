#ifndef DRAWABLETEXT_H
#define DRAWABLETEXT_H

#include "drawableelement.h"

class DrawableText : public DrawableElement
{
public:
	DrawableText(short x, short y, const QString &text, unsigned short fontSize, Color color);

	void display(DisplayManager *displayManager);
	void displayMoving(DisplayManager *displayManager, int deltaX, int deltaY);
	QString toString() {return QString("Texte");}
	QString parametersToString();

	static DrawableText* createText(QDomElement &element);
	void toXml(QDomElement* root);

	void move(int deltaX, int deltaY);

	short x() {return _x;}
	short y() {return _y;}
	QString text() {return _text;}
	unsigned short fontSize() {return _fontSize;}

	void setX(short x) {_x=x;}
	void setY(short y) {_y=y;}
	void setText(QString text) {_text=text;}
	void setFontSize(unsigned short fontSize) {_fontSize=fontSize;}

private:
	short _x;
	short _y;
	QString        _text;
	unsigned short _fontSize;
};

#endif // DRAWABLETEXT_H
