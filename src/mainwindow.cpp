#include "mainwindow.h"

#ifdef QT_DEBUG
#include <QtDebug>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    //UI
    //App Layout
    stackedLayout = new QStackedLayout();
    stackedLayout->setStackingMode(QStackedLayout::StackAll);
    mainWidget->setLayout(stackedLayout);

    //Export Form
    exportForm = new ExportForm(mainLayout,this);
    stackedLayout->addWidget(exportForm);
    exportForm->hide();

    //Project Form
    projectForm = new ProjectForm(this);
    stackedLayout->addWidget(projectForm);
    projectForm->hide();

    //Modules Form
    modulesForm = new ModulesForm(this);
    stackedLayout->addWidget(modulesForm);
    modulesForm->hide();

    //Grid
    gridWidget = new QWidget(this);
    mainLayout = new QGridLayout;
    mainLayout->setMargin(0);
    gridWidget->setLayout(mainLayout);
    //set grid color and size
    gridWidget->setStyleSheet("#gridWidget { background-color: #bbb; }");
    mainLayout->setSpacing(1);
    stackedLayout->addWidget(gridWidget);

    //INITIALIZE
    painting = false;
    currentTool = 1;
    rowCount = 0;
    columnCount = 0;
    for (int i = 0 ; i < 16 ; i++)
    {
        pixmaps << new QPixmap();
    }
    //load default
    loadModule(":/default module");

    //show grid
    mainLayout->setSpacing(1);

    //create default cells (seems to be a maximum at 3600 cells)
    createCells(15,15);

    //events
    mainWidget->installEventFilter(this);

    mapEvents();
}

void MainWindow::mapEvents()
{
    connect(exportForm,SIGNAL(finished()),this,SLOT(exportFinished()));
    connect(exportForm,SIGNAL(canceled()),this,SLOT(exportFinished()));

    connect(projectForm,SIGNAL(ok()),this,SLOT(projectSettingsChanged()));
    connect(projectForm,SIGNAL(cancel()),this,SLOT(projectSettingsCancelled()));

    connect(modulesForm,SIGNAL(ok(QString)),this,SLOT(modulesChanged(QString)));
    connect(modulesForm,SIGNAL(cancel()),this,SLOT(modulesCancelled()));
}

void MainWindow::loadModule(QString path)
{
    //add the missing / at the end
    if (!path.endsWith("/") || !path.endsWith("\\")) path = path + "/";

    QDir dir(path);
    QStringList files = dir.entryList(QDir::Files);

    //check if all files are here
    if (!path.startsWith(":/"))
    {
        for (int i = 0 ; i < 16 ; i++)
        {
            bool found = false;
            foreach (QString file,files)
            {
               if (file.startsWith(QString::number(i) + "."))
               {
                   found = true;
                   break;
               }
            }
            if (!found) return;
            //TODO alert missing file in kit
        }
    }

    //check file extension
    QString extension = "";
    //check
    foreach (QString file,files)
    {
        if (file.startsWith("0."))
        {
            extension = file.right(file.count() - file.lastIndexOf("."));
            break;
        }
    }

    //load images
    for (int i = 0 ; i < 16 ; i++)
    {
        pixmaps[i]->load(path + QString::number(i) + extension);
    }

    //update cells
    for (int i = 0 ; i < mainLayout->count() ; i++)
    {
        Cell *cell = qobject_cast<Cell*>(mainLayout->itemAt(i)->widget());
        cell->updatePixmap();
    }
}

void MainWindow::createCells(int numRows, int numColumns, QColor backgroundColor)
{
    //if cells are going to be removed, alert
    if (numRows < rowCount || numColumns < columnCount)
    {
        //TODO Alert
    }

    //if less rows, delete
    if (numRows < rowCount)
    {

        for (int row = mainLayout->rowCount()-1 ; row >= numRows ; row-- )
        {
            removeLine(row,-1);
            rowCount--;
        }
    }

    //if less columns, delete
    if (numColumns < columnCount)
    {
        for (int col = mainLayout->columnCount()-1 ; col >= numColumns ; col-- )
        {
            removeLine(-1,col);
            columnCount--;
        }
    }

    //if first cell, create it
    if (rowCount == 0 && columnCount == 0)
    {
        createCell(0,0,backgroundColor);
        rowCount = 1;
        columnCount = 1;
    }

    //if more columns, create
    if (numColumns > columnCount)
    {
        for (int col = columnCount; col < numColumns; col++)
        {
            for (int row = 0; row < rowCount; row++)
            {
                createCell(row,col,backgroundColor);
            }
            columnCount++;
        }
    }

    //if more rows, create
    if (numRows > rowCount)
    {
        for (int row = rowCount; row < numRows; row++)
        {
            for (int col = 0; col < columnCount; col++)
            {
                createCell(row,col,backgroundColor);
            }
            rowCount++;
        }
    }
}

void MainWindow::createCell(int row, int col, QColor backgroundColor)
{
    //create cell
    Cell *cell = new Cell(row,col,pixmaps,backgroundColor);
    mainLayout->addWidget(cell,row,col);


    //connections, to the left and the top cells
    if (col > 0)
    {
        QLayoutItem *item = mainLayout->itemAtPosition(row,col-1);
        Cell *cellLeft = qobject_cast<Cell*>(mainLayout->itemAtPosition(row,col-1)->widget());
        connect(cell,SIGNAL(swapped(bool)),cellLeft,SLOT(setRight(bool)));
        connect(cellLeft,SIGNAL(swapped(bool)),cell,SLOT(setLeft(bool)));
    }

    if (row > 0)
    {
        Cell *cellTop = qobject_cast<Cell*>(mainLayout->itemAtPosition(row-1,col)->widget());
        connect(cell,SIGNAL(swapped(bool)),cellTop,SLOT(setBottom(bool)));
        connect(cellTop,SIGNAL(swapped(bool)),cell,SLOT(setTop(bool)));
    }

}

void MainWindow::removeLine(int row, int column, bool deleteWidgets) {
    // We avoid usage of QGridLayout::itemAtPosition() here to improve performance.
    for (int i = mainLayout->count() - 1; i >= 0; i--) {
        int r, c, rs, cs;
        mainLayout->getItemPosition(i, &r, &c, &rs, &cs);
        if ((r <= row && r + rs - 1 >= row) || (c <= column && c + cs - 1 >= column)) {
            // This layout item is subject to deletion.
            QLayoutItem *item = mainLayout->takeAt(i);
            if (deleteWidgets) {
                delete item->widget();
            }
            delete item;
        }
    }
}

void MainWindow::showProjectForm()
{
    projectForm->show();
    exportForm->hide();
    modulesForm->hide();
}

void MainWindow::showExportForm()
{
    projectForm->hide();
    exportForm->show();
    modulesForm->hide();
}

void MainWindow::showModulesForm()
{
    projectForm->hide();
    exportForm->hide();
    modulesForm->show();
}

void MainWindow::showGrid()
{
    projectForm->hide();
    exportForm->hide();
    modulesForm->hide();
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

void MainWindow::exportFinished()
{
    exportForm->hide();
}

void MainWindow::projectSettingsCancelled()
{
    projectForm->hide();
}

void MainWindow::projectSettingsChanged()
{
    //cells
    createCells(projectForm->numRows(),projectForm->numColumns(),projectForm->BGColor());

    //background
    //check first cell color
    Cell *cell = qobject_cast<Cell*>(mainLayout->itemAt(0)->widget());
    QColor backgroundColor = cell->getBackgroundColor();
    //only if the color has changed
    if (backgroundColor != projectForm->BGColor())
    {
        for (int i = 0 ; i < mainLayout->count() ; i++)
        {
            Cell *cell = qobject_cast<Cell*>(mainLayout->itemAt(i)->widget());
            cell->setBackgroundColor(projectForm->BGColor());
        }
    }

    projectForm->hide();
}

void MainWindow::modulesChanged(QString path)
{
    loadModule(path);
    modulesForm->hide();
}

void MainWindow::modulesCancelled()
{
    modulesForm->hide();
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

void MainWindow::on_actionExport_triggered()
{
    exportForm->setNumRows(mainLayout->rowCount());
    exportForm->setNumColumns(mainLayout->columnCount());
    exportForm->setDefaultBGColor(QColor("#ffffff"));

    showExportForm();
}

void MainWindow::on_actionProject_settings_triggered()
{
    showProjectForm();
}

void MainWindow::on_actionModules_triggered()
{
    showModulesForm();
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

