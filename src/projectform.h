#ifndef PROJECTFORM_H
#define PROJECTFORM_H

#include "ui_projectform.h"

#include <QPainter>

class ProjectForm : public QWidget, private Ui::ProjectForm
{
    Q_OBJECT

public:
    explicit ProjectForm(QWidget *parent = 0);
    int numRows();
    int numColumns();
    QColor BGColor();

signals:
    void ok();
    void cancel();
private slots:
    void on_applyButton_clicked();
    void on_cancelButton_clicked();
private:
    void paintEvent(QPaintEvent *);
};

#endif // PROJECTFORM_H
