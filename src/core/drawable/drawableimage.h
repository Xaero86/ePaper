#ifndef DRAWABLEIMAGE_H
#define DRAWABLEIMAGE_H

#include "drawableelement.h"

#include <QImage>

class DrawableImage : public DrawableElement
{
public:
	DrawableImage(const QString &path, unsigned int colorDepth, QMap<unsigned int, Color> &palette, Color transparencyColor, const QRect area = QRect(), Direction expectedDirection = Direction::unknown);
	virtual ~DrawableImage();

	void display(DisplayManager *displayManager);
	void displayMoving(DisplayManager *displayManager, int deltaX, int deltaY);
	QString toString() {return QString("Image");}
	QString parametersToString();

	static DrawableImage* createImage(QDomElement &element, Color backgroundColor, Direction globalDirection);
	void toXml(QDomElement* root);

	void move(int deltaX, int deltaY);

	Direction getDirection() {return _directionPilot;}
	bool isValid() {return _isValid;}

private:
	bool                    _isValid;
	QString                 _path;
	QList<DrawableElement*> _componentsWhite;
	QList<DrawableElement*> _componentsLightGray;
	QList<DrawableElement*> _componentsDarkGray;
	QList<DrawableElement*> _componentsBlack;
	QMap<int, Color>        _palette;
	QMap<Color, QList<DrawableElement*> *> _colorMap;
	QRect                   _trueArea;
	QImage                  _image;
	unsigned int            _colorDepth;
	
	// ne sert que si l'image peut imposer la direction a l'ecran
	Direction               _directionPilot;

	Color getColor(int x, int y);
	void generateComponents();
	void addComponent(DrawableElement* component);
};

#endif // DRAWABLEIMAGE_H
