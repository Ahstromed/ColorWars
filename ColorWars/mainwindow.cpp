/** @file mainwindow.cpp
 *  @brief defines the different game start and end
 *  slots and signals
 *
 *  @author Ahmed Qureshi
 *  @bug No known bugs.
 */


#include "mainwindow.h"
#include "gameboard.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::easy_game_begin() {
    board = new GameBoard(this,4,5000);
    this->setCentralWidget(board);
}

void MainWindow::medium_game_begin() {
    board = new GameBoard(this,4,3500);
    this->setCentralWidget(board);
}

void MainWindow::hard_game_begin() {
    board = new GameBoard(this,4,2000);
    this->setCentralWidget(board);
}

void MainWindow::game_over() {

    // Take the current board game out of the central widget of MainWindow
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);
    // Line above will delete the widget gracefully (no memory errors)
    QMessageBox::information(
           this,
           tr("Color War"),
           tr("You lose: The computer hit 5 first") );

    // Reset the MainWindow with the initial startup screen
    ui->setupUi(this);
}

void MainWindow::game_over2(){

    // Take the current board game out of the central widget of MainWindow
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);
    // Line above will delete the widget gracefully (no memory errors)
    QMessageBox::information(
           this,
           tr("Color War"),
           tr("You win: You hit 5 first") );

    // Reset the MainWindow with the initial startup screen
    ui->setupUi(this);
}
void MainWindow::game_over3(){

    // Take the current board game out of the central widget of MainWindow
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);
    // Line above will delete the widget gracefully (no memory errors)
    QMessageBox::information(
           this,
           tr("Color War"),
           tr("You Quit") );
    // Reset the MainWindow with the initial startup screen
    ui->setupUi(this);
}





MainWindow::~MainWindow()
{
    delete ui;
}
