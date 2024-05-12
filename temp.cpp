#include <iostream>  // Подключение заголовочного файла для ввода-вывода данных
#include <sqlite3.h> // Подключение заголовочного файла для работы с базой данных SQLite

void listTables(const char *databasePath, std::string &selectedTable)
{                                 // Функция для вывода списка таблиц и выбора одной из них
    sqlite3 *db;                  // Указатель на базу данных
    char *errorMessage = nullptr; // Указатель на строку ошибки
    static int tableCount = 1;    // Счетчик таблиц

    int rc = sqlite3_open(databasePath, &db); // Открытие базы данных SQLite
    if (rc != SQLITE_OK)
    {                                                                            // Проверка успешного открытия базы данных
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl; // Вывод сообщения об ошибке
        sqlite3_close(db);                                                       // Закрытие базы данных
        return;                                                                  // Выход из функции
    }

    const char *query = "SELECT name FROM sqlite_master WHERE type='table';"; // SQL-запрос для получения списка таблиц
    std::cout << "Available Tables:" << std::endl;                            // Вывод заголовка списка таблиц

    rc = sqlite3_exec(db, query, [](void *data, int argc, char **argv, char **colName) -> int { // Выполнение SQL-запроса
        std::cout << "Table " << tableCount << ": " << argv[0] << std::endl;                    // Вывод названия таблицы с номером
        tableCount++;                                                                           // Увеличение счетчика таблиц
        return 0;                                                                               // Возврат успешного завершения
    },
                      nullptr, &errorMessage); // Параметры для обработки результатов

    if (rc != SQLITE_OK)
    {                                                            // Проверка успешного выполнения запроса
        std::cerr << "SQL error: " << errorMessage << std::endl; // Вывод сообщения об ошибке
        sqlite3_free(errorMessage);                              // Освобождение памяти, занятой ошибкой
    }

    sqlite3_close(db); // Закрытие базы данных SQLite

    std::cout << "Enter the number of the table you want to select: "; // Предложение пользователю выбрать таблицу
    int choice;
    std::cin >> choice; // Чтение выбора пользователя

    rc = sqlite3_open(databasePath, &db); // Повторное открытие базы данных
    if (rc != SQLITE_OK)
    {                                                                            // Проверка успешного открытия базы данных
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl; // Вывод сообщения об ошибке
        sqlite3_close(db);                                                       // Закрытие базы данных
        return;                                                                  // Выход из функции
    }

    tableCount = 1;                                                                                                   // Сброс счетчика таблиц
    rc = sqlite3_exec(db, query, [&selectedTable, choice](void *data, int argc, char **argv, char **colName) -> int { // Выполнение SQL-запроса
        if (tableCount == choice)
        {                            // Если номер таблицы совпадает с выбором пользователя
            selectedTable = argv[0]; // Запись названия выбранной таблицы в переменную
            return 1;                // Прерывание выполнения после выбора таблицы
        }
        tableCount++; // Увеличение счетчика таблиц
        return 0;     // Возврат успешного завершения
    },
                      nullptr, &errorMessage); // Параметры для обработки результатов

    if (rc != SQLITE_OK)
    {                                                            // Проверка успешного выполнения запроса
        std::cerr << "SQL error: " << errorMessage << std::endl; // Вывод сообщения об ошибке
        sqlite3_free(errorMessage);                              // Освобождение памяти, занятой ошибкой
    }

    sqlite3_close(db); // Закрытие базы данных SQLite
}

int main()
{                                                             // Основная функция программы
    const char *databasePath = "путь_к_вашей_базе_данных.db"; // Путь к базе данных SQLite
    std::string selectedTable;                                // Переменная для хранения выбранной таблицы

    listTables(databasePath, selectedTable); // Вызов функции для вывода списка таблиц и выбора одной из них

    std::cout << "Selected table: " << selectedTable << std::endl; // Вывод выбранной таблицы

    return 0; // Возврат успешного завершения программы
}