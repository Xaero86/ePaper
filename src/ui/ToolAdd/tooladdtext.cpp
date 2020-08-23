#include "tooladdtext.h"

#include "drawabletext.h"

#include <QString>
#include <QInputDialog>

ToolAddText::ToolAddText(QWidget *parent, Color color)
	: ToolAddElement(color), _parent(parent)
{

}

DrawableElement *ToolAddText::manageClickEvent(QMouseEvent *e)
{
	bool ok;
	QString text = QInputDialog::getText(_parent, "Inserer du texte", "Texte:", QLineEdit::Normal, "", &ok, Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
	if (ok && !text.isEmpty())
	{
		return new DrawableText(e->x(), e->y(), text, 1, _color);
	}
	return nullptr;
}
