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
    // Signals no need comments as long as they are named descriptively.
    void displaySequence(int color, int delay);
    void updateProgress(int progress);
    void gameOver(bool won);
    void scoreChanged(int newScore);
    void enablePlayerButtons(bool enable);
    void resetButtonColors();

private:
    QVector<int> colorSequence;
    int currentMove;
    int delay;
    int score;

    void generateNextMove();
    void playSequence();
    void incrementScore();
    void flashNextButton(int index);
};

#endif // MODEL_H
