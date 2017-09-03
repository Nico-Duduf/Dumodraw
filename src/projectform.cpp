#include "projectform.h"

ProjectForm::ProjectForm(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

int ProjectForm::numRows()
{
    return rowsEdit->value();
}

int ProjectForm::numColumns()
{
    return columnsEdit->value();
}

QColor ProjectForm::BGColor()
{
    return QColor(BGColorEdit->text());
}

void ProjectForm::on_applyButton_clicked()
{
    emit ok();
}

void ProjectForm::on_cancelButton_clicked()
{
    emit cancel();
}
