#include "elementsmodel.h"

#include <QWidget>

#include "tools.h"

ElementsModel::ElementsModel(QObject *parent, Session* session)
	: QAbstractListModel(parent), _session(session)
{

}

int ElementsModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
	{
		return 0;
	}
	return _session->getElements()->count();
}

int ElementsModel::columnCount(const QModelIndex &/*parent*/) const
{
	return 3;
}

QVariant ElementsModel::data(const QModelIndex &index, int role) const
{
	int row = index.row();
	int col = index.column();
	if (row >= _session->getElements()->count())
	{
		return QVariant();
	}
	DrawableElement* element = _session->getElements()->at(row);

	switch(role){
	case Qt::DisplayRole:
		if (col == 0)
		{
			return element->toString();
		}
		else if (col == 1)
		{
			return Tools::colorToQStringIHM(element->getColor());
		}
		else if (col == 2)
		{
			return element->parametersToString();
		}
		else
		{
			return QVariant();
		}
		break;
	case Qt::BackgroundRole:
		if (col == 1)
		{
			return QBrush(Tools::colorToQColor(element->getColor()));
		}
		else
		{
			return QVariant();
		}
		break;
	case Qt::ForegroundRole:
		if (col == 1)
		{
			switch (element->getColor()) {
			case Color::black:     return QBrush(Qt::white); break;
			case Color::darkGray:  return QBrush(Qt::black); break;
			case Color::lightGray: return QBrush(Qt::black); break;
			case Color::white:     return QBrush(Qt::black); break;
			default:               return QBrush(Qt::white); break;
			}
		}
		else
		{
			return QVariant();
		}
		break;
	}

	return QVariant();
}

bool ElementsModel::setData(const QModelIndex &index, const QVariant &/*value*/, int role)
{
	if (role == Qt::EditRole)
	{
		emit elementModified(index.row());
	}
	return true;
}

Qt::ItemFlags ElementsModel::flags(const QModelIndex & index) const
{
	if (!index.isValid())
	{
		return Qt::ItemIsDropEnabled;
	}
	int col = index.column();
	if (col != 0)
	{
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
	}
	else
	{
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
	}
}

QVariant ElementsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) {
			switch (section)
			{
			case 0:
				return QString("Type");
			case 1:
				return QString("Couleur");
			case 2:
				return QString("Parametres");
			}
		}
	}
	return QVariant();
}

bool ElementsModel::insertRows(int row, int count, const QModelIndex &parent)
{
	beginInsertRows(parent, row, row+count-1);
	endInsertRows();
	return true;
}

bool ElementsModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row+count-1);
	endRemoveRows();
	return true;
}

QStringList ElementsModel::mimeTypes() const
{
	QStringList types;
	types << "application/x-elementIndex";
	return types;
}

QMimeData *ElementsModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodeData;

	QDataStream stream(&encodeData, QIODevice::WriteOnly);

	foreach (QModelIndex index, indexes)
	{
		if (index.isValid() && index.column() == 0)
		{
			stream << index.row();
		}
	}

	mimeData->setData("application/x-elementIndex", encodeData);
	return mimeData;
}

Qt::DropActions ElementsModel::supportedDropActions() const
{
	return Qt::MoveAction;
}
