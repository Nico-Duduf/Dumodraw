#include "exportform.h"

ExportForm::ExportForm(QGridLayout *l, QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    numColumns = 10;
    numRows = 10;
    layout = l;
}

void ExportForm::setNumColumns(int n)
{
    numColumns = n;
    if (exportSettingsComboBox->currentIndex() == 0)
    {
        on_exportSettingsComboBox_currentIndexChanged(0);
    }
}

void ExportForm::setNumRows(int n)
{
    numRows = n;
    if (exportSettingsComboBox->currentIndex() == 0)
    {
        on_exportSettingsComboBox_currentIndexChanged(0);
    }
}

void ExportForm::setDefaultBGColor(QColor c)
{
    defaultBGColor = c;
}

void ExportForm::on_exportButton_clicked()
{
    QString saveFileName = QFileDialog::getSaveFileName(this,"Export image to...","","Windows Bitmap (*.bmp);;Joint Photographic Experts Group (*.jpg *.jpeg);;Portable Network Graphics (*.png);;Portable Pixmap (*.ppm);;X11 Bitmap (*.xbm *.xpm)");
    if (saveFileName == "") return;

    //size
    int width = widthEdit->value();
    int height = heightEdit->value();
    int rowSize = height/layout->columnCount();
    int columnSize = width/layout->rowCount();

    //create image
    QImage image(width,height,QImage::Format_ARGB32);
    QPainter painter(&image);
    if (transparentCheckBox->isChecked()) painter.fillRect(0,0,width,height,QColor(0,0,0,0));
    else painter.fillRect(0,0,width,height,QColor(BGColorEdit->text()));

    //check each cell
    for (int i = 0 ; i < layout->count() ; i++)
    {
        Cell *cell = qobject_cast<Cell*>(layout->itemAt(i)->widget());
        painter.drawPixmap(cell->getColumn()*columnSize,cell->getRow()*rowSize,columnSize,rowSize,*cell->getPixmap());
    }

    //save image
    if (colorsComboBox->currentIndex() == 0) image = image.convertToFormat(QImage::Format_Mono);
    else if (colorsComboBox->currentIndex() == 1) image = image.convertToFormat(QImage::Format_Grayscale8);

    image.setDotsPerMeterX(definitionEdit->value()*39.37008);
    image.setDotsPerMeterY(definitionEdit->value()*39.37008);

    image.save(saveFileName);

    emit finished();
}

void ExportForm::on_exportSettingsComboBox_currentIndexChanged(int index)
{
    if (index == 0) //Auto
    {
        //get from modules
        QImage test(":/0");
        widthEdit->setValue(numColumns*test.width());
        heightEdit->setValue(numRows*test.height());
        definitionEdit->setValue(72);
        transparentCheckBox->setChecked(true);
        BGColorEdit->setEnabled(false);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 1) //Custom
    {

    }
    else if (index  == 2) //Letter
    {
        widthEdit->setValue(2400);
        heightEdit->setValue(3300);
        definitionEdit->setValue(300);
        transparentCheckBox->setChecked(false);
        BGColorEdit->setEnabled(true);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 3) //Letter (DL)
    {
        widthEdit->setValue(1300);
        heightEdit->setValue(2600);
        definitionEdit->setValue(300);
        transparentCheckBox->setChecked(false);
        BGColorEdit->setEnabled(true);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 4) //PostCard
    {
        widthEdit->setValue(1200);
        heightEdit->setValue(1800);
        definitionEdit->setValue(300);
        transparentCheckBox->setChecked(false);
        BGColorEdit->setEnabled(true);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 5) //Business Card
    {
        widthEdit->setValue(1000);
        heightEdit->setValue(640);
        definitionEdit->setValue(300);
        transparentCheckBox->setChecked(false);
        BGColorEdit->setEnabled(true);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 6) //A3
    {
        widthEdit->setValue(3508);
        heightEdit->setValue(4961);
        definitionEdit->setValue(300);
        transparentCheckBox->setChecked(false);
        BGColorEdit->setEnabled(true);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 7) //A4
    {
        widthEdit->setValue(2480);
        heightEdit->setValue(3508);
        definitionEdit->setValue(300);
        transparentCheckBox->setChecked(false);
        BGColorEdit->setEnabled(true);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 8) //A5
    {
        widthEdit->setValue(1748);
        heightEdit->setValue(2480);
        definitionEdit->setValue(300);
        transparentCheckBox->setChecked(false);
        BGColorEdit->setEnabled(true);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 9) //A6
    {
        widthEdit->setValue(1240);
        heightEdit->setValue(1748);
        definitionEdit->setValue(300);
        transparentCheckBox->setChecked(false);
        BGColorEdit->setEnabled(true);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 10) //Square 1000
    {
        widthEdit->setValue(1000);
        heightEdit->setValue(1000);
        definitionEdit->setValue(300);
        transparentCheckBox->setChecked(false);
        BGColorEdit->setEnabled(true);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 11) //Square 2000
    {
        widthEdit->setValue(2000);
        heightEdit->setValue(2000);
        definitionEdit->setValue(300);
        transparentCheckBox->setChecked(false);
        BGColorEdit->setEnabled(true);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 12) //DCI 8K
    {
        widthEdit->setValue(8192);
        heightEdit->setValue(4320);
        definitionEdit->setValue(72);
        transparentCheckBox->setChecked(true);
        BGColorEdit->setEnabled(false);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 13) //HD 8K
    {
        widthEdit->setValue(7680);
        heightEdit->setValue(4320);
        definitionEdit->setValue(72);
        transparentCheckBox->setChecked(true);
        BGColorEdit->setEnabled(false);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 14) //DCI 4K Scope
    {
        widthEdit->setValue(4096 );
        heightEdit->setValue(1716);
        definitionEdit->setValue(72);
        transparentCheckBox->setChecked(true);
        BGColorEdit->setEnabled(false);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 15) //DCI 4K Flat
    {
        widthEdit->setValue(3996 );
        heightEdit->setValue(2160);
        definitionEdit->setValue(72);
        transparentCheckBox->setChecked(true);
        BGColorEdit->setEnabled(false);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 16) //HD 4K
    {
        widthEdit->setValue(3840 );
        heightEdit->setValue(2160);
        definitionEdit->setValue(72);
        transparentCheckBox->setChecked(true);
        BGColorEdit->setEnabled(false);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 17) //DCI 2K Scope
    {
        widthEdit->setValue(2048 );
        heightEdit->setValue(858 );
        definitionEdit->setValue(72);
        transparentCheckBox->setChecked(true);
        BGColorEdit->setEnabled(false);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 18) //DCI 2K Flat
    {
        widthEdit->setValue(1998 );
        heightEdit->setValue(1080);
        definitionEdit->setValue(72);
        transparentCheckBox->setChecked(true);
        BGColorEdit->setEnabled(false);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 19) //1080
    {
        widthEdit->setValue(1920);
        heightEdit->setValue(1080);
        definitionEdit->setValue(72);
        transparentCheckBox->setChecked(true);
        BGColorEdit->setEnabled(false);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
    else if (index  == 20) //720
    {
        widthEdit->setValue(1280);
        heightEdit->setValue(720);
        definitionEdit->setValue(72);
        transparentCheckBox->setChecked(true);
        BGColorEdit->setEnabled(false);
        BGColorEdit->setText(defaultBGColor.name());
        colorsComboBox->setCurrentIndex(2);
    }
}

void ExportForm::on_cancelButton_clicked()
{
    emit canceled();
}

void ExportForm::on_transparentCheckBox_clicked(bool checked)
{
    BGColorEdit->setEnabled(!checked);
}

void ExportForm::paintEvent(QPaintEvent *) {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }
