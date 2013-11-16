#include "Semantica.h"
#include "Parser.h"
#include <string>
#include <iostream>
#include <map>
#include <fstream>

using namespace std;

namespace System{


std::map< std::wstring , Variable*> tablaGlobals;
std::map<std::wstring , Variable*> tablaLocals;		
std::map<int , Cuadruplo*> record;
std::map<int , double> contantes;

/*Objecto que guardara una instancia en la tabla de variables*/
Variable::Variable(){
type=0;
slope=0;
dir=new Direction();
dimensiones=NULL;
}

/*Inicializa la varialble con un name, type, slope y objecto Direcion*/
Variable::Variable(wstring n, int t, int slope, Direction *d){
type=t;
name=n;
slope=slope;
dir=d; 
dimensiones=NULL;
}


/*Inicializa la tabla de variables*/
TablaVariables::TablaVariables(){
	
}

/*Regresa un true si el nombre de una variables en locales es unica*/
bool TablaVariables::checkOnlyLocal(const wchar_t* n){
	std::wstring identif=std::wstring(n);
	//buscar en local
		for (std::map<std::wstring , Variable* >::iterator it=tablaLocals.begin(); it!=tablaLocals.end(); ++it)
			if(it->first == identif){
				return false; 
			}
	return true;
		
}


/*Regresa un true si el nombre de una variables en globales es unica*/
bool TablaVariables::checkOnlyGlobal(const wchar_t* n){
	std::wstring identif=std::wstring(n);
	//buscar en global
		for (std::map<std::wstring , Variable* >::iterator it=tablaGlobals.begin(); it!=tablaGlobals.end(); ++it)
			if(it->first == identif ){
				return false;
			}
	return true;
		
}


/*Agrega un Variable global a la tabla de variables */
void TablaVariables::addGlobal(const wchar_t* n, int  t, int slope, Direction *d){
	std::wstring identif=std::wstring(n);
	if(checkOnlyGlobal(n)){
		Variable *temp = new Variable(n,t, slope, d);
		tablaGlobals.insert(std::make_pair(identif, temp));	
	}
	else wcout<<"There are two variables with the same name \""<<n<<"\n";
}

/*Agrega un Variable local a la tabla de variables */
void TablaVariables::addLocal(const wchar_t* n, int t, int slope, Direction *d){
	std::wstring identif=std::wstring(n);
	if(checkOnlyLocal(n)){
		Variable *temp = new Variable(n,t,slope, d);
		tablaLocals.insert(std::make_pair(identif, temp));		
	}
	else wcout<<"There are two variables with the same name \""<<n<<"\n";
}

/*Limpia todas las variables de locales*/		
void TablaVariables::removeAllLocals(){
	tablaLocals.clear();
	
}

/*Limpia todas las variables de globales*/
void  TablaVariables::removeAllGlobals(){
	tablaGlobals.clear();
}

/*Busca un variable con un nombre en la tabla de variables 
 * y si lo encuentra regresa un objecto Variable*/
Variable* TablaVariables::find(const wchar_t* n){
	std::wstring identif=std::wstring(n);
	
	for (std::map<std::wstring , Variable* >::iterator it=tablaLocals.begin(); it!=tablaLocals.end(); ++it)			
		if(it->first == identif) 
			return it->second;
	
	for (std::map<std::wstring , Variable* >::iterator it=tablaGlobals.begin(); it!=tablaGlobals.end(); ++it)
		if(it->first == identif)
			return it->second;		
			
	wcout<<"Error: The variable \""<<identif <<"\" has not been initialized yet.\n";


	return new Variable();

}

/*Muestra todas la variables locales. es usado para debuggear*/
void TablaVariables::displayLocals(){
  wcout << "Local Variables Table contains:\n";
  for (std::map<std::wstring , Variable* >::iterator it=tablaLocals.begin(); it!=tablaLocals.end(); ++it){
    wcout << it->first << " => " << it->second->type << "--" << it->second->slope <<"---direction--"<<it->second->dir->direction<<'\n';
    if(it->second->dimensiones != NULL){
		wcout<<"MOSTRANDO DIMENSION\n";
		it->second->dimensiones->mostrarResultados();
	}
}
}

/*Muestra todas la variables globales es usado para debuggear*/
void TablaVariables::displayGlobals(){
  cout << "Global Variables Table contains:\n";
  for (std::map<std::wstring , Variable* >::iterator it=tablaGlobals.begin(); it!=tablaGlobals.end(); ++it){
    wcout << it->first << " => type " << it->second->type << "-- function" << it->second->slope <<'\n';
		if(it->second->dimensiones != NULL){
			wcout<<"MOSTRANDO DIMENSION\n";
			it->second->dimensiones->mostrarResultados();
		}
	}
}

/*Objecto para un parametro*/
Parameter::Parameter(){
	next=NULL;
	type=0;
}

/*Inicializa el paramtro con el tipo de datos*/	
Parameter::Parameter(int t){
	type=t;
	next=NULL;
}


/*Pila para tener un control de la cantidad de parametros declarados*/
StackParameter::StackParameter(){
	actual=new Parameter();
}

/*Se agrega un tipo de dato a la pila de parametros*/
void StackParameter::push(int t){
	Parameter *new_node;
	new_node=new Parameter(t);
	
   if(!actual){
   actual=new_node;
}	 
      else{
      new_node->next = actual;
      actual = new_node;
      }	
}

/*Se saca un parametro a la pila*/
void StackParameter::pop(){
	actual = actual->next;
}

/*Se obtiene el top de la pila de parametros, regresa un objecto Parametro*/
Parameter* StackParameter::get(){
	Parameter *new_node;
	new_node=new Parameter(actual->type);
	return new_node;
}

/*Invierte toda la pila para tener el orden en la que fue declarada*/
StackParameter* StackParameter::invert(){
	Parameter *new_node = new Parameter();
	StackParameter *invertir =new StackParameter();
	new_node=actual;
	while(new_node->next != NULL){
		invertir->push(new_node->type);
		new_node = new_node->next;
	}
	
	return invertir;
	
}


/*Muestra la pila. Usado para debuggear*/
void StackParameter::showStack(){
	wcout<<"\tThe stack contains StackParameter\n";
	Parameter *par=actual;
	while(par->next != NULL){
		wcout<<"\tthe type is "<< par->type<<"\n";
		par=par->next;
	} 
}

/*Instancia para tener un control en las funcciones*/
Function::Function(){
	type=0;
	parametro=0;
	varLoc=0;
	begin=0;
	par=new StackParameter();
	next=NULL;
	direction=-1;
}

/*Si inicializa la pila de Funcciones*/
StackFunction::StackFunction(){
	actual=new Function();
}

/*Se agrega la funcion en el extremo de la pila de Funcciones*/
void StackFunction::push(Function *function){
	Function *new_node;
	new_node=function;
	
   if(!actual){
   actual=new_node;
}	 
      else{
      new_node->next = actual;
      actual = new_node;
      }	
}

/* Se saca el top de la pila de funciones*/
void StackFunction::pop(){
	actual = actual->next;
}

/*Obtiene el top de la pila de funcciones, regresa un objecto funccion*/
Function* StackFunction::get(){
	Function *new_node;
	new_node=actual;
	return new_node;
}

/*busca una funcion con un nombre, si lo encuentra regresa la funccion*/
Function* StackFunction::find(const wchar_t* n){
	std::wstring iden=std::wstring(n);
	Function *new_node = new Function();
	new_node=actual;
	while(new_node != NULL){
		if(new_node->name == iden)
		return new_node;
		new_node=new_node->next;
	}
	
	return new Function();
	
	}

/*muestra todo lo que contiene la pila de funcciones, usado para debugear*/
void StackFunction::showStack(){
	wcout<<"The Stack contains StackFunction\n";
	Function *fun=actual;
	while(fun->next != NULL){
		wcout<<"the name is "<< fun->name<<"\n";
		wcout<<"the type is "<< fun->type<<"\n";
		wcout<<"the varLoc is "<< fun->varLoc<<"\n";;
		wcout<<"the begin is "<< fun->begin<<"\n";
		wcout<<"the parameter is "<< fun->parametro<<"\n";
		fun->par->showStack();
		fun=fun->next;
	} 
}

/*Objecto para almacenar los datos de un direccion*/
Direction::Direction(){
	next=NULL;
	direction=-1;
	type=0;
	}
	
/*Inicializa la Direccion con un entero y con un tipo*/	
Direction::Direction(int d, int t){
	next=NULL;
	direction=d;
	type=t;
	}
	
/*Inicializa la pila de direcciones*/
StackDirection::StackDirection(){
	actual=new Direction();
}

/*agrega un tipo de dato y una direccion en la pila de direcciones*/
void StackDirection::push(int d, int t){
	Direction *new_node;
	new_node=new Direction(d,t);
	
   if(!actual ){
   actual=new_node;
}	 
      else{
      new_node->next = actual;
      actual = new_node;
      }
}


/*agrega una direccion en la pila de direcciones*/
void StackDirection::push(Direction* dir){
	
   if(!actual ){
   actual=dir;
}	 
      else{
      dir->next = actual;
      actual = dir;
      }
}

/* saca el tope de la pila de direcciones*/
void StackDirection::pop(){
	actual = actual->next;
}

/*Obtiene la direccion top de la pila de direcciones*/
Direction* StackDirection::get(){
	Direction *new_node;
	new_node=new Direction(actual->direction, actual->type);
	return new_node;
}

/*obtiene el tipo pero es el ultimo de la pila de direcciones*/
int StackDirection::getType1(){
	Direction *new_node;
	new_node=actual;
	return new_node->type;
}

/*obtiene el tipo pero es el penultimo de la pila de direcciones*/
int StackDirection::getType2(){
	Direction *new_node;
	new_node=actual;
	return new_node->next->type;
}


/*Muestra el contenido de la pila de direcciones, usado para debuggear*/	
void StackDirection::showStack(){
	wcout<<"The Stack contains StackDirection\n";
	Direction *dis=actual;
	while(dis != NULL){
		wcout<<"the value is "<< dis->direction;
		wcout<<"----"<< dis->type<<"\n";
		dis=dis->next;
	} 
}

/*Objecto Operador*/
Operator::Operator(){
	next=NULL;
	oper=0;
}

	
/*inicializa el operador con el tipo de operador*/	
Operator::Operator(int o){
	oper=o;
	next=NULL;
}


/*Se declara una pila del Operador*/
StackOperator::StackOperator(){
	actual=new Operator();
}

/*Agrega un tipo de  operador a la pila*/
void StackOperator::push(int o){
	Operator *new_node;
	new_node=new Operator(o);
	
   if(!actual){
   actual=new_node;
}	 
      else{
      new_node->next = actual;
      actual = new_node;
      }	
}

/*Saca el top de la pila de operadores*/
void StackOperator::pop(){
	actual = actual->next;
}

/*Obtiene el top de la pila de operadores*/
Operator* StackOperator::get(){
	Operator *new_node;
	new_node=new Operator(actual->oper);
	return new_node;
}

/*Agrega un fondo falso a la pila de operadores*/
void StackOperator::pushFake(){
	Operator *new_node;
	new_node=new Operator(1000);
   if(!actual){
   actual=new_node;
}	 
      else{
      new_node->next = actual;
      actual = new_node;
      }	
}

/*remueve el fondo falso en la pila de operadores*/
void StackOperator::popFake(){
		actual = actual->next;
}

/*muestra la pila de operadores, usado para debugear*/
void StackOperator::showStack(){
	wcout<<"The Stack contains StackOperator\n";
	Operator *op=actual;
	while(op != NULL){
		wcout<<"the value is "<< op->oper<<"\n";
		op=op->next;
	} 	
}

/*Objecto de saltos*/
Leap::Leap(){
	next=NULL;
	leaps=0;
}

/*Inicializa el salto con un entero que seran el numero de instruccion*/
Leap::Leap(int o){
	leaps=o;
	next=NULL;
}

/*Inicializa la pila de saltos*/
StackLeap::StackLeap(){
	actual=new Leap();
}

/*agrega un contador en la pila de saltos*/
void StackLeap::push(int p){
	Leap *new_node;
	new_node=new Leap(p);
	
   if(!actual){
   actual=new_node;
}	 
      else{
      new_node->next = actual;
      actual = new_node;
      }	
}

/*quita el top de la pila de saltos*/
void StackLeap::pop(){
	actual = actual->next;
}

/*obtiene el top de la pila de saltos*/
Leap* StackLeap::get(){
	Leap *new_node;
	new_node=new Leap(actual->leaps);
	return new_node;
}


/*muestra el contenido de la pila de saltos, usado para debuggear*/
void StackLeap::showLeap(){
	wcout<<"The Stack contains StackLeap\n";
	 Leap *leap=actual;
	while(leap != NULL){
		wcout<<"the value is "<< leap->leaps<<"\n";
		leap=leap->next;
	} 	
}	*/

/*inicialza todos los valores de la memoria*/
Memory::Memory(){
	glob_int=0000;
	glob_flo=2000;
	glob_bool=4000;
	local_int=6000;
	local_flo=8000;
	local_bool=10000;
	temp_int=12000;
	temp_flo=14000;
	temp_bool=16000;
	cons_int=18000;
	cons_flo=20000;
	cons_bool=22000;
	para_int=24000;
	para_flo=26000;
	para_bool=28000;
	func=30000;
	apunt=32000;
}


/*Obtiene un direccion depediendi de la particio y del tipo*/
Direction* Memory::save(int partition, int type, double value){
	
	if(partition==0 and type == 1){
		Direction *new_dir=new Direction(glob_int, type);
		glob_int++;
		return new_dir;
	}
	else if(partition==0 and type == 2){
		Direction *new_dir=new Direction(glob_flo, type);
		glob_flo++;
		return new_dir;
	}
	else if(partition==0 and type == 3){
		Direction *new_dir=new Direction(glob_bool, type);
		glob_bool++;
		return new_dir;
	}
	else if(partition == 1 and type ==1 ){
		Direction *new_dir=new Direction(local_int, type);
		local_int++;
		return new_dir;
	}
	else if(partition == 1 and type == 2){
		Direction *new_dir=new Direction(local_flo, type);
		local_flo++;
		return new_dir;
	}
	else if(partition == 1 and type == 3){
		Direction *new_dir=new Direction(local_bool, type);
		local_bool++;
		return new_dir;
	}
	else if(partition == 2 and type == 1){
		Direction *new_dir=new Direction(temp_int, type);
		temp_int++;
		return new_dir;
	}
	else if(partition == 2 and type == 2){
		Direction *new_dir=new Direction(temp_flo, type);
		temp_flo++;
		return new_dir;
	}
	else if(partition == 2 and type == 3){
		Direction *new_dir=new Direction(temp_bool, type);
		temp_bool++;
		return new_dir;
	}
	else if(partition == 3 and type == 1){
		contantes[cons_int]=value;
		Direction *new_dir=new Direction(cons_int, type);
		cons_int++;
		return new_dir;
	}
	else if(partition == 3 and type == 2){
		contantes[cons_flo]=value;
		Direction *new_dir=new Direction(cons_flo, type);
		cons_flo++;
		return new_dir;
	}
	else if(partition == 3 and type == 3){
		contantes[cons_bool]=value;
		Direction *new_dir=new Direction(cons_bool, type);
		cons_bool++;
		return new_dir;
	}
		else if(partition == 4 and type == 1){
		Direction *new_dir=new Direction(para_int, type);
		para_int++;
		return new_dir;
	}
		else if(partition == 4 and type == 2){
		Direction *new_dir=new Direction(para_flo, type);
		para_flo++;
		return new_dir;
	}
		else if(partition == 4 and type == 3){
		Direction *new_dir=new Direction(para_bool, type);
		para_bool++;
		return new_dir;
	}
		else if(partition == 5 and type == 0){
		Direction *new_dir=new Direction(func, type);
		func++;
		return new_dir;
	}
	else if(partition == 6){
		Direction *new_dir=new Direction(apunt, type);
		apunt++;
		return new_dir;
	}
	return new Direction(-1, 0);
}

/*inicializa todas las temporales cuando entra a una funcion*/
void Memory::clearTemp(){
	temp_int=12000;
	temp_flo=14000;
	temp_bool=16000;
	
}

/*inicializa todas la locales cuando entra a una funcion*/
void Memory::clearLocal(){
	local_int=6000;
	local_flo=8000;
	local_bool=10000;
	
}

/*inicializa todas los parametros cuando entra a una funcion*/
void Memory::clearPara(){
	para_int=24000;
	para_flo=26000;
	para_bool=28000;
	
}

/*inicializa un cuadruplo*/
Cuadruplo::Cuadruplo(){
}

/*inicializa un cuadrupo con cuatro valores, cont, operador, operando1 operando2*/
Cuadruplo::Cuadruplo(int a, int b, int c, int d){
	op=a;
	oper1=b;
	oper2=c;
	rec=d;
}


/*traduce el numero de instruccion a un cadena*/
const char* Cuadruplo::translate(int a){
	switch(a){
		case 0: return "GotoMain";
		case 1: return "+";
		case 2: return "-";
		case 3: return "*";
		case 4: return "/";
		case 5: return "%";
		case 6: return "==";
		case 7: return "<>";
		case 8: return ">";
		case 9: return "<";
		case 10: return ">=";
		case 11: return "<=";
		case 12: return "and";
		case 13: return "or";
		case 14: return "=";
		case 15: return "read";
		case 16: return "print";
		case 17: return "gotoFalse";
		case 18: return "gotoFin";
		case 19: return "gotoRetorno";
		case 20: return "gotoTrue";
		case 21: return "retorno";
		case 22: return "assig";
		case 23: return "ret";
		case 24: return "era";
		case 25: return "para";
		case 26: return "gotoSub";
		case 30: return "beginDraw";
		case 31: return "x_position";
		case 32: return "y_position";
		case 33: return "rotateRight";
		case 34: return "rotateLeft";
		case 35: return "size";
		case 36: return "thick";
		case 37: return "coloThick";
		case 38: return "colorFigure";
		case 41: return "point";
		case 42: return "line";
		case 43: return "triangle";
		case 44: return "square";
		case 45: return "circle";
		case 46: return "star";
		case 47: return "pentagon";
		case 48: return "hexagon";
		case 49: return "rhomboid";
		case 50: return "endDraw";
		case 51: return "Verifica";
		case 52: return "multiArray";
		case 53: return "sumaArray";
		case 54: return "sumaK";
		case 55: return "obtieneDir";
		case 99: return "end";
		
	}
	return "-1";
}

/*
 *
Operadores
1="+" 2="-" 3="*" 4="/" 5="%" 6="==" 7="<>" 8=">" 9="<" 10=">="  11="<=" 12="and" 13="or"
Operando
0=undef 1=int 2=float 3=bool
 * */

/*se inicializa el cubo semantico*/ 
LogicCube::LogicCube(){
	//operador, tipo1, tipo2 
	cube[1][1][1]=1;cube[1][1][2]=2;cube[1][1][3]=0;cube[1][2][1]=2;cube[1][2][2]=2;cube[1][2][3]=0;cube[1][3][1]=0;cube[1][3][2]=0; cube[1][3][3]=0;//suma
	cube[2][1][1]=1;cube[2][1][2]=2;cube[2][1][3]=0;cube[2][2][1]=2;cube[2][2][2]=2;cube[2][2][3]=0;cube[2][3][1]=0;cube[2][3][2]=0; cube[2][3][3]=0;//resta
	cube[3][1][1]=1;cube[3][1][2]=2;cube[3][1][3]=0;cube[3][2][1]=2;cube[3][2][2]=2;cube[3][2][3]=0;cube[3][3][1]=0;cube[3][3][2]=0; cube[3][3][3]=0;//mult
	cube[4][1][1]=2;cube[4][1][2]=2;cube[4][1][3]=0;cube[4][2][1]=2;cube[4][2][2]=2;cube[4][2][3]=0;cube[4][3][1]=0;cube[4][3][2]=0; cube[4][3][3]=0;//divi
	cube[5][1][1]=1;cube[5][1][2]=0;cube[5][1][3]=0;cube[5][2][1]=0;cube[5][2][2]=0;cube[5][2][3]=0;cube[5][3][1]=0;cube[5][3][2]=0; cube[5][3][3]=0;//module
	cube[6][1][1]=3;cube[6][1][2]=3;cube[6][1][3]=0;cube[6][2][1]=3;cube[6][2][2]=3;cube[6][2][3]=0;cube[6][3][1]=0;cube[6][3][2]=0; cube[6][3][3]=3;//equal
	cube[7][1][1]=3;cube[7][1][2]=3;cube[7][1][3]=0;cube[7][2][1]=3;cube[7][2][2]=3;cube[7][2][3]=0;cube[7][3][1]=0;cube[7][3][2]=0; cube[7][3][3]=3;//not equals
	cube[8][1][1]=3;cube[8][1][2]=3;cube[8][1][3]=0;cube[8][2][1]=3;cube[8][2][2]=3;cube[8][2][3]=0;cube[8][3][1]=0;cube[8][3][2]=0; cube[8][3][3]=0;//greater than
	cube[9][1][1]=3;cube[9][1][2]=3;cube[9][1][3]=0;cube[9][2][1]=3;cube[9][2][2]=3;cube[9][2][3]=0;cube[9][3][1]=0;cube[9][3][2]=0; cube[9][3][3]=0;//less than
	cube[10][1][1]=3;cube[10][1][2]=3;cube[10][1][3]=0;cube[10][2][1]=3;cube[10][2][2]=3;cube[10][2][3]=0;cube[10][3][1]=0;cube[10][3][2]=0; cube[10][3][3]=0;//greater than or equal to
	cube[11][1][1]=3;cube[11][1][2]=3;cube[11][1][3]=0;cube[11][2][1]=3;cube[11][2][2]=3;cube[11][2][3]=0;cube[11][3][1]=0;cube[11][3][2]=0; cube[11][3][3]=0;//Less than or equal to
	cube[12][1][1]=0;cube[12][1][2]=0;cube[12][1][3]=0;cube[12][2][1]=0;cube[12][2][2]=0;cube[12][2][3]=0;cube[12][3][1]=0;cube[12][3][2]=0; cube[12][3][3]=3;//and
	cube[13][1][1]=0;cube[13][1][2]=0;cube[13][1][3]=0;cube[13][2][1]=0;cube[13][2][2]=0;cube[13][2][3]=0;cube[13][3][1]=0;cube[13][3][2]=0; cube[13][3][3]=3;//or
 		
}
	
/*se inicializa el cubo semantico con un operador y dos operadores*/ 	
int LogicCube::allowed(int a, int b, int c){
	return cube[a][b][c];
}

/*inicializa el objecto dimension*/
Dimension::Dimension(){
	next=NULL;
	Li=0;
	Ls=0;
	Mdim=0;
}


/*inicializa la pila de dimensiones*/
Dimensiones::Dimensiones(){
	Dim=1;
	dirBase=0;
	actual=NULL;
}


/*coloca una dimension en la pila de dimensiones y hace algunos calculos*/
void Dimensiones::agregar(Dimension* dim){
	R= ( dim->Ls - dim->Li + 1) * R;
	if(actual == NULL)
	actual=dim;
	else{
		dim->next=actual;
		actual=dim;
	}
}

/*obtiene la dimension dependiendo del numero que se le da*/
Dimension* Dimensiones::getDimension(int num){
	Dimension *busqueda, *retorno;
	retorno = new Dimension();
	busqueda= actual;
	while(busqueda != NULL){
		if(busqueda->Dim == num){
			retorno->Dim=busqueda->Dim;
			retorno->Li=busqueda->Li;
			retorno->Ls=busqueda->Ls;
			retorno->Mdim=busqueda->Mdim;
			return retorno;
			}
			busqueda=busqueda->next;
			}
			return NULL;

}

/*Obtiene la k despues de obtene todas las dimensiones*/
void Dimensiones::sacarK(){
	invertir();
	tam=R;
	Dimension *temp=actual;
	while(temp !=  NULL){
		temp->Mdim= (R / (temp->Ls  - temp->Li + 1));
		R=temp->Mdim;
		K= K + temp->Li * temp->Mdim;
		temp=temp->next;
	}
	K=(K * -1);	
}


/*Invierte toda la pila */
void Dimensiones::invertir(){
	Dimensiones *invertir=new Dimensiones();
	Dimension *temp;
	while(actual != NULL){
		temp= new Dimension();
		temp->Li=actual->Li;
		temp->Ls=actual->Ls;
		temp->Dim=actual->Dim;
		invertir->agregar(temp);
		actual=actual->next;
	}
	actual=invertir->actual;
}


/*Mostrar los resultados de la pila de dimensiones, usado para debuggear */ 
void  Dimensiones::mostrarResultados(){
	Dimension *temp=actual;
	wcout<<"Numero Dimensiones"<<Dim<<"\n";
	wcout<<"K = "<<K<<"\n";
	wcout<<"tam = "<<tam<<"\n";
	while(temp != NULL){
		wcout<<"DIMENSION "<<temp->Dim<<"\n";
		wcout<<"Li "<<temp->Li<<"\n";
		wcout<<"Ls "<<temp->Ls<<"\n";
		wcout<<"Mdim "<<temp->Mdim<<"\n";
		temp=temp->next;
	}
}

/*Sacar la direccion base*/
void Dimensiones::sacarDirBase(int dir){
	dirBase=dir+ tam;
}

/*Sacar el top de la pila de dimensiones*/
void Dimensiones::pop(){
	actual=actual->next;
}


/*Objecto para llevar un contro cuando se  anidan mas de dos dimensiones*/
DimensionLlamar::DimensionLlamar(){
	id=NULL;
	next=NULL;
}

/* Se agregan los datos al control de llamada de dimensiones*/
DimensionLlamar::DimensionLlamar(int numero, Dimensiones *name, int t){
	num=numero;
	id=name;
	type=t;
	next=NULL;
}


StackDimeLlam::StackDimeLlam(){
	actual=NULL;
}

/*Se agrega una lista de dimensiones al control de dimensiones*/
void StackDimeLlam::push(int numero, Dimensiones *id, int t){
	DimensionLlamar *new_node;
	new_node=new DimensionLlamar(numero, id, t);
   if(!actual){
   actual=new_node;
}	 
      else{
      new_node->next = actual;
      actual = new_node;
      }	
		
}

/*Se saca una lista de dimensiones al control de dimensionesm de llamada*/
void StackDimeLlam::pop(){
	actual = actual->next;	
	}

/*Obtiene el numero de dimension que va la llamada*/	
int StackDimeLlam::getNum(){
	int no=actual->num;
	return no;
	}

/*Obtiene el numero de dimension que va la llamada*/	
int StackDimeLlam::getType(){
	int tipo=actual->type;
	return tipo;
	}

/*Actualza el numero en que va la llamada de dimensiones*/	
void StackDimeLlam::setNum(int no){
	actual->num=no;
}

/*Obtiene la lista de dimensiones*/	
Dimensiones* StackDimeLlam::getDimensiones(){
	Dimensiones *dimen=actual->id;
	return dimen;
}

/*Muestra lo que contiene la lista de dimensiones de llamada*/
void StackDimeLlam::showStackDimLlam(){
	
	wcout<<"The Stack contains StackDimeLlam\n";
	DimensionLlamar *dis=actual;
	while(dis != NULL){
		dis->id->mostrarResultados();
		wcout<<"the dimension----"<<dis->num<<"\n";
		dis=dis->next;
	} 
	
}


/*Cerebro de toda la semantica*/
Action::Action(){
	//tabla de variables
	tab=new TablaVariables();
	//pila de operadores
	op=new StackOperator();
	//pila de direcciones
	dir=new StackDirection();
	//pila de saltos
	leap= new StackLeap();
	//Cubo Semantico
	cube=new LogicCube();
	//Obtiene direcciones
	memory=new Memory();
	//cont de parametros
	contPara=0;
	
	//variables para crear procedimientos
	inifun=new Function();
	para= new StackParameter();
	
	//tabla de funciones
	fun=new StackFunction();
	
	//son las variables que se utilizaran para hacer la llamada de funciones
	llam_fun=new Function();
	
	
	cont=0;
	cant_loc=0;
	cant_para=0;
	
	//inicializacion de dimensiones 
	listaDimensiones = new Dimensiones();
	dimension = new Dimension();
	
	//llamado de dimensiones
	stackDim= new StackDimeLlam();
	}

/*Agregar una variable global*/	
void Action::addGlobal(const wchar_t* n, int  t, int slope){
	if(slope == 0){ //var
		Direction *d = memory->save(0, t, -1);
		tab->addGlobal(n,t,slope, d);
	}
	else if(slope == 2){//function
		Direction *d = memory->save(5, t, -1);
		tab->addGlobal(n,t,slope, d);
	}
}

/*Agregar una variable local*/
void Action::addLocal(const wchar_t* n, int  t, int slope){
	Direction *d;
	//si es parametro
	if(slope == 1){
		d = memory->save(1, t, -1);
		cant_para++;
	}
	// es local
	else if(slope == 0){
		d = memory->save(1, t, -1);
		cant_loc++;
	}
	tab->addLocal(n,t,slope,d);
	
}

/*encontra una variable en la tabla de variables*/
Variable* Action::find(const wchar_t* n){
	Variable* var =tab->find(n);
	return var;
}

/*limpiar las direcciones locales e inicializarlas*/ 
void Action::removeLocals(){
	tab->removeAllLocals();
	memory->clearLocal();
	memory->clearTemp();
	memory->clearPara();
	cant_loc=0;
	cant_para=0;
	para=new StackParameter();
	
}

/*limpiar las direcciones globales e inicializarlas*/ 
void Action::removeGlobals(){
	tab->removeAllGlobals();
}

/*Agregar una direccion a la pila de direcciones*/
void Action::addStackDir(int d, int t){
	dir->push(d,t);
}

/*Agregar un operador a la pila de operadores*/
void Action::addStackOpe(int o){
	op->push(o);
}

/*crea un cuadrupledo de operador*/
void Action::createCuadroOpe(){
	Operator *opera = op->get();
	int typerecord = cube->allowed(opera->oper, dir->getType1(), dir->getType2());
	if( typerecord > 0){
		Direction *top2 = dir->get(); dir->pop();
		Direction *top1 = dir->get(); dir->pop();
		op->pop();
		Direction *temp= memory->save(2,typerecord, -1);
		Cuadruplo *cuadro = new Cuadruplo(opera->oper, top1->direction, top2->direction, temp->direction);
		record.insert(std::make_pair(cont, cuadro));
		cont++;
		dir->push(temp);
	}
	else{
		wcout<<"Error: Types do not match...\n";
	}
}

/*Operador el termino*/
void Action::createCuadroOpeTerm(){
	Operator *p = op->get();
	if(p->oper == 1 or p->oper == 2){
		createCuadroOpe();
	}
}

/*Operador de factorial*/
void Action::createCuadroOpeFact(){
	Operator *p = op->get();
	if(p->oper == 3 or p->oper == 4 or p->oper == 5){
		createCuadroOpe();
	}
}

/*operador relacional*/
void Action::createCuadroOpeRel(){
	Operator *p = op->get();
	
	if(p->oper == 6 or p->oper == 7 or p->oper == 8 or p->oper == 9 or p->oper == 10 or p->oper == 11){
		createCuadroOpe();
	}
}

/*Operador de logica*/
void Action::createCuadroOpeLog(){
	Operator *p = op->get();
	if(p->oper == 12 or p->oper == 13){
		createCuadroOpe();	
	}
}

/*agregar un fondo falso*/
void Action::addFake(){
	op->pushFake();
}

/*quitar el fondo falso*/
void Action::removeFake(){
	op->popFake();
}

/*mostrar cuadrupledo*/
void Action::showCuadruplo(){
	 Cuadruplo *trans = new Cuadruplo();
	 for (std::map<int , Cuadruplo* >::iterator it=record.begin(); it!=record.end(); ++it){
    wcout << it->first<< " => " << trans->translate(it->second->op) << ", "; 
    wcout<< it->second->oper1 <<", " << it->second->oper2 <<", " << it->second->rec<<"\n";
	}
}

/*agregar un salto a la pila de saltos*/
void Action::addStackLeap(int l){
	leap->push(l);
}

/*Crear el goto falso de la condiccion*/
void  Action::createGotoFalseCond(){
	int aux=dir->getType1();
	if(aux == 3){
		Direction *result;
		result=dir->get();
		dir->pop();
		Cuadruplo *cuadru;
		cuadru=new Cuadruplo(17, result->direction, -1, -1);
		record.insert(std::make_pair(cont, cuadru));
		addStackLeap(cont);
		cont++;
	}
	else 
	wcout<<"The expression was waiting a boolean value\n";
	
}

/*crear el goto fin para salta el else*/
void  Action::createGotoFinCond(){
	Cuadruplo *cuadru;
	cuadru=new Cuadruplo(18, -1, -1, -1);
	record.insert(std::make_pair(cont, cuadru));
	int falso = leap->get()->leaps;
	leap->pop();
	addStackLeap(cont);
	Cuadruplo *cuadruGotoFalse = record[falso];
	cuadruGotoFalse->rec=cont+=1;
	record[falso]=cuadruGotoFalse;
}

/*llenar el salto en la goto fin*/
void  Action::createEndCond(){
	int fin = leap->get()->leaps;
	leap->pop();
	Cuadruplo *cuadruGotoFin = record[fin];
	cuadruGotoFin->rec=cont;
	record[fin]=cuadruGotoFin;
	
}


/*agrega el salto en la instruccion actual*/
void Action::addStackLeapActual(){
	addStackLeap(cont);
}

/*crea el goto false para el while*/
void Action::createGotoFalWhile(){
	int aux=dir->getType1();
	if(aux == 3){
		Direction *result;
		result=dir->get();
		dir->pop();
		Cuadruplo *cuadru;
		cuadru=new Cuadruplo(17, result->direction, -1, -1);
		record.insert(std::make_pair(cont, cuadru));
		addStackLeap(cont);
		rec++;
		cont++;
	}
	else 
	wcout<<"The expression was waiting a boolean value\n";
}

/*coloca el salo del goto*/
void Action::createEndWhile(){
	int falso = leap->get()->leaps;
	leap->pop();
	int retorno = leap->get()->leaps;
	leap->pop();
	Cuadruplo *cuadru;
	cuadru=new Cuadruplo(19, -1, -1, retorno);
	record.insert(std::make_pair(cont, cuadru));
	Cuadruplo *cuadruGotoFalse = record[falso];
	cuadruGotoFalse->rec=cont+=1;
	record[falso]=cuadruGotoFalse;
	
}

/*crea el goto true para el do-while*/
void Action::createWhileDo(){
	int verdadero = leap->get()->leaps;
	leap->pop();
	Direction *expresion =dir->get(); dir->pop();
	Cuadruplo *cuadru;
	cuadru=new Cuadruplo(20, expresion->direction, -1, verdadero);
	record.insert(std::make_pair(cont, cuadru));
	cont++;
	
}

/*crear el cuadrupledo que sera utilizado para asignacion*/
void Action::createCuadrAlloca(){

	int top1=dir->getType1();
	int top2=dir->getType2();
	if(top1 == top2){
		Direction *resul, *allocate;
		resul=dir->get(); dir->pop();
		allocate=dir->get(); dir->pop();
		Cuadruplo *cuadru;
		cuadru=new Cuadruplo(14, resul->direction, -1, allocate->direction);
		record.insert(std::make_pair(cont, cuadru));
		rec++;
		cont++;
	}
	else{
		wcout<<"Allocation failure: types do not match\n";
	}
	
}

/*crear el cuadrupledo que sera para lectura*/
void Action::createCuadrRead(int d, int t){
	
	Cuadruplo *leer;
	leer = new Cuadruplo(15, -1, -1, d);
	record.insert(std::make_pair(cont, leer));
	cont++;
	rec++;
}

/*crea el cuadrupledo para escritura*/
void Action::createCuadrPrint(){
	Direction *escrito;
	escrito = dir->get(); dir->pop();
	Cuadruplo *escribir;
	escribir = new Cuadruplo(16, escrito->direction, -1, -1);
	record.insert(std::make_pair(cont, escribir));
	cont++;
	rec++;
}

/*cuadruplo para inicializa los atributos*/
void Action::beginDraw(int figure){
	op->push(figure);
	Cuadruplo *draw;
	draw = new Cuadruplo(30, -1, -1, -1);
	record.insert(std::make_pair(cont, draw));
	cont++;
	
}
	
/*crear un cuadrupledo para los atributos enteros*/	
void Action::addAtributeInt(int attribute){
	Direction *entero;
	entero = dir->get();	dir->pop(); 
	Cuadruplo *draw;
	draw = new Cuadruplo(attribute, entero->direction, -1, -1);
	record.insert(std::make_pair(cont, draw));
	cont++;
	
	
}

/*crea un cuadruplo para los atributos de string*/
void Action::addAtributeString(int attribute, const wchar_t* n){
	char* value=coco_string_create_char(n);
	int value_i;
	
	if(attribute == 35){
		if( strcmp(value,"\"small\"") == 0 )
			value_i=1;
		else if(strcmp(value, "\"normal\"") == 0)
			value_i=2;
		else if(strcmp(value,"\"bigger\"") == 0)
			value_i=3;
		else
			wcout<<"The value "<<value<<" was not found\n";
		
	}
	else if(attribute == 36){
		
		if(strcmp(value,"\"SolidLine\"") == 0)
			value_i=1;
		else if(strcmp(value, "\"DashLine\"") == 0)
			value_i=2;
		else if(strcmp(value, "\"DotLine\"") == 0)
			value_i=3;
		else
			wcout<<"The value "<<value<<" was not found\n";
	}
	else if(attribute == 37){
		if(strcmp(value,"\"blue\"") == 0)
			value_i=1;
		else if(strcmp(value,"\"yellow\"") == 0)
			value_i=2;
		else if(strcmp(value,"\"red\"") == 0)
			value_i=3;
		else if(strcmp(value,"\"pink\"") == 0)
			value_i=4;
		else if(strcmp(value,"\"brown\"") == 0)
			value_i=5;
		else if(strcmp(value,"\"black\"") == 0)
			value_i=6;
		else if(strcmp(value,"\"white\"") == 0)
			value_i=7;
		else if(strcmp(value,"\"green\"") == 0)
			value_i=8;
		else if(strcmp(value,"\"grey\"") == 0)
			value_i=9;
		else if(strcmp(value,"\"orange\"") == 0)
			value_i=10;
		else if(strcmp(value,"\"purple\"") == 0)
			value_i=11;
		else 
			wcout<<"The value "<<value<<" was not found\n";
	}
	else if(attribute == 38){
		if(strcmp(value, "\"blue\"") == 0)
			value_i=1;
		else if(strcmp(value, "\"yellow\"") == 0)
			value_i=2;
		else if(strcmp(value,"\"red\"") == 0)
			value_i=3;
		else if(strcmp(value,"\"pink\"") == 0)
			value_i=4;
		else if(strcmp(value,"\"brown\"") == 0)
			value_i=5;
		else if(strcmp(value,"\"black\"") == 0)
			value_i=6;
		else if(strcmp(value,"\"white\"") == 0)
			value_i=7;
		else if(strcmp(value,"\"green\"") == 0)
			value_i=8;
		else if(strcmp(value,"\"grey\"") == 0)
			value_i=9;
		else if(strcmp(value,"\"orange\"") == 0)
			value_i=10;
		else if(strcmp(value,"\"purple\"") == 0)
			value_i=11;
		else 
			wcout<<"The value "<<value<<" was not found\n";
	}
	
	Cuadruplo *draw;
	draw = new Cuadruplo(attribute, value_i, -1, -1);
	record.insert(std::make_pair(cont, draw));
	cont++;	
	
	
}

/*crear el cuadruplo que sera para dibuja la figura*/
void Action::endDraw(){
	int figure=op->get()->oper;
	Cuadruplo *draw;
	draw = new Cuadruplo(50, figure, -1, -1);
	record.insert(std::make_pair(cont, draw));
	cont++;	
	op->pop();
}

/*goto para inicializa el programa*/
void Action::beginProgram(){
	Cuadruplo *gotoMain;
	gotoMain = new Cuadruplo(0, -1, -1, -1);
	record.insert(std::make_pair(cont, gotoMain));
	cont++;
}


/*agrega en el gotomain en donde inicia el programa*/
void Action::beginMain(){
	Cuadruplo *gotoMain = record[0];
	gotoMain->rec=cont;
	record[0]=gotoMain;
}

/*termina el programa*/
void Action::endProgram(){
	Cuadruplo *end;
	end = new Cuadruplo(99, -1, -1, -1);
	record.insert(std::make_pair(cont, end));
	cont++;
}

/*crea toda las constantes y el cuadruplo en un archivo*/
void Action::createObject(){
   ofstream myfile ("/home/ubuntu/Logo/Objecto.txt");
   if (myfile.is_open()){
	for (std::map<int , Cuadruplo* >::iterator it=record.begin(); it!=record.end(); ++it){
		myfile<<it->second->op<<"$"<<it->second->oper1<<"$"<<it->second->oper2<<"$"<<it->second->rec<<"$\n";
	}
    myfile.close();
  }
  else wcout<<"Error: file cannot be open...\n";
  
  ofstream file ("/home/ubuntu/Logo/Constantes.txt");
  if (file.is_open())
  {
	for (std::map<int , double >::iterator it=contantes.begin(); it!=contantes.end(); ++it){
		file<<it->first<<"$"<<it->second<<"$\n";
	}
    file.close();
  }
  else wcout<<"Error: file cannot be open...\n";
}

/*agrega a la funcion*/
void Action::addNameFunction(const wchar_t* n){
	inifun=new Function();
	std::wstring ide=std::wstring(n);
	inifun->name=ide;
	std::wstring name=std::wstring(n);
	int dir=tablaGlobals[name]->dir->direction;
	inifun->direction=dir;
}

/*agrega un parametro funcion*/
void Action::addParameter(int t){
	para->push(t);
}

/*agrega el tipo a la  funciones*/
void Action::addTypeFunction(int t){
	inifun->type=t;
}

/*agrega la catidad de parametro a la funcion*/
void Action::addNoParameters(){
	inifun->parametro=cant_para;
}

/*agrega el numero de locales a la funcion*/
void Action::addNoLocals(){
	inifun->varLoc=cant_loc;
}

/*agrega en que instruccion inicia la funcion*/
void Action::addContCuadruplo(){
	inifun->begin=cont;
	inifun->par=para;
	fun->push(inifun);
}

/* genera el retorno  y coloca en la tabla de funcciones la funcion*/
void Action::generateRetorno(){
	Direction *regresa;
	regresa=dir->get(); dir->pop();
	if(inifun->type == 1 or inifun->type == 2)
	{	
		if(regresa->type == inifun->type){
			Cuadruplo *retorno;
			retorno = new Cuadruplo(21, regresa->direction, -1, inifun->direction);
			record.insert(std::make_pair(cont, retorno));
			cont++;
			Cuadruplo *ret;
			ret = new Cuadruplo(23, -1, -1, -1);
			record.insert(std::make_pair(cont, ret));
			cont++;
		}
		else if(regresa->type == 0){
			wcout<<"There is no return value in the function "<<inifun->name<<" \n";
		}
		else if(regresa->type != inifun->type and regresa->type != 0){
			wcout<<"Return failure: incompatible types in the function "<<inifun->name<<" \n";
		    }
		}
	
	else if(inifun->type == 0 and regresa->direction == -1){
		Cuadruplo *retorno;
		retorno = new Cuadruplo(23, -1, -1, -1);
		record.insert(std::make_pair(cont, retorno));
		cont++;
	}
	else {
		wcout<<"Return not necessary\n";
	}
	
	Cuadruplo *vacio;
	vacio = new Cuadruplo(-1, -1, -1, -1);
	record.insert(std::make_pair(cont, vacio));
	cont++;
}

/*busca una funcion fue definida en la tabla de funcciones*/
void Action::findFunction(const wchar_t* n){
	llam_fun=fun->find(n);
	contPara=llam_fun->parametro;  //obtiene la cantidad de parametros
	llam_para=llam_fun->par->invert(); //invierte la pila para un orden correcto
	if(llam_fun->direction  == -1){
		wcout<<"the function "<<llam_fun->name<<" was not defined\n";
	}
}


/*crea el cuadruplo era*/
void Action::createERA(){
		Cuadruplo *era;
		era = new Cuadruplo(24, llam_fun->direction, -1, -1);
		record.insert(std::make_pair(cont, era));
		cont++;
}


/*crea el cuadruplo de para*/
void Action::createParameter(){
	contPara--; 
	Parameter *temp_para = llam_para->get(); llam_para->pop();
	Direction *argum=dir->get(); dir->pop();
	Direction *resul_para = memory->save(4 , temp_para->type, -1);
	if(temp_para->type == argum->type){
		Cuadruplo *para;
		para = new Cuadruplo(25, argum->direction, -1, resul_para->direction );
		record.insert(std::make_pair(cont, para));
		cont++;
		}
	else {
		wcout<<"There are incorrect types of parameters in the function "<<llam_fun->name<<"\n";
	}
	
}

/*revisa si faltan mas parametros*/
void Action::checkParameter(){
	if(contPara > 0){
		wcout<<"Missing more parameters in the function "<<llam_fun->name<<" \n";
	}
	else if(contPara < 0){
		wcout<<"Receiving more parameters than the declared function "<<llam_fun->name<<" \n";
	}
}



/*crea el gotosub*/
void Action::createGotoSub(){
			Cuadruplo *gtsub;
			gtsub = new Cuadruplo(26, llam_fun->begin , -1, -1 );
			record.insert(std::make_pair(cont, gtsub));
			cont++;
			if(llam_fun->type == 1 or llam_fun->type == 2){
				Cuadruplo *asig;
				Direction *tempReto = memory->save(2, llam_fun->type , -1);
				asig = new Cuadruplo(22, llam_fun->direction , -1, tempReto->direction );
				record.insert(std::make_pair(cont, asig));
				dir->push(tempReto);
				cont++;
			}
			else if(llam_fun->type == 0){
				Direction *tempVoid = new Direction();
				dir->push(tempVoid);
			}
			memory->clearPara();
}

/*inicializa la tabla de dimensiones para una variable*/
void Action::isDimension(){
	listaDimensiones = new Dimensiones();
}

/*obitne la dimension*/
void Action::getDimension(){
	dimension = new Dimension();
	listaDimensiones->Dim=0;
	listaDimensiones->R=1;
	dimension->Dim=listaDimensiones->Dim;
}

/*salva el inferior*/
void Action::saveInferior(int i){
	dimension->Li=i;	
}

/*salva el superior*/
void Action::saveSuperior(int s){
	dimension->Ls=s;
}

/*obtiene una nueva dimension*/
void Action::getNewDimension(){
	listaDimensiones->agregar(dimension);
	dimension = new Dimension();
	listaDimensiones->Dim=listaDimensiones->Dim+1;
	dimension->Dim=listaDimensiones->Dim;
}

/*obtiene la k*/
void Action::getKCons(){
	listaDimensiones->sacarK();
}

/*agrega la dimension a la tabla de variables de globales*/
void Action::addDimensionGlobal(const wchar_t* n){
	std::wstring identif=std::wstring(n);
	for (std::map<std::wstring , Variable* >::iterator it=tablaGlobals.begin(); it!=tablaGlobals.end(); ++it){
		if(it->first == identif ){
			it->second->dimensiones=listaDimensiones ;
			}
	}
}

/*agrega la dimension a la tabla de variables de locales*/
void Action::addDimensionLocal(const wchar_t* n){
	std::wstring identif=std::wstring(n);
	for (std::map<std::wstring , Variable* >::iterator it=tablaLocals.begin(); it!=tablaLocals.end(); ++it){
		if(it->first == identif ){
			it->second->dimensiones=listaDimensiones ;
			}
	}
}

/*obtiene la siguiente direccion de la siguiente variable declarado*/
void Action::getNextDirection(int partition, int type){
	if(partition == 0){
		if(type == 1 ){
			memory->glob_int=memory->glob_int+listaDimensiones->tam;
		}
		else if(type == 2){
			memory->glob_flo=memory->glob_flo+listaDimensiones->tam;
		}
	}
	else if(partition == 1){
		if(type == 1 ){
			memory->local_int=memory->local_int+listaDimensiones->tam;
		}
		else if(type == 2){
			memory->local_flo=memory->local_flo+listaDimensiones->tam;
		}
	}
	
}

/*obtiene la lista de dimensiones */
void Action::getDimensionId(Variable* var){
	if(var->dimensiones == NULL ){
		exit(0);
	}
	if( var->dimensiones != NULL && var != NULL){
		Direction *direccion =dir->get(); dir->pop();
		var->dimensiones->dirBase=direccion->direction;
		stackDim->push(1, var->dimensiones, var->type);
		addFake(); 
	}
	else wcout<<"A Variable is not dimension "<<var->name<<"\n";
	
}

/*verifica que no se pase del rango*/
void Action::generateVerifica(){
	if(stackDim != NULL){
		int num= stackDim->getNum();
		Dimensiones *listaDim= stackDim->getDimensiones();
		Dimension *desp= listaDim->getDimension(num-1);
		Direction *direccion =dir->get();

		if(direccion->type == 1){
			Cuadruplo *verif;
			verif = new Cuadruplo(51, direccion->direction , desp->Li, desp->Ls );
			record.insert(std::make_pair(cont, verif));
			cont++;
		
			if(num < listaDim->Dim ){
				Cuadruplo *factor;
				Direction *aux = dir->get();
				dir->pop();
				Direction *temp= memory->save(2, 1 , -1);
				factor= new Cuadruplo(52, aux->direction, desp->Mdim, temp->direction );
				record.insert(std::make_pair(cont, factor));
				cont++;
				dir->push(temp);
			}
		
			if(num > 1){
				Direction *aux2 = dir->get(); dir->pop();
				Direction *aux1= dir->get(); dir->pop();
				Direction *temp1= memory->save(2, 1 , -1);
				Cuadruplo *suma;
				suma = new Cuadruplo(53, aux1->direction,  aux2->direction, temp1->direction );
				record.insert(std::make_pair(cont, suma));
				cont++;
				dir->push(temp1);
				}
		}
	
	else wcout<<"The indexes should be integer";
	}
else wcout<<"The Variable Dimension have been initialzed\n";
}

/*actualiza el numero de stack en la que va la llamada*/
void Action::updateStackDim(){
	if(stackDim != NULL){
		int num= stackDim->getNum();
		num=num+1;
		stackDim->setNum(num);
	}
}

/*obtiene la siguiente direccion*/
void Action::getDirSumK(){
	if(stackDim != NULL){
		Dimensiones *listaDim= stackDim->getDimensiones();
		
		Direction *aux= dir->get(); dir->pop();
		Direction *temp= memory->save(2, 1 , -1);
		Cuadruplo *sumaK;
		sumaK = new Cuadruplo(54, aux->direction,  listaDim->K, temp->direction );
		record.insert(std::make_pair(cont, sumaK));
		cont++;
		
		Direction *apunt= memory->save(6, stackDim->getType() , -1);
		Cuadruplo *sumaBase;
		sumaBase = new Cuadruplo(55, temp->direction,  listaDim->dirBase, apunt->direction );
		record.insert(std::make_pair(cont, sumaBase));
		cont++;
		dir->push(apunt);
		removeFake();
		stackDim->pop();
	}
}




}//termina namespace

