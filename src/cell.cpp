#include "cell.h"

#ifdef QT_DEBUG
#include <QtDebug>
#endif

Cell::Cell(int r, int c, QList<QPixmap *> pix, QColor backgroundColor, QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    //----- STATE ------

    checked = false;
    selected = false;
    row = r;
    column = c;
    background = c;

    //----- BUILD UI ------

    //add layout and widgets
    QStackedLayout *mainLayout = new QStackedLayout();
    mainLayout->setStackingMode(QStackedLayout::StackAll);
    this->setLayout(mainLayout);

    hoverLabel = new QLabel();
    hoverLabel->setStyleSheet("QLabel { background-color: rgba(1,1,1,0); } QLabel:hover { background-color: rgba(113,1,50,50); border: 4px solid rgba(113,1,50,70); }");
    mainLayout->addWidget(hoverLabel);

    label = new QLabel();
    label->setScaledContents(true);
    label->setPixmap(QPixmap());
    mainLayout->addWidget(label);

    this->setStyleSheet("background-color: " + backgroundColor.name() + ";");

    //------ NEIGHBOURS -------

    topLeft = false;
    top = false;
    topRight = false;
    right = false;
    bottomRight = false;
    bottom = false;
    bottomLeft = false;
    left = false;

    pixmaps = pix;
}

void Cell::click(int currentTool)
{
    if (currentTool == 1) setChecked(true);
    else if (currentTool == 2) setChecked(false);
    else if (currentTool == 3 || currentTool == 5 || currentTool == 6) setSelected(true);
    else if (currentTool == 4) setSelected(false);
}

void Cell::setSelected(bool c)
{
    if (c)
    {
        hoverLabel->setStyleSheet("QLabel { background-color: rgba(236,24,24,50); } QLabel:hover { background-color: rgba(113,1,50,50); border: 4px solid rgba(113,1,50,70); }");
        selected = true;
    }
    else
    {
        hoverLabel->setStyleSheet("QLabel { background-color: rgba(236,24,24,0); } QLabel:hover { background-color: rgba(113,1,50,50); border: 4px solid rgba(113,1,50,70); }");
        selected = false;
    }
}

void Cell::setBackgroundColor(QColor c)
{
    this->setStyleSheet("background-color: " + c.name() + ";");
    background = c;
}

QColor Cell::getBackgroundColor()
{
    return background;
}

void Cell::setChecked(bool c)
{
    if (c)
    {
        checked = true;
    }
    else
    {
        checked = false;
    }
    updatePixmap();
    emit swapped(checked);
}

bool Cell::isSelected()
{
    return selected;
}

const QPixmap *Cell::getPixmap()
{
    return label->pixmap();
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
        label->setPixmap(*pixmaps[0]);
    }
    //TOP
    else if (top && !topRight && !right && !bottomRight && !bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(*pixmaps[1]);
    }
    //RIGHT
    else if (!top && !topRight && right && !bottomRight && !bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(*pixmaps[2]);
    }
    //BOTTOM
    else if (!top && !topRight && !right && !bottomRight && bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(*pixmaps[3]);
    }
    //LEFT
    else if (!top && !topRight && !right && !bottomRight && !bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(*pixmaps[4]);
    }
    //LEFT-TOP
    else if (top && !topRight && !right && !bottomRight && !bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(*pixmaps[5]);
    }
    //TOP-RIGHT
    else if (top && !topRight && right && !bottomRight && !bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(*pixmaps[6]);
    }
    //RIGHT-BOTTOM
    else if (!top && !topRight && right && !bottomRight && bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(*pixmaps[7]);
    }
    //BOTTOM-LEFT
    else if (!top && !topRight && !right && !bottomRight && bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(*pixmaps[8]);
    }
    //TOP-BOTTOM
    else if (top && !topRight && !right && !bottomRight && bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(*pixmaps[14]);
    }
    //LEFT-RIGHT
    else if (!top && !topRight && right && !bottomRight && !bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(*pixmaps[13]);
    }
    //BOTTOM-LEFT-TOP
    else if (top && !topRight && !right && !bottomRight && bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(*pixmaps[9]);
    }
    //LEFT-TOP-RIGHT
    else if (top && !topRight && right && !bottomRight && !bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(*pixmaps[10]);
    }
    //RIGHT-BOTTOM-LEFT
    else if (!top && !topRight && right && !bottomRight && bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(*pixmaps[12]);
    }
    //TOP-RIGHT-BOTTOM
    else if (top && !topRight && right && !bottomRight && bottom && !bottomLeft && !left && !topLeft)
    {
        label->setPixmap(*pixmaps[11]);
    }
    //CROSS
    else if (top && !topRight && right && !bottomRight && bottom && !bottomLeft && left && !topLeft)
    {
        label->setPixmap(*pixmaps[15]);
    }
    else
    {
        label->setPixmap(QPixmap());
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
