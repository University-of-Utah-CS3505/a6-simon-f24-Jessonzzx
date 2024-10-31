#include "mainwindow.h"
#include <QPropertyAnimation>
#include <QTimer>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(new Model(this))
{
    ui->setupUi(this);
    //connect the startButton
    this->setCursor(Qt::CrossCursor);
    enableStartButton(true);

    connect(ui->startButton, &QPushButton::clicked, this, [this]() {
        enableStartButton(false);
        fireCannonball(ui->startButton);
        model->startGame();
        resetUI();
    });

    // connect all the signals of model
    connect(model, &Model::displaySequence, this, &MainWindow::handleDisplaySequence);
    connect(model, &Model::updateProgress, this, &MainWindow::handleProgressUpdate);
    connect(model, &Model::gameOver, this, &MainWindow::handleGameOver);
    connect(model, &Model::scoreChanged, this, &MainWindow::updateScore);
    connect(model, &Model::enablePlayerButtons, this, &MainWindow::enablePlayerButtons);
    connect(model, &Model::resetButtonColors, this, &MainWindow::resetButtonColors);

    // These are the functions to connect the Cannonball to the correct button and trigger them.
    connect(ui->redButton, &QPushButton::clicked, this, [this]() {
        fireCannonball(ui->redButton); // Fire at red button
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
    enableStartButton(false);
    ui->messageLabel->clear();
    enablePlayerButtons(false);
    ui->scoreLabel->setText("Score: 0");
}

void MainWindow::enablePlayerButtons(bool enable)
{
    ui->redButton->setEnabled(enable);
    ui->blueButton->setEnabled(enable);
}
void MainWindow::enableStartButton(bool enable)
{
    ui->startButton->setEnabled(enable);
    if (enable) {
        ui->startButton->setStyleSheet("background-color: green;");
    } else {
        ui->startButton->setStyleSheet("");
    }
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

void MainWindow::handleGameOver(bool over)
{
    enableStartButton(true);
    ui->messageLabel->setAlignment(Qt::AlignCenter);
    if (over) {
        ui->messageLabel->setText("You Lose!");
        ui->messageLabel->setStyleSheet(
            "QLabel { color : red; font-size: 30px; font-weight: bold; }");
    }
    enablePlayerButtons(false);
}

void MainWindow::updateScore(int newScore)
{
    ui->scoreLabel->setText("Score: " + QString::number(newScore));//
}

void MainWindow::fireCannonball(QPushButton *targetButton)
{
    int turretCenterX = 270 + (171 / 2);
    int turretCenterY = 280 + (181 / 2);

    QRect targetGeometry = targetButton->geometry();
    int targetCenterX = targetGeometry.x() + (targetGeometry.width() / 2);
    int targetCenterY = targetGeometry.y() + (targetGeometry.height() / 2);

    double angle = (qAtan2(targetCenterY - turretCenterY, targetCenterX - turretCenterX) * 180 / M_PI) + 90;

    // Rotate the turret when Fire.
    QPixmap originalPixmap(":/turret.png");
    QTransform transform;
    transform.rotate(angle);
    QPixmap rotatedPixmap = originalPixmap.transformed(transform, Qt::SmoothTransformation);
    ui->turret->setPixmap(rotatedPixmap);

    QLabel *cannonball = new QLabel(this);
    cannonball->setGeometry(turretCenterX, turretCenterY, 20, 20);
    cannonball->setStyleSheet("background-color: black; border-radius: 10px;");
    cannonball->show();
    // Blast effect.
    QLabel *blast = new QLabel(this);
    blast->setPixmap(QPixmap(":/blast.png"));
    blast->setGeometry(targetCenterX - 20, targetCenterY - 20, 40, 40);  // Center the blast on the target
    blast->setAlignment(Qt::AlignCenter);
    blast->hide();

    QPropertyAnimation *animation = new QPropertyAnimation(cannonball, "geometry");
    animation->setDuration(500);
    animation->setStartValue(QRect(turretCenterX, turretCenterY, 20, 20));
    animation->setEndValue(QRect(targetCenterX - 10, targetCenterY - 10, 20, 20));

    connect(animation, &QPropertyAnimation::finished, this, [cannonball, blast, this]() {
        delete cannonball;
        blast->show();
        QTimer::singleShot(200, this, [blast]() {
            delete blast;
        });
    });

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::resetButtonColors()
{
    ui->redButton->setStyleSheet("background-color: red;");
    ui->blueButton->setStyleSheet("background-color: blue;");
}
