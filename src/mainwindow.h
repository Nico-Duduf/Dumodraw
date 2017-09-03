#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QMouseEvent>

#include "cell.h"
#include "exportform.h"
#include "projectform.h"
#include "modulesform.h"

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
    void on_actionDisconnect_triggered();
    void on_actionConnect_triggered();

    void on_actionExport_triggered();
    void on_actionProject_settings_triggered();
    void on_actionModules_triggered();

    void exportFinished();
    void projectSettingsCancelled();
    void projectSettingsChanged();
    void modulesChanged(QString path);
    void modulesCancelled();

private:
    ExportForm *exportForm;
    ProjectForm *projectForm;
    ModulesForm * modulesForm;
    QStackedLayout *stackedLayout;
    QWidget *gridWidget;
    QGridLayout *mainLayout;

    bool painting;
    Cell *currentCell;
    int currentTool;
    QList<Cell*> selectedCells;
    int rowCount;
    int columnCount;

    QList<QPixmap*> pixmaps;

    void mapEvents();

    void loadModule(QString path);

    void clickCell(QPoint pos);
    void swapTool();
    void connectSelectedCells(bool c);
    void selectAll(bool c);
    void createCells(int numRows, int numColumns, QColor backgroundColor = QColor("#ffffff"));
    void createCell(int row, int col, QColor backgroundColor = QColor("#ffffff"));
    void removeLine(int row, int column, bool deleteWidgets = true);

    void showProjectForm();
    void showExportForm();
    void showModulesForm();
    void showGrid();

    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
