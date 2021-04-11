#ifndef EPAPERWINDOW_H
#define EPAPERWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QLabel>
#include <QDockWidget>

#include "definition.h"
#include "session.h"
#include "widgetdisplay.h"
#include "elementswidget.h"
#include "elementsmodel.h"
#include "directionwidget.h"
#include "colorwidget.h"
#include "toolswidget.h"
#include "serialconfigwidget.h"
#include "loggerconsole.h"

#include "tooladdelement.h"
#include "toolmoveelement.h"

class EPaperWindow : public QMainWindow
{
	Q_OBJECT

public:
	EPaperWindow();

public slots:
	void newImage();
	void loadImage();
	void saveImage();
	void updateWidget();
	void handleElementsAdded(int row, int count);
	void handleElementsRemoved(int row, int count);
	void handleElementMoved(int dest, int count);
	void handleStatus(int status);
	void handleProgressBar(int percent);
	void handleDirectionChanged(bool fromIHM);
	void handleBackgroundColorChanged(bool fromIHM);
	void handleForegroundColorChanged(bool fromIHM);
	void handleToolChanged();
	void handleSelectionChanged();
	void displayErrorMsg(QString errorMsg);

private:
	Session             _session;
	WidgetDisplay*      _widgetDisplay;
	ElementsWidget*     _elementsWidget;
	ElementsModel*      _elementModel;
	DirectionWidget*    _directionWidget;
	ColorWidget*        _backColorWidget;
	ColorWidget*        _foreColorWidget;
	ToolsWidget*        _toolsWidget;
	SerialConfigWidget* _serialConfigWidget;
	LoggerConsole*      _loggerConsole;

	QWidget*        _centralWidget;

	// menu action
	QAction*        _actionNew;
	QAction*        _actionLoad;
	QAction*        _actionSave;
	QAction*        _actionQuit;

	QAction*        _actionDebug;

	QLabel*         _statusLabel;
	QProgressBar*   _statusProgressBar;
	QPushButton*    _cancelButton;

	QUrl            _lastUsedPath;

	// tools
	ToolMoveElement _movingTool;
};

#endif // EPAPERWINDOW_H
