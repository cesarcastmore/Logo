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

Variable::Variable(){
type=0;
slope=0;
dir=new Direction();
}

Variable::Variable(wstring n, int t, int slope, Direction *d){
type=t;
name=n;
slope=slope;
dir=d; 
}



TablaVariables::TablaVariables(){
	
}

bool TablaVariables::checkOnlyLocal(const wchar_t* n){
	std::wstring identif=std::wstring(n);
	//buscar en local
		for (std::map<std::wstring , Variable* >::iterator it=tablaLocals.begin(); it!=tablaLocals.end(); ++it)
			if(it->first == identif){
				return false; 
			}
	return true;
		
}


bool TablaVariables::checkOnlyGlobal(const wchar_t* n){
	std::wstring identif=std::wstring(n);
	//buscar en global
		for (std::map<std::wstring , Variable* >::iterator it=tablaGlobals.begin(); it!=tablaGlobals.end(); ++it)
			if(it->first == identif ){
				return false;
			}
	return true;
		
}


void TablaVariables::addGlobal(const wchar_t* n, int  t, int slope, Direction *d){
	std::wstring identif=std::wstring(n);
	if(checkOnlyGlobal(n)){
		Variable *temp = new Variable(n,t, slope, d);
		tablaGlobals.insert(std::make_pair(identif, temp));	
	}
	else wcout<<"There are two variables with the same name \""<<n<<"\n";
}


void TablaVariables::addLocal(const wchar_t* n, int t, int slope, Direction *d){
	std::wstring identif=std::wstring(n);
	if(checkOnlyLocal(n)){
		Variable *temp = new Variable(n,t,slope, d);
		tablaLocals.insert(std::make_pair(identif, temp));		
	}
	else wcout<<"There are two variables with the same name \""<<n<<"\n";
}

		
void TablaVariables::removeAllLocals(){
	tablaLocals.clear();
	
}

void  TablaVariables::removeAllGlobals(){
	tablaGlobals.clear();
}


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


void TablaVariables::displayLocals(){
  wcout << "tabla local contains:\n";
  for (std::map<std::wstring , Variable* >::iterator it=tablaLocals.begin(); it!=tablaLocals.end(); ++it)
    wcout << it->first << " => " << it->second->type << "--" << it->second->slope <<"---direction--"<<it->second->dir->direction<<'\n';
}


void TablaVariables::displayGlobals(){
  cout << "tabla global contains:\n";
  for (std::map<std::wstring , Variable* >::iterator it=tablaGlobals.begin(); it!=tablaGlobals.end(); ++it){
    wcout << it->first << " => type " << it->second->type << "-- function" << it->second->slope <<'\n';
	}
}


Parameter::Parameter(){
	next=NULL;
	type=0;
}
	
Parameter::Parameter(int t){
	type=t;
	next=NULL;
}


StackParameter::StackParameter(){
	actual=new Parameter();
}

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

void StackParameter::pop(){
	actual = actual->next;
}

Parameter* StackParameter::get(){
	Parameter *new_node;
	new_node=new Parameter(actual->type);
	return new_node;
}

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


void StackParameter::showStack(){
	wcout<<"\tla pila contiene StackParameter\n";
	Parameter *par=actual;
	while(par->next != NULL){
		wcout<<"\tel type es "<< par->type<<"\n";
		par=par->next;
	} 
}


Function::Function(){
	type=0;
	parametro=0;
	varLoc=0;
	begin=0;
	par=new StackParameter();
	next=NULL;
	direction=-1;
}


StackFunction::StackFunction(){
	actual=new Function();
}

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

void StackFunction::pop(){
	actual = actual->next;
}

Function* StackFunction::get(){
	Function *new_node;
	new_node=actual;
	return new_node;
}

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

void StackFunction::showStack(){
	wcout<<"la pila contiene StackFunction\n";
	Function *fun=actual;
	while(fun->next != NULL){
		wcout<<"el name es "<< fun->name<<"\n";
		wcout<<"el type es "<< fun->type<<"\n";
		wcout<<"el varLoc es "<< fun->varLoc<<"\n";;
		wcout<<"el begin es "<< fun->begin<<"\n";
		wcout<<"el parametro es "<< fun->parametro<<"\n";
		fun->par->showStack();
		fun=fun->next;
	} 
}


Direction::Direction(){
	next=NULL;
	direction=-1;
	type=0;
	}
	
Direction::Direction(int d, int t){
	next=NULL;
	direction=d;
	type=t;
	}
	

StackDirection::StackDirection(){
	actual=new Direction();
}

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



void StackDirection::push(Direction* dir){
	
   if(!actual ){
   actual=dir;
}	 
      else{
      dir->next = actual;
      actual = dir;
      }
}


void StackDirection::pop(){
	actual = actual->next;
}

Direction* StackDirection::get(){
	Direction *new_node;
	new_node=new Direction(actual->direction, actual->type);
	return new_node;
}

int StackDirection::getType1(){
	Direction *new_node;
	new_node=actual;
	return new_node->type;
}
int StackDirection::getType2(){
	Direction *new_node;
	new_node=actual;
	return new_node->next->type;
}
	
void StackDirection::showStack(){
	wcout<<"la pila contiene StackDirection\n";
	Direction *dis=actual;
	while(dis != NULL){
		wcout<<"el valor es "<< dis->direction;
		wcout<<"----"<< dis->type<<"\n";
		dis=dis->next;
	} 
}


Operator::Operator(){
	next=NULL;
	oper=0;
}

	
Operator::Operator(int o){
	oper=o;
	next=NULL;
}


StackOperator::StackOperator(){
	actual=new Operator();
}

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

void StackOperator::pop(){
	actual = actual->next;
}

Operator* StackOperator::get(){
	Operator *new_node;
	new_node=new Operator(actual->oper);
	return new_node;
}

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

void StackOperator::popFake(){
		actual = actual->next;
}

void StackOperator::showStack(){
	wcout<<"la pila contiene  StackOperator\n";
	Operator *op=actual;
	while(op != NULL){
		wcout<<"el valor es "<< op->oper<<"\n";
		op=op->next;
	} 	
}


Leap::Leap(){
	next=NULL;
	leaps=0;
}
	
Leap::Leap(int o){
	leaps=o;
	next=NULL;
}


StackLeap::StackLeap(){
	actual=new Leap();
}

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

void StackLeap::pop(){
	actual = actual->next;
}

Leap* StackLeap::get(){
	Leap *new_node;
	new_node=new Leap(actual->leaps);
	return new_node;
}


/*
void StackLeap::showLeap(){
	wcout<<"la pila contiene  StackLeap\n";
	 Leap *leap=actual;
	while(leap != NULL){
		wcout<<"el valor es "<< leap->leaps<<"\n";
		leap=leap->next;
	} 	
}	*/

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
}


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
	
	
	return new Direction(-1, 0);
}


void Memory::clearTemp(){
	temp_int=12000;
	temp_flo=14000;
	temp_bool=16000;
	
}

void Memory::clearLocal(){
	local_int=6000;
	local_flo=8000;
	local_bool=10000;
	
}

void Memory::clearPara(){
	para_int=24000;
	para_flo=26000;
	para_bool=28000;
	
}

Cuadruplo::Cuadruplo(){
}
Cuadruplo::Cuadruplo(int a, int b, int c, int d){
	op=a;
	oper1=b;
	oper2=c;
	rec=d;
}


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
 		
//PENDIENTE	
}
	
int LogicCube::allowed(int a, int b, int c){
	return cube[a][b][c];
}

Action::Action(){
	tab=new TablaVariables();
	op=new StackOperator();
	dir=new StackDirection();
	leap= new StackLeap();
	cube=new LogicCube();
	memory=new Memory();
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
	}
	
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

Variable* Action::find(const wchar_t* n){
	Variable* var =tab->find(n);
	return var;
}

void Action::removeLocals(){
	tab->removeAllLocals();
	memory->clearLocal();
	memory->clearTemp();
	memory->clearPara();
	cant_loc=0;
	cant_para=0;
	para=new StackParameter();
	
}

void Action::removeGlobals(){
	tab->removeAllGlobals();
}

void Action::addStackDir(int d, int t){
	dir->push(d,t);
}

void Action::addStackOpe(int o){
	op->push(o);
}

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
		wcout<<"incompatible types\n";
	}
}

void Action::createCuadroOpeTerm(){
	Operator *p = op->get();
	if(p->oper == 1 or p->oper == 2){
		createCuadroOpe();
	}
}

void Action::createCuadroOpeFact(){
	Operator *p = op->get();
	if(p->oper == 3 or p->oper == 4 or p->oper == 5){
		createCuadroOpe();
	}
}


void Action::createCuadroOpeRel(){
	Operator *p = op->get();
	
	if(p->oper == 6 or p->oper == 7 or p->oper == 8 or p->oper == 9 or p->oper == 10 or p->oper == 11){
		createCuadroOpe();
	}
}


void Action::createCuadroOpeLog(){
	Operator *p = op->get();
	if(p->oper == 12 or p->oper == 13){
		createCuadroOpe();	
	}
}


void Action::addFake(){
	op->pushFake();
}

void Action::removeFake(){
	op->popFake();
}


void Action::showCuadruplo(){
	 Cuadruplo *trans = new Cuadruplo();
	 for (std::map<int , Cuadruplo* >::iterator it=record.begin(); it!=record.end(); ++it){
    wcout << it->first<< " => " << trans->translate(it->second->op) << ", "; 
    wcout<< it->second->oper1 <<", " << it->second->oper2 <<", " << it->second->rec<<"\n";
	}
}

void Action::addStackLeap(int l){
	leap->push(l);
}

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
	wcout<<"The expresion was waiting a boolean\n";
	
}

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

void  Action::createEndCond(){
	int fin = leap->get()->leaps;
	leap->pop();
	Cuadruplo *cuadruGotoFin = record[fin];
	cuadruGotoFin->rec=cont;
	record[fin]=cuadruGotoFin;
	
}


void Action::addStackLeapActual(){
	addStackLeap(cont);
}


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
	wcout<<"The expresion was waiting a boolean\n";
}

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


void Action::createWhileDo(){
	int verdadero = leap->get()->leaps;
	leap->pop();
	Direction *expresion =dir->get(); dir->pop();
	Cuadruplo *cuadru;
	cuadru=new Cuadruplo(20, expresion->direction, -1, verdadero);
	record.insert(std::make_pair(cont, cuadru));
	cont++;
	
}

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
		wcout<<"Allocation failure: incompatible types\n";
	}
	
}


void Action::createCuadrRead(int d, int t){
	
	Cuadruplo *leer;
	leer = new Cuadruplo(15, -1, -1, d);
	record.insert(std::make_pair(cont, leer));
	cont++;
	rec++;
}

void Action::createCuadrPrint(){
	Direction *escrito;
	escrito = dir->get(); dir->pop();
	Cuadruplo *escribir;
	escribir = new Cuadruplo(16, escrito->direction, -1, -1);
	record.insert(std::make_pair(cont, escribir));
	cont++;
	rec++;
}

void Action::beginDraw(int figure){
	op->push(figure);
	Cuadruplo *draw;
	draw = new Cuadruplo(30, -1, -1, -1);
	record.insert(std::make_pair(cont, draw));
	cont++;
	
}
	
void Action::addAtributeInt(int attribute){
	Direction *entero;
	entero = dir->get();	dir->pop(); 
	Cuadruplo *draw;
	draw = new Cuadruplo(attribute, entero->direction, -1, -1);
	record.insert(std::make_pair(cont, draw));
	cont++;
	
	
}

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
			wcout<<"The value"<<value<<" was not find\n";
		
	}
	else if(attribute == 36){
		
		if(strcmp(value,"\"SolidLine\"") == 0)
			value_i=1;
		else if(strcmp(value, "\"DashLine\"") == 0)
			value_i=2;
		else if(strcmp(value, "\"DotLine\"") == 0)
			value_i=3;
		else
			wcout<<"The value"<<value<<" was not find\n";
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
			wcout<<"The value"<<value<<" was not find\n";
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
			wcout<<"The value"<<value<<" was not find\n";
	}
	
	Cuadruplo *draw;
	draw = new Cuadruplo(attribute, value_i, -1, -1);
	record.insert(std::make_pair(cont, draw));
	cont++;	
	
	
}


void Action::endDraw(){
	int figure=op->get()->oper;
	Cuadruplo *draw;
	draw = new Cuadruplo(50, figure, -1, -1);
	record.insert(std::make_pair(cont, draw));
	cont++;	
	op->pop();
}

void Action::beginProgram(){
	Cuadruplo *gotoMain;
	gotoMain = new Cuadruplo(0, -1, -1, -1);
	record.insert(std::make_pair(cont, gotoMain));
	cont++;
}


void Action::beginMain(){
	Cuadruplo *gotoMain = record[0];
	gotoMain->rec=cont;
	record[0]=gotoMain;
}


void Action::endProgram(){
	Cuadruplo *end;
	end = new Cuadruplo(99, -1, -1, -1);
	record.insert(std::make_pair(cont, end));
	cont++;
}

void Action::createObject(){
	ofstream myfile ("/home/ubuntu/Logo-master/Objecto.txt");
  if (myfile.is_open())
  {
	for (std::map<int , Cuadruplo* >::iterator it=record.begin(); it!=record.end(); ++it){
		myfile<<it->second->op<<"$"<<it->second->oper1<<"$"<<it->second->oper2<<"$"<<it->second->rec<<"$\n";
	}
    myfile.close();
  }
  else wcout<<"cannot open the file\n";
  
  ofstream file ("/home/ubuntu/Logo-master/Constantes.txt");
  if (file.is_open())
  {
	for (std::map<int , double >::iterator it=contantes.begin(); it!=contantes.end(); ++it){
		file<<it->first<<"$"<<it->second<<"$\n";
	}
    file.close();
  }
  else wcout<<"cannot open the file\n";
}

void Action::addNameFunction(const wchar_t* n){
	inifun=new Function();
	std::wstring ide=std::wstring(n);
	inifun->name=ide;
	std::wstring name=std::wstring(n);
	int dir=tablaGlobals[name]->dir->direction;
	inifun->direction=dir;
}


void Action::addParameter(int t){
	para->push(t);
}

void Action::addTypeFunction(int t){
	inifun->type=t;
}
void Action::addNoParameters(){
	inifun->parametro=cant_para;
}

void Action::addNoLocals(){
	inifun->varLoc=cant_loc;
}

void Action::addContCuadruplo(){
	inifun->begin=cont;
	inifun->par=para;
	fun->push(inifun);
}

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
			wcout<<"There are not return value in the function "<<inifun->name<<" \n";
		}
		else if(regresa->type != inifun->type and regresa->type != 0){
			wcout<<"Return failture: incompatible types in the function "<<inifun->name<<" \n";
		    }
		}
	
	else if(inifun->type == 0 and regresa->direction == -1){
		Cuadruplo *retorno;
		retorno = new Cuadruplo(23, -1, -1, -1);
		record.insert(std::make_pair(cont, retorno));
		cont++;
	}
	else {
		wcout<<"Return unnecessary\n";
	}
	
	Cuadruplo *vacio;
	vacio = new Cuadruplo(-1, -1, -1, -1);
	record.insert(std::make_pair(cont, vacio));
	cont++;
}


void Action::findFunction(const wchar_t* n){
	llam_fun=fun->find(n);
	contPara=llam_fun->parametro;  //obtiene la cantidad de parametros
	llam_para=llam_fun->par->invert(); //invierte la pila para un orden correcto
	if(llam_fun->direction  == -1){
		wcout<<"the function "<<llam_fun->name<<" was not defined\n";
	}
}



void Action::createERA(){
		Cuadruplo *era;
		era = new Cuadruplo(24, llam_fun->direction, -1, -1);
		record.insert(std::make_pair(cont, era));
		cont++;
}


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


void Action::checkParameter(){
	if(contPara > 0){
		wcout<<"Missing more parameters in the function "<<llam_fun->name<<" \n";
	}
	else if(contPara < 0){
		wcout<<"Receiving more parameters than the declared function "<<llam_fun->name<<" \n";
	}
}




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



}//termina namespace

