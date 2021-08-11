#pragma once

#include <QtWidgets/QWidget>
#include "ui_CSV_to_SQLite.h"

class CSV_to_SQLite : public QWidget
{
    Q_OBJECT

public:
    CSV_to_SQLite(QWidget *parent = Q_NULLPTR);

private:
    Ui::CSV_to_SQLiteClass ui;

    QList<QStringList> readCSV(QString file_path);
    void writeSQL(QList<QStringList> table, QString path);
    void saveSettings();
    void loadSettings();
    void setQTableWidget(QList<QStringList> table);
    QList<QStringList> getQTableWidget();

    void closeEvent(QCloseEvent* event);

private slots:
    void openClicked();
    void saveClicked();
    void updateButtonSave();
    void updateButtonOpen();
};
