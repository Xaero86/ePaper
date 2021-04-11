#include "elementswidget.h"

ElementsWidget::ElementsWidget(QWidget *parent, Session *session)
	: QTreeView(parent), _session(session)
{
	setRootIsDecorated(false);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::InternalMove);

	_contextMenu = new QMenu(this);
	_actionSuppr = new QAction("&Supprimer", this);
	_contextMenu->addAction(_actionSuppr);

	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(prepareMenu(QPoint)));
	connect(this, SIGNAL(keySuppr()),                         this, SLOT(handleSuppr()));
	connect(_actionSuppr, SIGNAL(triggered()),                this, SLOT(handleSuppr()));
}

void ElementsWidget::prepareMenu(const QPoint &pos)
{
	QModelIndex element = indexAt(pos);

	if (element.isValid())
	{
		_contextMenu->popup(viewport()->mapToGlobal(pos));
	}
}

void ElementsWidget::handleSuppr()
{
	QModelIndexList elementList = selectedIndexes();

	for (int i=elementList.count()-1; i >= 0; i--)
	{
		if (elementList.at(i).column() == 0)
		{
			_session->removeElement(elementList.at(i).row());
		}
	}
}

void ElementsWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Delete)
	{
		emit keySuppr();
	}
}

void ElementsWidget::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-elementIndex"))
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
}

void ElementsWidget::dropEvent(QDropEvent *event)
{
	if ((!event->mimeData()->hasFormat("application/x-elementIndex") && (event->dropAction() != Qt::MoveAction)))
	{
		event->ignore();
		return;
	}

	QByteArray dataEvent = event->mimeData()->data("application/x-elementIndex");
	QDataStream stream(&dataEvent, QIODevice::ReadOnly);
	QList<int> indexes;

	while (!stream.atEnd())
	{
		int index = -1;
		stream >> index;
		indexes.append(index);
	}

	QModelIndex elements = indexAt(event->pos());
	QList<int> elementsIndex;

	for (int i = 0; i < indexes.count(); i++)
	{
		elementsIndex.append(indexes.at(i));
	}
	_session->moveElements(elementsIndex, elements.row());
}
