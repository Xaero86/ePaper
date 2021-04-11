#include "session.h"

#include "drawablepoint.h"
#include "drawableline.h"
#include "drawablecircle.h"
#include "drawablerectangle.h"
#include "drawabletriangle.h"
#include "drawabletext.h"
#include "drawableimage.h"
#include "tools.h"
#include "properties.h"

#include <QtXml>

Session::Session(Direction direction, Color backgroundColor)
	: _serialDisplay(), _elements(), _direction(direction), _backgroundColor(backgroundColor)
{
	connect(&_serialDisplay, SIGNAL(statusChanged(int)),  this, SLOT(handleStatus(int)), Qt::DirectConnection);
	connect(&_serialDisplay, SIGNAL(newWriteStatus(int)), this, SLOT(updateWriteStatus(int)), Qt::DirectConnection);
}

Session::~Session()
{
	clearElements();
}

void Session::addElement(DrawableElement *element)
{
	_elements.append(element);
	emit elementsAdded(_elements.count(), 1);
}

void Session::removeElement(int index)
{
	if ((0 <= index) && (index < _elements.size()))
	{
		DrawableElement* element = _elements.takeAt(index);
		delete element;
		emit elementsRemoved(index, 1);
	}
}

void Session::moveElement(int from, int to)
{
	if ((0 <= from) && (from < _elements.size()) && (0 <= to) && (to < _elements.size()))
	{
		_elements.move(from, to);
		emit elementsMoved(to, 1);
	}
}

void Session::moveElements(QList<int> &fromList, int to)
{
	if ((0 <= to) && (to < _elements.size()))
	{
		std::sort(fromList.begin(),fromList.end());
		int currentIndex = to;
		QList<DrawableElement*> elementsToMove;
		for (int i = fromList.count() - 1; i >= 0; i--)
		{
			elementsToMove.append(_elements.takeAt(fromList.at(i)));
			if (fromList.at(i) <= currentIndex) currentIndex--;
		}

		for (int i = elementsToMove.count()-1; i >= 0; i--)
		{
			_elements.insert(currentIndex, elementsToMove.takeFirst());
		}
		emit elementsMoved(currentIndex, fromList.count());
	}
}

void Session::clearElements()
{
	emit elementsRemoved(0, _elements.count());
	qDeleteAll(_elements);
	_elements.clear();
}

void Session::setDirection(Direction direction)
{
	if (_direction != direction && direction != Direction::unknown)
	{
		_direction = direction;
		emit directionChanged(false);
	}
}

void Session::setBackgroundColor(Color backgroundColor)
{
	if (_backgroundColor != backgroundColor)
	{
		_backgroundColor = backgroundColor;
		emit backgroundColorChanged(false);
	}
}

void Session::drawSerial(bool asynch)
{
	if (asynch)
	{
		QThreadPool::globalInstance()->start(new SerialDrawRunnable(this, true, true));
	}
	else
	{
		sendData(false);
		refresh(false);
	}
}

void Session::sendData(bool asynch)
{
	if (asynch)
	{
		QThreadPool::globalInstance()->start(new SerialDrawRunnable(this, true, false));
	}
	else
	{
		_serialDisplay.setBackgroundColor(_backgroundColor);
		_serialDisplay.setDirection(_direction);
		_serialDisplay.clearScreen();

		for (int i = 0; i < _elements.size(); i++)
		{
			_elements[i]->display(&_serialDisplay);
		}
	}
}

void Session::refresh(bool asynch)
{
	if (asynch)
	{
		QThreadPool::globalInstance()->start(new SerialDrawRunnable(this, false, true));
	}
	else
	{
		_serialDisplay.refresh();
	}
}

void Session::SerialDrawRunnable::run()
{
	if (_sendData)
	{
		_session->sendData(false);
	}

	if (_sendReflesh)
	{
		_session->refresh(false);
	}
}

void Session::updateWriteStatus(int percent)
{
	emit newWriteStatus(percent);
}

void Session::waitReady()
{
	_serialDisplay.checkReady();
}

void Session::cancelSerial()
{
	_serialDisplay.cancelOperation();
}

void Session::changeSerialPort(const QString &port)
{
	_serialDisplay.changePort(port);
}

void Session::turnIdleSerial()
{
	_serialDisplay.turnIdle();
}

void Session::loadImage(QUrl &path)
{
	QFileInfo fileInfo(path.toLocalFile());
	if (QString::compare(fileInfo.completeSuffix(), "xml", Qt::CaseInsensitive) == 0)
	{
		QFile file(path.toLocalFile());
		if (file.open(QIODevice::ReadOnly))
		{
			QDomDocument doc("Image");
			if (doc.setContent(&file))
			{
				QDomElement root = doc.documentElement();
				if (QString::compare(root.nodeName(), "image", Qt::CaseInsensitive) == 0)
				{
					clearElements();
					_direction = Direction::unknown;
					QString direction = root.attribute("direction");
					setDirection(Tools::qStringToDirection(direction));
					QString backgroundColor = root.attribute("backgroundColor");
					setBackgroundColor(Tools::qStringToColor(backgroundColor, Properties::defaultBackgroundColor()));

					QDomNode node = root.firstChild();

					while(!node.isNull())
					{
						QDomElement element = node.toElement();
						if (QString::compare(element.nodeName(), "point", Qt::CaseInsensitive) == 0)
						{
							DrawableElement* newElement = DrawablePoint::createPoint(element);

							if (newElement != nullptr)
							{
								_elements.append(newElement);
							}
						}
						else if ((QString::compare(element.nodeName(), "line", Qt::CaseInsensitive) == 0) ||
								 (QString::compare(element.nodeName(), "ligne", Qt::CaseInsensitive) == 0))
						{
							DrawableElement* newElement = DrawableLine::createLine(element);

							if (newElement != nullptr)
							{
								_elements.append(newElement);
							}
						}
						else if (QString::compare(element.nodeName(), "rectangle", Qt::CaseInsensitive) == 0)
						{
							DrawableElement* newElement = DrawableRectangle::createRectangle(element);

							if (newElement != nullptr)
							{
								_elements.append(newElement);
							}
						}
						else if ((QString::compare(element.nodeName(), "circle", Qt::CaseInsensitive) == 0) ||
								 (QString::compare(element.nodeName(), "cercle", Qt::CaseInsensitive) == 0))
						{
							DrawableElement* newElement = DrawableCircle::createCircle(element);

							if (newElement != nullptr)
							{
								_elements.append(newElement);
							}
						}
						else if (QString::compare(element.nodeName(), "triangle", Qt::CaseInsensitive) == 0)
						{
							DrawableElement* newElement = DrawableTriangle::createTriangle(element);

							if (newElement != nullptr)
							{
								_elements.append(newElement);
							}
						}
						else if (QString::compare(element.nodeName(), "text", Qt::CaseInsensitive) == 0)
						{
							DrawableElement* newElement = DrawableText::createText(element);

							if (newElement != nullptr)
							{
								_elements.append(newElement);
							}
						}
						else if (QString::compare(element.nodeName(), "image", Qt::CaseInsensitive) == 0)
						{
							DrawableImage* newElement = DrawableImage::createImage(element, _backgroundColor, _direction);

							if (newElement != nullptr)
							{
								_elements.append(newElement);

								// si la direction n'est pas imposee par la session
								// elle est pilotee par l'image
								if (_direction == Direction::unknown)
								{
									setDirection(newElement->getDirection());
								}
							}
						}

						node = node.nextSibling();
					}
					// la direction n'a pas pu etre resolue par la session ou une image
					// => verticale par defaut
					if (_direction == Direction::unknown)
					{
						setDirection(Properties::defaultDirection());
					}
					emit elementsAdded(0, _elements.count());
				}
				else
				{
					emit errorLoading("Fichier " + path.toLocalFile() + " invalide");
				}
			}
			else
			{
				emit errorLoading("Fichier " + path.toLocalFile() + " invalide");
			}
		}
		else
		{
			emit errorLoading("Impossible d'ouvrir le fichier " + path.toLocalFile());
		}
	}
	else
	{
		// ouvrir un fichier image
		DrawableImage *newImage = new DrawableImage(path.toString(), Properties::defautImageColorDepth(), Properties::defaultColorPalette(Properties::defautImageColorDepth()), _backgroundColor);
		if (newImage->isValid())
		{
			clearElements();
			_elements.append(newImage);
			setDirection(newImage->getDirection());
			emit elementsAdded(0, 1);
		}
		else
		{
			delete newImage;
			emit errorLoading("Impossible d'ouvrir le fichier " + path.toLocalFile());
		}
	}
}

void Session::saveImage(QUrl &path)
{
	QFile file(path.toLocalFile());
	if (file.open(QIODevice::WriteOnly))
	{
		QDomDocument doc;
		QDomElement root = doc.createElement("image");
		doc.appendChild(root);

		root.setAttribute("direction", Tools::directionToQString(_direction));
		root.setAttribute("backgroundColor", Tools::colorToQString(_backgroundColor));

		for (int i = 0; i < _elements.size(); i++)
		{
			_elements[i]->toXml(&root);
		}

		QTextStream fileStream(&file);
		fileStream << doc.toString();
		file.close();
	}
	else
	{
		emit errorSaving("Impossible d'enregistrer le fichier " + path.toLocalFile());
	}
}

void Session::handleStatus(int status)
{
	emit statusChanged(status);
}
