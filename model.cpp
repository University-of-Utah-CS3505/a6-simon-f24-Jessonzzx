#include "Model.h"
#include <QRandomGenerator>
#include <QTimer>

Model::Model(QObject *parent)
    : QObject(parent)
    , currentMove(0)
    , delay(1000)
    , score(0)
{}

void Model::startGame()
{
    colorSequence.clear();
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
    colorSequence.append(nextMove);
}

void Model::playSequence()
{
    currentMove = 0;
    emit enablePlayerButtons(false); // Disable buttons during computer's turn

    // Start flashing sequence with the first button
    flashNextButton(0);

    // Adjust delay for next round, if needed
    delay = qMax(100, delay - 50);
}

void Model::flashNextButton(int index)
{
    if (index >= colorSequence.size()) {
        // All flashing is done, now enable buttons for player's turn
        emit updateProgress(0);
        emit scoreChanged(score);
        emit enablePlayerButtons(true);
        return;
    }

    // Flash the current button
    emit displaySequence(colorSequence[index], delay);

    // Schedule the next flash after `delay` milliseconds
    QTimer::singleShot(delay, this, [this, index]() {
        flashNextButton(index + 1); // Recursive call to flash the next button
    });
}

void Model::incrementScore()
{
    score += 10;
    emit scoreChanged(score);
}
