#pragma once

#include <QtWidgets/QWidget>
#include "ui_DialogSaveSettings.h"

class DialogSaveSettings : public QDialog
{
    Q_OBJECT

public:
    DialogSaveSettings(QDialog* parent = Q_NULLPTR);
    QString getFilePath();


private:
    Ui::Dialog ui;
    QString directory, name;

    void saveSettings();
    void loadSettings();
    void okClicked();

private slots:
    void directoryClicked();
};