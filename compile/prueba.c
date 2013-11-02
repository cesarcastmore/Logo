
#include "Semantica.h"
#include "Parser.h"
#include <iostream>
#include <stdio.h>
#include <wchar.h>

using namespace System;
using namespace std;

int  main(){
	int a[3]={3,4,5}; 
	int b[2]={1,2};
	int c[6]={6,7,8,9,10,11};
	int d[2]={12,13};
	
	System::Action *action = new System::Action();
	action->addStackDir(1,1);//paso 1
	action->createCuadro(a);// paso 5
	action->createCuadro(b);// paso 4
	action->addStackOpe(1);// paso 2
	action->addStackDir(2 , 1);//paso 1
	action->createCuadro(a);//paso 5
	action->addStackOpe(3);//paso 3
	action->addStackDir(3,1); //paso 1
	action->createCuadro(a);// paso 5
	action->createCuadro(b);// paso 4
	action->addStackOpe(8); //paso 8
	action->addStackDir(2,1);//paso 1
	action->createCuadro(a);//paso 5
	action->createCuadro(b);// paso 4
	action->createCuadro(c);// paso 9
	action->createCuadro(d);// paso 10
	action->addStackOpe(12); //paso 11
	action->addStackDir(1,1);//paso 1
	action->createCuadro(a);//paso 5
	action->createCuadro(b);// paso 4
	action->addStackOpe(9); //paso 8
	action->addStackDir(3,1);//paso 1
	action->createCuadro(a);//paso 5
	action->createCuadro(b);// paso 4
	action->createCuadro(c);// paso 9
	action->createCuadro(d);// paso 10
	
	
	
	
	
	
	action->showCuadruplo();
	

	
return 0;
}

