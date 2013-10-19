

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"


namespace System {


void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::PROGRAMA() {
		Expect(5 /* "program" */);
		Expect(6 /* "{" */);
		InitDeclarations(); 
		action->beginProgram(); 
		while (la->kind == 8 /* "global" */) {
			GLOBAL();
		}
		BLOQUE();
		Expect(7 /* "}" */);
		action->endProgram();
		action->tab->removeAllLocals();
		action->showCuadruplo(); 
		action->createObject();
}

void Parser::GLOBAL() {
		wchar_t* name; int type; 
		Expect(8 /* "global" */);
		TIPO(type);
		IDENTI(name);
		action->addGlobal(name, type, var);  
		if (la->kind == 9 /* "[" */) {
			Get();
			Expect(_integer);
			Expect(10 /* "]" */);
		}
		while (la->kind == 11 /* "," */) {
			Get();
			IDENTI(name);
			action->addGlobal(name, type, var); 
			if (la->kind == 9 /* "[" */) {
				Get();
				Expect(_integer);
				Expect(10 /* "]" */);
			}
		}
		Expect(12 /* ";" */);
}

void Parser::BLOQUE() {
		while (la->kind == 18 /* "module" */) {
			MODULE();
		}
		MAIN();
}

void Parser::TIPO(int  &type) {
		if (la->kind == 13 /* "int" */) {
			Get();
			type =entero; 
		} else if (la->kind == 14 /* "float" */) {
			Get();
			type=flotante; 
		} else SynErr(62);
}

void Parser::IDENTI(wchar_t* &name) {
		Expect(_id);
		name =  coco_string_create(t->val); 
}

void Parser::MODULE() {
		wchar_t* name; int typemod; int no; int can; 
		Expect(18 /* "module" */);
		TIPO_MOD(typemod);
		IDENTI(name);
		action->addNameFunction(name); 
		action->addTypeFunction(typemod);
		Expect(16 /* "(" */);
		if (la->kind == 13 /* "int" */ || la->kind == 14 /* "float" */) {
			PARAMETROS();
		}
		action->addNoParameters(); 
		Expect(17 /* ")" */);
		Expect(6 /* "{" */);
		while (la->kind == 13 /* "int" */ || la->kind == 14 /* "float" */) {
			LOCAL();
		}
		action->addNoLocals();
		action->addContCuadruplo(); 
		while (StartOf(1)) {
			ESTATUTO();
		}
		if (la->kind == 20 /* "return" */) {
			RETORNO();
		}
		action->generateRetorno(); 
		Expect(7 /* "}" */);
		action->removeLocals(); 
}

void Parser::MAIN() {
		Expect(15 /* "main" */);
		Expect(16 /* "(" */);
		Expect(17 /* ")" */);
		int can; 
		Expect(6 /* "{" */);
		action->beginMain(); 
		while (la->kind == 13 /* "int" */ || la->kind == 14 /* "float" */) {
			LOCAL();
		}
		while (StartOf(1)) {
			ESTATUTO();
		}
		Expect(7 /* "}" */);
		action->removeLocals(); 
}

void Parser::LOCAL() {
		wchar_t* name; int type; 
		TIPO(type);
		IDENTI(name);
		action->addLocal(name, type, var);  
		if (la->kind == 9 /* "[" */) {
			Get();
			Expect(_integer);
			Expect(10 /* "]" */);
		}
		while (la->kind == 11 /* "," */) {
			Get();
			IDENTI(name);
			action->addLocal(name, type, var); 
			if (la->kind == 9 /* "[" */) {
				Get();
				Expect(_integer);
				Expect(10 /* "]" */);
			}
		}
		Expect(12 /* ";" */);
}

void Parser::ESTATUTO() {
		switch (la->kind) {
		case 27 /* "read" */: {
			LECTURA();
			break;
		}
		case 30 /* "draw" */: {
			GRAFICO();
			break;
		}
		case _id: {
			ASIGMODULO();
			break;
		}
		case 23 /* "if" */: {
			CONDICION();
			break;
		}
		case 28 /* "print" */: {
			ESCRITURA();
			break;
		}
		case 26 /* "while" */: {
			CICLO();
			break;
		}
		default: SynErr(63); break;
		}
}

void Parser::TIPO_MOD(int &typemod) {
		if (la->kind == 13 /* "int" */ || la->kind == 14 /* "float" */) {
			int type;  
			TIPO(type);
			typemod = type; 
		} else if (la->kind == 19 /* "void" */) {
			Get();
			typemod = undefined; 
		} else SynErr(64);
}

void Parser::PARAMETROS() {
		PARAM_COMA();
		while (la->kind == 11 /* "," */) {
			Get();
			PARAM_COMA();
		}
}

void Parser::RETORNO() {
		Expect(20 /* "return" */);
		EXP();
		Expect(12 /* ";" */);
}

void Parser::PARAM_COMA() {
		wchar_t* name; int type; int can; 
		TIPO(type);
		IDENTI(name);
		action->addLocal(name, type, var); 
		action->addParameter(type); 
}

void Parser::EXP() {
		int term; 
		TERMINO();
		if (la->kind == 21 /* "-" */ || la->kind == 22 /* "+" */) {
			TERM_OP(term);
			action->addStackOpe(term); 
			EXP();
		}
		action->createCuadroOpeTerm(); 
}

void Parser::VARCTE() {
		int signo; float number; Direction *dir; int tipo;
		if (la->kind == 21 /* "-" */ || la->kind == 22 /* "+" */) {
			if (la->kind == 21 /* "-" */) {
				Get();
				signo=-1; 
			} else {
				Get();
				signo=1; 
			}
		}
		if (la->kind == _integer) {
			Get();
			swscanf(t->val, L"%f", &number); tipo=entero; 
		} else if (la->kind == _floating) {
			Get();
			swscanf(t->val, L"%f", &number); tipo=flotante; 
		} else SynErr(65);
		dir=action->memory->save(3,tipo, number*signo);
		action->dir->push(dir); 
}

void Parser::LECTURA() {
		wchar_t* name; Variable *obj; 
		Expect(27 /* "read" */);
		Expect(16 /* "(" */);
		IDENTI(name);
		obj=action->find(name, var);
		action->createCuadrRead(obj->dir->direction, obj->type); 
		Expect(17 /* ")" */);
		Expect(12 /* ";" */);
}

void Parser::GRAFICO() {
		int figure; 
		Expect(30 /* "draw" */);
		FIGURA(figure);
		action->beginDraw(figure); 
		Expect(31 /* ":" */);
		while (StartOf(2)) {
			ATRIBUTOS();
		}
		Expect(32 /* "end" */);
		action->endDraw();
}

void Parser::ASIGMODULO() {
		wchar_t* name; Variable *obj; 
		IDENTI(name);
		if (la->kind == 16 /* "(" */) {
			obj=action->find(name, function);
			LLAMAR_MODULO();
		} else if (la->kind == 9 /* "[" */ || la->kind == 25 /* "=" */) {
			obj=action->find(name, var); 
			action->addStackDir(obj->dir->direction,obj->type); 
			ASIGNACION();
		} else SynErr(66);
}

void Parser::CONDICION() {
		Expect(23 /* "if" */);
		Expect(16 /* "(" */);
		EXPRESION();
		Expect(17 /* ")" */);
		action->createGotoFalseCond(); 
		Expect(6 /* "{" */);
		while (StartOf(1)) {
			ESTATUTO();
		}
		Expect(7 /* "}" */);
		if (la->kind == 24 /* "else" */) {
			Get();
			action->createGotoFinCond(); 
			Expect(6 /* "{" */);
			while (StartOf(1)) {
				ESTATUTO();
			}
			Expect(7 /* "}" */);
		}
		action->createEndCond(); 
}

void Parser::ESCRITURA() {
		Expect(28 /* "print" */);
		Expect(16 /* "(" */);
		if (StartOf(3)) {
			EXP();
		} else if (la->kind == _chain) {
			Get();
		} else SynErr(67);
		action->createCuadrPrint(); 
		while (la->kind == 29 /* "|" */) {
			Get();
			if (StartOf(3)) {
				EXP();
			} else if (la->kind == _chain) {
				Get();
			} else SynErr(68);
			action->createCuadrPrint(); 
		}
		Expect(17 /* ")" */);
		Expect(12 /* ";" */);
}

void Parser::CICLO() {
		Expect(26 /* "while" */);
		action->addStackLeapActual(); 
		Expect(16 /* "(" */);
		EXPRESION();
		Expect(17 /* ")" */);
		action->createGotoFalWhile(); 
		Expect(6 /* "{" */);
		while (StartOf(1)) {
			ESTATUTO();
		}
		action->createEndWhile(); 
		Expect(7 /* "}" */);
}

void Parser::EXPRESION() {
		int log; 
		EXP_LOG();
		while (la->kind == 56 /* "and" */ || la->kind == 57 /* "or" */) {
			LOG_OPE(log);
			action->addStackOpe(log); 
			EXP_LOG();
		}
		action->createCuadroOpeLog(); 
}

void Parser::EXP_LOG() {
		int rel; 
		EXP();
		if (StartOf(4)) {
			RELACIONAL(rel);
			action->addStackOpe(rel); 
			EXP();
		}
		action->createCuadroOpeRel(); 
}

void Parser::LOG_OPE(int &log) {
		if (la->kind == 56 /* "and" */) {
			Get();
			log=log_and;
		} else if (la->kind == 57 /* "or" */) {
			Get();
			log=log_or; 
		} else SynErr(69);
}

void Parser::RELACIONAL(int &rel) {
		switch (la->kind) {
		case 50 /* "==" */: {
			Get();
			rel=eq;
			break;
		}
		case 51 /* "<>" */: {
			Get();
			rel=ne;
			break;
		}
		case 52 /* "<=" */: {
			Get();
			rel=lte;
			break;
		}
		case 53 /* ">=" */: {
			Get();
			rel=gte;
			break;
		}
		case 54 /* "<" */: {
			Get();
			rel=lt;
			break;
		}
		case 55 /* ">" */: {
			Get();
			rel=gt;;
			break;
		}
		default: SynErr(70); break;
		}
}

void Parser::LLAMAR_MODULO() {
		Expect(16 /* "(" */);
		if (StartOf(3)) {
			EXP();
			while (la->kind == 11 /* "," */) {
				Get();
				EXP();
			}
		}
		Expect(17 /* ")" */);
		Expect(12 /* ";" */);
}

void Parser::ASIGNACION() {
		wchar_t* name; 
		if (la->kind == 9 /* "[" */) {
			Get();
			if (la->kind == _id) {
				IDENTI(name);
			} else if (la->kind == _integer) {
				Get();
			} else SynErr(71);
			Expect(10 /* "]" */);
		}
		Expect(25 /* "=" */);
		EXP();
		Expect(12 /* ";" */);
		action->createCuadrAlloca(); 
}

void Parser::PARENTESIS() {
		Expect(16 /* "(" */);
		if (StartOf(3)) {
			EXP();
			while (la->kind == 11 /* "," */) {
				Get();
				EXP();
			}
		}
		Expect(17 /* ")" */);
}

void Parser::TERMINO() {
		int fact; 
		FACTOR();
		if (la->kind == 58 /* "*" */ || la->kind == 59 /* "/" */ || la->kind == 60 /* "%" */) {
			FACTOR_OP(fact);
			action->addStackOpe(fact); 
			TERMINO();
		}
		action->createCuadroOpeFact(); 
}

void Parser::TERM_OP(int &term) {
		if (la->kind == 22 /* "+" */) {
			Get();
			term=plus;
		} else if (la->kind == 21 /* "-" */) {
			Get();
			term=minus;
		} else SynErr(72);
}

void Parser::FACTOR() {
		wchar_t* name; Variable *obj; int signo; Direction *dir; 
		if (la->kind == 16 /* "(" */) {
			Get();
			action->addFake(); 
			EXPRESION();
			Expect(17 /* ")" */);
			action->removeFake(); 
		} else if (StartOf(5)) {
			VARCTE();
		} else if (la->kind == _id) {
			IDENTI(name);
			if (StartOf(6)) {
				obj=action->find(name, var); 
				action->addStackDir(obj->dir->direction,obj->type); 
			} else if (la->kind == 16 /* "(" */) {
				PARENTESIS();
				obj=action->find(name, function); 
			} else SynErr(73);
		} else SynErr(74);
}

void Parser::FACTOR_OP(int &fact) {
		if (la->kind == 58 /* "*" */) {
			Get();
			fact=multiply;
		} else if (la->kind == 59 /* "/" */) {
			Get();
			fact=divide;
		} else if (la->kind == 60 /* "%" */) {
			Get();
			fact=module;
		} else SynErr(75);
}

void Parser::FIGURA(int &figure) {
		switch (la->kind) {
		case 33 /* "point" */: {
			Get();
			figure=31;
			break;
		}
		case 34 /* "line" */: {
			Get();
			figure=32;
			break;
		}
		case 35 /* "triangle" */: {
			Get();
			figure=33;
			break;
		}
		case 36 /* "square" */: {
			Get();
			figure=34;
			break;
		}
		case 37 /* "circle" */: {
			Get();
			figure=35;
			break;
		}
		case 38 /* "star" */: {
			Get();
			figure=36;
			break;
		}
		case 39 /* "pentagon" */: {
			Get();
			figure=37;
			break;
		}
		case 40 /* "hexagon" */: {
			Get();
			figure=38;
			break;
		}
		case 41 /* "rhomboid" */: {
			Get();
			figure=39;
			break;
		}
		default: SynErr(76); break;
		}
}

void Parser::ATRIBUTOS() {
		if (StartOf(7)) {
			ATRIBUTO_ENTERO();
		} else if (StartOf(8)) {
			ATRIBUTO_STRING();
		} else SynErr(77);
		Expect(12 /* ";" */);
}

void Parser::ATRIBUTO_ENTERO() {
		int attribute; 
		if (la->kind == 42 /* "y_position" */) {
			Get();
			attribute=1;
		} else if (la->kind == 43 /* "x_position" */) {
			Get();
			attribute=2;
		} else if (la->kind == 44 /* "rotateRight" */) {
			Get();
			attribute=3;
		} else if (la->kind == 45 /* "rotateLeft" */) {
			Get();
			attribute=4;
		} else SynErr(78);
		Expect(25 /* "=" */);
		EXP();
		action->addAtributeInt(attribute);
}

void Parser::ATRIBUTO_STRING() {
		wchar_t* name; int attribute; 
		if (la->kind == 46 /* "size" */) {
			Get();
			attribute=5;
		} else if (la->kind == 47 /* "thick" */) {
			Get();
			attribute=6;
		} else if (la->kind == 48 /* "colorThick" */) {
			Get();
			attribute=7;
		} else if (la->kind == 49 /* "colorFigure" */) {
			Get();
			attribute=8;
		} else SynErr(79);
		Expect(25 /* "=" */);
		CADENA(name);
		action->addAtributeString(attribute, name);
}

void Parser::CADENA(wchar_t* &name) {
		Expect(_chain);
		name =  coco_string_create(t->val); 
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	PROGRAMA();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 61;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[9][63] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,T, x,x,T,T, T,x,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, T,T,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,T,T,x, T,x,x,x, x,x,x,x, x,x,x,x, T,x,x,x, x,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, T,T,T,T, x,x,x,x, x,x,x},
		{x,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,T, T,x,x,x, x,T,x,x, x,T,T,x, x,x,x,x, x,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, T,T,T,T, T,T,T,T, T,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"integer expected"); break;
			case 2: s = coco_string_create(L"floating expected"); break;
			case 3: s = coco_string_create(L"chain expected"); break;
			case 4: s = coco_string_create(L"id expected"); break;
			case 5: s = coco_string_create(L"\"program\" expected"); break;
			case 6: s = coco_string_create(L"\"{\" expected"); break;
			case 7: s = coco_string_create(L"\"}\" expected"); break;
			case 8: s = coco_string_create(L"\"global\" expected"); break;
			case 9: s = coco_string_create(L"\"[\" expected"); break;
			case 10: s = coco_string_create(L"\"]\" expected"); break;
			case 11: s = coco_string_create(L"\",\" expected"); break;
			case 12: s = coco_string_create(L"\";\" expected"); break;
			case 13: s = coco_string_create(L"\"int\" expected"); break;
			case 14: s = coco_string_create(L"\"float\" expected"); break;
			case 15: s = coco_string_create(L"\"main\" expected"); break;
			case 16: s = coco_string_create(L"\"(\" expected"); break;
			case 17: s = coco_string_create(L"\")\" expected"); break;
			case 18: s = coco_string_create(L"\"module\" expected"); break;
			case 19: s = coco_string_create(L"\"void\" expected"); break;
			case 20: s = coco_string_create(L"\"return\" expected"); break;
			case 21: s = coco_string_create(L"\"-\" expected"); break;
			case 22: s = coco_string_create(L"\"+\" expected"); break;
			case 23: s = coco_string_create(L"\"if\" expected"); break;
			case 24: s = coco_string_create(L"\"else\" expected"); break;
			case 25: s = coco_string_create(L"\"=\" expected"); break;
			case 26: s = coco_string_create(L"\"while\" expected"); break;
			case 27: s = coco_string_create(L"\"read\" expected"); break;
			case 28: s = coco_string_create(L"\"print\" expected"); break;
			case 29: s = coco_string_create(L"\"|\" expected"); break;
			case 30: s = coco_string_create(L"\"draw\" expected"); break;
			case 31: s = coco_string_create(L"\":\" expected"); break;
			case 32: s = coco_string_create(L"\"end\" expected"); break;
			case 33: s = coco_string_create(L"\"point\" expected"); break;
			case 34: s = coco_string_create(L"\"line\" expected"); break;
			case 35: s = coco_string_create(L"\"triangle\" expected"); break;
			case 36: s = coco_string_create(L"\"square\" expected"); break;
			case 37: s = coco_string_create(L"\"circle\" expected"); break;
			case 38: s = coco_string_create(L"\"star\" expected"); break;
			case 39: s = coco_string_create(L"\"pentagon\" expected"); break;
			case 40: s = coco_string_create(L"\"hexagon\" expected"); break;
			case 41: s = coco_string_create(L"\"rhomboid\" expected"); break;
			case 42: s = coco_string_create(L"\"y_position\" expected"); break;
			case 43: s = coco_string_create(L"\"x_position\" expected"); break;
			case 44: s = coco_string_create(L"\"rotateRight\" expected"); break;
			case 45: s = coco_string_create(L"\"rotateLeft\" expected"); break;
			case 46: s = coco_string_create(L"\"size\" expected"); break;
			case 47: s = coco_string_create(L"\"thick\" expected"); break;
			case 48: s = coco_string_create(L"\"colorThick\" expected"); break;
			case 49: s = coco_string_create(L"\"colorFigure\" expected"); break;
			case 50: s = coco_string_create(L"\"==\" expected"); break;
			case 51: s = coco_string_create(L"\"<>\" expected"); break;
			case 52: s = coco_string_create(L"\"<=\" expected"); break;
			case 53: s = coco_string_create(L"\">=\" expected"); break;
			case 54: s = coco_string_create(L"\"<\" expected"); break;
			case 55: s = coco_string_create(L"\">\" expected"); break;
			case 56: s = coco_string_create(L"\"and\" expected"); break;
			case 57: s = coco_string_create(L"\"or\" expected"); break;
			case 58: s = coco_string_create(L"\"*\" expected"); break;
			case 59: s = coco_string_create(L"\"/\" expected"); break;
			case 60: s = coco_string_create(L"\"%\" expected"); break;
			case 61: s = coco_string_create(L"??? expected"); break;
			case 62: s = coco_string_create(L"invalid TIPO"); break;
			case 63: s = coco_string_create(L"invalid ESTATUTO"); break;
			case 64: s = coco_string_create(L"invalid TIPO_MOD"); break;
			case 65: s = coco_string_create(L"invalid VARCTE"); break;
			case 66: s = coco_string_create(L"invalid ASIGMODULO"); break;
			case 67: s = coco_string_create(L"invalid ESCRITURA"); break;
			case 68: s = coco_string_create(L"invalid ESCRITURA"); break;
			case 69: s = coco_string_create(L"invalid LOG_OPE"); break;
			case 70: s = coco_string_create(L"invalid RELACIONAL"); break;
			case 71: s = coco_string_create(L"invalid ASIGNACION"); break;
			case 72: s = coco_string_create(L"invalid TERM_OP"); break;
			case 73: s = coco_string_create(L"invalid FACTOR"); break;
			case 74: s = coco_string_create(L"invalid FACTOR"); break;
			case 75: s = coco_string_create(L"invalid FACTOR_OP"); break;
			case 76: s = coco_string_create(L"invalid FIGURA"); break;
			case 77: s = coco_string_create(L"invalid ATRIBUTOS"); break;
			case 78: s = coco_string_create(L"invalid ATRIBUTO_ENTERO"); break;
			case 79: s = coco_string_create(L"invalid ATRIBUTO_STRING"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}

} // namespace

