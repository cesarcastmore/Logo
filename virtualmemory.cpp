#include "virtualmemory.h"
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iostream>

using namespace std;



MemoriaLocales::MemoriaLocales(){
    instruccion=0;
    direccionFuncion=0;
}

void MemoriaLocales::save(int direccion, int value){
     double valor=(double)(value);
     locales[direccion]=valor;
}

void MemoriaLocales::save(int direccion, double value){
    locales[direccion]=value;
}

void MemoriaLocales::save(int direccion, bool value){
    if(value == true){
        double bo=1.00;
        locales[direccion]=bo;
    }
    else{
        double bo=0.0;
         locales[direccion]=bo;
    }
}


double MemoriaLocales::get(int direccion){
    double valor=locales[direccion];
    return valor;

}


Memoria::Memoria(){
    memoriaActual=new MemoriaLocales();
}

void Memoria::save(int direccion, int value){
    if((0 <= direccion and direccion < 6000) or
        (18000 <= direccion and direccion < 24000) or
        (30000 <= direccion and direccion < 32000)){
            double valor=(int)(value);
            glob_cons[direccion] = valor;
            }
    else if	((6000 <= direccion and direccion < 18000) or
        (24000 <= direccion and direccion < 30000))
        memoriaActual->save(direccion, value);
}

void Memoria::save(int direccion, bool value){
    if((0 <= direccion and direccion < 6000) or
    (18000 <= direccion and direccion < 24000) or
    (30000 <= direccion and direccion < 32000)){
        if(value == true){
            double bo=1.00;
            glob_cons[direccion]=bo;
            }
            else{
                double bo=0.0;
                glob_cons[direccion]=bo;
                }
        }
        else if	((6000 <= direccion and direccion < 18000) or
            (24000 <= direccion and direccion < 30000))
            memoriaActual->save(direccion, value);

}

void Memoria::save(int direccion, double value){
    if((0 <= direccion and direccion < 6000) or
    (18000 <= direccion and direccion < 24000) or
    (30000 <= direccion and direccion < 32000)){
        glob_cons[direccion] = value;
    }
    else if	((6000 <= direccion and direccion < 18000) or
        (24000 <= direccion and direccion < 30000))
        memoriaActual->save(direccion, value);

}

void Memoria::salvarConstantes(){
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

void Memoria::dormirMemoria(){
    MemoriaLocales *new_memoria = new MemoriaLocales();
    new_memoria->next=memoriaActual;
    memoriaActual=new_memoria;
}

void Memoria::colocarMemoria(MemoriaLocales *memoria){
    memoriaActual->locales= memoria->locales;

}

void Memoria::despertarMemoria(){
    memoriaActual=memoriaActual->next;
}

double  Memoria::get(int direccion){
    double retorno;
    if((0 <= direccion and direccion < 6000) or
    (18000 <= direccion and direccion < 24000) or
    (30000 <= direccion and direccion < 32000)){
        retorno=glob_cons[direccion];
    }
    else if	((6000 <= direccion and direccion < 18000) or
        (24000 <= direccion and direccion < 30000))
        retorno=memoriaActual->get(direccion);

        return retorno;
}


