#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(new Model(this))
{
    ui->setupUi(this);
    //connect the startButton
    this->setCursor(Qt::CrossCursor);

    connect(ui->startButton, &QPushButton::clicked, this, [this]() {
        model->startGame();
        resetUI();
    });

    // connect all the signals of model
    connect(model, &Model::displaySequence, this, &MainWindow::handleDisplaySequence);
    connect(model, &Model::updateProgress, this, &MainWindow::handleProgressUpdate);
    connect(model, &Model::gameOver, this, &MainWindow::handleGameOver);
    connect(model, &Model::scoreChanged, this, &MainWindow::updateScore);
    connect(model, &Model::enablePlayerButtons, this, &MainWindow::enablePlayerButtons);

    // These are the functions to connect the Cannonball to the correct button and trigger them.
    connect(ui->startButton, &QPushButton::clicked, this, [this]() {
        fireCannonball(ui->startButton); // Fire at start
        model->startGame();
    });
    connect(ui->redButton, &QPushButton::clicked, this, [this]() {
        fireCannonball(ui->redButton);  // Fire at red button
        model->playerProgress(0);
    });

    connect(ui->blueButton, &QPushButton::clicked, this, [this]() {
        fireCannonball(ui->blueButton); // Fire at Blue button
        model->playerProgress(1);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetUI()
{
    ui->progressBar->setValue(0);
    ui->startButton->setEnabled(false);
    ui->messageLabel->clear();
    ui->redButton->setEnabled(false);
    ui->blueButton->setEnabled(false);
    ui->scoreLabel->setText("Score: 0");
}

void MainWindow::enablePlayerButtons(bool enable)
{
    ui->redButton->setEnabled(enable);
    ui->blueButton->setEnabled(enable);
}

void MainWindow::handleDisplaySequence(int color, int delay)
{
    QPushButton *button = (color == 0) ? ui->redButton : ui->blueButton;

    // Flash the button
    button->setStyleSheet("background-color: yellow;");
    QTimer::singleShot(delay, this, [button, color]() {
        button->setStyleSheet(color == 0 ? "background-color: red;" : "background-color: blue;");
    });
}

void MainWindow::handleProgressUpdate(int progress)
{
    ui->progressBar->setValue(progress);
}

void MainWindow::handleGameOver(bool won)
{
    ui->startButton->setEnabled(true);
    ui->messageLabel->setAlignment(Qt::AlignCenter);
    if (won) {
        ui->messageLabel->setText("You Win!");
        ui->messageLabel->setStyleSheet("QLabel { c`olor : green; font-size: 15px; font-weight: bold; }");
    } else {
        ui->messageLabel->setText("You Lose!");
        ui->messageLabel->setStyleSheet("QLabel { color : red; font-size: 15px; font-weight: bold; }");
    }
    ui->redButton->setEnabled(false);
    ui->blueButton->setEnabled(false);
}

void MainWindow::updateScore(int newScore)
{
    ui->scoreLabel->setText("Score: " + QString::number(newScore));
}

void MainWindow::fireCannonball(QPushButton *targetButton) {
    enablePlayerButtons(false);

    // Get the center position of the turret
    int turretCenterX = 270 + (171 / 2);
    int turretCenterY = 280 + (181 / 2);

    // Get the center position of the target button
    QRect targetGeometry = targetButton->geometry();
    int targetCenterX = targetGeometry.x() + (targetGeometry.width() / 2);
    int targetCenterY = targetGeometry.y() + (targetGeometry.height() / 2);

    double angle = (qAtan2(targetCenterY - turretCenterY, targetCenterX - turretCenterX) * 180 / M_PI)+90;

    // Apply rotation to the turret QLabel using QTransform
    QPixmap originalPixmap(":/turret.png");
    QTransform transform;
    transform.rotate(angle);
    QPixmap rotatedPixmap = originalPixmap.transformed(transform, Qt::SmoothTransformation);
    ui->turret->setPixmap(rotatedPixmap);

    // Create a cannonball QLabel and will be fire at the turret’s center
    QLabel *cannonball = new QLabel(this);
    cannonball->setGeometry(turretCenterX, turretCenterY, 20, 20);
    cannonball->setStyleSheet("background-color: black; border-radius: 10px;");
    cannonball->show();

    // Animate the cannonball towards the target button
    QPropertyAnimation *animation = new QPropertyAnimation(cannonball, "geometry");
    animation->setDuration(500);  // Time to reach the target
    animation->setStartValue(QRect(turretCenterX, turretCenterY, 20, 20));
    animation->setEndValue(QRect(targetCenterX - 10, targetCenterY - 10, 20, 20));

    // Handle the end of the animation
    connect(animation, &QPropertyAnimation::finished, this, [this, cannonball, targetButton]() {
        delete cannonball;
        targetButton->click();
        enablePlayerButtons(true);
    });

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
