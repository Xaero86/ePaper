#include "comboboxdelegate.h"

#include <QComboBox>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent, Session *session)
	: QStyledItemDelegate(parent), _session(session)
{
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
	ComboBoxCustom *combo = new ComboBoxCustom(parent);

	QStringList list;
	list.append("Black");
	list.append("Dark gray");
	list.append("Light gray");
	list.append("White");

	combo->addItems(list);
	combo->setItemData(0, QColor(Qt::black),     Qt::BackgroundRole);
	combo->setItemData(0, QColor(Qt::white),     Qt::ForegroundRole);
	combo->setItemData(1, QColor(Qt::darkGray),  Qt::BackgroundRole);
	combo->setItemData(2, QColor(Qt::lightGray), Qt::BackgroundRole);
	combo->setItemData(3, QColor(Qt::white),     Qt::BackgroundRole);

	connect(combo, SIGNAL(hasBeenClosed(QWidget*)), this, SLOT(comboClosed(QWidget*)));
	return combo;
}

void ComboBoxDelegate::comboClosed(QWidget *editor)
{
	if (editor != nullptr)
	{
		emit commitData(editor);
		emit closeEditor(editor);
	}
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	int row = index.row();

	if (row < _session->getElements()->count())
	{
		Color color = _session->getElements()->at(row)->getColor();
		int colorIndex = 0;
		switch (color) {
		case Color::black:     colorIndex = 0; break;
		case Color::darkGray:  colorIndex = 1; break;
		case Color::lightGray: colorIndex = 2; break;
		case Color::white:     colorIndex = 3; break;
		default:               colorIndex = 0; break;
		}

		QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
		comboBox->setCurrentIndex(colorIndex);
	}
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	int row = index.row();

	if (row < _session->getElements()->count())
	{
		QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
		Color newColor = Color::black;
		switch (comboBox->currentIndex()) {
		case 0: newColor = Color::black;     break;
		case 1: newColor = Color::darkGray;  break;
		case 2: newColor = Color::lightGray; break;
		case 3: newColor = Color::white;     break;
		}
		_session->getElements()->at(row)->setColor(newColor);

		model->setData(index, QVariant(), Qt::EditRole);
	}
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
	((QComboBox *) editor)->showPopup();
}

ComboBoxCustom::ComboBoxCustom(QWidget *parent) : QComboBox(parent)
{
	connect(this, SIGNAL(activated(int)), this, SLOT(handleClosed()), Qt::DirectConnection);
}

void ComboBoxCustom::handleClosed()
{
	emit hasBeenClosed(this);
}
