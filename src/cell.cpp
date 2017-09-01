#include "cell.h"

#ifdef QT_DEBUG
#include <QtDebug>
#endif

Cell::Cell(int r, int c, QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    checked = false;
    row = r;
    column = c;

    //initialize
    this->setStyleSheet("background-color: #fff;");

    topLeft = false;
    top = false;
    topRight = false;
    right = false;
    bottomRight = false;
    bottom = false;
    bottomLeft = false;
    left = false;
}

void Cell::swap(int currentTool)
{
    if (currentTool == 1) setChecked(true);
    else if (currentTool == 2) setChecked(false);
    else if (currentTool == 3) setChecked(!checked);
}

void Cell::setChecked(bool c)
{
    if (c)
    {
        checked = true;
        this->setStyleSheet("background-color: #000;");
    }
    else
    {
        checked = false;
        this->setStyleSheet("background-color: #fff;");
    }
    updatePixmap();
    emit swapped(checked);
}

void Cell::updatePixmap()
{
    if (!checked)
    {
        label->setPixmap(QPixmap());
        return;
    }

    //NONE
    if (!top && !topRight && !right && !bottomRight && !bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(QPixmap(":/0"));
    }
    //TOP
    else if (top && !topRight && !right && !bottomRight && !bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(QPixmap(":/top"));
    }
    //RIGHT
    else if (!top && !topRight && right && !bottomRight && !bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(QPixmap(":/right"));
    }
    //BOTTOM
    else if (!top && !topRight && !right && !bottomRight && bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(QPixmap(":/bottom"));
    }
    //LEFT
    else if (!top && !topRight && !right && !bottomRight && !bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(QPixmap(":/left"));
    }
    //LEFT-TOP
    else if (top && !topRight && !right && !bottomRight && !bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(QPixmap(":/left-top"));
    }
    //TOP-RIGHT
    else if (top && !topRight && right && !bottomRight && !bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(QPixmap(":/top-right"));
    }
    //RIGHT-BOTTOM
    else if (!top && !topRight && right && !bottomRight && bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(QPixmap(":/right-bottom"));
    }
    //BOTTOM-LEFT
    else if (!top && !topRight && !right && !bottomRight && bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(QPixmap(":/bottom-left"));
    }
    //TOP-BOTTOM
    else if (top && !topRight && !right && !bottomRight && bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(QPixmap(":/top-bottom"));
    }
    //LEFT-RIGHT
    else if (!top && !topRight && right && !bottomRight && !bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(QPixmap(":/left-right"));
    }
    //BOTTOM-LEFT-TOP
    else if (top && !topRight && !right && !bottomRight && bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(QPixmap(":/bottom-left-top"));
    }
    //LEFT-TOP-RIGHT
    else if (top && !topRight && right && !bottomRight && !bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(QPixmap(":/left-top-right"));
    }
    //RIGHT-BOTTOM-LEFT
    else if (!top && !topRight && right && !bottomRight && bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(QPixmap(":/right-bottom-left"));
    }
    //TOP-RIGHT-BOTTOM
    else if (top && !topRight && right && !bottomRight && bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(QPixmap(":/top-right-bottom"));
    }
    //CROSS
    else if (top && !topRight && right && !bottomRight && bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(QPixmap(":/all"));
    }
    else
    {
        label->setPixmap(QPixmap(""));
    }
}

void Cell::setTopLeft(bool c)
{
    topLeft = c;
    updatePixmap();
}

void Cell::setTop(bool c)
{
    top = c;

#ifdef QT_DEBUG
    qDebug() << "top";
    qDebug() << c;
#endif
    updatePixmap();
}

void Cell::setTopRight(bool c)
{
    topRight = c;
    updatePixmap();
}

void Cell::setRight(bool c)
{
    right = c;
    updatePixmap();
}

void Cell::setBottomRight(bool c)
{
    bottomRight = c;
    updatePixmap();
}

void Cell::setBottom(bool c)
{
    bottom = c;
    updatePixmap();
}

void Cell::setBottomLeft(bool c)
{
    bottomLeft = c;
    updatePixmap();
}

void Cell::setLeft(bool c)
{
    left = c;
    updatePixmap();
}

int Cell::getRow()
{
    return row;
}

int Cell::getColumn()
{
    return column;
}

bool Cell::operator==(Cell *c)
{
    return c->getRow() == row && c->getColumn() == column;
}

bool Cell::operator ==(Cell c)
{
    return c.getRow() == row && c.getColumn() == column;
}

bool Cell::operator !=(Cell *c)
{
    return c->getRow() != row || c->getColumn() != column;
}

bool Cell::operator !=(Cell c)
{
    return c.getRow() != row || c.getColumn() != column;
}

void Cell::paintEvent(QPaintEvent *) {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }