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
    void playerProgress(int color); // Processes the player's progress by checking if their input matches the current sequence.
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
    int currentColor;
    int delay;
    int score;

    void generateNextColor(); //  Generates the next random color to be added to the sequence.
    void playSequence();
    void incrementScore();
    void flashNextButton(int index);
};

#endif // MODEL_H
