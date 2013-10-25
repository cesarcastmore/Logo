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

class MemoriaLocales{
public :
    std::map<int , double> locales;
    int instruccion;
    int direccionFuncion;
    MemoriaLocales *next;
    MemoriaLocales();
    void save(int direccion, int value);
    void save(int direccion, bool value);
    void save(int direccion, double value);
    double get(int direccion);

};


class Memoria{
public:
    std::map<int , double> glob_cons;
    MemoriaLocales *memoriaActual;
    Memoria();
    void save(int direccion, int value);
    void save(int direccion, bool value);
    void save(int direccion, double value);
    void salvarConstantes();
    void dormirMemoria();
    void colocarMemoria(MemoriaLocales *memoria);
    void despertarMemoria();
    double get(int direccion);


};


#endif // VIRTUALMEMORY_H
