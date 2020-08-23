#ifndef ELEMENTSMODEL_H
#define ELEMENTSMODEL_H

#include <QAbstractListModel>

#include "session.h"

class ElementsModel : public QAbstractListModel
{
	Q_OBJECT
public:
	ElementsModel(QObject *parent, Session *session);

	int rowCount(const QModelIndex &parent) const override;
	int columnCount(const QModelIndex &parent) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	bool setData(const QModelIndex &index, const QVariant & value, int role) override;
	Qt::ItemFlags flags(const QModelIndex & index) const override ;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	QStringList mimeTypes() const override;
	QMimeData *mimeData(const QModelIndexList &indexes) const override;
	Qt::DropActions supportedDropActions() const override;

signals:
	void elementModified(int index);

private:
	Session*        _session;
};

#endif // ELEMENTSMODEL_H
