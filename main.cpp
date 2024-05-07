#include <iostream>
//#include "headders_src/global_var.h"
//#include "functions.cpp"

extern int menu(int arg);
extern int menu_ch;


int main()
{
	std::cout << "hello!" << std::endl;	
	menu(menu_ch);
	std::cout << menu_ch << std::endl;
}
