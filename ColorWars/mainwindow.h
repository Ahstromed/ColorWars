/** @file mainwindow.h
 *  @brief lays the foundation for the mainwindow
 *  and its slots and signals
 *
 *  @author Ahmed Qureshi
 *  @bug No known bugs.
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameboard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void easy_game_begin();
    void medium_game_begin();
    void hard_game_begin();

    void game_over();
    void game_over2();
    void game_over3();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GameBoard* board;
};

#endif // MAINWINDOW_H
