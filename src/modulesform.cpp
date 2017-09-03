#include "modulesform.h"

#ifdef QT_DEBUG
#include <QtDebug>
#endif

ModulesForm::ModulesForm(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    customPath = "";
}

void ModulesForm::on_okButton_clicked()
{
    QString path = "";
    if (includedButton->isChecked())
    {
        if (includedComboBox->currentIndex() == 0)
        {
            path = ":/module/default";
        }
        if (includedComboBox->currentIndex() == 1)
        {
            path = ":/module/simple";
        }
        if (includedComboBox->currentIndex() == 2)
        {
            path = ":/module/double";
        }
        if (includedComboBox->currentIndex() == 3)
        {
            path = ":/module/angle";
        }
        if (includedComboBox->currentIndex() == 4)
        {
            path = ":/module/oblique";
        }
    }
    else
    {
        path = customPath;
    }

    if (path != "") emit ok(path);
}

void ModulesForm::on_cancelButton_clicked()
{
    emit cancel();
}

void ModulesForm::on_exportButton_clicked()
{
    QString path = "";
    QString name = "";
    QString newPath = QFileDialog::getExistingDirectory(this,"Select the folder where you want to export the kit");
    if (newPath == "") return;

    if (includedComboBox->currentIndex() == 0)
    {
        path = ":/default module";
        name = "Default DuModraw Kit";
        QDir(newPath).mkdir(name);
        for (int i = 0; i < 16 ; i++)
        {
            QFile::copy(path + "/" + QString::number(i),newPath + "/" + name + "/" +  QString::number(i) + ".png");
        }
    }
}

void ModulesForm::paintEvent(QPaintEvent *) {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }


void ModulesForm::on_browseButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Select the folder where your kit is");
    if (path == "") return;
    customPath = path;
    browseButton->setText(QDir(path).dirName());
}
