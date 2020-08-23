#include "drawableimage.h"

#include "properties.h"
#include "tools.h"
#include "drawablepoint.h"
#include "drawableline.h"

#include <QImageReader>
#include <QImage>

#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <iostream>

DrawableImage::DrawableImage(const QString &path, unsigned int colorDepth, QMap<unsigned int, Color> &palette, Color transparencyColor, const QRect area, Direction expectedDirection)
	: DrawableElement(transparencyColor), _isValid(false), _path(path),
	  _componentsWhite(), _componentsLightGray(), _componentsDarkGray(), _componentsBlack(),
	  _palette(), _colorMap(), _colorDepth(colorDepth), _directionPilot(Direction::unknown)
{
	QFile file(path);
	QUrl url(path);
	QImage newImage;

	if (file.exists())
	{
		QImageReader reader(path);
		newImage = reader.read();
	}
	else if (url.isLocalFile())
	{
		QImageReader reader(url.toLocalFile());
		newImage = reader.read();
	}
	else
	{
		QNetworkAccessManager manager;
		QNetworkReply *response = manager.get(QNetworkRequest(QUrl(url)));
		QEventLoop event;
		QObject::connect(response, SIGNAL(finished()), &event, SLOT(quit()));
		event.exec();
		QImageReader reader(response);
		newImage = reader.read();
	}

	if (newImage.isNull())
	{
		return;
	}
	
	_palette[0] = Color::unknown;
	_palette[1] = Color::unknown;
	_palette[2] = Color::unknown;
	_palette[3] = Color::unknown;
	
	if (_colorDepth == 2)
	{
		_palette[0] = palette[0];
		_palette[1] = palette[1];
	}
	else if (_colorDepth == 3)
	{
		_palette[0] = palette[0];
		_palette[1] = palette[1];
		_palette[2] = palette[2];
	}
	else if (_colorDepth == 4)
	{
		_palette[0] = palette[0];
		_palette[1] = palette[1];
		_palette[2] = palette[2];
		_palette[3] = palette[3];
	}
	
	_colorMap[Color::white] = &_componentsWhite;
	_colorMap[Color::lightGray] = &_componentsLightGray;
	_colorMap[Color::darkGray] = &_componentsDarkGray;
	_colorMap[Color::black] = &_componentsBlack;

	_isValid = true;
	QRect validArea = area;

	// area non specifiee
	if (!area.isValid())
	{
		// direction non specifiee
		if (expectedDirection == Direction::unknown)
		{
			// direction choisie en fonction de la taille de l'image
			if (newImage.width() > newImage.height())
			{
				expectedDirection = Properties::defaultHorizontal();
			}
			else
			{
				expectedDirection = Properties::defaultVertical();
			}
			_directionPilot = expectedDirection;
		}
		// taille de l'area en fonction de la direction
		switch (expectedDirection) {
		case Direction::horizontal0:
		case Direction::horizontal2:
			validArea = QRect(0,0,800,600);
			break;
		case Direction::vertical1:
		case Direction::vertical3:
		default:
			validArea = QRect(0,0,600,800);
			break;
		}
	}

	// transformation de l'image pour qu'elle rentre dans l'area: KeepAspectRatio=image pas deformee et a l'interieur
	QImage sizedImage = newImage.scaled(validArea.width(), validArea.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

	// obtenir les coordonnees du coin superieur gauche de l'image
	QPoint imageOrigin(validArea.x() + (validArea.width() - sizedImage.width())/2, validArea.y() + (validArea.height() - sizedImage.height())/2);

	_trueArea = QRect(imageOrigin, QSize(sizedImage.size()));
	
	if (Properties::defautImageColorDepth() == 2)
	{
		_image = sizedImage.convertToFormat(QImage::Format_Mono, Qt::AutoColor);
	}
	else
	{
		// TODO ...
	}

	generateComponents();
}

DrawableImage::~DrawableImage()
{
	qDeleteAll(_componentsWhite);
	qDeleteAll(_componentsLightGray);
	qDeleteAll(_componentsDarkGray);
	qDeleteAll(_componentsBlack);
	_componentsWhite.clear();
	_componentsLightGray.clear();
	_componentsDarkGray.clear();
	_componentsBlack.clear();
}

Color DrawableImage::getColor(int x, int y)
{
	if (_colorDepth == 2)
	{
		if (QColor(_image.pixel(x,y)) == Qt::black)
		{
			return _palette[1];
		}
		else
		{
			return _palette[0];
		}
	}
	else if (_colorDepth == 3)
	{
		if (QColor(_image.pixel(x,y)) == Qt::black)
		{
			return _palette[2];
		}
		else if (QColor(_image.pixel(x,y)) == Qt::white)
		{
			return _palette[0];
		}
		else
		{
			return _palette[1];
		}
	}
	else
	{
		if (QColor(_image.pixel(x,y)) == Qt::black)
		{
			return _palette[3];
		}
		else if (QColor(_image.pixel(x,y)) == Qt::white)
		{
			return _palette[0];
		}
		else if (QColor(_image.pixel(x,y)).value() > 128)
		{
			return _palette[2];
		}
		else
		{
			return _palette[1];
		}
	}
}

void DrawableImage::generateComponents()
{
	int minX = qMax(0, _trueArea.x());
	int maxX = qMin(800, _trueArea.x()+_trueArea.width());
	int minY = qMax(0, _trueArea.y());
	int maxY = qMin(800, _trueArea.y()+_trueArea.height());

	Color singlePointMap[maxX-minX][maxY-minY];
	for (int y = minY; y < maxY; y++)
	{
		Color previousColor = _color;
		int lineOrigin = minX-1;
		for (int x = minX; x < maxX; x++)
		{
			Color col = getColor(x-minX, y-minY);
			singlePointMap[x-minX][y-minY] = _color;
			// la couleur precedente n'est pas la couleur actuelle:
			// on fini la ligne ou le point precedent
			// et on commence un point ou bien une nouvelle ligne
			if (col != previousColor)
			{
				// si lineOrigin est valid i.e. lineOrigin != minX-1
				if (lineOrigin != minX-1)
				{
					if (lineOrigin == x-1)
					{
						// couleur differente de la couleur de fond
						if (previousColor != _color)
						{
							singlePointMap[x-1-minX][y-minY] = previousColor;
						}
					}
					else
					{
						// couleur differente de la couleur de fond
						if (previousColor != _color)
						{
							addComponent(new DrawableLine(lineOrigin,y,x-1,y,previousColor));
						}
					}
				}
				lineOrigin = x;
			}
			// on essaie de combiner les points seuls avec des lignes verticales
			// soit on est a la derniere ligne, soit le point sur la ligne en dessous est d'une autre couleur
			// sinon le regroupement vertival se fera a partir de la ligne en dessous
			// on travaille toujours sur la colonne precedente x-1 donc x != minX
			if ((previousColor != _color) && (x != minX) &&
				((y == maxY-1) || (previousColor != getColor(x-1-minX, y-minY+1))))
			{
				int nbSinglePoint = 0;
				int lastSinglePoint = y;
				// on remonte la vertical et on compte le nombre de point seul
				for (int z = y; z >= minY; z--)
				{
					if (singlePointMap[x-1-minX][z-minY] == previousColor)
					{
						// c'est un point seul de la bonne couleur
						nbSinglePoint++;
						lastSinglePoint = z;
					}
					else if (getColor(x-1-minX, z-minY) != previousColor)
					{
						// pas la bonne couleur, on arrete la verticale
						break;
					}
				}
				// on a trouve au moins 2 points seuls sur la verticale, ca vaut le coup de faire une ligne
				if (nbSinglePoint >= 2)
				{
					addComponent(new DrawableLine(x-1,y,x-1,lastSinglePoint,previousColor));
					// on supprime les points seuls qui sont sur la ligne (sinon ca sert a rien)
					for (int z = y; z >= lastSinglePoint; z--)
					{
						singlePointMap[x-1-minX][z-minY] = _color;
					}
				}
			}
			previousColor = col;
		}
		// fin de la ligne, on genere le dernier element
		if (previousColor != _color)
		{
			if (lineOrigin == maxX)
			{
				singlePointMap[maxX-minX][y-minY] = previousColor;
			}
			else
			{
				addComponent(new DrawableLine(lineOrigin,y,maxX,y,previousColor));
			}
		}
	}

	// on ajoute les points seul a la fin
	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			Color color = singlePointMap[x-minX][y-minY];
			if (color != _color)
			{
				addComponent(new DrawablePoint(x,y,color));
			}
		}
	}

	int total = 0;
	for (Color color : _colorMap.keys())
	{
		total += _colorMap[color]->count();
		// std::cout << "generateComponents: " << Tools::colorToQString(color).toStdString() << "=" << _colorMap[color]->count() << std::endl;
	}
	std::cout << "Elements to display:" << total << std::endl;
}

void DrawableImage::addComponent(DrawableElement* component)
{
	_colorMap[component->getColor()]->append(component);
}

void DrawableImage::display(DisplayManager *displayManager)
{
	if (!_isValid) return;
	// parcours de tous les poids du plus leger au plus lourd
	for (unsigned int index = 0; index < _colorDepth; index++)
	{
		for (int i = 0; i < _colorMap[_palette[index]]->size(); i++)
		{
			_colorMap[_palette[index]]->at(i)->display(displayManager);
		}
	}
}

void DrawableImage::displayMoving(DisplayManager *displayManager, int /*deltaX*/, int /*deltaY*/)
{
	if (!_isValid) return;
	// Not implemented
	display(displayManager);
}

QString DrawableImage::parametersToString()
{
	if (!_isValid)
	{
		return QString();
	}
	else
	{
		return _path;
	}
}

DrawableImage* DrawableImage::createImage(QDomElement &element, Color backgroundColor, Direction globalDirection)
{
	QString path = element.attribute("path", "");

	Color transparencyColor = Tools::qStringToColor(element.attribute("transparencyColor", ""));
	if (transparencyColor == Color::unknown)
	{
		transparencyColor = backgroundColor;
	}

	bool okx, oky, okw, okh, okd;
	int x = element.attribute("imageOriginX", "").toInt(&okx);
	int y = element.attribute("imageOriginY", "").toInt(&oky);
	int w = element.attribute("imageWidth", "").toInt(&okw);
	int h = element.attribute("imageHeight", "").toInt(&okh);

	QRect area;
	if (okx && oky && okw && okh)
	{
		area = QRect(x, y, w, h);
	}
	
	int d = element.attribute("colorDepth", "").toInt(&okd);
	unsigned int depth;
	if (okd && (d >= 2) && (d <= 4))
	{
		depth = d;
	}
	else
	{
		depth = Properties::defautImageColorDepth();
	}
	
	QMap<unsigned int, Color> palette = Properties::defaultColorPalette(depth);
	
	palette[0] = Tools::qStringToColor(element.attribute("paletteColor0"), palette[0]);
	palette[1] = Tools::qStringToColor(element.attribute("paletteColor1"), palette[1]);
	if (depth > 2)
	{
		palette[2] = Tools::qStringToColor(element.attribute("paletteColor2"), palette[2]);
		if (depth > 3)
		{
			palette[3] = Tools::qStringToColor(element.attribute("paletteColor3"), palette[3]);
		}
	}

	DrawableImage *newImage = new DrawableImage(path, depth, palette, transparencyColor, area, globalDirection);
	if (newImage->_isValid)
	{
		return newImage;
	}
	else
	{
		delete newImage;
		return nullptr;
	}
}

void DrawableImage::toXml(QDomElement* root)
{
	if (!_isValid) return;
	QDomElement element = root->ownerDocument().createElement("image");
	element.setAttribute("path", _path);
	element.setAttribute("transparencyColor", Tools::colorToQString(_color));
	element.setAttribute("imageOriginX", _trueArea.x());
	element.setAttribute("imageOriginY", _trueArea.y());
	element.setAttribute("imageWidth", _trueArea.width());
	element.setAttribute("imageHeight", _trueArea.height());
	element.setAttribute("colorDepth", _colorDepth);
	element.setAttribute("paletteColor0", Tools::colorToQString(_palette[0]));
	element.setAttribute("paletteColor1", Tools::colorToQString(_palette[1]));
	if (_colorDepth > 2)
	{
		element.setAttribute("paletteColor2", Tools::colorToQString(_palette[2]));
		if (_colorDepth > 3)
		{
			element.setAttribute("paletteColor3", Tools::colorToQString(_palette[3]));
		}
	}
	root->appendChild(element);
}

void DrawableImage::move(int /*deltaX*/, int /*deltaY*/)
{
	if (!_isValid) return;
	// Not implemented
}
