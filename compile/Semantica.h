#ifndef SEMANTICA_H
#define SEMANTICA_H

#include "Scanner.h"
#include <map>
#include <string>
#include <fstream>

using namespace std;

namespace System { 

//clases 
class Parser;
class Errors;

class Dimension{
public:
	int Li;
	int Ls;
	int Mdim;
	int Dim;
	Dimension *next;
	Dimension();
};

class Dimensiones{
public:
	int R;
	int Dim;
	Dimension *actual;
	int tam;
	int K;
	int dirBase;
	Dimensiones();
	void agregar(Dimension* dim);
	void pop();
	Dimension *getDimension(int num);
	void invertir();
	void sacarK();
	void mostrarResultados();
	void sacarDirBase(int dir);
};

class Direction{
public:
	int direction;
	int type;
	Direction *next;
	Direction();
	Direction(int d, int t);
};

class Variable
{
public :
	std::wstring name;
	Direction *dir;
	int type; 
	int slope;
	Dimensiones *dimensiones;
	Variable();
	Variable(std::wstring n, int t, int slope, Direction *d);	

};

class DimensionLlamar{
public:	
	Dimensiones *id;
	int num; 
	int type;
	DimensionLlamar *next;
	DimensionLlamar();
	DimensionLlamar(int num, Dimensiones *id, int type);

};

class StackDimeLlam{
public:
	DimensionLlamar *actual;
	StackDimeLlam();
	void push(int num, Dimensiones *id, int type);
	void pop();
	int getNum();
	int getType();
	void setNum(int no);
	Dimensiones* getDimensiones(); 
	void showStackDimLlam();


};


class StackDirection{
public:	
	Direction *actual;
	StackDirection();
	void push(Direction* d);
	void push(int d, int t);
	void pop();
	Direction* get();
	int getType1();
	int getType2();
	void showStack();

};


class TablaVariables{
public:
	TablaVariables();
	void addGlobal(const wchar_t* n , int t, int slope, Direction *d);
	void addLocal(const wchar_t* n, int t, int slope, Direction *d);
	void removeAllLocals();
	void removeAllGlobals();
	bool checkOnlyLocal(const wchar_t* n);
	bool checkOnlyGlobal(const wchar_t* n);
	Variable* find(const wchar_t* n);
	void displayLocals();
	void displayGlobals();
};

class Parameter{
public:	
	int type;
	Parameter *next;
	Parameter();
	Parameter(int t);

};

class StackParameter{
public:
	Parameter *actual;
	StackParameter();
	void push(int t);
	void pop();
	Parameter* get();
	int  getInt();
	StackParameter* invert();
	void showStack();

};


class Function{
public: 	
	int type;//void int float
	std::wstring name;
	int parametro;
	int varLoc;
	int begin;
	int direction;
	StackParameter *par;
	Function *next;
	Function();

};

class StackFunction{
public:
	Function *actual;
	StackFunction();
	void push(Function *f);
	void pop();
	Function* get();
	Function* find(const wchar_t* n);
	void showStack();

};



class Operator{
public:
	int oper;
	Operator *next;
	Operator();
	Operator(int o);
};

class StackOperator{
public:
	Operator *actual;
	StackOperator();
	void push(int o);
	void pop();
	Operator* get();
	void pushFake();
	void popFake();
	void showStack();

};

class Leap{
public:
	int leaps;
	Leap *next;
	Leap();
	Leap(int l);
};

class StackLeap{
public:	
	Leap *actual;
	StackLeap();
	void push(int l);
	void pop();
	Leap* get();
	void showStack();

};
class LogicCube{
public:
	int cube[30][3][3];
	LogicCube();
	int allowed(int a, int b, int c);

};

class Cuadruplo{
public:
	int op, oper1, oper2, rec;
	Cuadruplo();
	Cuadruplo(int a, int b, int c, int d);
	const char* translate(int a);


};

class Memory{
public :
	int glob_int;
	int glob_flo;
	int glob_bool;
	int local_int;
	int local_flo;
	int local_bool;
	int temp_int;
	int temp_flo;
	int temp_bool;
	int cons_int;
	int cons_flo;
	int cons_bool;
	int para_int;
	int para_flo;
	int para_bool;
	int func;
	int apunt;
	Memory();
	Direction* save(int partition, int type, double value);
	void clearLocal();
	void clearTemp();
	void clearPara();

};



class Action{
public:
	LogicCube *cube;
	int cont;
	int rec;
	TablaVariables *tab;	
	StackOperator *op;
	StackDirection *dir;
	StackLeap *leap;
	StackFunction *fun;
	Function *inifun;
	StackParameter *para;
	Function *llam_fun;
	StackParameter *llam_para;
	int contPara;
	Memory *memory;
	Dimensiones *listaDimensiones;
	Dimension *dimension;
	StackDimeLlam *stackDim;
	int cant_loc;
	int cant_para;
	Action();
	void addGlobal(const wchar_t* n , int t, int slope);
	void addLocal(const wchar_t* n, int t, int slope);
	void removeLocals();
	void removeGlobals();
	Variable* find(const wchar_t* n);
	void addStackDir(int d, int t);
	void addStackOpe(int o);
	void createCuadroOpeTerm();
	void createCuadroOpeFact();	
	void createCuadroOpeRel();
	void createCuadroOpeLog();
	void createCuadroOpe();
	void addFake();
	void removeFake();
	void showCuadruplo();
	void addStackLeap(int l);
	void createGotoFalseCond();
	void createGotoFinCond();
	void createEndCond();
	void addStackLeapActual();
	void createGotoFalWhile();
	void createEndWhile();
	void createWhileDo();
	void createCuadrAlloca();
	void createCuadrRead(int d, int t);
	void createCuadrPrint();
	void beginDraw(int figure);
	void addAtributeInt(int attribute);
	void addAtributeString(int attribute, const wchar_t* n);
	void endDraw();
	void beginProgram();
	void beginMain();
	void endProgram();
	void createObject();
	void addTableFunction(Function *f);
	void addNameFunction(const wchar_t* n);
	void addParameter(int t);
	void addNoParameters();
	void addNoLocals();
	void addContCuadruplo();
	void generateRetorno();
	void addTypeFunction(int t);
	void findFunction(const wchar_t* n);
	void createERA();
	void createParameter();
	void checkParameter();
	void createGotoSub();
	void isDimension();
	void getDimension();
	void saveInferior(int i);
	void saveSuperior(int s);
	void getNewDimension();
	void getKCons();
	void getNextDirection(int partition, int type);
	void addDimensionLocal(const wchar_t* n);
	void addDimensionGlobal(const wchar_t* n);
	void getDimensionId(Variable* var);
	void generateVerifica();
	void updateStackDim();
	void getDirSumK();
};




}//end namespace
#endif
