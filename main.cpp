#include <iostream>
#include "headders_src/extern.h"

int main()
{
	open_or_create_db();	//открываем базу данных
	std::cout << "hello!" << std::endl;	
	menu_ch = menu();
	std::cout << "result: " << menu_ch << std::endl;
}
