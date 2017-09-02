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
        cell->click(currentTool);
        if (currentTool == 3 || currentTool == 5 || currentTool == 6)
        {
            selectedCells << cell;
        }
        else if (currentTool == 4)
        {
            selectedCells.removeAll(cell);
        }
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
    else if (currentTool == 3)
    {
        currentTool = 4;
        actionSelect->setChecked(false);
        actionDeselect->setChecked(true);
    }
    else if (currentTool == 4)
    {
        currentTool = 3;
        actionSelect->setChecked(true);
        actionDeselect->setChecked(false);
    }
    else if (currentTool == 5)
    {
        currentTool = 6;
        actionConnect->setChecked(false);
        actionDisconnect->setChecked(true);
    }
    else if (currentTool == 6)
    {
        currentTool = 5;
        actionConnect->setChecked(true);
        actionDisconnect->setChecked(false);
    }
}

void MainWindow::connectSelectedCells(bool c)
{
    //parcourir toutes les cellules sélectionnées
    foreach(Cell *cell,selectedCells)
    {
        //chercher les voisines sélectionnées
        //top
        if (cell->getRow() > 0)
        {
            Cell *top = qobject_cast<Cell*>(mainLayout->itemAtPosition(cell->getRow()-1,cell->getColumn())->widget());
            if (top->isSelected()) cell->setTop(c);
        }
        //right
        if (cell->getRow() < mainLayout->rowCount()-1)
        {
            Cell *right = qobject_cast<Cell*>(mainLayout->itemAtPosition(cell->getRow(),cell->getColumn()+1)->widget());
            if (right->isSelected()) cell->setRight(c);
        }
        //bottom
        if (cell->getColumn() < mainLayout->columnCount()-1)
        {
            Cell *bottom = qobject_cast<Cell*>(mainLayout->itemAtPosition(cell->getRow()+1,cell->getColumn())->widget());
            if (bottom->isSelected()) cell->setBottom(c);
        }
        //left
        if (cell->getRow() > 0)
        {
            Cell *left = qobject_cast<Cell*>(mainLayout->itemAtPosition(cell->getRow(),cell->getColumn()-1)->widget());
            if (left->isSelected()) cell->setLeft(c);
        }
    }

    selectAll(false);
}

void MainWindow::selectAll(bool c)
{
    selectedCells.clear();
    for (int i = 0 ; i < mainLayout->count() ; i++)
    {
        Cell *cell = qobject_cast<Cell*>(mainLayout->itemAt(i)->widget());
        cell->setSelected(c);
        if (c) selectedCells << cell;
    }
}

//ACTIONS

void MainWindow::on_actionPaint_triggered()
{
    actionPaint->setChecked(true);
    actionDeselect->setChecked(false);
    actionSelect->setChecked(false);
    actionErase->setChecked(false);
    actionDisconnect->setChecked(false);
    actionConnect->setChecked(false);
    currentTool = 1;
}

void MainWindow::on_actionErase_triggered()
{

    actionPaint->setChecked(false);
    actionDeselect->setChecked(false);
    actionSelect->setChecked(false);
    actionErase->setChecked(true);
    actionDisconnect->setChecked(false);
    actionConnect->setChecked(false);
    currentTool = 2;
}

void MainWindow::on_actionSelect_triggered()
{
    actionPaint->setChecked(false);
    actionDeselect->setChecked(false);
    actionSelect->setChecked(true);
    actionErase->setChecked(false);
    actionDisconnect->setChecked(false);
    actionConnect->setChecked(false);
    currentTool = 3;
}

void MainWindow::on_actionDeselect_triggered()
{
    actionPaint->setChecked(false);
    actionDeselect->setChecked(true);
    actionSelect->setChecked(false);
    actionErase->setChecked(false);
    actionDisconnect->setChecked(false);
    actionConnect->setChecked(false);
    currentTool = 4;
}

void MainWindow::on_actionDisconnect_triggered()
{
    actionPaint->setChecked(false);
    actionDeselect->setChecked(false);
    actionSelect->setChecked(false);
    actionErase->setChecked(false);
    actionDisconnect->setChecked(true);
    actionConnect->setChecked(false);
    currentTool = 6;
}

void MainWindow::on_actionConnect_triggered()
{
    actionPaint->setChecked(false);
    actionDeselect->setChecked(false);
    actionSelect->setChecked(false);
    actionErase->setChecked(false);
    actionDisconnect->setChecked(false);
    actionConnect->setChecked(true);
    currentTool = 5;
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

      //connect or disconnect
      if (currentTool == 5) connectSelectedCells(true);
      else if (currentTool == 6) connectSelectedCells(false);

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
