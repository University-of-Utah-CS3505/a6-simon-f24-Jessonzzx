#include "Model.h"
#include <QRandomGenerator>
#include <QTimer>

Model::Model(QObject *parent)
    : QObject(parent), currentMove(0), delay(1000), score(0) {

}

void Model::startGame()
{
    sequence.clear();
    currentMove = 0;
    delay = 1000; // Initial delay
    score = 0;
    emit scoreChanged(score); // Update score to 0 at the beginning of the game
    generateNextMove();
    playSequence();
}

void Model::generateNextMove()
{
    int nextMove = QRandomGenerator::global()->bounded(2);
    sequence.append(nextMove);
}

void Model::playSequence()
{
    currentMove = 0;

    // Schedule each button flash sequentially
    for (int i = 0; i < sequence.size(); ++i) {
        QTimer::singleShot(i * delay, this, [this, i]() {
            emit displaySequence(sequence[i], delay);
        });
    }

    // After the flash, enable the button to let user click
    QTimer::singleShot(sequence.size() * delay, this, [this]() {
        emit updateProgress(0);
        emit scoreChanged(score);
        emit enablePlayerButtons(true);
    });

    delay = qMax(100, delay - 50);
}

void Model::playerProgress(int color)
{
    if (sequence[currentMove] == color) {
        currentMove++;
        emit updateProgress((currentMove * 100) / sequence.size());

        if (currentMove == sequence.size()) {
            incrementScore();
            generateNextMove();
            QTimer::singleShot(500, this, &Model::playSequence);
        }
    } else {
        emit gameOver(false);
    }
}

void Model::incrementScore()
{
    score += 10;
    emit scoreChanged(score);
}
