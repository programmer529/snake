#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Настройка окна
    this->setFixedSize(width * gridSize, height * gridSize);
    setWindowTitle("Змейка");

    // Создание таймера
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::gameLoop);
    initGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (gameOver) return;

    switch (event->key()) {
    case Qt::Key_Up:
        if(snakeDir.y() != 1) snakeDir = QPoint(0, -1);
        break;
    case Qt::Key_Down:
        if(snakeDir.y() != -1) snakeDir = QPoint(0, 1);
        break;
    case Qt::Key_Left:
        if(snakeDir.x() != 1) snakeDir = QPoint(-1, 0);
        break;
    case Qt::Key_Right:
        if(snakeDir.x() != -1) snakeDir = QPoint(1, 0);
        break;
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawGame(&painter);
}

void MainWindow::gameLoop()
{
    if (gameOver) {
        gameTimer->stop();
        return;
    }

    moveSnake();
    checkCollision();
    update(); // Перерисовать окно
}

void MainWindow::initGame()
{
    snake.clear();
    snake.push_back(QPoint(width / 2, height / 2));
    snakeDir = QPoint(1, 0); // Начальное направление вправо
    generateFood();
    gameOver = false;

    gameTimer->start(speed);
}

void MainWindow::moveSnake()
{
    QPoint head = snake.first();
    QPoint newHead = head + snakeDir;


    snake.insert(0, newHead); // Добавляем голову в начало змеи

    // Если змея съела еду
    if (newHead == foodPos) {
        generateFood();
    } else {
        snake.removeLast(); // Удаляем последний сегмент, если не съели еду
    }
}

void MainWindow::checkCollision()
{
    if(checkSelfCollision() || checkWallCollision()){
        gameOver = true;
    }
}


void MainWindow::generateFood()
{
    do {
        foodPos.setX(QRandomGenerator::global()->bounded(0, width));
        foodPos.setY(QRandomGenerator::global()->bounded(0, height));
    } while (std::find(snake.begin(), snake.end(), foodPos) != snake.end()); // Убеждаемся, что еда не в змее
}

bool MainWindow::checkSelfCollision() {
    QPoint head = snake.first();
    for(int i = 1; i < snake.size(); i++){
        if (snake[i] == head) {
            return true;
        }
    }
    return false;
}

bool MainWindow::checkWallCollision() {
    QPoint head = snake.first();

    if (head.x() < 0 || head.x() >= width || head.y() < 0 || head.y() >= height) {
        return true; // Столкновение со стеной
    }
    return false;

}


void MainWindow::drawGame(QPainter *painter)
{
    // Заливка фона
    painter->fillRect(rect(), QColor(25, 25, 25));
    // Рисуем змейку
    painter->setBrush(QColor(255, 255, 255));
    for (const QPoint &point : snake) {
        painter->drawRect(point.x() * gridSize, point.y() * gridSize, gridSize, gridSize);
    }

    // Рисуем еду
    painter->setBrush(QColor(255, 0, 0));
    painter->drawRect(foodPos.x() * gridSize, foodPos.y() * gridSize, gridSize, gridSize);

    if(gameOver) {
        painter->setPen(QColor(255,255,255));
        painter->drawText(rect(), Qt::AlignCenter, "Game Over!");
    }
}
