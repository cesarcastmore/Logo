

#if !defined(System_COCO_PARSER_H__)
#define System_COCO_PARSER_H__

#include "Semantica.h"
#include "wchar.h"
#include <iostream>
#include <string>
#include <map>
#include <fstream>


#include "Scanner.h"

namespace System {


class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_integer=1,
		_floating=2,
		_chain=3,
		_id=4
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

Action *action;

void Err(wchar_t* msg) {
	errors->Error(la->line, la->col, msg);
}


int var, para, entero, flotante, undefined;

int plus, minus, multiply, divide, module, eq, ne, gt, lt, gte, lte, log_and ,log_or ;

void InitDeclarations() { 
	
	var=0; para=1;
	undefined=0;entero=1; flotante=2;
	plus=1; minus=2;
	multiply=3; divide=4; module=5;
	eq=6; ne=7; gt=8; lt=9; gte=10; lte=11;
	log_and=12; log_or=13;
	action=new Action();
}



	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void PROGRAMA();
	void GLOBAL();
	void BLOQUE();
	void TIPO(int  &type);
	void IDENTI(wchar_t* &name);
	void MODULE();
	void MAIN();
	void LOCAL();
	void ESTATUTO();
	void TIPO_MOD(int &typemod);
	void PARAMETROS();
	void RETORNO();
	void PARAM_COMA();
	void EXP();
	void VARCTE();
	void LECTURA();
	void GRAFICO();
	void ASIGMODULO();
	void CONDICION();
	void ESCRITURA();
	void CICLO();
	void EXPRESION();
	void EXP_LOG();
	void LOG_OPE(int &log);
	void RELACIONAL(int &rel);
	void LLAMAR_MODULO();
	void ASIGNACION();
	void PARENTESIS();
	void TERMINO();
	void TERM_OP(int &term);
	void FACTOR();
	void FACTOR_OP(int &fact);
	void FIGURA(int &figure);
	void ATRIBUTOS();
	void ATRIBUTO_ENTERO();
	void ATRIBUTO_STRING();
	void CADENA(wchar_t* &name);

	void Parse();

}; // end Parser

} // namespace


#endif

