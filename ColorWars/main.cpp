/** @file main.cpp
 *  @brief builds game using Qapplication
 *
 *  @author Ahmed Qureshi
 *  @bug No known bugs.
 */


#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
