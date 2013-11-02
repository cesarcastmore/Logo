#ifndef LOGO_H
#define LOGO_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include "virtualmemory.h"

namespace Ui {
class Logo;
}

class Logo : public QDialog
{
    Q_OBJECT

public:
    explicit Logo(QWidget *parent = 0);
    ~Logo();

    //clases de compiladores
private slots:
    void on_compileButton_clicked();
    void on_clearButton_clicked();
    void on_runButton_clicked();

private:
    Ui::Logo *ui;
    QGraphicsScene *scene;
    QGraphicsEllipseItem * ellipse;
    QGraphicsRectItem *rectangle;
    QPolygonF polytri;
    QGraphicsItem *triangle;
    QPolygonF polypenta;
    QGraphicsItem *pentagon;
    QPolygonF polyhex;
    QGraphicsItem *hexagon;
    QPolygonF polyrhom;
    QGraphicsItem *rhomboid;
    QPolygonF polystar;
    QGraphicsItem *star;
    Memoria *memoria;

    void compeling();
    void createMessage();
    void setMessage();
    void clearMessage();
    void MachineVirtual();
    void saveRead(int dir, double value);
    void setExamples();
    void dibujaFigura(int figura, int x, int y,  int rotate, int scale, QBrush &colorFig, QPen &colorThi);
<<<<<<< HEAD

    bool atob(double a);
=======
>>>>>>> 82b17707f2aee85209224db4403e72be6eb179f1
};




#endif // LOGO_H
