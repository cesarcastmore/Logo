#include "virtualmemory.h"
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iostream>

using namespace std;

//es un objecto que sera usado como instancia donde guardara direccion y valor
Instancia::Instancia(){
	direccion=0;
	valor=0;
	next=NULL;
	
}


ListaInstancias::ListaInstancias(){
	actual=NULL;
	}



void ListaInstancias::save(int direccion, double valor){
	Instancia *busqueda, *temp;
	if(actual == NULL){
	actual=new Instancia();
	actual->direccion=direccion;
	actual->valor=valor;
}
	else {
		busqueda=actual;
		while(busqueda != NULL){
				if(busqueda->direccion == direccion){
				busqueda->valor=valor;
				return;
			}
			busqueda=busqueda->next;
		}
		if(busqueda == NULL){
			temp=new Instancia();
			temp->direccion=direccion;
			temp->valor=valor;
			temp->next=actual;
			actual=temp;
		}
	}
	
	
}

double ListaInstancias::get(int direccion){
	Instancia *busqueda;
	busqueda= actual; 
	while(busqueda != NULL){
		if(busqueda->direccion == direccion){
			return busqueda->valor;
			}
		busqueda=busqueda->next;
	}

}



void ListaInstancias::mostrarInstancias(){
	Instancia *busqueda;
	busqueda= actual; 
	while(busqueda != NULL){
		busqueda=busqueda->next;
	}
	
	
}

MemoriaLocales::MemoriaLocales(){
	locales = new ListaInstancias();
	temporales = new ListaInstancias();
    instruccion=0; //bandera en caso de dormir memoria
    direccionFuncion=0;
}

//6000 --> 7999 son enteras, 8000 --> 9999 son flotantes, 10000 --> 11999 son booleanas
//6000 --> 11999 son locales, 12000 --> 17999 son temporales
void MemoriaLocales::save(int direccion, double value){
    if(6000 <= direccion and direccion < 12000 ){
	locales->save(direccion, value);
	}
    else if(12000 <= direccion and direccion < 18000){
		temporales->save(direccion, value);
	}
}

void MemoriaLocales::save(int direccion, int value){
	 double valor= (double)(value);
     save(direccion, valor);
}


void MemoriaLocales::save(int direccion, bool value){
	if(value == true){
		save(direccion , 1.0);
    }
    else{
         save(direccion , 0.0);
    }
}


double MemoriaLocales::get(int direccion){
	double valor;
    if(6000 <= direccion and direccion < 12000 ){
		valor=locales->get(direccion);
	}
    else if(12000 <= direccion and direccion < 18000){
		valor=temporales->get(direccion);
	}
	return valor;
	
}
	

Memoria::Memoria(){
	memoriaActual=new MemoriaLocales();
	globales= new ListaInstancias();
	contantes= new ListaInstancias();
	funciones = new ListaInstancias();
    apuntadores = new ListaInstancias();
}	


//0 --> 5999 son globales, 18000 --> 23999 son constantes, 30000 --> 31999 son funciones

void Memoria::save(int direccion, double value){
	if(0 <= direccion and direccion < 6000){
		globales->save(direccion, value);
	}
    else if (18000 <= direccion and direccion < 24000){
		contantes->save(direccion, value);
	}
	
	else if (30000 <= direccion and direccion < 32000){
		funciones->save(direccion, value);
	}
    else if	(32000 <= direccion and direccion < 34000){
        int dirValor=(int) (apuntadores->get(direccion));
        save(dirValor, value);
    }
	else if	(6000 <= direccion and direccion < 18000){
		memoriaActual->save(direccion, value);
	}
	
}

void Memoria::save(int direccion, int value){

    double valor=(double)(value);
	save(direccion, valor);
}


void Memoria::save(int direccion, bool value){
	if(value == true){
		save(direccion, 1.0);
		
	}
	else {
		save(direccion, 0.0);
	}
	
}

 void  Memoria::saveApuntador(int apuntador, int  direccion){
     apuntadores->save(apuntador, direccion);
 }


void Memoria::salvarConstantes(){
	string line;
    ifstream myfile ("/home/ubuntu/Logo/Constantes.txt");

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
	double valor;
	if(0 <= direccion and direccion < 6000){
		valor=globales->get(direccion);
	}
	else if (18000 <= direccion and direccion < 24000) {
		valor=contantes->get(direccion);
	}
	
	else if (30000 <= direccion and direccion < 32000){
		valor=funciones->get(direccion);
	}
    else if (32000 <= direccion and direccion < 34000){
        int dirValor= (int)(apuntadores->get(direccion));
        valor=get(dirValor);
    }
	else if	(6000 <= direccion and direccion < 18000){
		valor=memoriaActual->get(direccion);
	}
	return valor;
}
