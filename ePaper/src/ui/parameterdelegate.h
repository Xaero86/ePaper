#ifndef PARAMETERDELEGATE_H
#define PARAMETERDELEGATE_H

#include <QItemDelegate>

#include "session.h"

class ParameterDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	ParameterDelegate(QObject *parent, Session *session);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
	Session*        _session;
};

#endif // PARAMETERDELEGATE_H
