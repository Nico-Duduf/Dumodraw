#ifndef CELL_H
#define CELL_H

#include "ui_cell.h"

#include <QPainter>
#include <QStackedLayout>
#include <QLabel>

class Cell : public QWidget, private Ui::Cell
{
    Q_OBJECT


public:
    explicit Cell(int r, int c,QList<QPixmap*> pix, QColor backgroundColor = QColor("#ffffff"), QWidget *parent = 0);
    void click(int currentTool);
    void setChecked(bool c);
    bool isChecked();
    void setSelected(bool c);
    void setBackgroundColor(QColor c);
    QColor getBackgroundColor();
    bool isSelected();
    const QPixmap *getPixmap();

    int getRow();
    int getColumn();

public slots:
    void setTopLeft(bool c);
    void setTop(bool c);
    void setTopRight(bool c);
    void setRight(bool c);
    void setBottomRight(bool c);
    void setBottom(bool c);
    void setBottomLeft(bool c);
    void setLeft(bool c);

    bool getTopLeft();
    bool getTop();
    bool getTopRight();
    bool getRight();
    bool getBottomRight();
    bool getBottom();
    bool getBottomLeft();
    bool getLeft();

    void updatePixmap();

signals:
    void swapped(bool);

private:

    //------ WIDGETS & UI ---------

    QLabel *label;
    QLabel *hoverLabel;

    //------ STATE ---------

    /**
     * @brief row The row containing this cell
     */
    int row;
    /**
     * @brief column The column containing this cell
     */
    int column;
    /**
     * @brief checked True if the cell is filled
     */
    bool checked;
    /**
     * @brief selected True if cell is selected
     */
    bool selected;
    /**
     * @brief background The color of empty cells
     */
    QColor background;

    //------ NEIGHBOURS ---------

    bool topLeft;
    bool top;
    bool topRight;
    bool right;
    bool bottomRight;
    bool bottom;
    bool bottomLeft;
    bool left;

    QList<QPixmap*> pixmaps;

    //------ OPERATORS ---------

    bool operator ==(Cell *c);
    bool operator ==(Cell c);
    bool operator !=(Cell *c);
    bool operator !=(Cell c);

    //------ EVENTS ---------

    /**
     * @brief paintEvent Reimplements the paintEvent which is not implemented in QWidget
     * Needed to be able to set its style.
     */
    void paintEvent(QPaintEvent *);

};

#endif // CELL_H
