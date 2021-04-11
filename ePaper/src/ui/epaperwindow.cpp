#include "epaperwindow.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>

#include "properties.h"
#include "comboboxdelegate.h"
#include "parameterdelegate.h"

EPaperWindow::EPaperWindow()
	: QMainWindow(), _session(Properties::defaultDirection(), Properties::defaultBackgroundColor()),
	  _lastUsedPath(QUrl::fromLocalFile(QDir::home().absolutePath())), _movingTool()
{
	_widgetDisplay = new WidgetDisplay(this, &_session, Properties::defaultDirection(), Properties::defaultBackgroundColor(), &_movingTool);

	QVBoxLayout* vbl = new QVBoxLayout;
	vbl->addStretch();
	vbl->addWidget(_widgetDisplay);
	vbl->addStretch();

	QHBoxLayout* hbl = new QHBoxLayout;
	hbl->addStretch();
	hbl->addLayout(vbl);
	hbl->addStretch();

	_centralWidget = new QWidget;
	_centralWidget->setFixedSize(800,800);
	_centralWidget->setLayout(hbl);

	QScrollArea *scrollCenter = new QScrollArea;
	scrollCenter->setWidget(_centralWidget);

	setCentralWidget(scrollCenter);

	_elementsWidget = new ElementsWidget(this, &_session);
	_elementModel = new ElementsModel(this, &_session);
	_elementsWidget->setModel(_elementModel);
	_elementsWidget->setItemDelegateForColumn(1, new ComboBoxDelegate(this, &_session));
	_elementsWidget->setItemDelegateForColumn(2, new ParameterDelegate(this, &_session));
	_elementsWidget->header()->setStretchLastSection(false);

	QDockWidget::DockWidgetFeatures defaultDockFeature = QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable;

	QDockWidget *_rightDockWidget = new QDockWidget(this);
	_rightDockWidget->setWidget(_elementsWidget);
	_rightDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
	addDockWidget(Qt::RightDockWidgetArea, _rightDockWidget);

	_directionWidget = new DirectionWidget(this, Properties::defaultDirection());
	QDockWidget *_directionDockWidget = new QDockWidget("Direction", this);
	_directionDockWidget->setWidget(_directionWidget);
	_directionDockWidget->setFeatures(defaultDockFeature);
	addDockWidget(Qt::LeftDockWidgetArea, _directionDockWidget);

	_backColorWidget = new ColorWidget(this, Properties::defaultBackgroundColor());
	QDockWidget *_backColorDockWidget = new QDockWidget("Couleur de fond", this);
	_backColorDockWidget->setWidget(_backColorWidget);
	_backColorDockWidget->setFeatures(defaultDockFeature);
	addDockWidget(Qt::LeftDockWidgetArea, _backColorDockWidget);

	_foreColorWidget = new ColorWidget(this, Properties::defaultForegroundColor());
	QDockWidget *_foreColorDockWidget = new QDockWidget("Couleur", this);
	_foreColorDockWidget->setWidget(_foreColorWidget);
	_foreColorDockWidget->setFeatures(defaultDockFeature);
	addDockWidget(Qt::LeftDockWidgetArea, _foreColorDockWidget);

	_toolsWidget = new ToolsWidget(this, Properties::defaultForegroundColor());
	QDockWidget *_toolsDockWidget = new QDockWidget("Outils", this);
	_toolsDockWidget->setWidget(_toolsWidget);
	_toolsDockWidget->setFeatures(defaultDockFeature);
	addDockWidget(Qt::LeftDockWidgetArea, _toolsDockWidget);

	_serialConfigWidget = new SerialConfigWidget(this);
	QDockWidget *_serialDockWidget = new QDockWidget("Connexion", this);
	_serialDockWidget->setWidget(_serialConfigWidget);
	_serialDockWidget->setFeatures(defaultDockFeature);
	addDockWidget(Qt::RightDockWidgetArea, _serialDockWidget);

	setDockOptions(dockOptions() & ~QMainWindow::AllowTabbedDocks);

	_loggerConsole = new LoggerConsole(this);
	_loggerConsole->setWindowFlags(_loggerConsole->windowFlags() | Qt::Window | Qt::WindowStaysOnTopHint);
	_loggerConsole->setMinimumSize(300,200);
	_loggerConsole->setWindowTitle(QCoreApplication::applicationName() + " - Console");

	_movingTool.setElementsList(_elementsWidget->selectionModel());

	// MENU FILE
	QMenu *menuFile = menuBar()->addMenu("&Fichier");
	_actionNew = new QAction("&Nouveau", this);
	menuFile->addAction(_actionNew);
	_actionLoad = new QAction("&Ouvrir", this);
	menuFile->addAction(_actionLoad);
	_actionSave = new QAction("&Enregistrer", this);
	menuFile->addAction(_actionSave);
	menuFile->addSeparator();
	_actionQuit = new QAction("&Quitter", this);
	menuFile->addAction(_actionQuit);

	_actionDebug = new QAction("&Debug Console", this);

	if (Properties::debug())
	{
		QMenu *menuDebug = menuBar()->addMenu("&Debug");
		menuDebug->addAction(_actionDebug);
	}

	// STATUS BAR
	_statusLabel = new QLabel;
	_statusLabel->setText("Non connecte");
	_statusProgressBar = new QProgressBar;
	_statusProgressBar->setTextVisible(true);
	_statusProgressBar->setMaximumSize(100, 10);
	_statusProgressBar->hide();
	_cancelButton = new QPushButton("Annuler");
	_cancelButton->hide();

	statusBar()->addPermanentWidget(_statusLabel);
	statusBar()->addPermanentWidget(_statusProgressBar,1);
	statusBar()->addPermanentWidget(_cancelButton);

	// CONNECTION
	connect(_actionNew,   SIGNAL(triggered()), this, SLOT(newImage()));
	connect(_actionLoad,  SIGNAL(triggered()), this, SLOT(loadImage()));
	connect(_actionSave,  SIGNAL(triggered()), this, SLOT(saveImage()));
	connect(_actionQuit,  SIGNAL(triggered()), this, SLOT(close()));
	connect(_actionDebug, SIGNAL(triggered()), _loggerConsole, SLOT(show()));
	connect(&_session, SIGNAL(elementsAdded(int,int)),       this, SLOT(updateWidget()));
	connect(&_session, SIGNAL(elementsRemoved(int,int)),     this, SLOT(updateWidget()));
	connect(&_session, SIGNAL(elementsAdded(int,int)),       this, SLOT(handleElementsAdded(int,int)));
	connect(&_session, SIGNAL(elementsRemoved(int,int)),     this, SLOT(handleElementsRemoved(int,int)));
	connect(&_session, SIGNAL(elementsMoved(int,int)),       this, SLOT(handleElementMoved(int,int)));
	connect(&_session, SIGNAL(directionChanged(bool)),       this, SLOT(handleDirectionChanged(bool)));
	connect(&_session, SIGNAL(backgroundColorChanged(bool)), this, SLOT(handleBackgroundColorChanged(bool)));
	connect(&_session, SIGNAL(statusChanged(int)),           this, SLOT(handleStatus(int)));
	connect(&_session, SIGNAL(newWriteStatus(int)),          this, SLOT(handleProgressBar(int)));
	connect(&_session, SIGNAL(errorLoading(QString)),        this, SLOT(displayErrorMsg(QString)));
	connect(&_session, SIGNAL(errorSaving(QString)),         this, SLOT(displayErrorMsg(QString)));
	connect(_serialConfigWidget, SIGNAL(changeSerialPort(QString)), &_session, SLOT(changeSerialPort(QString)));
	connect(_serialConfigWidget, SIGNAL(commandDraw()),             &_session, SLOT(drawSerial()));
	connect(_serialConfigWidget, SIGNAL(commandIdle()),             &_session, SLOT(turnIdleSerial()));
	connect(_cancelButton, SIGNAL(clicked(bool)),                   &_session, SLOT(cancelSerial()));
	connect(_directionWidget, SIGNAL(directionChanged(bool)),   this, SLOT(handleDirectionChanged(bool)));
	connect(_backColorWidget, SIGNAL(colorChanged(bool)),       this, SLOT(handleBackgroundColorChanged(bool)));
	connect(_foreColorWidget, SIGNAL(colorChanged(bool)),       this, SLOT(handleForegroundColorChanged(bool)));
	connect(_toolsWidget, SIGNAL(toolChanged()),                this, SLOT(handleToolChanged()));
	connect(_elementsWidget->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(handleSelectionChanged()));
	connect(_elementModel, SIGNAL(elementModified(int)), this, SLOT(updateWidget()));
	connect(_widgetDisplay, SIGNAL(elementMoved()),      this, SLOT(updateWidget()));

	// resize et centre
	adjustSize();
	QPoint center = QApplication::desktop()->availableGeometry(this).center();
	move(center.x() - width()/2, center.y() - height()/2);
	setAttribute(Qt::WA_QuitOnClose, true);

	_serialConfigWidget->setSerialPort(Properties::defaultSerialPort());
}

void EPaperWindow::newImage()
{
	_session.clearElements();
}

void EPaperWindow::loadImage()
{
	QString filter = "Fichier image (*.xml)";
	QUrl imageFile = QFileDialog::getOpenFileUrl(this, "Ouvrir une image", _lastUsedPath, "Fichier image (*.xml);;Tous les fichiers (*.*)", &filter);
	if (!imageFile.isEmpty())
	{
		_lastUsedPath = imageFile;
		_session.loadImage(imageFile);
	}
}

void EPaperWindow::saveImage()
{
	QString filter = "Fichier image (*.xml)";
	QUrl imageFile = QFileDialog::getSaveFileUrl(this, "Enregistrer une image", _lastUsedPath, "Fichier image (*.xml)", &filter);
	if (!imageFile.isEmpty())
	{
		_lastUsedPath = imageFile;
		_session.saveImage(imageFile);
	}
}

void EPaperWindow::updateWidget()
{
	_centralWidget->update();
}

void EPaperWindow::handleElementsAdded(int row, int count)
{
	_elementModel->insertRows(row, count);
}

void EPaperWindow::handleElementsRemoved(int row, int count)
{
	_elementModel->removeRows(row, count);
}

void EPaperWindow::handleElementMoved(int dest, int count)
{
	_elementsWidget->selectionModel()->clearSelection();
	for (int i = 0; i < count; i++)
	{
		_elementsWidget->selectionModel()->select(_elementModel->index(dest+i), QItemSelectionModel::Select);
	}
}

void EPaperWindow::handleStatus(int status)
{
	switch (status)
	{
	case NOT_CONNECTED:
		_statusLabel->setText("Non connecte");
		break;
	case CONNECTING:
		_statusLabel->setText("Connexion...");
		break;
	case CONNECTED:
		_statusLabel->setText("Connecte");
		break;
	}
}

void EPaperWindow::handleProgressBar(int percent)
{
	if ((0 <= percent) && (percent <= 100))
	{
		_statusProgressBar->setValue(percent);
		_statusProgressBar->show();
		_cancelButton->show();
	}
	else
	{
		_statusProgressBar->hide();
		_cancelButton->hide();
	}
}

void EPaperWindow::handleDirectionChanged(bool fromIHM)
{
	if (fromIHM)
	{
		Direction dir = _directionWidget->getDirection();
		_session.setDirection(dir);
	}
	else
	{
		Direction dir = _session.getDirection();
		_widgetDisplay->setDirection(dir);
		_centralWidget->update();
		_directionWidget->setDirection(dir);
	}
}

void EPaperWindow::handleBackgroundColorChanged(bool fromIHM)
{
	if (fromIHM)
	{
		Color col = _backColorWidget->getColor();
		_session.setBackgroundColor(col);
	}
	else
	{
		Color col = _session.getBackgroundColor();
		_widgetDisplay->setBackgroundColor(col);
		_centralWidget->update();
		_backColorWidget->setColor(col);
	}
}

void EPaperWindow::handleForegroundColorChanged(bool fromIHM)
{
	if (fromIHM)
	{
		Color col = _foreColorWidget->getColor();
		_toolsWidget->setToolsColor(col);
	}
}

void EPaperWindow::handleToolChanged()
{
	ToolAddElement* currentTools = _toolsWidget->getCurrentTool();
	if (currentTools != nullptr)
	{
		_movingTool.setActive(false);
		currentTools->reset();
	}
	else
	{
		_movingTool.setActive(true);
	}
	_widgetDisplay->setToolAddElement(currentTools);
	_centralWidget->update();
	_elementsWidget->selectionModel()->clearSelection();
}

void EPaperWindow::handleSelectionChanged()
{
	if (_elementsWidget->selectionModel()->selectedRows().count() > 0)
	{
		_toolsWidget->unselectTool();
		_movingTool.setActive(true);
		_widgetDisplay->setToolAddElement(nullptr);
		_centralWidget->update();
	}
}

void EPaperWindow::displayErrorMsg(QString errorMsg)
{
	QMessageBox::warning(this, "Erreur", errorMsg);
}
