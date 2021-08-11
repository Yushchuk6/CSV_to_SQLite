#include "DialogSaveSettings.h"
#include <QFileDialog>
#include <QSettings>


DialogSaveSettings::DialogSaveSettings(QDialog* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.directoryButton, SIGNAL(clicked()), this, SLOT(directoryClicked()));
	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(okClicked()));

	loadSettings();
}

void DialogSaveSettings::okClicked() {
	saveSettings();
}

void DialogSaveSettings::directoryClicked() {
	QString dirPath = QFileDialog::getExistingDirectory(this, "Select directory", ui.lineEditDirectory->text());
	ui.lineEditDirectory->setText(dirPath);
}

void DialogSaveSettings::saveSettings()
{
	QSettings settings("dev", "CSV_to_SQLite");
	settings.beginGroup("DialogSave");
	settings.setValue("dir", ui.lineEditDirectory->text());
	settings.setValue("name", ui.lineEditName->text());
	settings.endGroup();
}

void DialogSaveSettings::loadSettings()
{
	QSettings settings("dev", "CSV_to_SQLite");
	settings.beginGroup("DialogSave");
	ui.lineEditDirectory->setText(settings.value("dir", "").toString());
	ui.lineEditName->setText(settings.value("name", "").toString());
	settings.endGroup();
}

QString DialogSaveSettings::getFilePath() {
	auto directory = ui.lineEditDirectory->text();
	auto name = ui.lineEditName->text();
	return directory + '/' + name;
}