#include "Model.h"
#include <QRandomGenerator>
#include <QTimer>

Model::Model(QObject *parent)
    : QObject(parent)
    , currentColor(0)
    , delay(1000)
    , score(0)
{}

void Model::startGame()
{
    colorSequence.clear();
    currentColor = 0;
    delay = 1000;
    score = 0;
    emit scoreChanged(score); // Update score to 0 at the beginning of the game
    generateNextColor();
    playSequence();
}

void Model::generateNextColor()
{
    int nextColor = QRandomGenerator::global()->bounded(2);
    colorSequence.append(nextColor);
}

void Model::playSequence()
{
    currentColor = 0;
    emit enablePlayerButtons(false); // Disable buttons during computer's turn
    int roundDelay = 1000;

    QTimer::singleShot(roundDelay, this, [this]() {
        flashNextButton(0);  // Start flashing the buttons
    });

    delay = qMax(120, delay - 50); // Control the speed of flash
}

void Model::flashNextButton(int index)
{
    if (index >= colorSequence.size()) {
        // When all flashing is done, enable buttons for player's turn
        emit updateProgress(0);
        emit scoreChanged(score);
        emit enablePlayerButtons(true);
        return;
    }

    emit resetButtonColors();
    QTimer::singleShot(100, this, [this, index]() {
        // Flash the current button in the sequence
        emit displaySequence(colorSequence[index], delay);

        QTimer::singleShot(delay, this, [this, index]() {
            flashNextButton(index + 1);
        });
    });
}

void Model::playerProgress(int color)
{
    if (colorSequence[currentColor] == color) {
        currentColor++;
        emit updateProgress((currentColor * 100) / colorSequence.size());

        if (currentColor == colorSequence.size()) {
            incrementScore();
            generateNextColor();
            QTimer::singleShot(500, this, &Model::playSequence);
        }
    } else {
        emit gameOver(true);
    }
}

void Model::incrementScore()
{
    score += 10;
    emit scoreChanged(score);
}
