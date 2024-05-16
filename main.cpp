#include <iostream>
#include "headders_src/extern.h"

int main()
{
	/* откроем базу данных */
	sqlite3 *db = open_or_create_db(); // открываем базу данных
	/* дадим выбор пользователю с какой таблицей работать либо создать новую */
	chosen_table  = list_table(db);	//запишем результат в глобальную переменную

	std::cout << "its work: "<< chosen_table;
	menu_ch = menu();
	jump_to_choice(menu_ch, db, chosen_table);
	//std::cout << "hello!" << std::endl;
	//std::cout << "result: " << menu_ch << std::endl;
}
