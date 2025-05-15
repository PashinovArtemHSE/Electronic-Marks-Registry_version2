#include "mainwindow.h"
#include "database.h"
#include <QApplication>

int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);

    //Инициализация базы данных перед заходом в программу
    Database::initializeDatabase();
    MainWindow w;
    w.show();

    return a.exec();
}
