/* этот файл для написания функций на sqlite3 */

#include <iostream>
// #include <vector>
#include <map>
#include "sql_src/sqlite3.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* функция создания таблицы с нуля */
std::string create_table(sqlite3 *db)
{
    /* дадим пользователю ввести название таблицы вручную */
    //
    std::cout << std::endl;
    std::cout << "input table name: "; // выводим сообщение
    std::string table_name;            // переменная для имени создаваемой таблицы
    /* V!!! ОБЯЗАТЕЛЬНО использовать ignore !!!V */
    std::cin.ignore();                  // Очищаем символ новой строки из буфера
    std::getline(std::cin, table_name); // Считываем введенное пользователем имя таблицы

    /* для начала выведим текс что открываем таблицу */
    std::cout << std::endl;
    std::cout << "opening table...";
    std::cout << std::endl;

    /* создаем переменную для сообщения об ошибках */
    char *errMsg = 0; // указатель для сообщения об ошибках
    /* ниже подготавливаем текст для запроса, в него вставляем нашу переменную с названием таблицы */
    std::string cr_tb = "CREATE TABLE IF NOT EXISTS " + table_name + " (id INT PRIMARY KEY NOT NULL,"
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
    return table_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* данная функция выводит список таблиц, если их нет то предлогает создать таблицу */
std::string list_table(sqlite3 *db) // функция автоматического создания таблицы в базе данных если ее не существует
{
    /* выведим список доступных таблиц */
    std::cout << "Tables in the database:" << std::endl;                      // Вывод заголовка
    std::string query = "SELECT name FROM sqlite_master WHERE type='table';"; // текст самого запроса
    std::cout << "Avaliable tables in the database:" << std::endl;            // выводим заголовок

    char *errorMessage = nullptr; // Объявление указателя на строку ошибки
    static int tableCount = 1;    // переменная для нумерации таблиц

    /* флаг сработает тогда если запрос ниже даст результат */
    static bool tableFound = false; // флаг для находа книг

    /* переменная для выбора номера таблицы */
    int numTable;

    /* переменная для хранения наименования выбранной таблицы */
    std::string chosenTable; // будет храниться имя выбранной таблицы

    /* список для хранения списка таблиц в БД чтоб можно было их выдергивать по ключу */
    static std::map<int, std::string> tables; // список

    /* выводим предложение о создании новой таблицы для начала */
    std::cout << std::endl;                            //
    std::cout << "none_ 0: create table" << std::endl; // выведем сообщение о том что набрав ноль пользователь сможет создать новую таблицу

    /* выводим список таблиц */
    int rc = sqlite3_exec(db, query.c_str(), [](void *data, int argc, char **argv, char **colName) -> int { // Выполнение SQL-запроса, это лямбда функция
        if (argc > 0)                                                                                       // Проверка наличия таблиц в базе данных
        {
            std::cout << "Table " << tableCount << ": " << argv[0] << std::endl; // Вывод названия таблицы с номером
            // tables.push_back(std::string(argv[0]));                              // Добавление названия таблицы в векторs
            tables[tableCount] = std::move(argv[0]);
            tableCount++;
            tableFound = true; // Установка флага на нахождение таблицы
        }
        return 0; // Возврат успешного завершения
    },
                          nullptr, &errorMessage); // Передача параметров для обработки результатов

    if (rc != SQLITE_OK)                                         // проверка запроса
    {                                                            // Проверка на успешное выполнение запроса
        std::cerr << "SQL error: " << errorMessage << std::endl; // Вывод сообщения об ошибке
        sqlite3_free(errorMessage);                              // Освобождение памяти, занятой ошибкой
    }
    /* обнуляем tableCount */
    tableCount = 1; // обнуляем tableCount

    /* тестовый блок для отладки */
    // for (const auto &row : tables)
    // {
    //     // std::cout <<row.first << " " << row.second << std::endl;
    //     std::cout << row.second << std::endl;
    // }

    /* проверка если не найдены таблицы и флаг поднят */
    if (!tableFound) // Проверка на нахождение таблицы
    {
        tableFound = false;                                           // обнуляем
        std::cout << std::endl;                                       //
        std::cout << "No tables found in the database." << std::endl; // Сообщение о отсутствии таблиц
        chosenTable = create_table(db);                               // создаем таблицу
    }
    /* выводим сообщение о предолжении ввести номер таблицы или создать новую */
    else
    {
        std::cout << std::endl;
        std::cout << "Choose your table(input num): "; // Сообщение о предложении выбора таблицы
        std::cin >> numTable;                          // воодим значение таблицы
        std::cout << std::endl
                  << std::endl;

        /* создаем условие если выбран 0 то создаем таблицу, если > 0 то заносим имя таблицы в переменную */
        if (numTable == 0) // если 0, то создам таблицу
        {
            chosenTable = create_table(db); // создаем таблицу
        }
        else // запускаем выборку таблиц
        {
            for (const auto &row : tables)
            {
                /* ищем совпадение по ключу */
                if (row.first == numTable) //
                {
                    std::cout << "find !" << std::endl;
                    chosenTable = row.second; // добавляем в нашу переменную для хранения работы с таблицой наименования таблицы
                    std::cout << chosenTable << std::endl;
                }
            }
        }
    }
    return chosenTable; // возвращаем значение выбранной таблицы
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* эта функция служит для удаления таблицы */
void del_table(sqlite3 *db, std::string table_name_for_del)
{
    /* блок диалога с пользователем, уверен ли он в удалении */
    std::cout << std::endl;
    std::cout << "ATANTION !" << std::endl;
    std::cout << "are you sure you want to delete !?" << std::endl;
    std::cout << "1 - yes" << std::endl;
    std::cout << "2 - no" << std::endl;
    std::cout << "...: ";

    int i;         // переменная выбора
    std::cin >> i; // вводим значени

    /* обработка введенного значения */
    switch (i)
    {

    case 1: // удаляем
        /* code */
        break;

    case 2: // отмена, возвращаем
        // return
        break;

    default:
        break;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
