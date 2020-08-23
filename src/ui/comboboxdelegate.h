#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

#include "session.h"

#include <QComboBox>

class ComboBoxDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ComboBoxDelegate(QObject *parent, Session *session);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected slots:
	void comboClosed(QWidget *editor);

private:
	Session*        _session;
};

class ComboBoxCustom : public QComboBox
{
	Q_OBJECT
public:
	ComboBoxCustom(QWidget *parent);

public slots:
	void handleClosed();

signals:
	void hasBeenClosed(QWidget *self);
};

#endif // COMBOBOXDELEGATE_H
