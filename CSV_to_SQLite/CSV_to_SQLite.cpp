#include "CSV_to_SQLite.h"
#include "DialogSaveSettings.h"
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>
#include <QCloseEvent>
#include <ranges>


CSV_to_SQLite::CSV_to_SQLite(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.buttonOpen, SIGNAL(pressed()), this, SLOT(openClicked()));
	connect(ui.buttonSave, SIGNAL(pressed()), this, SLOT(saveClicked()));
	connect(ui.lineEditFile, SIGNAL(editingFinished()), this, SLOT(updateButtonSave()));
	connect(ui.lineEditSeparator, SIGNAL(editingFinished()), this, SLOT(updateButtonOpen()));

	loadSettings();
}

void CSV_to_SQLite::closeEvent(QCloseEvent* event) {
	saveSettings();
}

void CSV_to_SQLite::setQTableWidget(QList<QStringList> table) {
	ui.tableWidget->setColumnCount(table[0].count());
	ui.tableWidget->setHorizontalHeaderLabels(table[0]);
	for (int i = 0; i < table.count() - 1; i++) {
		ui.tableWidget->insertRow(i);
		for (int j = 0; j < table[0].count(); j++)
		{
			ui.tableWidget->setItem(i, j, new QTableWidgetItem(table[i + 1][j]));
		}
	}
}

QList<QStringList> CSV_to_SQLite::getQTableWidget() {
	QList<QStringList> table;

	table.append(QStringList());
	for (int i = 0; i < ui.tableWidget->columnCount(); i++)
	{
		table[0].append(ui.tableWidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString());
	}

	for (int i = 0; i < ui.tableWidget->rowCount(); i++) {
		table.append(QStringList());
		for (int j = 0; j < ui.tableWidget->columnCount(); j++) {
			table[i+1].append(ui.tableWidget->item(i, j)->text());
		}
	}

	return table;
}

void CSV_to_SQLite::updateButtonSave() {
	if (ui.lineEditFile->text() != "" && ui.lineEditSeparator->text() != "") {
		ui.buttonSave->setEnabled(true);
	}
	else {
		ui.buttonSave->setEnabled(false);
	}
}

void CSV_to_SQLite::updateButtonOpen() {
	if (ui.lineEditSeparator->text() != "") {
		ui.buttonOpen->setEnabled(true);
	}
	else {
		ui.buttonOpen->setEnabled(false);
	}
}

void CSV_to_SQLite::openClicked() {
	QString filePath = QFileDialog::getOpenFileName(this, "Select CSV", ui.lineEditFile->text(), "CSV file (*.csv)");
	ui.lineEditFile->setText(filePath);

	auto table = readCSV(filePath);
	setQTableWidget(table);
}

void CSV_to_SQLite::saveClicked() {
	DialogSaveSettings dialog;
	dialog.setModal(true);

	if (dialog.exec() == QDialog::Accepted) {
		QString path;
		path = dialog.getFilePath();
		writeSQL(getQTableWidget(), path);
	}
}

QList<QStringList> CSV_to_SQLite::readCSV(QString filePath) {
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << file.errorString();
	}

	QList<QStringList> table;
	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine();
		table.append(line.split(','));
	}

	return table;
}

void CSV_to_SQLite::saveSettings() {
	QSettings settings("dev", "CSV_to_SQLite");
	settings.beginGroup("MainWindow");
	settings.setValue("separator", ui.lineEditSeparator->text());
	settings.setValue("file_path", ui.lineEditFile->text());
	settings.endGroup();
}

void CSV_to_SQLite::loadSettings() {
	QSettings settings("dev", "CSV_to_SQLite");
	settings.beginGroup("MainWindow");
	ui.lineEditSeparator->setText(settings.value("separator", "").toString());
	ui.lineEditFile->setText(settings.value("file_path", "").toString());
	settings.endGroup();
}

void CSV_to_SQLite::writeSQL(QList<QStringList> table, QString path) {
	//Open the database
	auto db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(path);
	if (!db.open()) {
		exit(-1);
	}

	QSqlQuery query;
	//Suppression + creation of the table
	query.exec("DROP TABLE main;");

	auto table_fields = table[0];
	QString req = "CREATE TABLE main(";
	for (QString field : table_fields) {
		req.append("'" + field + "' char(255),");
	}
	req.chop(1);
	req.append(");");
	qDebug() << req;
	query.exec(req);

	for (QStringList list : table | std::views::drop(1)) {
		QString req = "INSERT INTO main VALUES(";
		for (QString value : list) {
			req.append("'" + value + "',");
		}
		req.chop(1);
		req.append(");");
		query.exec(req);
	}
	//if (f.open(QIODevice::ReadOnly)) {
	//	QSqlQuery que;
	//	QTextStream ts(&f);

	//	//Travel through the csv file "excel.csv"
	//	while (!ts.atEnd()) {
	//		QString req = "INSERT INTO main VALUES(";
	//		// split every lines on comma
	//		QStringList line = ts.readLine().split(',');
	//		/*for every values on a line,
	//			append it to the INSERT request*/
	//		for (int i = 0; i < line.length(); ++i) {
	//			req.append(line.at(i));
	//			req.append(",");
	//		}
	//		req.chop(1); // remove the trailing comma
	//		req.append(");"); // close the "VALUES([...]" with a ");"
	//		que.exec(req);
	//	}
	//	f.close();
	//}
}