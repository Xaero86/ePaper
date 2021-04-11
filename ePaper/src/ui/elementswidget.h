#ifndef ELEMENTSWIDGET_H
#define ELEMENTSWIDGET_H

#include <QTreeView>

#include "session.h"

#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMenu>

class ElementsWidget : public QTreeView
{
	Q_OBJECT
public:
	ElementsWidget(QWidget *parent, Session* session);

signals:
	void keySuppr();

public slots:
	void prepareMenu(const QPoint &pos);
	void handleSuppr();

protected:
	void keyPressEvent(QKeyEvent* event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

private:
	Session*  _session;
	QMenu*    _contextMenu;
	QAction*  _actionSuppr;
};

#endif // ELEMENTSWIDGET_H
