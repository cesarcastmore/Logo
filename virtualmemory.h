#ifndef VIRTUALMEMORY_H
#define VIRTUALMEMORY_H
#include <map>
#include <string>
#include <fstream>

using namespace std;

namespace Ui {
class Memoria;
class MemoriaLocales;
}

class Instancia{
public: 	
	int direccion;
	double valor;
	Instancia *next;
	Instancia();

};

class ListaInstancias{
public:
	Instancia *actual;
	ListaInstancias();
	void save(int direccion, double valor);
	double get(int direccion);
 	void mostrarInstancias();
};


class MemoriaLocales{
	public :
	ListaInstancias *temporales;
	ListaInstancias *locales;
	MemoriaLocales *next;
    int instruccion;
    int direccionFuncion;
	MemoriaLocales();
	void save(int direccion, int value);
	void save(int direccion, bool value);
	void save(int direccion, double value);
	double get(int direccion);
	
};


class Memoria{
	public:
	ListaInstancias *globales;
	ListaInstancias *contantes;
	ListaInstancias *funciones;
    ListaInstancias *apuntadores;
	MemoriaLocales *memoriaActual;
	Memoria();
	void save(int direccion, int value);
	void save(int direccion, bool value);
	void save(int direccion, double value);
    void saveApuntador(int apuntador, int  direccion);
	void salvarConstantes();
	void dormirMemoria();
	void colocarMemoria(MemoriaLocales *memoria);
	void despertarMemoria();
	double get(int direccion);
	
};





#endif // VIRTUALMEMORY_H
