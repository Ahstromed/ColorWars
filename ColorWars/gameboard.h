/** @file gameboard.h
 *  @brief lays the foundation for gameboard
 *  and its slots and signals
 *
 *  @author Ahmed Qureshi
 *  @bug No known bugs.
 */

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <random>
#include <chrono>
#include <QTimer>
#include <iostream>
#include <QVector>


namespace Ui {
class GameBoard;
}

class GameBoard : public QWidget
{
    Q_OBJECT

public slots:
    void processOneThing();

signals:
    void game_over();
    void game_over2();
    void game_over3();

public:
    explicit GameBoard(QWidget *parent = 0, size_t board_size = 10, int reaction = 5000);

    ~GameBoard();

       void keyPressEvent(QKeyEvent *event);
       void showEvent(QShowEvent *e);

       bool munchNumber(int x, int y);
       void updateAfterMunch(bool flag);
       bool isValidMunch(int x, int y);


private:
    Ui::GameBoard *ui;

    // Top bar variables
    QLabel* game_type_value;
    QLabel* level_value;
    QWidget* Top;
    QLabel** lives;

    // Board Variables
    QWidget *Board;
    size_t board_size;
    int reaction;
    QLabel** labels;
    int* values;
    QLabel** toplabels;
    QLabel** scorelabel;
    int counter = 0;
    int comcounter = 0;
    bool pressed = true;
    QTimer *Timer;
    int vectorcounter;
};

#endif // GAMEBOARD_H

