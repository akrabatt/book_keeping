/* здесь будут находиться все extern функции и переменные */

#include <vector>

/* заголовочные файлы */
#include "sql_op_cr.h" // заголовочный файл с операциями создания базы данных

/* переменные */
extern int menu_ch;              // переменная для хранения выбранного пункта меню
extern std::string chosen_table; // переменная для хранения наименования выбранной таблицы

/* функции */
extern int menu();                            // функция меню
extern std::string list_table(sqlite3 *db);   // функция по выводу списка таблиц
extern std::string create_table(sqlite3 *db); // функция по созданию таблицы
extern void jump_to_choice(int choice, sqlite3 *db, std::string ch_tb); // функция по переадрисации в зависимости от выбранного значения
extern void del_table(sqlite3 *db, std::string table_name_for_del);	//функция по удалению таблицы из базы данных
extern void add_book_in_table(sqlite3 *db, const std::string &table_name);	//функция добавления книги в таблицу
extern std::vector<std::string> get_tables(sqlite3 *db); // функция для получения вектора таблиц
