#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    void startGame();
    void playerProgress(int color);
    void resetGame();

signals:
    /// @brief Signal that display the colors Sequence
    /// @param color
    /// @param delay
    void displaySequence(int color, int delay);
    /// @brief Signal that can update the progress bar
    /// @param progress
    void updateProgress(int progress);
    /// @brief Signal to over the game and show result
    /// @param won
    void gameOver(bool won);
    /// @brief Signal to change the score
    /// @param newScore
    void scoreChanged(int newScore);
    /// @brief Signal to enable the red and blue buttons
    /// @param enable
    void enablePlayerButtons(bool enable);

private:
    QVector<int> sequence;
    int currentMove;
    int delay;
    int score;

    void generateNextMove();
    void playSequence();
    void incrementScore();
};

#endif // MODEL_H
