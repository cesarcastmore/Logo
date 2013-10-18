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
std::map<int , float> contantes;

Variable::Variable(){
type=0;
function=0;
dir=new Direction();
}

Variable::Variable(wstring n, int t, int f, Direction *d){
type=t;
name=n;
function=f;
dir=d; 
}



TablaVariables::TablaVariables(){
	
}

bool TablaVariables::checkOnlyLocal(const wchar_t* n, int f){
	std::wstring identif=std::wstring(n);
	//buscar en local
		for (std::map<std::wstring , Variable* >::iterator it=tablaLocals.begin(); it!=tablaLocals.end(); ++it)
			if(it->first == identif  and f == 0 ){
				return false; 
			}
	return true;
		
}


bool TablaVariables::checkOnlyGlobal(const wchar_t* n, int f){
	std::wstring identif=std::wstring(n);
	//buscar en global
		for (std::map<std::wstring , Variable* >::iterator it=tablaGlobals.begin(); it!=tablaGlobals.end(); ++it)
			if(it->first == identif and it->second->function == f){
				return false;
			}
	return true;
		
}


void TablaVariables::addGlobal(const wchar_t* n, int  t, int f, Direction *d){
	std::wstring identif=std::wstring(n);
	if(checkOnlyGlobal(n, f)){
		Variable *temp = new Variable(n,t,f, d);
		tablaGlobals.insert(std::make_pair(identif, temp));	
	}
	else wcout<<"There are two variables with the same name \""<<n<<"\"\n";
}


void TablaVariables::addLocal(const wchar_t* n, int t, int f, Direction *d){
	std::wstring identif=std::wstring(n);
	if(checkOnlyLocal(n,f)){
		Variable *temp = new Variable(n,t,f, d);
		tablaLocals.insert(std::make_pair(identif, temp));		
	}
	else wcout<<"There are two variables with the same name \""<<n<<"\"\n";
}

		
void TablaVariables::removeAllLocals(){
	tablaLocals.clear();
	
}

void  TablaVariables::removeAllGlobals(){
	tablaGlobals.clear();
}


Variable* TablaVariables::find(const wchar_t* n, int f){
	std::wstring identif=std::wstring(n);
	
	for (std::map<std::wstring , Variable* >::iterator it=tablaLocals.begin(); it!=tablaLocals.end(); ++it)			
		if(it->first == identif  and f == 0 ) 
			return it->second;
	
	for (std::map<std::wstring , Variable* >::iterator it=tablaGlobals.begin(); it!=tablaGlobals.end(); ++it)
		if(it->first == identif and it->second->function == f)
			return it->second;		
			
	if(f == 0)
	wcout<<"Error: The variable \""<<identif <<"\" has not been initialized yet.\n";
		else 
		wcout<<"Error: The function \""<<identif <<"\" has not been initialized yet<<.\n";


	return new Variable();

}


void TablaVariables::displayLocals(){
  wcout << "tabla local contains:\n";
  for (std::map<std::wstring , Variable* >::iterator it=tablaLocals.begin(); it!=tablaLocals.end(); ++it)
    wcout << it->first << " => " << it->second->type << "--" << it->second->function <<'\n';
}


void TablaVariables::displayGlobals(){
  cout << "tabla global contains:\n";
  for (std::map<std::wstring , Variable* >::iterator it=tablaGlobals.begin(); it!=tablaGlobals.end(); ++it){
    wcout << it->first << " => type " << it->second->type << "-- function" << it->second->function <<'\n';
	}
}



Direction::Direction(){
	next=NULL;
	direction=0;
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
	glob_int=1000;
	glob_flo=3000;
	glob_bool=5000;
	local_int=7000;
	local_flo=9000;
	local_bool=11000;
	temp_int=13000;
	temp_flo=15000;
	temp_bool=17000;
	cons_int=19000;
	cons_flo=21000;
	cons_bool=23000;
}


Direction* Memory::save(int partition, int type, float value){
	
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
		contantes[temp_int]=value;
		Direction *new_dir=new Direction(temp_int, type);
		temp_int++;
		return new_dir;
	}
	else if(partition == 2 and type == 2){
		contantes[temp_flo]=value;
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
		Direction *new_dir=new Direction(cons_bool-=1, type);
		cons_bool++;
		return new_dir;
	}
	
	return new Direction(-1, 0);
}


void Memory::clearTemp(){
	temp_int=13000;
	temp_flo=15000;
	temp_bool=17000;
	
}

void Memory::clearLocal(){
	local_int=7000;
	local_flo=9000;
	local_bool=11000;
	
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
		case 20: return "gotoMain";
		case 30: return "beginDraw";
		case 31: return "point";
		case 32: return "line";
		case 33: return "triangle";
		case 34: return "square";
		case 35: return "circle";
		case 36: return "star";
		case 37: return "pentagon";
		case 38: return "hexagon";
		case 39: return "rhomboid";
		case 50: return "endDraw";
		case 99: return "end";
		
	}
	return "0";
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
	cont=0;
	}
	
void Action::addGlobal(const wchar_t* n, int  t, int f){
	Direction *d = memory->save(0, t, -1);
	tab->addGlobal(n,t,f, d);
}

void Action::addLocal(const wchar_t* n, int  t, int f){
	Direction *d = memory->save(1, t, -1);
	tab->addLocal(n,t,f,d);
}

Variable* Action::find(const wchar_t* n, int f){
	Variable* var =tab->find(n,f);
	return var;
}

void Action::removeLocals(){
	tab->removeAllLocals();
	memory->clearLocal();
	memory->clearTemp();
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
		wcout<<"Allocation failure: incompatible types\n"<<top1<<"--"<<top2<<"\n";
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
	int figure=op->get()->oper;
	wcout<<figure;
	Direction *entero;
	entero = dir->get();	dir->pop(); 
	Cuadruplo *draw;
	draw = new Cuadruplo(figure, attribute, entero->direction, -1);
	record.insert(std::make_pair(cont, draw));
	cont++;
			
;
	
	
}

void Action::addAtributeString(int attribute, const wchar_t* n){
	char* value=coco_string_create_char(n);
	int value_i;
	int figure=op->get()->oper;
	
	if(attribute == 5){
		if( strcmp(value,"\"small\"") == 0 )
			value_i=1;
		else if(strcmp(value, "\"normal\"") == 0)
			value_i=2;
		else if(strcmp(value,"\"bigger\"") == 0)
			value_i=2;
		else
			wcout<<"The value"<<value<<" was not find\n";
		
	}
	else if(attribute == 6){
		
		if(strcmp(value,"\"small\"") == 0)
			value_i=1;
		else if(strcmp(value, "\"normal\"") == 0)
			value_i=2;
		else if(strcmp(value, "\"bigger\"") == 0)
			value_i=2;
		else
			wcout<<"The value"<<value<<" was not find\n";
	}
	else if(attribute == 7){
		if(strcmp(value,"\"blue\"") == 0)
			value_i=1;
		else if(strcmp(value,"\"yellow\"") == 0)
			value_i=2;
		else if(strcmp(value,"\"red\"") == 0)
			value_i=2;
		else 
			wcout<<"The value"<<value<<" was not find\n";
	}
	else if(attribute == 8){
		if(strcmp(value, "\"blue\"") == 0)
			value_i=1;
		else if(strcmp(value, "\"yellow\"") == 0)
			value_i=2;
		else if(strcmp(value,"\"red\"") == 0)
			value_i=2;
		else 
			wcout<<"The value"<<value<<" was not find\n";
	}
	
	Cuadruplo *draw;
	draw = new Cuadruplo(figure, attribute, value_i, -1);
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
	gotoMain = new Cuadruplo(20, -1, -1, -1);
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
	ofstream myfile ("Objecto.txt");
  if (myfile.is_open())
  {
	for (std::map<int , Cuadruplo* >::iterator it=record.begin(); it!=record.end(); ++it){
		myfile<<"$"<<it->second->op<<"$"<<it->second->oper1<<"$"<<it->second->oper2<<"$"<<it->second->rec<<"$\n";
	}
    myfile.close();
  }
  else wcout<<"cannot open the file\n";
}


}//termina namespace

