#include "logo.h"
#include "ui_logo.h"
#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QTextDocument>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

Logo::Logo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logo)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 100, 100);
    ui->graphic->setScene(scene);

}

Logo::~Logo()
{
    delete ui;
}



void Logo::on_clearButton_clicked()
{

    clearMessage();

}

void Logo::on_compileButton_clicked()
{
    compeling();
    createMessage();
    setMessage();



}

void Logo::compeling(){
    QString document=ui->textProgram->toPlainText();
    QFile file("/home/castillo/Logo/in.pr");
    if (!file.open(QIODevice::WriteOnly))
        cout<<"no encontro el arcivho";
    QTextStream out(&file);
    out <<document;
}

void Logo::createMessage(){
    system("/home/castillo/Logo/compile/run /home/castillo/Logo/in.pr > /home/castillo/Logo/message.txt");

}


void Logo::setMessage(){
    QFile in("/home/castillo/Logo/message.txt");
    if (!in.open(QFile::ReadOnly | QFile::Text))
        cout<<"no encontro el arcivho";
    QTextStream ReadFile(&in);
    ui->textMessage->setText(ReadFile.readAll());

}
void Logo::clearMessage(){
    ui->textMessage->setText("");
    QFile file("/home/castillo/Logo/compile/message.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        cout<<"no encontro el arcivho";
    QTextStream out(&file);
    out <<"";
}

void Logo::on_runButton_clicked()
{
   MachineVirtual();
}


void Logo::MachineVirtual(){

    /*llenar el map desde un archivo
     el archivo va a tener la siguiente formato
     #30#-1#-1#-1#
     #35#1#50#-1#
     #50#35#-1#-1#
     ahorita es un arreglo fiticio
     */
        int map[12][4]={{0,1,-1,-1},
                        {30,-1,-1,-1},
                        {35,1,50,-1},
                        {35,1,50,-1},
                        {35,7,1,-1},
                        {35,8,2,-1},
                        {50,35,-1,-1},
                        {30,-1,-1},
                        {34,1,0,-1},
                        {34,2,0,-1},
                        {50,34,-1,-1},
                       {99,-1,-1,-1}} ;

        int y_position=0;
        int x_position=0;
        QBrush colorFigure(Qt::blue);
        QPen colorThick(Qt::yellow);

        int cont=0;
        while(cont != 10000){

            int instruccion = map[cont][0];

            switch(instruccion){
            case 0:
                cont=map[cont][1];
                break;
            case 1:
                cont++;
                break;
            case 2:
                cont++;
                break;
            case 3:
                cont++;
                break;
            case 4:
                cont++;
                break;
            case 5:
                cont++;
                break;
            case 6:
                cont++;
                break;
            case 7:
                cont++;
                break;
            case 8:
                cont++;
                break;
            case 9:
                cont++;
                break;
            case 10:
                cont++;
                break;
            case 11:
                cont++;
                break;
            case 12:
                cont++;
                break;
            case 13:
                cont++;
                break;
            case 14:
                cont++;
                break;
            case 15:
                cont++;
                break;
            case 16:
                cont++;
                break;
            case 17:
                cont++;
                break;
            case 18:
                cont++;
                break;
            case 19:
                cont++;
                break;
            //begin draw
            case 30:
                y_position=0;
                x_position=0;
                colorFigure.setColor(Qt::white);
                colorThick.setColor(Qt::black);
                cont++;
                break;
            //draw point
            case 31:
                cont++;
                break;
            //draw line
            case 32:
                cont++;
                break;
            //draw triangule
            case 33:
                cont++;
                break;
            //draw a square
            case  34:
                switch(map[cont][1]){
                case 1:
                    y_position=map[cont][2];
                    break;
                case 2:
                    x_position=map[cont][2];
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
                case 7:
                    switch(map[cont][2]){
                    case 1:
                        colorThick.setColor(Qt::blue);
                        break;
                    case 2:
                        colorThick.setColor(Qt::yellow);
                        break;
                    case 3:
                        colorThick.setColor(Qt::red);
                        break;
                    }
                    break;
                case 8:
                    switch(map[cont][2]){
                    case 1:
                        colorFigure.setColor(Qt::blue);
                        break;
                    case 2:
                        colorFigure.setColor(Qt::yellow);
                        break;
                    case 3:
                        colorFigure.setColor(Qt::red);
                        break;
                    }
                    break;
                }
                cont++;
                break;
            //draw circle
            case  35:
                switch(map[cont][1]){
                case 1:
                    y_position=map[cont][2];
                case 2:
                    x_position=map[cont][2];
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
                case 7:
                    switch(map[cont][2]){
                    case 1:
                        colorThick.setColor(Qt::blue);
                        break;
                    case 2:
                        colorThick.setColor(Qt::yellow);
                        break;
                    case 3:
                        colorThick.setColor(Qt::red);
                        break;
                    }
                    break;
                case 8:
                    switch(map[cont][2]){
                    case 1:
                        colorFigure.setColor(Qt::blue);
                        break;
                    case 2:
                        colorFigure.setColor(Qt::yellow);
                        break;
                    case 3:
                        colorFigure.setColor(Qt::red);
                        break;
                    }
                }
                cont++;
                break;
            // draw star
            case 36:
                cont++;
                break;
            //draw pentagon
            case 37:
                cont++;
                break;
            //draw hexagon
            case 38:
                cont++;
                break;
            //rhomboid
            case 39:
                cont++;
                break;
            //end draw
            case 50:
                switch(map[cont][1]){
                case 34:
                    rectangle = scene->addRect(x_position, y_position, 50, 50, colorThick, colorFigure);
                    break;
                case 35:
                    ellipse = scene->addEllipse(x_position,y_position,100,100, colorThick, colorFigure);
                    break;
                }

                cont++;
                break;
            case 99:
                cont=10000;
                break;

            }
        }

}
