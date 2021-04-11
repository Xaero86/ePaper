#ifndef TOOLSWIDGET_H
#define TOOLSWIDGET_H

#include "definition.h"

#include <QWidget>
#include <QPushButton>

#include "tooladdpoint.h"
#include "tooladdline.h"
#include "tooladdrectangle.h"
#include "tooladdcircle.h"
#include "tooladdtriangle.h"
#include "tooladdtext.h"

class ToolButton : public QPushButton
{
	Q_OBJECT
public:
	ToolButton(const QString &text, QWidget *parent = nullptr);

	void paintEvent(QPaintEvent *event);

private:
	QString _text;
	int     _width;
	int     _height;
};

class ToolsWidget : public QWidget
{
	Q_OBJECT
public:
	ToolsWidget(QWidget *parent, Color toolsColor);

	ToolAddElement* getCurrentTool();
	void unselectTool();

	void setToolsColor(Color toolsColor);

signals:
	void toolChanged();

private slots:
	void toolHasChanged(QAbstractButton *button);

private:
	QButtonGroup* _buttonGroup;

	ToolButton*  _pbPoint;
	ToolButton*  _pbLine;
	ToolButton*  _pbRectangleEmpty;
	ToolButton*  _pbRectangleFilled;
	ToolButton*  _pbCircleEmpty;
	ToolButton*  _pbCircleFilled;
	ToolButton*  _pbTriangleEmpty;
	ToolButton*  _pbTriangleFilled;
	ToolButton*  _pbText;
	ToolButton*  _pbNone;

	ToolAddPoint     _toolPoint;
	ToolAddLine      _toolLine;
	ToolAddRectangle _toolRectangle;
	ToolAddCircle    _toolCircle;
	ToolAddTriangle  _toolTriangle;
	ToolAddText      _toolText;
};

#endif // TOOLSWIDGET_H
