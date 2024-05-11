#include <iostream>
#include "headders_src/extern.h"

int main()
{
	//
	// ниже получается создаем переменную т.е. создаем указатель на объект базы данных
	// которую вернула нам наша функция, там мы сможем работать с базой данных и передавать ее
	sqlite3 *db = open_or_create_db(); // открываем базу данных
	//
	create_table(db);
	std::cout << "hello!" << std::endl;
	menu_ch = menu();
	std::cout << "result: " << menu_ch << std::endl;
}
