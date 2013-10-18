#ifndef LOGO_H
#define LOGO_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>

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
    void compeling();
    void createMessage();
    void setMessage();
    void clearMessage();
    void MachineVirtual();

};

#endif // LOGO_H
