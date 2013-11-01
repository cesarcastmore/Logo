#include "logo.h"
#include "ui_logo.h"

#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QTextDocument>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <input.h>
#include <QGraphicsItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QTransform>
#include "virtualmemory.h"


using namespace std;


Logo::Logo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logo)
{
    ui->setupUi(this);
    memoria=new Memoria();
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 100, 100);
    ui->graphic->setScene(scene);


    polytri << QPointF(0,0) << QPointF(100,0) << QPointF(50,-86.6);
    polystar << QPointF(0,0) << QPointF(-11,37) << QPointF(-50,37) << QPointF(-19,59) << QPointF(-31,95) << QPointF(0,73.1) << QPointF(31,95) << QPointF(19,59) << QPointF(50,37) << QPointF(10,37);
    polypenta << QPointF(0,0) << QPointF(18.16,55.9) << QPointF(76.94,55.9) << QPointF(95.1,0) << QPointF(47.55,-34.55);
    polyhex << QPointF(0,0) << QPointF(-25,43.3) << QPointF(0,86.6) << QPointF(50,86.6) << QPointF(75,43.3) << QPointF(50,0);
    polyrhom << QPointF(0,0) << QPointF(-25,50) << QPointF(75,50) << QPointF(100,0);
    setExamples();


}

Logo::~Logo()
{
    delete ui;
}

void Logo::saveRead(int dir, double value){
   memoria->save(dir, value);
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
    QFile file("/home/ubuntu/Logo-master/in.pr");
    if (!file.open(QIODevice::WriteOnly))
        cout<<"no encontro el archivo";
    QTextStream out(&file);
    out<<document;
}

void Logo::setExamples(){
    QFile in("/home/ubuntu/Logo-master/muchoejemplos.txt");
    if (!in.open(QFile::ReadOnly | QFile::Text))
        cout<<"no encontro el archivo";
    QTextStream ReadFile(&in);
    ui->examplesText->setText(ReadFile.readAll());
}

void Logo::createMessage(){
    system("/home/ubuntu/Logo-master/compile/run /home/ubuntu/Logo-master/in.pr > /home/ubuntu/Logo-master/message.txt");

}


void Logo::setMessage(){
    QFile in("/home/ubuntu/Logo-master/message.txt");
    if (!in.open(QFile::ReadOnly | QFile::Text))
        cout<<"no encontro el archivo";
    QTextStream ReadFile(&in);
    ui->textMessage->setText(ReadFile.readAll());

}
void Logo::clearMessage(){
    ui->textMessage->setText("");
    QFile file("/home/ubuntu/Logo-master/message.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        cout<<"no encontro el archivo";
    QTextStream out(&file);
    out <<"";
}

void Logo::on_runButton_clicked()
{
   MachineVirtual();
}




void Logo::dibujaFigura(int figura, int x, int y, int rotate, int scale, QBrush &colorFig,   QPen &colorThi){
    switch(figura){
    case 31:
        break;
    case 32:
        break;
    case 33:
        triangle = scene->addPolygon(polytri,colorThi,colorFig);
        triangle->resetTransform();
        triangle->setPos(x,y);
        triangle->setScale(scale);
        triangle->setRotation(rotate);
        break;
    case 34:
       rectangle = scene->addRect(0, 0, 50, 50, colorThi, colorFig);
       rectangle->resetTransform();
       rectangle->setPos(x,y);
       rectangle->setScale(scale);
       rectangle->setRotation(rotate);
        break;
    case 35:
        ellipse = scene->addEllipse(0,0,100,100, colorThi, colorFig);
        ellipse->resetTransform();
        ellipse->setPos(x,y);
        ellipse->setScale(scale);
        ellipse->setRotation(rotate);
        break;
    case 36:
        star = scene->addPolygon(polystar,colorThi,colorFig);
        star->resetTransform();
        star->setPos(x,y);
        star->setScale(scale);
        star->setRotation(rotate);
        break;
    case 37:
        pentagon = scene->addPolygon(polypenta,colorThi,colorFig);
        pentagon->resetTransform();
        pentagon->setPos(x,y);
        pentagon->setScale(scale);
        pentagon->setRotation(rotate);
        break;
    case 38:
        hexagon = scene->addPolygon(polyhex,colorThi,colorFig);
        hexagon->resetTransform();
        hexagon->setPos(x,y);
        hexagon->setScale(scale);
        hexagon->setRotation(rotate);
        break;
    case 39:
        rhomboid = scene->addPolygon(polyrhom,colorThi,colorFig);
        rhomboid->resetTransform();
        rhomboid->setPos(x,y);
        rhomboid->setScale(scale);
        rhomboid->setRotation(rotate);
        break;
    }


}

bool Logo::atob(double a){
    if(a >= 1.0){
        return true;
    } else {
        return false;
    }
}


void Logo::MachineVirtual(){
//int contador=0;

    MemoriaLocales *memoria_locales;

    memoria->salvarConstantes();

    double re; QString doc, s1; //allocation
    double value1, value2, temp, resul; int mod1, mod2, direccion; //expresiones
    bool logica;
    Input *read; //lectura
    int seguir; //entero donde seguira la funcion

    //donde se llena los cuadruplos
    int program[1000][4];
    int instr = 0;
    string line;
    ifstream myfile ("/home/ubuntu/Logo-master/Objecto.txt");
    if (myfile.is_open())
    {
      while ( getline (myfile,line) )
      {
          int lon = line.find("$");
          string operator_s = line.substr(0,lon);

          line=line.substr(line.find("$")+1);
          lon=line.find("$");
          string operando_1s = line.substr(0,lon);

          line=line.substr(line.find("$")+1);
          lon=line.find("$");
          string operando_2s = line.substr(0,lon);

          line=line.substr(line.find("$")+1);
          lon=line.find("$");
          string resul_s = line.substr(0,lon);

          int operator_i= atoi(operator_s.c_str());
          int operando_1i= atoi(operando_1s.c_str());
          int operando_2i= atoi(operando_2s.c_str());
          int resul_i= atoi(resul_s.c_str());

          program[instr][0]=operator_i;
          program[instr][1]=operando_1i;
          program[instr][2]=operando_2i;
          program[instr][3]=resul_i;

          instr++;


      }
      myfile.close();
    }

    else cout << "Unable to open file";


    //initializa atributos
    int x_position=0;
    int y_position=0;
    int rotate=0;
    int scale=1;

    QBrush colorFigure(Qt::yellow);
    QPen colorThick(Qt::black);

    //en cuanto ejecuta MemoriaVirtual() se limpia la ventana principal
        scene->clear();

        int cont=0;
        while(cont != 10000){
            int instruccion = program[cont][0];

            switch(instruccion){
            //GOtoMain
            case 0:
                cont=program[cont][3];
                break;
            //plus "+"
            case 1:
                value1=memoria->get(program[cont][1]);
                value2=memoria->get(program[cont][2]);
                temp=program[cont][3];
                resul=value1+value2;
                memoria->save(temp,resul);
                cont++;
                break;
            //minus "-"
            case 2:;
                value1=memoria->get(program[cont][1]);
                value2=memoria->get(program[cont][2]);
                temp=program[cont][3];
                resul=value1-value2;
                memoria->save(temp,resul);
                cont++;
                break;
            //multiply "*"
            case 3:
                value1=memoria->get(program[cont][1]);
                value2=memoria->get(program[cont][2]);
                temp=program[cont][3];
                resul=value1*value2;
                memoria->save(temp,resul);
                cont++;
                break;
            //divide
            case 4:
                value1=memoria->get(program[cont][1]);
                value2=memoria->get(program[cont][2]);
                temp=program[cont][3];
                resul=value1/value2;
                memoria->save(temp,resul);
                cont++;
                break;
            //module
            case 5:;
                value1=memoria->get(program[cont][1]);
                value2=memoria->get(program[cont][2]);
                temp=program[cont][3];
                mod1=(int)value1;
                mod2=(int)value2;
                resul=mod1%mod2;
                memoria->save(temp,resul);
                cont++;
                break;
            //equal "=="
            case 6:
                value1=memoria->get(program[cont][1]);
                value2=memoria->get(program[cont][2]);
                temp=program[cont][3];
                logica=(value1==value2);
                memoria->save(temp,logica);
                cont++;
                break;
            //no equal "!="
            case 7:
                value1=memoria->get(program[cont][1]);
                value2=memoria->get(program[cont][2]);
                temp=program[cont][3];
                logica=(value1!=value2);
                memoria->save(temp,logica);
                cont++;
                break;
            //greater than
            case 8:
                value1=memoria->get(program[cont][1]);
                value2=memoria->get(program[cont][2]);
                temp=program[cont][3];
                logica=(value1>value2);
                memoria->save(temp,logica);
                cont++;
                break;
            //less than
            case 9:
                value1=memoria->get(program[cont][1]);
                value2=memoria->get(program[cont][2]);
                temp=program[cont][3];
                logica=(value1<value2);
                memoria->save(temp,logica);
                cont++;
                break;
            //greater or equal than
            case 10:
                value1=memoria->get(program[cont][1]);
                value2=memoria->get(program[cont][2]);
                temp=program[cont][3];
                logica=(value1>=value2);
                memoria->save(temp,logica);
                cont++;
                break;
            //less or equal than
            case 11:
                value1=memoria->get(program[cont][1]);
                value2=memoria->get(program[cont][2]);
                temp=program[cont][3];
                logica=(value1<=value2);
                memoria->save(temp,logica);
                cont++;
                break;
            //and
            case 12:
                value1=memoria->get(program[cont][1]);
                value2=memoria->get(program[cont][2]);
                temp=program[cont][3];
                logica=(atob(value1) && atob(value2));
                memoria->save(temp,logica);
                cont++;
                break;
            //or
            case 13:
                value1=memoria->get(program[cont][1]);
                value2=memoria->get(program[cont][2]);
                temp=program[cont][3];
                logica=(atob(value1) || atob(value2));
                memoria->save(temp,logica);
                cont++;
                break;
            //allocation "="
            case 14:
                value1= memoria->get(program[cont][1]);
                direccion=program[cont][3];
                memoria->save(direccion, value1);
                cont++;
                break;
            //read
            case 15:
                value1=program[cont][3];
                read=new Input(*this , value1, &Logo::saveRead);
                read->exec();
                delete read;
                cont++;
                break;
            //print
            case 16:
               re=memoria->get(program[cont][1]);
               doc=ui->textMessage->toPlainText();
               s1= QString::number(re);
               ui->textMessage->setText(doc+ s1 +"\n");
                cont++;
                break;
           //gotoFalse
            case 17:
                value1=memoria->get(program[cont][1]);
                value2=0;
                if(value1 == value2 )
                    cont=program[cont][3];
                else
                    cont++;
                break;
           //gotoFin en caso de IF ELSE y no entre al ELSE
            case 18:
                 cont=program[cont][3];
                break;
           //gotoRetorno en caso de regresare al principio del WHILE
            case 19:
                cont=program[cont][3];
                break;
            //GotoTrue tambien en case de DO WHILE
            case 20:
                value1=memoria->get(program[cont][1]);
                value2=1;
                if(value1 == value2 )
                    cont=program[cont][3];
                else
                    cont++;
                break;
            //retorno
            case 21:
                value1=memoria->get(program[cont][1]);
                direccion=program[cont][3];
                memoria->save(direccion, value1);
                cont++;
                break;
            //assig
            case 22:
                value1=memoria->get(program[cont][1]);
                direccion=program[cont][3];
                memoria->save(direccion, value1);
                cont++;
                break;
            //RET
            case 23:
                memoria->despertarMemoria();
                cont=memoria->memoriaActual->instruccion;
                break;
            //ERA
            case 24:
                memoria_locales=new MemoriaLocales();
                //memoria_locales->direccionFuncion=program[cont][1];
                cont++;
                break;
            //PARA
            case 25:
                value1=memoria->get(program[cont][1]);
                memoria_locales->save(program[cont][3]-18000,value1);
                cont++;
                break;
            //GOTOSUB
            case 26:
                seguir = cont+1;
                memoria->memoriaActual->instruccion=seguir;
                memoria->dormirMemoria();
                memoria->colocarMemoria(memoria_locales);
                cont=program[cont][1];
                break;
            //ESTA CASE VA SER PARA INICIALIZAR LA FIGURA Y COLCARLA EN EL SCENE
            //DRAW
            case 30:
                x_position=0;
                y_position=0;
                rotate=0;
                scale=1;
                colorFigure.setColor(Qt::white);
                colorThick.setColor(Qt::black);
                cont++;
                break;
            //A PARTIR DE AQUI SE CAMBIAN LOS ATRIBUTOS DEPENDIENTE DE LOS QUE SE HAYA SALECCIONADO
            //x_position
            case 31:
                x_position=(int)memoria->get(program[cont][1]);
                cont++;
                break;
            //y_position
            case 32:
                y_position=(int)memoria->get(program[cont][1]);
                cont++;
                break;
            //rotate, antes rotateRight
            case 33:
                rotate=(int)memoria->get(program[cont][1]);
                 cont++;
                break;
            //rotateLeft -- ATENCION NO SIRVE
            case 34:
                //rotateLeft=(int)memoria->get(program[cont][1]);
                cont++;
                break;
            //size
            case 35:
                switch(program[cont][1]){
                case 1:
                    scale=1;//pequeño
                    break;
                case 2:
                    scale=2;//normal
                    break;
                case 3:
                    scale=3;//grande
                    break;
                }
                cont++;
                break;
            //thick
            case 36:
                 switch(program[cont][1]){
                 case 1:
                     colorThick.setStyle(Qt::SolidLine);
                     break;
                 case 2:
                     colorThick.setStyle(Qt::DashLine);
                     break;
                 case 3:
                     colorThick.setStyle(Qt::DotLine);
                     break;

            }
                cont++;
                break;
               //colorThick
            case 37:
                switch(program[cont][1]){
                case 1:
                    colorThick.setColor(Qt::blue);
                    break;
                case 2:
                    colorThick.setColor(Qt::yellow);
                    break;
                case 3:
                    colorThick.setColor(Qt::red);
                    break;
                case 4:
                    colorThick.setColor(QColor("#ff69b4")); //pink
                    break;
                case 5:
                    colorThick.setColor(QColor("#8b4513")); //brown
                    break;
                case 6:
                    colorThick.setColor(Qt::black);
                    break;
                case 7:
                    colorThick.setColor(Qt::white);
                    break;
                case 8:
                    colorThick.setColor(Qt::green);
                    break;
                case 9:
                    colorThick.setColor(Qt::gray);
                    break;
                case 10:
                    colorThick.setColor(QColor("#ffa500")); //orange
                    break;
                case 11:
                    colorThick.setColor(QColor("#a020f0")); //purple
                    break;
                }
                cont++;
                break;
            //colorFIgure
            case 38:
                switch(program[cont][1]){
                case 1:
                    colorFigure.setColor(Qt::blue);
                    break;
                case 2:
                    colorFigure.setColor(Qt::yellow);
                    break;
                case 3:
                    colorFigure.setColor(Qt::red);
                    break;
                case 4:
                    colorFigure.setColor(QColor("#ff69b4")); //pink
                    break;
                case 5:
                    colorFigure.setColor(QColor("#8b4513")); //brown
                    break;
                case 6:
                    colorFigure.setColor(Qt::black);
                    break;
                case 7:
                    colorFigure.setColor(Qt::white);
                    break;
                case 8:
                    colorFigure.setColor(Qt::green);
                    break;
                case 9:
                    colorFigure.setColor(Qt::gray);
                    break;
                case 10:
                    colorFigure.setColor(QColor("#ffa500")); //orange
                    break;
                case 11:
                    colorFigure.setColor(QColor("#a020f0")); //purple
                    break;
                }
                cont++;
                break;
            //END
            //finalmente se dibujan las figuras en la ventana
            case 50:
                switch(program[cont][1]){
                case 41:
                    break;
                case 42:
                    break;
                case 43:
                    dibujaFigura(33, x_position, y_position, rotate, scale, colorFigure, colorThick);
                    break;
                case 44:
                    dibujaFigura(34, x_position, y_position, rotate, scale, colorFigure, colorThick);
                    break;
                case 45:
                    dibujaFigura(35, x_position, y_position, rotate, scale, colorFigure, colorThick);
                    break;
                case 46:
                    dibujaFigura(36, x_position, y_position, rotate, scale, colorFigure, colorThick);
                    break;
                case 47:
                    dibujaFigura(37, x_position, y_position, rotate, scale, colorFigure, colorThick);
                    break;
                case 48:
                    dibujaFigura(38, x_position, y_position, rotate, scale, colorFigure, colorThick);
                    break;
                case 49:
                    dibujaFigura(39, x_position, y_position, rotate, scale, colorFigure, colorThick);
                    break;
                }
                cont++;
                break;
            //end program
            case 99:
                cont=10000;
                break;
            default:
                cont++;
                break;

            }
            //contador++;
        }

}
