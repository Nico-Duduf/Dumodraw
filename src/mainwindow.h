#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QMouseEvent>

#include "cell.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
private slots:
    void on_actionPaint_triggered();
    void on_actionErase_triggered();
    void on_actionSelect_triggered();
    void on_actionDeselect_triggered();

private:
    bool painting;
    Cell *currentCell;
    int currentTool;
    void clickCell(QPoint pos);
    void swapTool();
    void createCells(int numRows, int numColumns);
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
