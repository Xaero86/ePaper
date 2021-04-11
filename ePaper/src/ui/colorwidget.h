#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include "definition.h"

#include <QWidget>
#include <QRadioButton>

class ColorWidget : public QWidget
{
	Q_OBJECT
public:
	ColorWidget(QWidget *parent, Color defaultColor);

	Color getColor();

	void setColor(Color color);

signals:
	void colorChanged(bool fromIHM);

private slots:
	void colorHasChanged();

private:
	QRadioButton* _rbColorWhite;
	QRadioButton* _rbColorLightGray;
	QRadioButton* _rbColorDarkGray;
	QRadioButton* _rbColorBlack;
};

#endif // COLORWIDGET_H
