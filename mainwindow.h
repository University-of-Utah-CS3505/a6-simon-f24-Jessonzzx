#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QPropertyAnimation>
#include <QPushButton>
#include "Model.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /// @brief Slot to handle the display of the color sequence.
    /// @param color button color
    /// @param delay the delay of the next color display
    void handleDisplaySequence(int color, int delay);
    /// @brief Slot to update the progress bar.
    /// @param progress current progress in percentage
    void handleProgressUpdate(int progress);
    /// @brief Slot to handle game over logic and display result.
    /// @param won player win or not
    void handleGameOver(bool won);
    /// @brief Slot to update the player's score in the UI.
    /// @param newScore new score to display
    void updateScore(int newScore);

private:
    Ui::MainWindow *ui;
    Model *model;

    /// @brief Reset the user interface to its initial state.
    void resetUI();
    /// @brief Enable or disable the player buttons.
    /// @param enable
    void enablePlayerButtons(bool enable);
    /// @brief Fire a cannonball towards a target button.
    /// @param targetButton the target
    //void fireCannonball(QPushButton *targetButton);
};

#endif // MAINWINDOW_H
