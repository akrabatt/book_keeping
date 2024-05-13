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
/* функция для вывода списка таблиц */
void list_table(sqlite3 *db) // функция автоматического создания таблицы в базе данных если ее не существует
{
    /* выведим список доступных таблиц */
    std::cout << "Tables in the database:" << std::endl;                                                    // Вывод заголовка
    std::string query = "SELECT name FROM sqlite_master WHERE type='table';";                               // текст самого запроса
    std::cout << "Avaliable tables in the database:" << std::endl;                                          // выводим заголовок
    char *errorMessage = nullptr;                                                                           // Объявление указателя на строку ошибки                       // SQL-запрос для получения списка таблиц
    static int tableCount = 1;                                                                              // переменная для нумерации таблиц
    int rc = sqlite3_exec(db, query.c_str(), [](void *data, int argc, char **argv, char **colName) -> int { // Выполнение SQL-запроса, это лямбда функция
        if (argc > 0)                                                                                       // Проверка наличия таблиц в базе данных
        {
            for (int i = 0; i < argc; i++)
            {                                                   // Цикл для вывода списка таблиц
                std::cout << i << ": " << argv[i] << std::endl; // Вывод названия таблицы
            }
        }
        else
        {
            std::cout << "No tables found in the database." << std::endl; // Сообщение о отсутствии таблиц
        }
        return 0; // Возврат успешного завершения
    },
                          nullptr, &errorMessage); // Передача параметров для обработки результатов
    if (rc != SQLITE_OK)
    {                                                            // Проверка на успешное выполнение запроса
        std::cerr << "SQL error: " << errorMessage << std::endl; // Вывод сообщения об ошибке
        sqlite3_free(errorMessage);                              // Освобождение памяти, занятой ошибкой
    }

    /* дадим пользователю ввести название таблицы вручную */
    //
    std::cout << std::endl;
    std::cout << "input table name: ";  // выводим сообщение
    std::string table_name;             // переменная для имени создаваемой таблицы
    std::getline(std::cin, table_name); // Считываем введенное пользователем имя таблицы
    // тут возможно надо раскоментить строку ниже
    // std::cin.ignore();         // Очищаем символ новой строки из буфера

    /* для начала выведим текс что открываем таблицу */
    std::cout << std::endl;
    std::cout << "opening table...";
    std::cout << std::endl;

    /* создаем переменную для сообщения об ошибках */
    char *errMsg = 0; // указатель для сообщения об ошибках
    /* ниже подготавливаем текст для запроса, в него вставляем нашу переменную с названием таблицы */
    std::string cr_tb = "CREATE TABLE IF NOT EXISTS " + table_name + "(id INT PRIMARY KEY NOT NULL,"
                                                                     "title TEXT NOT NULL,"
                                                                     "author TEXT NOT NULL,"
                                                                     "year INT NOT NULL,"
                                                                     "genre TEXT NOT NULL);";

    int exitCode = sqlite3_exec(db, cr_tb.c_str(), 0, 0, &errMsg); // выполняем сам запрос и сохраняем результат его выполнения в переменную

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
/* функция создания таблицы с нуля */
// возможно надо изменеть тип функции с void на int или char
void create_table(sqlite3 *db)
{
}