#include "toolswidget.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QButtonGroup>
#include <QPainter>

#include "tools.h"

ToolsWidget::ToolsWidget(QWidget *parent, Color toolsColor)
	: QWidget(parent), _toolPoint(toolsColor), _toolLine(toolsColor), _toolRectangle(toolsColor),
	  _toolCircle(toolsColor), _toolTriangle(toolsColor), _toolText(parent, toolsColor)
{
	_pbPoint           = new ToolButton("point");
	_pbLine            = new ToolButton("line");
	_pbRectangleEmpty  = new ToolButton("rectangle e");
	_pbRectangleFilled = new ToolButton("rectangle f");
	_pbCircleEmpty     = new ToolButton("circle e");
	_pbCircleFilled    = new ToolButton("circle f");
	_pbTriangleEmpty   = new ToolButton("triangle e");
	_pbTriangleFilled  = new ToolButton("triangle f");
	_pbText            = new ToolButton("text");
	_pbNone            = new ToolButton("move");

	_pbPoint->setCheckable(true);
	_pbLine->setCheckable(true);
	_pbRectangleEmpty->setCheckable(true);
	_pbRectangleFilled->setCheckable(true);
	_pbCircleEmpty->setCheckable(true);
	_pbCircleFilled->setCheckable(true);
	_pbTriangleEmpty->setCheckable(true);
	_pbTriangleFilled->setCheckable(true);
	_pbText->setCheckable(true);
	_pbNone->setCheckable(true);
	_pbNone->setChecked(true);

	_buttonGroup = new QButtonGroup();
	_buttonGroup->addButton(_pbPoint);
	_buttonGroup->addButton(_pbLine);
	_buttonGroup->addButton(_pbRectangleEmpty);
	_buttonGroup->addButton(_pbRectangleFilled);
	_buttonGroup->addButton(_pbCircleEmpty);
	_buttonGroup->addButton(_pbCircleFilled);
	_buttonGroup->addButton(_pbTriangleEmpty);
	_buttonGroup->addButton(_pbTriangleFilled);
	_buttonGroup->addButton(_pbText);
	_buttonGroup->addButton(_pbNone);
	_buttonGroup->setExclusive(true);

	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->addWidget(_pbPoint,           0, 0);
	mainLayout->addWidget(_pbLine,            0, 1);
	mainLayout->addWidget(_pbRectangleEmpty,  1, 0);
	mainLayout->addWidget(_pbRectangleFilled, 1, 1);
	mainLayout->addWidget(_pbCircleEmpty,     2, 0);
	mainLayout->addWidget(_pbCircleFilled,    2, 1);
	mainLayout->addWidget(_pbTriangleEmpty,   3, 0);
	mainLayout->addWidget(_pbTriangleFilled,  3, 1);
	mainLayout->addWidget(_pbText,            4, 0);
	mainLayout->addWidget(_pbNone,            4, 1);

	setLayout(mainLayout);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

	connect(_buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(toolHasChanged(QAbstractButton*)));
}

void ToolsWidget::setToolsColor(Color toolsColor)
{
	_toolPoint.setColor(toolsColor);
	_toolLine.setColor(toolsColor);
	_toolRectangle.setColor(toolsColor);
	_toolCircle.setColor(toolsColor);
	_toolTriangle.setColor(toolsColor);
	_toolText.setColor(toolsColor);
}

void ToolsWidget::toolHasChanged(QAbstractButton */*button*/)
{
	emit toolChanged();
}

ToolAddElement* ToolsWidget::getCurrentTool()
{
	if (_pbPoint->isChecked())
	{
		return &_toolPoint;
	}
	else if (_pbLine->isChecked())
	{
		return &_toolLine;
	}
	else if (_pbRectangleEmpty->isChecked())
	{
		_toolRectangle.setFilled(false);
		return &_toolRectangle;
	}
	else if (_pbRectangleFilled->isChecked())
	{
		_toolRectangle.setFilled(true);
		return &_toolRectangle;
	}
	else if (_pbCircleEmpty->isChecked())
	{
		_toolCircle.setFilled(false);
		return &_toolCircle;
	}
	else if (_pbCircleFilled->isChecked())
	{
		_toolCircle.setFilled(true);
		return &_toolCircle;
	}
	else if (_pbTriangleEmpty->isChecked())
	{
		_toolTriangle.setFilled(false);
		return &_toolTriangle;
	}
	else if (_pbTriangleFilled->isChecked())
	{
		_toolTriangle.setFilled(true);
		return &_toolTriangle;
	}
	else if (_pbText->isChecked())
	{
		return &_toolText;
	}
	else
	{
		return nullptr;
	}
}

void ToolsWidget::unselectTool()
{
	_pbNone->setChecked(true);
}

ToolButton::ToolButton(const QString &text, QWidget *parent)
	: QPushButton(parent), _text(text), _width(40), _height(40)
{
	setMinimumSize(_width,_height);
	setMaximumSize(_width,_height);
}

void ToolButton::paintEvent(QPaintEvent *event)
{
	QPushButton::paintEvent(event);
	QPainter painter(this);
	painter.save();
	painter.setPen(Qt::black);
	if (QString::compare(_text, "point", Qt::CaseInsensitive) == 0)
	{
		painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
		painter.drawEllipse(_width/2-4,_height/2-4,2*4,2*4);
	}
	else if (QString::compare(_text, "line", Qt::CaseInsensitive) == 0)
	{
		painter.drawLine(_width/5,_height/5,(4*_width)/5,(4*_height)/5);
	}
	else if (QString::compare(_text, "rectangle e", Qt::CaseInsensitive) == 0)
	{
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(_width/5,_height/5,(3*_width)/5,(3*_height)/5);
	}
	else if (QString::compare(_text, "rectangle f", Qt::CaseInsensitive) == 0)
	{
		painter.setBrush(Qt::SolidPattern);
		painter.fillRect(_width/5,_height/5,(3*_width)/5,(3*_height)/5,Qt::black);
	}
	else if (QString::compare(_text, "circle e", Qt::CaseInsensitive) == 0)
	{
		painter.setBrush(Qt::NoBrush);
		painter.drawArc(_width/5,_height/5,(3*_width)/5,(3*_height)/5,0,16*360);
	}
	else if (QString::compare(_text, "circle f", Qt::CaseInsensitive) == 0)
	{
		painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
		painter.drawEllipse(_width/5,_height/5,(3*_width)/5,(3*_height)/5);
	}
	else if (QString::compare(_text, "triangle e", Qt::CaseInsensitive) == 0)
	{
		QPoint points[4] = {
			QPoint(_width/2, _width/5),
			QPoint(4*_width/5, 4*_width/5),
			QPoint(_width/5, 4*_width/5),
			QPoint(_width/2, _width/5)
		};

		painter.setBrush(Qt::NoBrush);
		painter.drawPolyline(points, 4);
	}
	else if (QString::compare(_text, "triangle f", Qt::CaseInsensitive) == 0)
	{
		QPoint points[3] = {
			QPoint(_width/2, _width/5),
			QPoint(4*_width/5, 4*_width/5),
			QPoint(_width/5, 4*_width/5)
		};

		painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
		painter.drawPolygon(points, 3);
	}
	else if (QString::compare(_text, "text", Qt::CaseInsensitive) == 0)
	{
		QFont font = painter.font();
		font.setPointSize(24);
		painter.setFont(font);
		painter.drawText(_width/4, 4*_height/5, "T");
	}
	else if (QString::compare(_text, "move", Qt::CaseInsensitive) == 0)
	{
		painter.setBrush(Qt::SolidPattern);
		painter.fillRect(_width/2-1,_height/5,2,(3*_height)/5,Qt::black);
		painter.fillRect(_width/5,_height/2-1,(3*_width)/5,2,Qt::black);
		QPoint points1[3] = {
			QPoint(_width/2, _width/5-3),
			QPoint(_width/2+4, _width/5+3),
			QPoint(_width/2-4, _width/5+3)
		};
		painter.drawPolygon(points1, 3);
		QPoint points2[3] = {
			QPoint(_width/2, 4*_width/5+3),
			QPoint(_width/2+4, 4*_width/5-3),
			QPoint(_width/2-4, 4*_width/5-3)
		};
		painter.drawPolygon(points2, 3);
		QPoint points3[3] = {
			QPoint(_width/5-3, _width/2),
			QPoint(_width/5+3, _width/2+4),
			QPoint(_width/5+3, _width/2-4)
		};
		painter.drawPolygon(points3, 3);
		QPoint points4[3] = {
			QPoint(4*_width/5+3, _width/2),
			QPoint(4*_width/5-3, _width/2+4),
			QPoint(4*_width/5-3, _width/2-4)
		};
		painter.drawPolygon(points4, 3);
	}

	painter.restore();
}

