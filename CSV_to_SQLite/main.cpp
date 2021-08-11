#include "CSV_to_SQLite.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CSV_to_SQLite w;
    w.show();
    return a.exec();
}
