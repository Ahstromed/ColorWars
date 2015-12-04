/** @file gameboard.cpp
 *  @brief holds the structure of the game
 *
 *  @author Ahmed Qureshi
 *  @bug No known bugs.
 */

#include "gameboard.h"
#include "mainwindow.h"
#include "ui_gameboard.h"
#include <QPushButton>
#include <chrono>

//Random number generator
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator (seed);
std::uniform_int_distribution<int> distribution(1,10);

//holds all the values of color
QVector <int> ints = {0,1,2,3,4,5,6,7,8,9};
QVector <int> colors = {0,1,2,3,4};
QSet <int> check = {0,1,2,3,4,5,6,7,8,9,10,11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,96,97,98,99};

//temp vector to hold top 3 cards
QVector <int> temp = {};

//vector to hold a timer for matching key presses
QVector<QTimer*> vector;
int vectorcounter = 0;

//Constructs UI and gameboard
GameBoard::GameBoard(QWidget *parent, size_t board_sz, int reactiontime) :
    QWidget(parent),
    ui(new Ui::GameBoard), board_size(board_sz), reaction(reactiontime)
{
    ui->setupUi(this);

    Top = new QWidget;
    Top->setStyleSheet("QLabel { background-color : black; color : white; }");



    // This layout will space out the elements above
    QHBoxLayout *top_bar = new QHBoxLayout(Top);

    toplabels = new QLabel*[3];

    // This line above is important, it is an array of QLabel POINTERS.
    // You have to input heap memory pointers for the memory management system to work properly.
    scorelabel = new QLabel*[1];



    // Set the labels to blank black cards on top with the Top 3 card label on right

    for(size_t i=0;i<4;i++) {
        toplabels[i] = new QLabel;

        toplabels[i]->setMinimumSize(40,40);
        toplabels[i]->setMaximumSize(80,80);
        toplabels[i]->setScaledContents(true);
        if (i == 3){
           toplabels[i]->setStyleSheet("QLabel { background-color : black; color : white; }");
           toplabels[i]->setText("Top 3 Cards");
        }
        toplabels[i]->setAlignment(Qt::AlignCenter);
        top_bar->addWidget(toplabels[i]);
    }



    // Add in some horizontal space before next labels
//    QSpacerItem* horizontal_space = new QSpacerItem(100,10);
    QSpacerItem* horizontal_space = new QSpacerItem(10,10);

    top_bar->addSpacerItem(horizontal_space);



    // M M M     Multiples of #
    QLabel* game_type_text = new QLabel("Your Score ");
  //  game_type_value = new QLabel(QString::number(counter)); // start with multiples of 2.

    top_bar->addWidget(game_type_text);
   // top_bar->addWidget(game_type_value);

    // M M M     Multiples of #      Level: #
    QLabel* level_text = new QLabel("Computer Score");


    QSpacerItem* horizontal_space_2 = new QSpacerItem(*horizontal_space);
    // Add in some horizontal space before next labels
    top_bar->addSpacerItem(horizontal_space_2);

 //   adds widgets to top ba for score
    for(size_t i=0;i<2;i++) {
        scorelabel[i] = new QLabel;
        if (i == 0){
            scorelabel[i]= new QLabel(QString::number(comcounter));
            top_bar->addWidget(scorelabel[i]);
            top_bar->addWidget(level_text);
        }
        if (i == 1){
            scorelabel[i]= new QLabel(QString::number(counter));
            top_bar->addWidget(scorelabel[i]);
            top_bar->addWidget(game_type_text);
        }
    }



    // This code Creates the Board
    Board = new QWidget;
    labels = new QLabel*[board_size*board_size];
    values = new int[board_size*board_size];
    QGridLayout *SquareGrid = new QGridLayout(Board);

    SquareGrid->setGeometry(QRect());
    //SquareGrid->setSpacing(0);


    // Create each label for the board, initialize with random value.
    for(size_t i=0;i<board_size;i++) {
        for(size_t j=0;j<board_size;j++) {


            // Random values
            values[i*board_size+j] = distribution(generator);

            // Create label and set properties.
            if(i == j){
            labels[i*board_size+j] = new QLabel;
            if (j == 1  || j == 2){
            //labels[i*board_size+j]->setText(QString::number( values[i*board_size+j]));
            }
            labels[i*board_size+j]->setMinimumSize(40,40);
            labels[i*board_size+j]->setMaximumSize(40,40);
            labels[i*board_size+j]->setStyleSheet("QLabel { background-color : black; color : white; }");
            labels[i*board_size+j]->setFrameStyle(3);
            labels[i*board_size+j]->setAlignment(Qt::AlignCenter);
            // Add label to the layout
            SquareGrid -> addWidget(labels[i*board_size+j] ,i,j);
            }
        }
    }

    Board->setFixedSize(400,400);


    //    Top
    // --------
    //   Board

    // Do not allow board to be resized, otherwise spacings get messed up
      setFixedSize(500,550);

      // Create a vertical box layout for the two pieces
      QVBoxLayout *piece_together = new QVBoxLayout;

      piece_together->addWidget(Top,0,Qt::AlignCenter);
      piece_together->addWidget(Board,0,Qt::AlignCenter);

      QPushButton* quit = new QPushButton("Quit");
      QObject::connect(quit, SIGNAL(clicked()), parent, SLOT(game_over3()));

      piece_together->addWidget(quit);

      this->setLayout(piece_together);
      this->setStyleSheet("QLabel { background-color : black; color : white; }");


      connect(this, SIGNAL(game_over()), parent, SLOT(game_over()));
      connect(this, SIGNAL(game_over2()), parent, SLOT(game_over2()));
      QTimer *Timer = new QTimer(this);
      //timer->setSingleShot(true);
      Timer->start(500000);
      vector.push_back(Timer);

}

GameBoard::~GameBoard()
{
    // According to
    delete [] values;

    delete ui;
}


void GameBoard::keyPressEvent(QKeyEvent *event){
    std::cout << "This is current vector";
    for (int i = 0; i < vector.length(); i++){
        std::cout << vector[i]->remainingTime() << ",";
    }
    std::cout << std::endl;




    switch (event->key()) {
    case Qt::Key_Left:{
        if(vector.size() > 1 && vector.last()->remainingTime() > 5000){
        std::cout << "This is vectorcounter pre stop: " << vectorcounter << std::endl;
         this->processOneThing();
        if (vector[vectorcounter-1]->isActive())
            vector[vectorcounter-1]->stop();
            this->processOneThing();
            for (int i = 0; i < vector.length()-2; i++){
                if (vector[i]->isActive())
                    vector[i]->stop();
            }
        }
        std::cout << "This is vectorcounter post stop: " << vectorcounter << std::endl;
        /*QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(processOneThing()));
        timer->start(100);
        */

        //CURRENT "WORKING"
        //if (labels[5]->text() == labels[10]->text() || toplabels[0]->text() == toplabels[1]->text())
        //QTimer::singleShot(3000, this, SLOT(processOneThing()));

        if (labels[5]->text() == labels[10]->text() || toplabels[0]->text() == toplabels[1]->text()){
        QTimer *Timer = new QTimer(this);
        //timer->setSingleShot(true);
        vector.push_back(Timer);
         // Timer->setSineShot(true);
        //int a = vectorcounter;
        Timer->start(reaction);
        connect(Timer, SIGNAL(timeout()), this, SLOT(processOneThing()));
        vectorcounter++;
        //delete Timer;
        }

        /*if (timer->isActive() == true){
            int counter = 123;
            std::cout << counter;
            scorelabel[1]->setText(QString::number(counter));
            repaint();
        }
        */

        int num;
        int numcol;
        int numcomb;
        bool yay = false;
        while (yay == false && check.size() > 1){
        num = (distribution(generator)-1);
        numcol = qrand()%5;
        numcomb = numcol*10 + num;

        //for top labels adds current color card to top
        if (temp.size() == 3){
            toplabels[1]->setText(QString::number(temp.at(2)%10));
            toplabels[2]->setText(QString::number(temp.at(1)%10));
            if ((temp.at(2)-temp.at(2)%10)/10 == 0)
                toplabels[1]->setStyleSheet("background-color:blue;");
            if ((temp.at(2)-temp.at(2)%10)/10 == 1)
                toplabels[1]->setStyleSheet("background-color:green;");
            if ((temp.at(2)-temp.at(2)%10)/10 == 2)
                toplabels[1]->setStyleSheet("background-color:red;");
            if ((temp.at(2)-temp.at(2)%10)/10 == 3)
                toplabels[1]->setStyleSheet("background-color:brown;");
            if ((temp.at(2)-temp.at(2)%10)/10 == 4)
                toplabels[1]->setStyleSheet("background-color:orange;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 0)
                toplabels[2]->setStyleSheet("background-color:blue;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 1)
                toplabels[2]->setStyleSheet("background-color:green;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 2)
                toplabels[2]->setStyleSheet("background-color:red;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 3)
                toplabels[2]->setStyleSheet("background-color:brown;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 4)
                toplabels[2]->setStyleSheet("background-color:orange;");
             temp.remove(0);
        }
     //for top labels when only 2 cards on top adds current color card to top
        if (temp.size() == 2){
            toplabels[1]->setText(QString::number(temp.at(1)%10));
            toplabels[2]->setText(QString::number(temp.at(0)%10));
            if ((temp.at(1)-temp.at(1)%10)/10 == 0)
                toplabels[1]->setStyleSheet("background-color:blue;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 1)
                toplabels[1]->setStyleSheet("background-color:green;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 2)
                toplabels[1]->setStyleSheet("background-color:red;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 3)
                toplabels[1]->setStyleSheet("background-color:brown;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 4)
                toplabels[1]->setStyleSheet("background-color:orange;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 0)
                toplabels[2]->setStyleSheet("background-color:blue;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 1)
                toplabels[2]->setStyleSheet("background-color:green;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 2)
                toplabels[2]->setStyleSheet("background-color:red;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 3)
                toplabels[2]->setStyleSheet("background-color:brown;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 4)
                toplabels[2]->setStyleSheet("background-color:orange;");
             temp.remove(0);
        }

        //for top labels when only 1 card on top adds current color card to top
        if (temp.size() == 1){
            toplabels[1]->setText(QString::number(temp.at(0)%10));
            if ((temp.at(0)-temp.at(0)%10)/10 == 0)
                toplabels[1]->setStyleSheet("background-color:blue;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 1)
                toplabels[1]->setStyleSheet("background-color:green;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 2)
                toplabels[1]->setStyleSheet("background-color:red;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 3)
                toplabels[1]->setStyleSheet("background-color:brown;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 4)
                toplabels[1]->setStyleSheet("background-color:orange;");
        }

        //removes numcomb from check set if that card still exists
        if (check.remove(numcomb) == true){
            temp.append(numcomb);
            check.insert(numcomb);
            labels[5]->setText(QString::number(num));
            toplabels[0]->setText(QString::number(num));
            if (numcol == 0){
                labels[5]->setStyleSheet("background-color:blue;");
                toplabels[0]->setStyleSheet("background-color:blue;");
            }
            if (numcol == 1){
                labels[5]->setStyleSheet("background-color:green;");
                toplabels[0]->setStyleSheet("background-color:green;");
            }
            if (numcol == 2){
                labels[5]->setStyleSheet("background-color:red;");
                toplabels[0]->setStyleSheet("background-color:red;");
            }
            if (numcol == 3){
                labels[5]->setStyleSheet("background-color:brown;");
                toplabels[0]->setStyleSheet("background-color:brown;");
            }
            if (numcol == 4){
                labels[5]->setStyleSheet("background-color:orange;");
                toplabels[0]->setStyleSheet("background-color:orange;");
            }
            yay = true;
        }
        }
        if (comcounter == 5){
          this->game_over();
        }
        if (counter == 5){
          this->game_over2();
        }

        toplabels[2]->setStyleSheet("background-color:white;");

        break;



    }

    case Qt::Key_Right:{
        if(vector.size() > 1 && vector.last()->remainingTime() > 5000){
            std::cout << "This is vectorcounter pre stop: " << vectorcounter << std::endl;
            this->processOneThing();
        if (vector[vectorcounter-1]->isActive()){
            vector[vectorcounter-1]->stop();
            this->processOneThing();
            for (int i = 0; i < vector.length()-2; i++){
            if (vector[i]->isActive())
                vector[i]->stop();
            }
        }
        std::cout << "This is vectorcounter post stop: " << vectorcounter << std::endl;
        }
        //if (toplabels[0]->text() == toplabels[1]->text() && toplabels[0]->text() != 00)
         //   break;
        /*if (labels[5]->text() == labels[10]->text() || toplabels[0]->text() == toplabels[1]->text())
        QTimer::singleShot(5000, this, SLOT(()));
        */
        if (labels[5]->text() == labels[10]->text() || toplabels[0]->text() == toplabels[1]->text()){
        QTimer *Timer = new QTimer(this);
        vector.push_back(Timer);
         // Timer->setSineShot(true);
        std::cout << "Vector[vectorcounter: " << vectorcounter << std::endl;
        //int a = vectorcounter;
        Timer->start(reaction);

        connect(Timer, SIGNAL(timeout()), this, SLOT(processOneThing()));\
        vectorcounter++;
        }



        int num;
        int numcol;
        int numcomb;
        bool yay = false;
        while (yay == false && check.size() > 1){
        num = (distribution(generator));
        numcol = qrand()%5;
        numcomb = numcol*10 + num;
        //for top labels when only 3 card on top adds current color card to top

        if (temp.size() == 3){
            toplabels[1]->setText(QString::number(temp.at(2)%10));
            toplabels[2]->setText(QString::number(temp.at(1)%10));
            if ((temp.at(2)-temp.at(2)%10)/10 == 0)
                toplabels[1]->setStyleSheet("background-color:blue;");
            if ((temp.at(2)-temp.at(2)%10)/10 == 1)
                toplabels[1]->setStyleSheet("background-color:green;");
            if ((temp.at(2)-temp.at(2)%10)/10 == 2)
                toplabels[1]->setStyleSheet("background-color:red;");
            if ((temp.at(2)-temp.at(2)%10)/10 == 3)
                toplabels[1]->setStyleSheet("background-color:brown;");
            if ((temp.at(2)-temp.at(2)%10)/10 == 4)
                toplabels[1]->setStyleSheet("background-color:orange;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 0)
                toplabels[2]->setStyleSheet("background-color:blue;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 1)
                toplabels[2]->setStyleSheet("background-color:green;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 2)
                toplabels[2]->setStyleSheet("background-color:red;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 3)
                toplabels[2]->setStyleSheet("background-color:brown;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 4)
                toplabels[2]->setStyleSheet("background-color:orange;");
             temp.remove(0);
        }
        //for top labels when only 2 card on top adds current color card to top

        if (temp.size() == 2){
            toplabels[1]->setText(QString::number(temp.at(1)%10));
            toplabels[2]->setText(QString::number(temp.at(0)%10));
            if ((temp.at(1)-temp.at(1)%10)/10 == 0)
                toplabels[1]->setStyleSheet("background-color:blue;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 1)
                toplabels[1]->setStyleSheet("background-color:green;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 2)
                toplabels[1]->setStyleSheet("background-color:red;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 3)
                toplabels[1]->setStyleSheet("background-color:brown;");
            if ((temp.at(1)-temp.at(1)%10)/10 == 4)
                toplabels[1]->setStyleSheet("background-color:orange;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 0)
                toplabels[2]->setStyleSheet("background-color:blue;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 1)
                toplabels[2]->setStyleSheet("background-color:green;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 2)
                toplabels[2]->setStyleSheet("background-color:red;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 3)
                toplabels[2]->setStyleSheet("background-color:brown;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 4)
                toplabels[2]->setStyleSheet("background-color:orange;");
             temp.remove(0);
        }
        //for top labels when only 1 card on top adds current color card to top

        if (temp.size() == 1){
            toplabels[1]->setText(QString::number(temp.at(0)%10));
            if ((temp.at(0)-temp.at(0)%10)/10 == 0)
                toplabels[1]->setStyleSheet("background-color:blue;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 1)
                toplabels[1]->setStyleSheet("background-color:green;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 2)
                toplabels[1]->setStyleSheet("background-color:red;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 3)
                toplabels[1]->setStyleSheet("background-color:brown;");
            if ((temp.at(0)-temp.at(0)%10)/10 == 4)
                toplabels[1]->setStyleSheet("background-color:orange;");
        }

        //removes numcomb from check set if that card still exists

        if (check.remove(numcomb) == true){
            temp.append(numcomb);
            check.insert(numcomb);
            labels[10]->setText(QString::number(num));
            toplabels[0]->setText(QString::number(num));
            if (numcol == 0){
                labels[10]->setStyleSheet("background-color:blue;");
                toplabels[0]->setStyleSheet("background-color:blue;");
            }
            if (numcol == 1){
                labels[10]->setStyleSheet("background-color:green;");
                toplabels[0]->setStyleSheet("background-color:green;");
            }
            if (numcol == 2){
                labels[10]->setStyleSheet("background-color:red;");
                toplabels[0]->setStyleSheet("background-color:red;");
            }
            if (numcol == 3){
                labels[10]->setStyleSheet("background-color:brown;");
                toplabels[0]->setStyleSheet("background-color:brown;");
            }
            if (numcol == 4){
                labels[10]->setStyleSheet("background-color:orange;");
                toplabels[0]->setStyleSheet("background-color:orange;");
            }
            yay = true;
        }
        }
        if (comcounter == 5){
          this->game_over();
        }
        if (counter == 5){
          this->game_over2();
        }

        toplabels[2]->setStyleSheet("background-color:white;");

        break;



    }
    case Qt::Key_Up:

        break;
    case Qt::Key_Down:

        break;
    //slap checks for first and second card
    case Qt::Key_Space:
        if (labels[5]->text() == 00 && labels[10]->text() == 00 )
            break;
        if (temp.size() == 2){
        munchNumber(temp.at(1)%10,temp.at(0)%10);
        }
        if (temp.size() == 3)
        munchNumber(temp.at(0)%10,temp.at(2)%10);
        if (comcounter == 5){
          this->game_over();
        }
        if (counter == 5){
          this->game_over2();
        }


        break;
    default:
        QWidget::keyPressEvent(event);
    }

    // Need both lines to force a repaint.

    // This line forces processor to process all previously promised events.
    QCoreApplication::processEvents();

    // This one QUEUES up a repaint
    repaint();


    return;
}

/*void GameBoard::paintEvent(QPaintEvent *e) {

}
*/

void GameBoard::showEvent(QShowEvent *e) {

    this->activateWindow();
    this->setFocus();
    QWidget::showEvent(e);
}

//if valid Slap return true and update cards
bool GameBoard::munchNumber(int x, int y) {
    if(isValidMunch(x,y)) {
        updateAfterMunch(true);
            return true;
    }
    else {
        updateAfterMunch(false);
            return false;
    }
    return true;
}

//checks for valid slap
bool GameBoard::isValidMunch(int x, int y) {
    if(x==y){
        counter++;
        scorelabel[1]->setText(QString::number(counter));
        this->updateAfterMunch(true);
        return true;
    }
    comcounter++;
    scorelabel[0]->setText(QString::number(comcounter));
    this->updateAfterMunch(true);
    return false;
}

//updates board affter valid slap
void GameBoard::updateAfterMunch(bool flag) {
    if(flag){
        labels[10]->setStyleSheet("QLabel { color : black;}");
        labels[5]->setStyleSheet("QLabel {color : black;}");
        toplabels[0]->setStyleSheet("QLabel {color : black;}");
        toplabels[1]->setStyleSheet("QLabel {color : black;}");
        toplabels[2]->setStyleSheet("QLabel {color : black;}");
        labels[10]->setText(00);
        labels[5]->setText(00);
        temp.clear();

    }
    else {
    }

}

void GameBoard::processOneThing(){
    if (temp.size() == 2){
        if (temp.at(1)%10 == temp.at(0)%10){
             comcounter++;
             scorelabel[0]->setText(QString::number(comcounter));
              this->updateAfterMunch(true);
             //Timer->stop();
            //munchNumber(temp.at(1)%10,temp.at(0)%10);
        }
    }
    if (temp.size() == 3){
        if (temp.at(0)%10 == temp.at(2)%10){
             comcounter++;
             scorelabel[0]->setText(QString::number(comcounter));
             this->updateAfterMunch(true);
             //if (Timer->isActive())
             //Timer->stop();
            //munchNumber(temp.at(0)%10,temp.at(2)%10);
        }
    }
}
