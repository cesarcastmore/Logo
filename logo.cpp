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
using namespace std;

std::map<int , double> memoria;


MemoryVirtual::MemoryVirtual(){

}

double MemoryVirtual::get(int dir){
    return memoria[dir];
}

void MemoryVirtual::save(int dir, double value){
    memoria[dir]=value;
    if((dir >= 0000 and dir < 2000 ) or
            (dir >= 6000 and dir < 8000) or
            (dir >= 12000 and dir < 14000 ) or
            (dir >= 18000 and dir < 20000 ) or
            (dir >= 24000 and dir < 26000 )){
     int entero = (double)(value);
     value=(double)(entero);
     memoria[dir]=value;
    }
    else if((dir >= 2000 and dir < 4000) or
            (dir >= 8000 and dir < 10000 ) or
            (dir >= 14000 and dir < 16000 ) or
            (dir >= 20000 and dir < 22000 ) or
            (dir >= 26000 and dir < 28000 )
            ){
        memoria[dir]=value;
    }
}

void MemoryVirtual::save(int dir, bool value){
    if(value == true){
        double bo=1.00;
        memoria[dir]=bo;
    }
    else{
        double bo=0.0;
        memoria[dir]=bo;
    }
}

void MemoryVirtual::save(int dir, int value){
    int entero = (double)(value);
    value=(double)(entero);
    memoria[dir]=value;

}


void MemoryVirtual::saveCons(){
    string line;
    ifstream myfile ("/home/castillo/Logo/Constantes.txt");
    if (myfile.is_open())
    {
      while ( getline (myfile,line) )
      {
          int lon = line.find("$");
          string dir_s = line.substr(0,lon);

          line=line.substr(line.find("$")+1);
          lon=line.find("$");
          string value_s = line.substr(0,lon);


          int dir_i= atoi(dir_s.c_str());
          double value_f=atof(value_s.c_str());
          save(dir_i, value_f);


      }
      myfile.close();
    }
}

void MemoryVirtual::displayMemory(){
    cout <<"Memory\n";
    for (std::map<int , double >::iterator it=memoria.begin(); it!=memoria.end(); ++it){
        cout <<"Memory\n"<< it->first << " => " << it->second << "\n";
    }
}

void Logo::saveRead(int dir, double value){
    memoria[dir]=value;
}

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

    MemoryVirtual *memo= new MemoryVirtual();
    cout<<"entrooooo\n";

    //salvar las constante en un archivo
    memo->saveCons();
    memo->displayMemory();

    double re;QString doc, s1, s2;//allocation
    double value1, value2, temp, resul; int mod1, mod2;//expresiones
    bool logica;
     Input *read;//lectura

    int program[1000][4];
    int instr=0;
    string line;
    ifstream myfile ("/home/castillo/Logo/Objecto.txt");
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


          cout<<operator_i<<"|"<<operando_1i<<"|"<<operando_2i<<"|"<<resul_i<<"\n";



      }
      myfile.close();
    }

    else cout << "Unable to open file";

/*
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
                       {99,-1,-1,-1}} ;*/

        //initializa atributos
        int y_position=0;
        int x_position=0;
        QBrush colorFigure(Qt::blue);
        QPen colorThick(Qt::yellow);
        scene->clear();

        int cont=0;
        while(cont != 10000){

            int instruccion = program[cont][0];

            switch(instruccion){
            case 0:
                cont=program[cont][1];
                break;
            //plus
            case 1:
                value1=memo->get(program[cont][1]);
                value2=memo->get(program[cont][2]);
                temp=program[cont][3];
                resul=value1+value2;
                memo->save(temp,resul);
                cont++;
                break;
            //minus
            case 2:;
                value1=memo->get(program[cont][1]);
                value2=memo->get(program[cont][2]);
                temp=program[cont][3];
                cout<<"la direccion es "<<temp;
                resul=value1-value2;
                memo->save(temp,resul);
                cont++;
                break;
            //multiply
            case 3:
                value1=memo->get(program[cont][1]);
                value2=memo->get(program[cont][2]);
                temp=program[cont][3];
                resul=value1*value2;
                memo->save(temp,resul);
                cont++;
                break;
            //divide
            case 4:
                value1=memo->get(program[cont][1]);
                value2=memo->get(program[cont][2]);
                temp=program[cont][3];
                resul=value1/value2;
                memo->save(temp,resul);
                cont++;
                break;
            //module
            case 5:;
                value1=memo->get(program[cont][1]);
                value2=memo->get(program[cont][2]);
                temp=program[cont][3];
                mod1=(int)value1;
                mod2=(int)value2;
                resul=mod1%mod2;
                memo->save(temp,resul);
                cont++;
                break;
            //equal
            case 6:
                value1=memo->get(program[cont][1]);
                value2=memo->get(program[cont][2]);
                temp=program[cont][3];
                logica=(value1==value2);
                memo->save(temp,logica);
                cont++;
                break;
            //no equal
            case 7:
                value1=memo->get(program[cont][1]);
                value2=memo->get(program[cont][2]);
                temp=program[cont][3];
                logica=(value1!=value2);
                memo->save(temp,logica);
                cont++;
                break;
            //greater than
            case 8:
                value1=memo->get(program[cont][1]);
                value2=memo->get(program[cont][2]);
                temp=program[cont][3];
                logica=(value1>value2);
                memo->save(temp,logica);
                cont++;
                break;
            //less than
            case 9:
                value1=memo->get(program[cont][1]);
                value2=memo->get(program[cont][2]);
                temp=program[cont][3];
                logica=(value1<value2);
                memo->save(temp,logica);
                cont++;
                break;
            //greater or equal than
            case 10:
                value1=memo->get(program[cont][1]);
                value2=memo->get(program[cont][2]);
                temp=program[cont][3];
                logica=(value1>=value2);
                memo->save(temp,logica);
                cont++;
                break;
            //less or equal than
            case 11:
                value1=memo->get(program[cont][1]);
                value2=memo->get(program[cont][2]);
                temp=program[cont][3];
                logica=(value1<=value2);
                memo->save(temp,logica);
                cont++;
                break;
            //and
            case 12:
                cont++;
                break;
            //or
            case 13:
                cont++;
                break;
            //allocation
            case 14:
                double valor;
                valor= memo->get(program[cont][1]);
                int direccion;
                direccion= program[cont][3];
                memo->save(direccion, valor);
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
            //write
            case 16:
               re=memo->get(program[cont][1]);
               doc=ui->textMessage->toPlainText();
               s1= QString::number(re);
               ui->textMessage->setText(doc+ s1 +"\n");
                cont++;
                break;
           //gotoFalse
            case 17:
                value1=memo->get(program[cont][1]);
                value2=0;
                if(value1 == value2 )
                    cont=program[cont][3];
                else
                    cont++;
                break;
           //gotoFin
            case 18:
                 cont=program[cont][3];
                break;
           //gotoRetorno
            case 19:
                 cont=program[cont][3];
                break;
            //GotoMain
            case 20:
                cont++;
                break;
            case 21:
                cont++;
                break;
            case 22:
                cont++;
                break;
 /****************************************************************************************************/
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
                switch(program[cont][1]){
                case 1:
                    y_position=(int)(memo->get(program[cont][2]));
                    break;
                case 2:
                    x_position=(int)(memo->get(program[cont][2]));
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
                    switch(program[cont][2]){
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
                    switch(program[cont][2]){
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
                switch(program[cont][1]){
                case 1:
                    y_position=(int)(memo->get(program[cont][2]));
                    break;
                case 2:
                    x_position=(int)(memo->get(program[cont][2]));
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
                    switch(program[cont][2]){
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
                    switch(program[cont][2]){
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
                switch(program[cont][1]){
                case 34:
                    rectangle = scene->addRect(x_position, y_position, 50, 50, colorThick, colorFigure);
                    break;
                case 35:
                    ellipse = scene->addEllipse(x_position,y_position,100,100, colorThick, colorFigure);
                    break;
                }

                cont++;
                break;
 /**************************************************************************************************************/
           //end Program
            case 99:
                cont=10000;
                break;

            }
        }

}
