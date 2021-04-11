#include "parameterdelegate.h"

#include "parametereditor.h"

ParameterDelegate::ParameterDelegate(QObject *parent, Session *session)
	: QItemDelegate(parent), _session(session)
{

}

QWidget *ParameterDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
	int row = index.row();

	if (row < _session->getElements()->count())
	{
		_session->getElements()->at(row);
		return new ParameterEditor(parent, _session->getElements()->at(row));
	}
	return nullptr;
}

void ParameterDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	int row = index.row();

	if (row < _session->getElements()->count())
	{
		ParameterEditor* parameterEditor = qobject_cast<ParameterEditor*>(editor);
		parameterEditor->setEditorData();
	}
}

void ParameterDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	int row = index.row();

	if (row < _session->getElements()->count())
	{
		ParameterEditor* parameterEditor = qobject_cast<ParameterEditor*>(editor);
		parameterEditor->setModelData();
		model->setData(index, QVariant(), Qt::EditRole);
	}
}

void ParameterDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
	QRect rect = option.rect;
	rect.setSize(editor->sizeHint());
	editor->setGeometry(rect);
}
