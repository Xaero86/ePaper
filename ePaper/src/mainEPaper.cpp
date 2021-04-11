#include "epaperwindow.h"
#include "epapercontroller.h"
#include "properties.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication appli(argc, argv);
	QCoreApplication::setApplicationName("ePaper");
	QCoreApplication::setApplicationVersion("1.1");
	
	QCommandLineParser parser;
	parser.setApplicationDescription("Permet l'affichage d'image sur l'ecran Waveshare 4.3inch e-Paper\n" \
									"Sans argument ni option, ePaper demarre en mode interactif");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("image", "Image a afficher", "[image]");
	
	QCommandLineOption propertiesOption("c", "Fichier de configuration (mode interactif et non-interactif)", "fichier");
	parser.addOption(propertiesOption);
	
	QCommandLineOption veilleOption("i", "Envoie la commande de veille");
	parser.addOption(veilleOption);

	QCommandLineOption prepareOption("p", "Charge l'image mais n'envoie pas la commande d'affichage");
	parser.addOption(prepareOption);

	QCommandLineOption sendOption("s", "Envoie uniquement la commande d'affichage. L'argument image et l'option p sont ignores");
	parser.addOption(sendOption);

	parser.process(appli);
	
	const QStringList args = parser.positionalArguments();
	QString propertiesFile = parser.value(propertiesOption);
	bool sendVeille = parser.isSet(veilleOption);
	bool prepare = parser.isSet(prepareOption);
	bool refreshOnly = parser.isSet(sendOption);
	
	if (propertiesFile.isEmpty())
	{
		propertiesFile = QCoreApplication::applicationDirPath() + "/properties.xml";
	}
	Properties::loadPropertyFile(propertiesFile);

	if ((args.size() == 0) && !sendVeille && !prepare && !refreshOnly)
	{
		EPaperWindow mainWindow;
		mainWindow.show();
		return appli.exec();
	}
	else if (refreshOnly)
	{
		EPaperController mainController;
		return mainController.refresh(sendVeille);
	}
	else
	{
		QString file;
		if (args.size() > 0)
		{
			file = args.at(0);
		}
		EPaperController mainController;
		return mainController.display(file, sendVeille, !prepare);
	}
}
