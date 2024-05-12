/* этот файл для написания функций на sqlite3 */

#include <iostream>
#include "sql_src/sqlite3.h"

//
/* данная функция будте вызываться в самом начале и служит для открытия/создания базы данных */
sqlite3 *open_or_create_db()
{
    /* открываем базу данных */
    sqlite3 *db;                                     // создаем указатель на базу данных
    int exitCode = sqlite3_open("lib_base.db", &db); // открываем базу данных

    /* вывод об процессе */
    std::cout << std::endl;
    std::cout << "openning database..."; // выводим сообщение о процессе открытия базы данных
    std::cout << std::endl;

    /* проверяем результат процесса открытия базы данных */
    if (exitCode)
    {
        std::cerr << "status: error (open_or_create_db): " << sqlite3_errmsg(db) << std::endl; // выводим сообщение об ошибке
    }
    else
    {
        std::cout << "status: success(open_or_create_db)" << std::endl; // выводим сообщение о успешном открытии базы данных
    }

    // sqlite3_close(db); // Закрываем базу данных
    return db; // возвращаем указатель на базу данных
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* данная функция служит для создания таблицы */
void create_table(sqlite3 *db)  //функция автоматического создания таблицы в базе данных если ее не существует
{
    /* для начала выведим текс что открываем таблицу */
    std::cout << std::endl;
    std::cout << "opening table...";
    std::cout << std::endl;

    /* создаем переменную для сообщения об ошибках */
    char *errMsg = 0; // указатель для сообщения об ошибках
    /* ниже подготавливаем текси запроса */
    const char *sql = "CREATE TABLE IF NOT EXISTS BOOKS_2 (id INT PRIMARY KEY NOT NULL,"
                      "title TEXT NOT NULL,"
                      "author TEXT NOT NULL,"
                      "year INT NOT NULL,"
                      "genre TEXT NOT NULL);";

    int exitCode = sqlite3_exec(db, sql, 0, 0, &errMsg); // выполняем сам запрос и сохраняем результат его выполнения в переменную

    /* выполним проверку */
    if (exitCode)
    {
        std::cerr << "status: error (create_table): " << errMsg << std::endl; // выводим сообщение об ошибке
    }
    else
    {
        std::cout << "status: success(create_table)" << std::endl; // выводим сообщение о успешном создании таблицы
    }

    sqlite3_free(errMsg); // освобождаем память
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
