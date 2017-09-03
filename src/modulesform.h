#ifndef MODULESFORM_H
#define MODULESFORM_H

#include "ui_modulesform.h"

#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QPainter>

class ModulesForm : public QWidget, private Ui::ModulesForm
{
    Q_OBJECT

public:
    explicit ModulesForm(QWidget *parent = 0);
signals:
    void ok(QString path);
    void cancel();

public slots:
private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

    void on_exportButton_clicked();

    void on_browseButton_clicked();

private:
    QString customPath;

    void paintEvent(QPaintEvent *);
};

#endif // MODULESFORM_H
