#include "parametereditor.h"

#include <QValidator>
#include <QVBoxLayout>

ParameterEditor::ParameterEditor(QWidget *parent, DrawableElement *element)
	: QGroupBox(parent), _parent(parent), _element(element)
{
	DrawablePoint* point         = dynamic_cast<DrawablePoint*>(_element);
	DrawableLine* line           = dynamic_cast<DrawableLine*>(_element);
	DrawableRectangle* rectangle = dynamic_cast<DrawableRectangle*>(_element);
	DrawableCircle* circle       = dynamic_cast<DrawableCircle*>(_element);
	DrawableTriangle* triangle   = dynamic_cast<DrawableTriangle*>(_element);
	DrawableText* text           = dynamic_cast<DrawableText*>(_element);
	DrawableImage* image         = dynamic_cast<DrawableImage*>(_element);

	setAutoFillBackground(true);
	if (point)     createPointEditor();
	if (line)      createLineEditor();
	if (rectangle) createRectangleEditor();
	if (circle)    createCircleEditor();
	if (triangle)  createTriangleEditor();
	if (text)      createTextEditor();
	if (image)     createImageEditor();
}

void ParameterEditor::setEditorData()
{
	DrawablePoint* point         = dynamic_cast<DrawablePoint*>(_element);
	DrawableLine* line           = dynamic_cast<DrawableLine*>(_element);
	DrawableRectangle* rectangle = dynamic_cast<DrawableRectangle*>(_element);
	DrawableCircle* circle       = dynamic_cast<DrawableCircle*>(_element);
	DrawableTriangle* triangle   = dynamic_cast<DrawableTriangle*>(_element);
	DrawableText* text           = dynamic_cast<DrawableText*>(_element);
	DrawableImage* image         = dynamic_cast<DrawableImage*>(_element);

	if (point)     setEditorDataPoint(point);
	if (line)      setEditorDataLine(line);
	if (rectangle) setEditorDataRectangle(rectangle);
	if (circle)    setEditorDataCircle(circle);
	if (triangle)  setEditorDataTriangle(triangle);
	if (text)      setEditorDataText(text);
	if (image)     setEditorDataImage(image);
}

void ParameterEditor::setModelData()
{
	DrawablePoint* point         = dynamic_cast<DrawablePoint*>(_element);
	DrawableLine* line           = dynamic_cast<DrawableLine*>(_element);
	DrawableRectangle* rectangle = dynamic_cast<DrawableRectangle*>(_element);
	DrawableCircle* circle       = dynamic_cast<DrawableCircle*>(_element);
	DrawableTriangle* triangle   = dynamic_cast<DrawableTriangle*>(_element);
	DrawableText* text           = dynamic_cast<DrawableText*>(_element);
	DrawableImage* image         = dynamic_cast<DrawableImage*>(_element);

	if (point)     setModelDataPoint(point);
	if (line)      setModelDataLine(line);
	if (rectangle) setModelDataRectangle(rectangle);
	if (circle)    setModelDataCircle(circle);
	if (triangle)  setModelDataTriangle(triangle);
	if (text)      setModelDataText(text);
	if (image)     setModelDataImage(image);
}

void ParameterEditor::createPointEditor()
{
	createLine();
	QVBoxLayout *vbl = new QVBoxLayout;
	vbl->addLayout(_line);
	vbl->addStretch(1);
	setLayout(vbl);
}

void ParameterEditor::createLineEditor()
{
	createLine0();
	createLine1();
	QVBoxLayout *vbl = new QVBoxLayout;
	vbl->addLayout(_line0);
	vbl->addLayout(_line1);
	vbl->addStretch(1);
	setLayout(vbl);
}

void ParameterEditor::createRectangleEditor()
{
	createLine0();
	createLine1();
	createLineFill();
	QVBoxLayout *vbl = new QVBoxLayout;
	vbl->addLayout(_line0);
	vbl->addLayout(_line1);
	vbl->addLayout(_lineFill);
	vbl->addStretch(1);
	setLayout(vbl);
}

void ParameterEditor::createCircleEditor()
{
	createLine();
	createLineRadius();
	createLineFill();
	QVBoxLayout *vbl = new QVBoxLayout;
	vbl->addLayout(_line);
	vbl->addLayout(_lineRadius);
	vbl->addLayout(_lineFill);
	vbl->addStretch(1);
	setLayout(vbl);
}

void ParameterEditor::createTriangleEditor()
{
	createLine0();
	createLine1();
	createLine2();
	createLineFill();
	QVBoxLayout *vbl = new QVBoxLayout;
	vbl->addLayout(_line0);
	vbl->addLayout(_line1);
	vbl->addLayout(_line2);
	vbl->addLayout(_lineFill);
	vbl->addStretch(1);
	setLayout(vbl);
}

void ParameterEditor::createTextEditor()
{
	createLine();
	createLineText();
	QVBoxLayout *vbl = new QVBoxLayout;
	vbl->addLayout(_line);
	vbl->addLayout(_lineText);
	vbl->addStretch(1);
	setLayout(vbl);
}

void ParameterEditor::createImageEditor()
{
	QVBoxLayout *vbl = new QVBoxLayout;
	vbl->addWidget(new QLabel("Image non modifiable", _parent));
	vbl->addStretch(1);
	setLayout(vbl);
}

void ParameterEditor::setEditorDataPoint(DrawablePoint* point)
{
	_editX->setText(QString::number(point->x()));
	_editY->setText(QString::number(point->y()));
}

void ParameterEditor::setEditorDataLine(DrawableLine* line)
{
	_editX0->setText(QString::number(line->x0()));
	_editY0->setText(QString::number(line->y0()));
	_editX1->setText(QString::number(line->x1()));
	_editY1->setText(QString::number(line->y1()));
}

void ParameterEditor::setEditorDataRectangle(DrawableRectangle* rectangle)
{
	_editX0->setText(QString::number(rectangle->x0()));
	_editY0->setText(QString::number(rectangle->y0()));
	_editX1->setText(QString::number(rectangle->x1()));
	_editY1->setText(QString::number(rectangle->y1()));
	_checkFill->setChecked(rectangle->filled());
}

void ParameterEditor::setEditorDataCircle(DrawableCircle* circle)
{
	_editX->setText(QString::number(circle->x()));
	_editY->setText(QString::number(circle->y()));
	_editRadius->setText(QString::number(circle->radius()));
	_checkFill->setChecked(circle->filled());
}

void ParameterEditor::setEditorDataTriangle(DrawableTriangle* triangle)
{
	_editX0->setText(QString::number(triangle->x0()));
	_editY0->setText(QString::number(triangle->y0()));
	_editX1->setText(QString::number(triangle->x1()));
	_editY1->setText(QString::number(triangle->y1()));
	_editX2->setText(QString::number(triangle->x2()));
	_editY2->setText(QString::number(triangle->y2()));
	_checkFill->setChecked(triangle->filled());
}

void ParameterEditor::setEditorDataText(DrawableText* text)
{
	_editX->setText(QString::number(text->x()));
	_editY->setText(QString::number(text->y()));
	_editText->setText(text->text());
	int index = _comboSizeText->findText(QString::number(text->fontSize()));
	if (index != -1)
	{
		_comboSizeText->setCurrentIndex(index);
	}
}

void ParameterEditor::setEditorDataImage(DrawableImage* /*image*/)
{
	// Immutable
}

void ParameterEditor::setModelDataPoint(DrawablePoint* point)
{
	bool okx, oky;
	short newX = _editX->text().toShort(&okx);
	short newY = _editY->text().toShort(&oky);
	if (okx && oky)
	{
		point->setX(newX);
		point->setY(newY);
	}
}

void ParameterEditor::setModelDataLine(DrawableLine* line)
{
	bool okx0, oky0, okx1, oky1;
	short newX0 = _editX0->text().toShort(&okx0);
	short newY0 = _editY0->text().toShort(&oky0);
	short newX1 = _editX1->text().toShort(&okx1);
	short newY1 = _editY1->text().toShort(&oky1);
	if (okx0 && oky0 && okx1 && oky1)
	{
		line->setX0(newX0);
		line->setY0(newY0);
		line->setX1(newX1);
		line->setY1(newY1);
	}
}

void ParameterEditor::setModelDataRectangle(DrawableRectangle* rectangle)
{
	bool okx0, oky0, okx1, oky1;
	short newX0 = _editX0->text().toShort(&okx0);
	short newY0 = _editY0->text().toShort(&oky0);
	short newX1 = _editX1->text().toShort(&okx1);
	short newY1 = _editY1->text().toShort(&oky1);
	if (okx0 && oky0 && okx1 && oky1)
	{
		rectangle->setX0(newX0);
		rectangle->setY0(newY0);
		rectangle->setX1(newX1);
		rectangle->setY1(newY1);
		rectangle->setFilled(_checkFill->isChecked());
	}
}

void ParameterEditor::setModelDataCircle(DrawableCircle* circle)
{
	bool okx, oky, okr;
	short newX = _editX->text().toShort(&okx);
	short newY = _editY->text().toShort(&oky);
	short newR = _editRadius->text().toShort(&okr);
	if (okx && oky && okr)
	{
		circle->setX(newX);
		circle->setY(newY);
		circle->setRadius(newR);
		circle->setFilled(_checkFill->isChecked());
	}
}

void ParameterEditor::setModelDataTriangle(DrawableTriangle* triangle)
{
	bool okx0, oky0, okx1, oky1, okx2, oky2;
	short newX0 = _editX0->text().toShort(&okx0);
	short newY0 = _editY0->text().toShort(&oky0);
	short newX1 = _editX1->text().toShort(&okx1);
	short newY1 = _editY1->text().toShort(&oky1);
	short newX2 = _editX2->text().toShort(&okx2);
	short newY2 = _editY2->text().toShort(&oky2);
	if (okx0 && oky0 && okx1 && oky1 && okx2 && oky2)
	{
		triangle->setX0(newX0);
		triangle->setY0(newY0);
		triangle->setX1(newX1);
		triangle->setY1(newY1);
		triangle->setX2(newX2);
		triangle->setY2(newY2);
		triangle->setFilled(_checkFill->isChecked());
	}
}

void ParameterEditor::setModelDataText(DrawableText* text)
{
	bool okx, oky, okf;
	short newX = _editX->text().toShort(&okx);
	short newY = _editY->text().toShort(&oky);
	short newF = _comboSizeText->currentText().toShort(&okf);
	if (okx && oky && okf && (1<=newF) && (newF<=3))
	{
		text->setX(newX);
		text->setY(newY);
		text->setText(_editText->text());
		text->setFontSize(newF);
	}
}

void ParameterEditor::setModelDataImage(DrawableImage* /*image*/)
{
	// Immutable
}

void ParameterEditor::createLine()
{
	_line = new QHBoxLayout;
	_labelX = new QLabel("X: ", _parent);
	_editX = new QLineEdit(_parent);
	_editX->setValidator(new QIntValidator(-1000,1000,_parent));
	_editX->setFixedWidth(QFontMetrics(_editX->font()).width("-00000"));
	_labelY = new QLabel("Y: ", _parent);
	_editY = new QLineEdit(_parent);
	_editY->setValidator(new QIntValidator(-1000,1000,_parent));
	_editY->setFixedWidth(QFontMetrics(_editY->font()).width("-00000"));
	_line->addWidget(_labelX);
	_line->addWidget(_editX);
	_line->addWidget(_labelY);
	_line->addWidget(_editY);
	_line->addStretch(1);
}

void ParameterEditor::createLine0()
{
	_line0 = new QHBoxLayout;
	_labelX0 = new QLabel("X0: ", _parent);
	_editX0 = new QLineEdit(_parent);
	_editX0->setValidator(new QIntValidator(-1000,1000,_parent));
	_editX0->setFixedWidth(QFontMetrics(_editX0->font()).width("-00000"));
	_labelY0 = new QLabel("Y0: ", _parent);
	_editY0 = new QLineEdit(_parent);
	_editY0->setValidator(new QIntValidator(-1000,1000,_parent));
	_editY0->setFixedWidth(QFontMetrics(_editY0->font()).width("-00000"));
	_line0->addWidget(_labelX0);
	_line0->addWidget(_editX0);
	_line0->addWidget(_labelY0);
	_line0->addWidget(_editY0);
	_line0->addStretch(1);
}

void ParameterEditor::createLine1()
{
	_line1 = new QHBoxLayout;
	_labelX1 = new QLabel("X1: ", _parent);
	_editX1 = new QLineEdit(_parent);
	_editX1->setValidator(new QIntValidator(-1000,1000,_parent));
	_editX1->setFixedWidth(QFontMetrics(_editX1->font()).width("-00000"));
	_labelY1 = new QLabel("Y1: ", _parent);
	_editY1 = new QLineEdit(_parent);
	_editY1->setValidator(new QIntValidator(-1000,1000,_parent));
	_editY1->setFixedWidth(QFontMetrics(_editY1->font()).width("-00000"));
	_line1->addWidget(_labelX1);
	_line1->addWidget(_editX1);
	_line1->addWidget(_labelY1);
	_line1->addWidget(_editY1);
	_line1->addStretch(1);
}

void ParameterEditor::createLine2()
{
	_line2 = new QHBoxLayout;
	_labelX2 = new QLabel("X2: ", _parent);
	_editX2 = new QLineEdit(_parent);
	_editX2->setValidator(new QIntValidator(-1000,1000,_parent));
	_editX2->setFixedWidth(QFontMetrics(_editX2->font()).width("-00000"));
	_labelY2 = new QLabel("Y2: ", _parent);
	_editY2 = new QLineEdit(_parent);
	_editY2->setValidator(new QIntValidator(-1000,1000,_parent));
	_editY2->setFixedWidth(QFontMetrics(_editY2->font()).width("-00000"));
	_line2->addWidget(_labelX2);
	_line2->addWidget(_editX2);
	_line2->addWidget(_labelY2);
	_line2->addWidget(_editY2);
	_line2->addStretch(1);
}

void ParameterEditor::createLineText()
{
	_lineText = new QHBoxLayout;
	_editText = new QLineEdit(_parent);
	_editText->setMaxLength(1020);
	_editText->setFixedWidth(QFontMetrics(_editText->font()).width("0000000000"));
	_labelSizeText = new QLabel("Taille: ", _parent);
	_comboSizeText = new QComboBox(_parent);
	_comboSizeText->addItem("1");
	_comboSizeText->addItem("2");
	_comboSizeText->addItem("3");
	_lineText->addWidget(_editText);
	_lineText->addWidget(_labelSizeText);
	_lineText->addWidget(_comboSizeText);
	_lineText->addStretch(1);
}

void ParameterEditor::createLineFill()
{
	_lineFill = new QHBoxLayout;
	_labelFill = new QLabel("Rempli: ", _parent);
	_checkFill = new QCheckBox(_parent);
	_lineFill->addWidget(_labelFill);
	_lineFill->addWidget(_checkFill);
	_lineFill->addStretch(1);
}

void ParameterEditor::createLineRadius()
{
	_lineRadius = new QHBoxLayout;
	_labelRadius = new QLabel("Rayon: ", _parent);
	_editRadius = new QLineEdit(_parent);
	_editRadius->setValidator(new QIntValidator(0,1000,_parent));
	_editRadius->setFixedWidth(QFontMetrics(_editRadius->font()).width("-00000"));
	_lineRadius->addWidget(_labelRadius);
	_lineRadius->addWidget(_editRadius);
	_lineRadius->addStretch(1);
}
