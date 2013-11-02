
#include "Semantica.h"
#include "Parser.h"
#include <iostream>
#include <stdio.h>
#include <wchar.h>

using namespace System;
using namespace std;

int  main(int argc, char *argv[]){
	
	wchar_t *fileName = coco_string_create(argv[1]);
	System::Scanner *scanner= new System::Scanner(fileName);
	System::Parser *parser = new System::Parser(scanner);
	parser->Parse();
	cout<<"The compilation was successful\n";

	
return 0;
}

