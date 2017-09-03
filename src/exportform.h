#ifndef EXPORTFORM_H
#define EXPORTFORM_H

#include "ui_exportform.h"

#include "cell.h"

#include <QPainter>
#include <QFileDialog>

class ExportForm : public QWidget, private Ui::ExportForm
{
    Q_OBJECT

public:
    explicit ExportForm(QGridLayout *l, QWidget *parent = 0);
    void setNumColumns(int n);
    void setNumRows(int n);
    void setBackgroundColor(QColor c);
    void setWidth(int w);
    void setHeight(int h);

signals:
    void finished();
    void canceled();

private slots:
    void on_exportButton_clicked();
    void on_exportSettingsComboBox_currentIndexChanged(int index);
    void on_cancelButton_clicked();

    void on_transparentCheckBox_clicked(bool checked);

private:
    int numColumns;
    int numRows;
    QColor backgroundColor;
    int width;
    int height;
    QGridLayout *layout;


    void paintEvent(QPaintEvent *);
};

#endif // EXPORTFORM_H
