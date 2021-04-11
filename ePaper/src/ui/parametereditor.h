#ifndef PARAMETEREDITOR_H
#define PARAMETEREDITOR_H

#include <QGroupBox>

#include "drawableelement.h"
#include "drawablepoint.h"
#include "drawableline.h"
#include "drawablerectangle.h"
#include "drawablecircle.h"
#include "drawabletriangle.h"
#include "drawabletext.h"
#include "drawableimage.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>

#define EDITOR_NUM_WIDTH  40
#define EDITOR_TEXT_WIDTH 110

class ParameterEditor : public QGroupBox
{
	Q_OBJECT
public:
	ParameterEditor(QWidget *parent, DrawableElement *element);

	void setEditorData();
	void setModelData();

private:
	QWidget*         _parent;
	DrawableElement* _element;

	void createPointEditor();
	void createLineEditor();
	void createRectangleEditor();
	void createCircleEditor();
	void createTriangleEditor();
	void createTextEditor();
	void createImageEditor();

	void setEditorDataPoint(DrawablePoint* point);
	void setEditorDataLine(DrawableLine *line);
	void setEditorDataRectangle(DrawableRectangle *rectangle);
	void setEditorDataCircle(DrawableCircle *circle);
	void setEditorDataTriangle(DrawableTriangle *triangle);
	void setEditorDataText(DrawableText *text);
	void setEditorDataImage(DrawableImage* image);

	void setModelDataPoint(DrawablePoint *point);
	void setModelDataLine(DrawableLine* line);
	void setModelDataRectangle(DrawableRectangle* rectangle);
	void setModelDataCircle(DrawableCircle* circle);
	void setModelDataTriangle(DrawableTriangle* triangle);
	void setModelDataText(DrawableText* text);
	void setModelDataImage(DrawableImage* image);

	QHBoxLayout* _line;
	QLabel*      _labelX;
	QLineEdit*   _editX;
	QLabel*      _labelY;
	QLineEdit*   _editY;
	void createLine();

	QHBoxLayout* _line0;
	QLabel*      _labelX0;
	QLineEdit*   _editX0;
	QLabel*      _labelY0;
	QLineEdit*   _editY0;
	void createLine0();

	QHBoxLayout* _line1;
	QLabel*      _labelX1;
	QLineEdit*   _editX1;
	QLabel*      _labelY1;
	QLineEdit*   _editY1;
	void createLine1();

	QHBoxLayout* _line2;
	QLabel*      _labelX2;
	QLineEdit*   _editX2;
	QLabel*      _labelY2;
	QLineEdit*   _editY2;
	void createLine2();

	QHBoxLayout* _lineFill;
	QLabel*      _labelFill;
	QCheckBox*   _checkFill;
	void createLineFill();

	QHBoxLayout* _lineText;
	QLineEdit*   _editText;
	void createLineText();

	QHBoxLayout* _lineTextSize;
	QLabel*      _labelSizeText;
	QComboBox*   _comboSizeText;
	void createLineTextSize();

	QHBoxLayout* _lineRadius;
	QLabel*      _labelRadius;
	QLineEdit*   _editRadius;
	void createLineRadius();
};

#endif // PARAMETEREDITOR_H
