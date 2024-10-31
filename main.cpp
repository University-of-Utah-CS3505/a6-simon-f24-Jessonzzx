/**
 * Name: Zhengxi Zhang, Yinhao Chen
 * Course: CS 3505 Fall2024
 * Assignment Name: A6: Qt Simon Game
 * Descrption: In this simon project, we add the new features of adding a turret to fire to the buttons,
 * which can be more fun to play with.
 *
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
