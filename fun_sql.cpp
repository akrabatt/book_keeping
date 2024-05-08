/* этот файл для написания функций на sqlite3 */

#include <iostream>
#include "sql_src/sqlite3.h"

/* данная функция будте вызываться в самом начале и служит для открытия/создания базы данных */
void open_or_create_db()
{
    /* открываем базу данных */
    sqlite3 *db;    //создаем указатель на базу данных
    char *zErrMsg = 0;  //указатель на строку с сообщение об ошибке
    int exitCode = sqlite3_open("lib_base.db", &db);  //открываем базу данных

    /* вывод об процессе */
    std::cout << std::endl;
    std::cout << "openning database..."; //выводим сообщение о процессе открытия базы данных
    std::cout << std::endl;

    /* проверяем результат процесса открытия базы данных */
    if(exitCode)
    {
        std::cerr << "status: error (open_or_create_db): " << sqlite3_errmsg(db) << std::endl; //выводим сообщение об ошибке
    }
    else
    {
        std::cout << "status: success(open_or_create_db)" << std::endl; //выводим сообщение о успешном открытии базы данных
    }

    //sqlite3_close(db); // Закрываем базу данных
}
