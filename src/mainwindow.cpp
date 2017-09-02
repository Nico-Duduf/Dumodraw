#include "mainwindow.h"

#ifdef QT_DEBUG
#include <QtDebug>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    //INITIALIZE
    painting = false;
    currentTool = 1;

    //show grid
    mainLayout->setSpacing(1);

    //create default cells (seems to be a maximum at 3600 cells)
    createCells(15,15);

    //events
    mainWidget->installEventFilter(this);

}

void MainWindow::createCells(int numRows, int numColumns)
{
    //add cells
    QList<Cell*> rowBefore;
    for (int row = 0; row < numRows; row++)
    { 
        QList<Cell*> currentRow;
        for (int col = 0; col < numColumns; col++)
        {
            Cell *cell = new Cell(row,col);
            mainLayout->addWidget(cell,row,col);
            currentRow << cell;
            //connections
            if (col > 0)
            {
                connect(cell,SIGNAL(swapped(bool)),currentRow[col-1],SLOT(setRight(bool)));
                connect(currentRow[col-1],SIGNAL(swapped(bool)),cell,SLOT(setLeft(bool)));
            }
            if (row > 0)
            {
                connect(cell,SIGNAL(swapped(bool)),rowBefore[col],SLOT(setBottom(bool)));
                connect(rowBefore[col],SIGNAL(swapped(bool)),cell,SLOT(setTop(bool)));
            }

        }
        rowBefore = currentRow;
    }
}

void MainWindow::clickCell(QPoint pos)
{
    //Get the widget
    Cell *cell = qobject_cast<Cell*>(qApp->widgetAt(pos)->parent());
    if (!cell) return;
    if (currentCell != cell)
    {
        currentCell = cell;
        cell->swap(currentTool);
    }

}

void MainWindow::swapTool()
{
    if (currentTool == 1)
    {
        currentTool = 2;
        actionErase->setChecked(true);
        actionPaint->setChecked(false);
    }
    else if (currentTool == 2)
    {
        currentTool = 1;
        actionErase->setChecked(false);
        actionPaint->setChecked(true);
    }
}

//ACTIONS

void MainWindow::on_actionPaint_triggered(bool checked)
{
 if (checked)
 {
     actionErase->setChecked(false);
     actionSwap->setChecked(false);
 }
 else
 {
     actionPaint->setChecked(true);
 }
 currentTool = 1;
}

void MainWindow::on_actionErase_triggered(bool checked)
{
    if (checked)
    {
        actionPaint->setChecked(false);
        actionSwap->setChecked(false);
        currentTool = 2;
    }
    else
    {
        actionPaint->setChecked(true);
        currentTool = 1;
    }
}

void MainWindow::on_actionSwap_triggered(bool checked)
{
    if (checked)
    {
        actionErase->setChecked(false);
        actionPaint->setChecked(false);
        currentTool = 3;
    }
    else
    {
        actionPaint->setChecked(true);
        currentTool = 1;
    }
}

//EVENT FILTER

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseButtonPress)
  {
      QMouseEvent *mouseEvent = (QMouseEvent*)event;

      //revert tool if right click
      if (mouseEvent->button() == Qt::RightButton)
      {
          swapTool();
      }

      //send click to cell
      if (mouseEvent->button() == Qt::LeftButton || mouseEvent->button() == Qt::RightButton)
      {
        painting = true;
        clickCell(mouseEvent->globalPos());
        event->accept();
      }

      return true;
  }
  else if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *mouseEvent = (QMouseEvent*)event;
    if (mouseEvent->buttons())
    {
        //send click to cell
        if (painting) clickCell(mouseEvent->globalPos());

        event->accept();
    }
    return true;
  }
  else if (event->type() == QEvent::MouseButtonRelease)
  {
      QMouseEvent *mouseEvent = (QMouseEvent*)event;
      //restore original tool
      if (mouseEvent->button() == Qt::RightButton)
      {
          swapTool();
      }

      painting = false;
      currentCell = 0;
      return true;
  }
  else
  {
      // standard event processing
      return QObject::eventFilter(obj, event);
  }
}

