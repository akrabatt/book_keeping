/* здесь будут находиться все extern функции и переменные */

/* заголовочные файлы */
#include "sql_op_cr.h" // заголовочный файл с операциями создания базы данных

/* переменные */
extern int menu_ch; // переменная для хранения выбранного пункта меню

/* функции */
extern int menu(); // функция меню
extern void list_table(sqlite3 *db);