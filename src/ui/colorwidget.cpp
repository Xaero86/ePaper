#include "colorwidget.h"

#include <QVBoxLayout>

#include "tools.h"

ColorWidget::ColorWidget(QWidget *parent, Color defaultColor)
	: QWidget(parent)
{
	_rbColorBlack = new QRadioButton(Tools::colorToQStringIHM(Color::black));
	_rbColorBlack->setStyleSheet("background-color:black;color:white;");
	_rbColorDarkGray = new QRadioButton(Tools::colorToQStringIHM(Color::darkGray));
	_rbColorDarkGray->setStyleSheet("background-color:darkGray;color:black;");
	_rbColorLightGray = new QRadioButton(Tools::colorToQStringIHM(Color::lightGray));
	_rbColorLightGray->setStyleSheet("background-color:lightGray;color:black;");
	_rbColorWhite = new QRadioButton(Tools::colorToQStringIHM(Color::white));
	_rbColorWhite->setStyleSheet("background-color:white;color:black;");
	_rbColorWhite->setChecked(true);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(_rbColorBlack);
	mainLayout->addWidget(_rbColorDarkGray);
	mainLayout->addWidget(_rbColorLightGray);
	mainLayout->addWidget(_rbColorWhite);
	mainLayout->addStretch(1);

	setLayout(mainLayout);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

	setColor(defaultColor);

	connect(_rbColorBlack, SIGNAL(clicked()),     this, SLOT(colorHasChanged()));
	connect(_rbColorDarkGray, SIGNAL(clicked()),  this, SLOT(colorHasChanged()));
	connect(_rbColorLightGray, SIGNAL(clicked()), this, SLOT(colorHasChanged()));
	connect(_rbColorWhite, SIGNAL(clicked()),     this, SLOT(colorHasChanged()));
}

void ColorWidget::colorHasChanged()
{
	emit colorChanged(true);
}

Color ColorWidget::getColor()
{
	if (_rbColorBlack->isChecked())
	{
		return Color::black;
	}
	else if (_rbColorDarkGray->isChecked())
	{
		return Color::darkGray;
	}
	else if (_rbColorLightGray->isChecked())
	{
		return Color::lightGray;
	}
	else if (_rbColorWhite->isChecked())
	{
		return Color::white;
	}
	else
	{
		return Color::white;
	}
}

void ColorWidget::setColor(Color color)
{
	switch (color)
	{
	case Color::black:     _rbColorBlack->setChecked(true);     break;
	case Color::darkGray:  _rbColorDarkGray->setChecked(true);  break;
	case Color::lightGray: _rbColorLightGray->setChecked(true); break;
	case Color::white:     _rbColorWhite->setChecked(true);     break;
	default:               _rbColorWhite->setChecked(true);     break;
	}
}

