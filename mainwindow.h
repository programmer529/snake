#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void gameLoop();
private:
    Ui::MainWindow *ui;

    // Параметры игры
    int gridSize = 20; // Размер одной клетки
    int width = 20; // Количество клеток по ширине
    int height = 20; // Количество клеток по высоте
    int speed = 150; // Интервал таймера

    // Змейка
    QVector<QPoint> snake;
    QPoint snakeDir; // Направление движения змейки
    QPoint foodPos; // Позиция еды
    bool gameOver = false;

    // Таймер
    QTimer *gameTimer;

    // Методы игры
    void initGame();
    void moveSnake();
    void checkCollision();
    void generateFood();
    bool checkSelfCollision();
    void drawGame(QPainter *painter);
};
#endif // MAINWINDOW_H
