#include "directionwidget.h"

#include <QVBoxLayout>

#include "tools.h"

DirectionWidget::DirectionWidget(QWidget *parent, Direction defaultDirection)
	: QWidget(parent)
{
	_rbDirVertical = new QRadioButton("Vertical");
	_rbDirHorizontal = new QRadioButton("Horizontal");
	_rbDirVertical->setChecked(true);
	_cbInvertDir = new QCheckBox("Inverse");

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(_rbDirVertical);
	mainLayout->addWidget(_rbDirHorizontal);
	mainLayout->addWidget(_cbInvertDir);
	mainLayout->addStretch(1);

	setLayout(mainLayout);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

	setDirection(defaultDirection);

	connect(_rbDirVertical, SIGNAL(clicked()),    this, SLOT(directionHasChanged()));
	connect(_rbDirHorizontal, SIGNAL(clicked()),  this, SLOT(directionHasChanged()));
	connect(_cbInvertDir, SIGNAL(clicked()),      this, SLOT(directionHasChanged()));
}

void DirectionWidget::directionHasChanged()
{
	emit directionChanged(true);
}

Direction DirectionWidget::getDirection()
{
	if (_rbDirVertical->isChecked())
	{
		if (_cbInvertDir->isChecked())
		{
			return Direction::vertical1;
		}
		else
		{
			return Direction::vertical3;
		}
	}
	else
	{
		if (_cbInvertDir->isChecked())
		{
			return Direction::horizontal0;
		}
		else
		{
			return Direction::horizontal2;
		}
	}
}

void DirectionWidget::setDirection(Direction direction)
{
	switch (direction)
	{
	case Direction::vertical1:
	case Direction::vertical3:
	case Direction::unknown:
		_rbDirVertical->setChecked(true);
		break;
	case Direction::horizontal0:
	case Direction::horizontal2:
		_rbDirHorizontal->setChecked(true);
		break;
	}

	_cbInvertDir->setChecked((direction == Direction::horizontal0) || (direction == Direction::vertical1));
}

