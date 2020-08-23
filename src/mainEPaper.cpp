#include "epaperwindow.h"
#include "epapercontroller.h"
#include "properties.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication appli(argc, argv);
	QCoreApplication::setApplicationName("ePaper");
	QCoreApplication::setApplicationVersion("1.0");
	
	QCommandLineParser parser;
	parser.setApplicationDescription("Permet l'affichage d'image sur l'ecran Waveshare 4.3inch e-Paper");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("image", "Image a afficher, optionnel. Si positionne, ePaper est ouvert en mode non-interactif", "[image]");
	
	QCommandLineOption propertiesOption("p", "Fichier de parametrage", "fichier");
	parser.addOption(propertiesOption);
	
	QCommandLineOption veilleOption("i", "Envoie la commande de veille. Si positionne, ePaper est ouvert en mode non-interactif");
	parser.addOption(veilleOption);
	
	parser.process(appli);
	
	const QStringList args = parser.positionalArguments();
	QString propertiesFile = parser.value(propertiesOption);
	bool sendVeille = parser.isSet(veilleOption);
	
	if (propertiesFile.isEmpty())
	{
		propertiesFile = QCoreApplication::applicationDirPath() + "/properties.xml";
	}
	Properties::loadPropertyFile(propertiesFile);

	if ((args.size() == 0) && !sendVeille)
	{
		EPaperWindow mainWindow;
		mainWindow.show();
		return appli.exec();
	}
	else
	{
		QString file;
		if (args.size() > 0)
		{
			file = args.at(0);
		}
		EPaperController mainController;
		return mainController.display(file, sendVeille);
	}
}
