#ifndef DIRECTIONWIDGET_H
#define DIRECTIONWIDGET_H

#include "definition.h"

#include <QWidget>
#include <QRadioButton>
#include <QCheckBox>

class DirectionWidget : public QWidget
{
	Q_OBJECT
public:
	DirectionWidget(QWidget *parent, Direction defaultDirection);

	Direction getDirection();

	void setDirection(Direction direction);

signals:
	void directionChanged(bool fromIHM);

private slots:
	void directionHasChanged();

private:
	QRadioButton* _rbDirHorizontal;
	QRadioButton* _rbDirVertical;
	QCheckBox*    _cbInvertDir;
};

#endif // DIRECTIONWIDGET_H
