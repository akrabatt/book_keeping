/* этот файл для написания функций на sqlite3 */
#include <iostream>
#include <map>     // для списков
#include <vector>  //для векторов
#include <string>  //для строк
#include <utility> //для пары
#include <limits>
#include "sql_src/sqlite3.h"
#include "headders_src/extern.h"
#include "headders_src/str_book.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Открывает базу данных SQLite или создает новую, если она не существует.
 *
 * Функция открывает базу данных с именем "lib_base.db". Если файл базы данных не существует,
 * он будет создан. Возвращает указатель на объект базы данных SQLite.
 *
 * @return Указатель на объект базы данных SQLite.
 */
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
/**
 * @brief Создает новую таблицу в базе данных SQLite, если она не существует.
 *
 * Функция запрашивает у пользователя имя для новой таблицы и создает таблицу с полями id, title, author, year, genre,
 * если таблицы с таким именем еще нет в базе данных. Возвращает имя созданной таблицы.
 *
 * @param db Указатель на объект базы данных SQLite.
 * @return Имя созданной таблицы.
 */
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
/**
 * @brief Отображает список таблиц в базе данных SQLite и позволяет выбрать одну из них или создать новую таблицу.
 *
 * Функция выполняет SQL-запрос для получения списка всех таблиц в базе данных и выводит их на экран с порядковыми номерами.
 * Пользователь может выбрать существующую таблицу по номеру или создать новую таблицу, выбрав соответствующий вариант.
 * В случае отсутствия таблиц в базе данных, функция автоматически создает новую таблицу.
 *
 * @param db Указатель на объект базы данных SQLite.
 * @return Имя выбранной таблицы или вновь созданной таблицы.
 */
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
        // chosenTable = create_table(db);                               // создаем таблицу
        chosen_table = create_table(db); // создаем таблицу
    }
    /* выводим сообщение о предолжении ввести номер таблицы или создать новую */
    else
    {
        std::cout << std::endl;
        std::cout << "Choose your table(input num): "; // Сообщение о предложении выбора таблицы
        std::cin >> numTable;                          // воодим значение таблицы
        std::cout << std::endl
                  << std::endl;

        /* создадим переменные для проверки адекватности введенного значения */
        // auto numTable_begin = tables.begin(); // эта переменная для начального значения списка с таблицами

        auto numTable_end = tables.end(); // эта переменная для конечного значения списка с таблицами
        if (numTable < 0 || numTable > numTable_end->first)
        {
            std::cout << std::endl;
            std::cout << "ERROR: the key was entered incorrectly"
                      << ::std::endl;
            chosen_table = list_table(db);
        }
        /* создаем условие если выбран 0 то создаем таблицу, если > 0 то заносим имя таблицы в переменную */
        if (numTable == 0) // если 0, то создам таблицу
        {
            // chosenTable = create_table(db); // создаем таблицу
            chosen_table = create_table(db); // создаем таблицу
        }
        else // запускаем выборку таблиц
        {
            for (const auto &row : tables)
            {
                /* ищем совпадение по ключу */
                if (row.first == numTable) //
                {
                    std::cout << "find !" << std::endl;
                    // chosenTable = row.second; // добавляем в нашу переменную для хранения работы с таблицой наименования таблицы
                    chosen_table = row.second; // добавляем в нашу переменную для хранения работы с таблицой наименования таблицы
                    // std::cout << chosenTable << std::endl;
                    std::cout << chosen_table << std::endl;
                }
            }
        }
    }
    // return chosenTable; // возвращаем значение выбранной таблицы
    return chosen_table; // возвращаем значение выбранной таблицы
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Удаляет указанную таблицу из базы данных SQLite после подтверждения пользователя.
 *
 * Функция запрашивает у пользователя подтверждение на удаление таблицы. Если пользователь подтверждает
 * удаление, то функция выполняет SQL-запрос для удаления указанной таблицы. В случае отмены удаления или
 * некорректного ввода, функция возвращается в меню.
 *
 * @param db Указатель на объект базы данных SQLite.
 * @param table_name_for_del Имя таблицы, которую необходимо удалить.
 */
void del_table(sqlite3 *db, std::string table_name_for_del) // функция принимает указатель на БД и наименование таблицы
{
    /* блок диалога с пользователем, уверен ли он в удалении */
    std::cout << std::endl;
    std::cout << "ATANTION !" << std::endl;
    std::cout << "are you sure you want to delete !?" << std::endl;
    std::cout << "1 - yes" << std::endl;
    std::cout << "2 - no" << std::endl;
    std::cout << "...: ";

    int i;  // переменная выбора
    int rc; // переменная для запроса
    char *sqlite3_errmsg;

    while (true)
    {
        std::cin >> i;

        if (std::cin.fail() || i < 1 || i > 4)
        {
            std::cin.clear();                                                   // Очистка флагов ошибок
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера
            std::cout << "Invalid input. Please enter a number between 1 or 2: ";
        }
        else
        {
            break; // Корректный ввод
        }
    }

    /* создаем текст для sql запроса */
    std::string dl_tbl = "DROP TABLE IF EXISTS " + table_name_for_del + ";";

    // int menu_again; // определяем переменную для возврата в меню

    /* обработка введенного значения */
    switch (i)
    {
        /* позиция если согласен с удалением */
    case 1:                                                                       // удаляем
        rc = sqlite3_exec(db, dl_tbl.c_str(), nullptr, nullptr, &sqlite3_errmsg); // удаляем таблицу

        /* проверяем */
        if (rc != SQLITE_OK)
        {
            std::cerr << "SQL error: " << sqlite3_errmsg << std::endl; // Вывод сообщения об ошибке
            sqlite3_free(sqlite3_errmsg);                              // Освобождение памяти, занятой ошибкой
        }

        /* возвращаемся обратно */
        chosen_table = list_table(db);                    // функция по выводу списка таблиц
        menu_ch = menu();                                 // возвращаемся в меню
        return jump_to_choice(menu_ch, db, chosen_table); // обрабатываем опять
        break;

        /* позиция если отменяешь удаление */
    case 2:               // отмена, возвращаем
        menu_ch = menu(); // возвращаемся в меню

        return jump_to_choice(menu_ch, db, table_name_for_del); // обрабатываем опять
        break;

    default: // неправильное значение
        /* если выбрано неправильное значение, то возвращаем функцию заново */
        std::cout << std::endl;
        std::cout << "wrong value" << std::endl; // выводим сообщение об ошибке
        std::cout << std::endl;
        del_table(db, table_name_for_del); // рекурсивно вызываем функцию
        break;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Добавляет книгу в указанную таблицу базы данных SQLite.
 *
 * Функция запрашивает у пользователя данные о книге (название, автор, жанр, год) и добавляет запись в таблицу.
 * Перед добавлением определяется максимальный существующий идентификатор (id) и новый id назначается на 1 больше.
 * Если таблица пуста, то id будет равен 1.
 *
 * @param db Указатель на объект базы данных SQLite.
 * @param table_name Имя таблицы, в которую будет добавлена запись.
 */
void add_book_in_table(sqlite3 *db, const std::string &table_name)
{
    /* создадим переменные которые будут служить для данных о книге */
    std::string title, author, genre; // строковые переменные
    int year, id;                     // численная переменная
    sqlite3_stmt *stmt_id;            // указатель для выражения, будет служить для id

    /* запрос для получения максимального id */
    std::string sql_id = "SELECT MAX(id) FROM " + table_name + ";"; // запрос для получения максимального значения

    /* подготавливаем запрос на получения id */
    int result = sqlite3_prepare_v2(db, sql_id.c_str(), -1, &stmt_id, NULL); // заносим в переменную результат подготовки запроса
    if (result != SQLITE_OK)                                                 // если запрос подготовлен неуспешно
    {
        std::cerr << "SQLite error: " << sqlite3_errmsg(db) << std::endl; // сообщение об ошибке
        return;
    }

    result = sqlite3_step(stmt_id); // выполним шаг
    if (result == SQLITE_ROW)
    {
        id = sqlite3_column_int(stmt_id, 0); // заносим в переменную id максимальное значение
        id++;                                // увеличиваем id
    }
    else
    {
        id = 1; // если таблица пустая, то присваиваем уникальное значение
    }

    sqlite3_finalize(stmt_id); // освобождаем память

    /* далее начнем воод данных */
    /* название таблицы */
    std::cout << "enter book title: ";
    std::cin.ignore(); // очищаем буфер
    std::getline(std::cin, title);
    std::cout << std::endl;

    /* автор */
    std::cout << "enter book author: ";
    // std::cin.ignore(); // очищаем буфер
    std::getline(std::cin, author);
    std::cout << std::endl;

    /* жанр */
    std::cout << "enter book genre: ";
    // std::cin.ignore(); // очищаем буфер
    std::getline(std::cin, genre);
    std::cout << std::endl;

    /* год */
    // Проверка ввода года
    while (true) // пока не будет прерван с помощью breake
    {
        /* пердлагаем ввод */
        std::cout << "enter book year: ";
        std::cin >> year;

        // Проверка на успешное преобразование в число
        if (std::cin.fail()) // проверяет успешно ли прошло приобразование введенного значения в число, если НЕ успешно
        {
            std::cin.clear();                                                      // Очистка флага ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');    // Очистка потока
            std::cout << "Invalid input. Please enter a valid year." << std::endl; // выводим сообщение об ошибкеы
        }
        else // если введено правильно
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка остатка потока

            break; // ВЫХОД
        }
    }

    /* создадим запрос */
    std::string sql_add_book = "INSERT INTO " + table_name + " (id, title, author, year, genre) VALUES (?, ?, ?, ?, ?);"; // создаем текст запроса

    sqlite3_stmt *stmt; // результат запроса
    const char *pzTest;

    /* подготовка запроса */
    if (sqlite3_prepare_v2(db, sql_add_book.c_str(), -1, &stmt, &pzTest) == SQLITE_OK) // сама функция подготовки запроса, ее включили в условие
    {
        /* теперь привязываем значения */
        sqlite3_bind_int(stmt, 1, id);                                 // добавляем год книги
        sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_STATIC);  // добавляем название книги
        sqlite3_bind_text(stmt, 3, author.c_str(), -1, SQLITE_STATIC); // добавляем автора книги
        sqlite3_bind_int(stmt, 4, year);                               // добавляем год книги
        sqlite3_bind_text(stmt, 5, genre.c_str(), -1, SQLITE_STATIC);  // добавляем жанр книги

        /* выполнение самого sql - запроса */
        if (sqlite3_step(stmt) != SQLITE_DONE) // возвращаем значение и если его результат не равен "сделано"
        {
            /* возвращаем ошибку */
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        }
        else // успешное выполнение
        {
            std::cout << "Book added successfully" << std::endl; // выводим сообщение об успешном добавлении
        }
        /* завершаем работу с запросом */
        sqlite3_finalize(stmt);
    }
    else // если подготовка запроса не удалась
    {
        std::cerr << "SQL error: failed to prepare statement" << std::endl;
    }

    /* далее дадим выбор пользователю  */
    std::cout << " \nwould you like to create a new book again ?\n";
    while (true)
    {
        std::cout << "1. Yes, create a new book\n";
        std::cout << "2. No, return in main menu\n";
        std::cout << "..: ";
        int ch;
        std::cin >> ch;
        std::cout << "\n";

        /* проверка на то что введено число, а не символы или буквв */
        if (std::cin.fail())
        {
            std::cin.clear();                                                   // Очистка флага ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка потока
            std::cout << "Invalid input. Please enter a number 1 or 2 !!!\n";   // выводим сообщение об ошибкеы
            continue;
        }

        if (ch == 1) // рекурсивно возвращаем функцию
        {
            return add_book_in_table(db, table_name); // рекурсия
        }
        else if (ch == 2) // если хотим вернуться в меню
        {
            menu_ch = menu(); // возвращаемся в меню

            return jump_to_choice(menu_ch, db, table_name); // обрабатываем опять
            break;
        }

        std::cout << "Invalid input. Please enter a valid choise !!!\n"; // в случае неправильного значения
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Функция для получения вектора имен таблиц из базы данных SQLite.
 *
 * Эта функция выполняет запрос к базе данных SQLite для получения списка таблиц
 * и возвращает вектор строк с именами таблиц.
 *
 * @param db Указатель на объект базы данных SQLite.
 * @return std::vector<std::string> Вектор строк с именами таблиц.
 *
 * @details
 * Внутри функции формируется SQL-запрос для получения имен таблиц из системной таблицы sqlite_master.
 * После подготовки запроса и его выполнения, имена таблиц добавляются в вектор.
 * При возникновении ошибки выполнения запроса, функция выводит сообщение об ошибке в стандартный вывод ошибок.
 */
std::vector<std::string> get_tables(sqlite3 *db) // функция для получения вектора таблиц
{
    /* функция для получения списка таблиц */
    std::vector<std::string> vector_tables; // создаем вектор таблиц
    sqlite3_stmt *stmt_get_tables;          // результат запроса

    std::string sql_get_tables = "SELECT name FROM sqlite_master WHERE type = 'table';"; // текст запроса где получим имена таблиц

    /* делаем подготовку запроса */
    if (sqlite3_prepare_v2(db, sql_get_tables.c_str(), -1, &stmt_get_tables, nullptr) == SQLITE_OK)
    {
        /* выполняем запрос */
        while (sqlite3_step(stmt_get_tables) == SQLITE_ROW)
        {
            /* добавляем имя таблицы в вектор */
            /* Получить значение первого столбца текущей строки результата SQL-запроса, преобразовать его в тип const char* и добавить в конец вектора tables */
            vector_tables.push_back(reinterpret_cast<const char *>(sqlite3_column_text(stmt_get_tables, 0)));
        }
        sqlite3_finalize(stmt_get_tables); // очищаем ресурсы занятые выражением
    }
    else
    {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl; // сообщение об ошибке
    }
    return vector_tables;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Функция для поиска книги во всех таблицах.
 *
 * Эта функция выполняет запрос к базе данных SQLite для получения книги, ищит во всех таблицах
 * и возвращает пару наименование_таблицы:наименование_книги
 *
 * @param sqlite3 *db указатель на базу данных.
 * @param std::vector<std::string> &input_vector_tables вектор с наименованиями таблиц .
 * @param std::string &book_title адресс на значение книги.
 * @return std::string book_title наименование искомой книги.
 *
 * @details
 * Внутри функции циклически формируется SQL-запрос для поиска книги по названию, по очереди проходят циклически все наименования таблицы
 * и после того как книга найдена, наименование ее таблицы и самой книги вносится в пару и возвращается.
 */
std::pair<std::string, std::string> find_book(sqlite3 *db, std::vector<std::string> &input_vector_tables, std::string &book_title)
{
    // объявляем пару таблица:книга
    std::pair<std::string, std::string> pair_table_book; // пара таблица:книга

    bool found = false; // Флаг для отслеживания, была ли найдена книга

    // Проходим по каждой таблице
    for (const std::string &table : input_vector_tables) // запускаем цикл
    {
        // Формируем текст SQL запрос
        std::string sql_find_book = "SELECT * FROM " + table + " WHERE title LIKE ?;"; // текст запроса

        sqlite3_stmt *stmt_find_book; // Создаем указатель на объект выражения

        // Подготавливаем запрос
        if (sqlite3_prepare_v2(db, sql_find_book.c_str(), -1, &stmt_find_book, nullptr) == SQLITE_OK)
        {
            // Привязываем значение параметра к запросу
            sqlite3_bind_text(stmt_find_book, 1, book_title.c_str(), -1, SQLITE_STATIC); // привязываем значения к запросу

            // Выполняем запрос
            while (true)
            {
                int step_result = sqlite3_step(stmt_find_book); // Выполняем запрос и заносим результат сразу в переменную
                if (step_result == SQLITE_ROW)                  // Если найдена строка
                {
                    std::cout << "Found in table " << table << ": "; // Выводим название таблицы
                    /* заносим данные в таблицу */
                    pair_table_book.first = table;                                                                   // заносим в пару название таблицы
                    pair_table_book.second = reinterpret_cast<const char *>(sqlite3_column_text(stmt_find_book, 1)); // заносим в пару значения книги

                    // std::cout << pair_table_book.first << ":" << pair_table_book.second << std::endl;

                    std::cout << std::endl;
                    found = true; // Устанавливаем флаг, что книга найдена
                }
                else if (step_result == SQLITE_DONE) // Если все строки обработаны
                {
                    break; // Выходим из цикла
                }
                else // Обработка ошибок
                {
                    std::cerr << "SQL error in table " << table << ": " << sqlite3_errmsg(db) << std::endl; // Выводим сообщение об ошибке
                    menu_ch = menu();                                                                       // выходим в меню обратно
                    jump_to_choice(menu_ch, db, chosen_table);                                              // переходим к выбору
                    break;                                                                                  // Выходим из цикла при ошибке
                }
            }
            sqlite3_finalize(stmt_find_book); // Освобождаем ресурсы
        }
        else // если запрос неуспешно подготовлен
        {
            std::cerr << "SQL error in preparing statement for table " << table << ": " << sqlite3_errmsg(db) << std::endl; // Выводим сообщение об ошибке подготовки запроса
            menu_ch = menu();                                                                                               // выходим в меню обратно
            jump_to_choice(menu_ch, db, chosen_table);                                                                      // переходим к выбору
        }
    }

    // Если книга не найдена
    if (!found)
    {
        std::cout << "Book not found in any table.\n1 - Try again.\n2 - Go to main menu\n ...: "; // Выводим сообщение, что книга не найдена
        while (true)                                                                              // запускаем предоставление выбора
        {
            /* даем выбор пользователю либо выйти либо попробовать заново */
            int choice; // переменная для выбора действия

            // std::cin.ignore(); // очищаем буфер
            std::cin >> choice; // вводим переменную

            /* проверка на то что нет символов, а введены только цифры */
            if (std::cin.fail())
            {
                std::cin.clear();                                                                                            // Очистка флага ошибки
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');                                          // Очистка потока
                std::cout << "Invalid input. Please enter a number 1 or 2 !!!\n1 - Try again.\n2 - Go to main menu\n ...: "; // выводим сообщение об ошибкеы
                continue;
            }

            /* обрабатываем выбор */
            switch (choice)
            {
            case 1:                        // если хотим еще раз попробовать
                find_change_info_book(db); // возвращаемся и пробуем еще раз
                break;

            case 2:                                        // если хотим выйти в главное меню
                menu_ch = menu();                          // выходим в меню обратно
                jump_to_choice(menu_ch, db, chosen_table); // переходим к выбору
                break;

            default: // если введено неправильное значение
                std::cout << "Error: invalid value, try again" << std::endl;
                break;
            }
        }
    }
    return pair_table_book; // Возвращаем пару таблица:книгаu
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Функция для получения информации о книге из базы данных SQLite.
 * Данная функция выполняет запрос к базе данных SQLite для получения информации о книге по заданному названию.
 * Результат запроса сохраняется в структуру типа BOOK.
 *
 *@param db Указатель на объект базы данных SQLite.
 *@param table_book Пара, содержащая название таблицы и название книги, по которой идет поиск.
 *@return Структура типа BOOK, содержащая информацию о книге.
 */
BOOK book_info(sqlite3 *db, std::pair<std::string, std::string> table_book)
{
    BOOK book_struct; // создадим экземпляр структуры книги

    book_struct.table.first = "table_name";
    book_struct.table.second = table_book.first;

    /* данная фукнция служит для вывода информации о книге */
    // создаем текст для запроса
    std::string sql_book_info = "SELECT * FROM " + table_book.first + " WHERE title = '" + table_book.second + "';"; // создаем указатель на запрос

    // создаем указатель на объект выражения
    sqlite3_stmt *stmt_book_info; // указатель на объект

    // теперь подготавливаем запрос, если успешно, то выполняем его
    // Подготавливаем SQL-запрос. sqlite3_prepare_v2 компилирует SQL-запрос в байт-код, который будет выполняться SQLite.
    // Если подготовка прошла успешно (возвращено значение SQLITE_OK), переходим к выполнению запроса.
    if (sqlite3_prepare_v2(db, sql_book_info.c_str(), -1, &stmt_book_info, nullptr) == SQLITE_OK)
    {
        // пока есть строки результата выполняем запрос
        // Функция sqlite3_step выполняет подготовленный запрос и возвращает по одной строке результата
        // за раз. Если возвращается SQLITE_ROW, значит есть строка для обработки.
        std::cout << "=============================================================" << std::endl;
        while (sqlite3_step(stmt_book_info) == SQLITE_ROW)
        {
            // Цикл перебирает все колонки в текущей строке результата.
            // sqlite3_column_count возвращает количество колонок.
            // так же выполняем приобразование типов
            for (int i = 0; i < sqlite3_column_count(stmt_book_info); i++)
            {
                std::cout << sqlite3_column_name(stmt_book_info, i) << ": ";
                // смотрим тип значения в таблице sql
                switch (sqlite3_column_type(stmt_book_info, i))
                {
                case SQLITE_INTEGER: // если эта графа типа int

                    /* заносим в структуру данные */
                    switch (i)
                    {
                    case 0: // заносим id

                        book_struct.id.first = sqlite3_column_name(stmt_book_info, i); // заносим название колонки
                        book_struct.id.second = sqlite3_column_int(stmt_book_info, i); // заносим значение
                        break;
                        ;
                    case 3: // заносим год

                        book_struct.year.first = sqlite3_column_name(stmt_book_info, i); // заносим название колонки
                        book_struct.year.second = sqlite3_column_int(stmt_book_info, i); // заносим значение
                        break;
                        ;
                    default: // выводим ошибку по дефолту
                        std::cout << "ERROR in switch case" << std::endl;
                        break;
                    }
                    // выводим численное значение
                    std::cout << sqlite3_column_int(stmt_book_info, i) << std::endl;
                    break;

                case SQLITE_TEXT: // если эта графа типа text

                    switch (i)
                    {
                    case 1: // заносим данные об названии книги
                        book_struct.title.first = sqlite3_column_name(stmt_book_info, i);
                        book_struct.title.second = reinterpret_cast<const char *>(sqlite3_column_text(stmt_book_info, i));
                        break;
                        ;
                    case 2: // заносим данные об авторе книги
                        book_struct.author.first = sqlite3_column_name(stmt_book_info, i);
                        book_struct.author.second = reinterpret_cast<const char *>(sqlite3_column_text(stmt_book_info, i));
                        break;
                        ;
                    case 4: // заносим данные о жанре книги
                        book_struct.genre.first = sqlite3_column_name(stmt_book_info, i);
                        book_struct.genre.second = reinterpret_cast<const char *>(sqlite3_column_text(stmt_book_info, i));
                        break;
                        ;
                    default:
                        std::cout << "ERROR in switch case" << std::endl;
                        break;
                    }
                    // выводим текст
                    std::cout << sqlite3_column_text(stmt_book_info, i) << std::endl;
                    break;

                default:
                    std::cout << "error!!!\n";
                    break;
                }
            }
        }
        sqlite3_finalize(stmt_book_info); // высвобождаем память
        std::cout << "=============================================================" << std::endl;
    }
    // если запрос выдает ошибку то выкидываем обратно в меню
    else
    {
        std::cerr << "SQL error in preparing statement: " << sqlite3_errmsg(db) << std::endl; // Выводим сообщение об ошибке подготовки запроса
        menu_ch = menu();                                                                     // выходим в меню обратно
        jump_to_choice(menu_ch, db, chosen_table);                                            // переходим к выбору
    }

    return book_struct; // возвращаем структуру с данными о книге
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Удаляет книгу из базы данных по указанным параметрам.
 *
 * Эта функция принимает указатель на базу данных SQLite и структуру, содержащую информацию о книге,
 * и удаляет запись о книге из таблицы базы данных.
 *
 * @param db Указатель на объект базы данных SQLite.
 * @param book_info Указатель на структуру BOOK, содержащую информацию о книге (таблица и название).
 *
 * Пример использования:
 * @code
 * BOOK book_info;
 * book_info.table.second = "books";
 * book_info.title.second = "Some Book Title";
 * del_book(db, &book_info);
 * @endcode
 *
 * В случае успешного удаления выводится сообщение "The book was successfully deleted".
 * В случае ошибки удаления выводится сообщение "The book could not be deleted".
 * В случае ошибки подготовки запроса выводится сообщение об ошибке и возвращается в главное меню.
 */
void del_book(sqlite3 *db, BOOK *book_info)
{
    // создаем текс запроса
    std::string sql_del_book = "DELETE FROM " + book_info->table.second + " WHERE title = '" + book_info->title.second + "';";

    // создаем указатель на объект выражения
    sqlite3_stmt *stmt_del_book; // указатель на объект

    // подготавливаем запрос
    if (sqlite3_prepare_v2(db, sql_del_book.c_str(), -1, &stmt_del_book, nullptr) == SQLITE_OK) // если запрос подготовлен успешно
    {
        // выполняем запрос
        if (sqlite3_step(stmt_del_book) == SQLITE_DONE) // если книга удалена
        {
            std::cout << "The book was successfully deleted" << std::endl;
        }
        else // если книга не удалена
        {
            std::cout << "The book could not be deleted" << std::endl;
        }
    }
    else // если все плохо
    {
        std::cerr << "SQL error in preparing statement: " << sqlite3_errmsg(db) << std::endl; // Выводим сообщение об ошибке подготовки запроса
        menu_ch = menu();                                                                     // выходим в меню обратно
        jump_to_choice(menu_ch, db, chosen_table);                                            // переходим к выбору
    }
    sqlite3_finalize(stmt_del_book);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Изменяет информацию о книге в базе данных SQLite.
 *
 * Функция предоставляет пользователю возможность выбрать параметр книги для изменения
 * (название, автор, год издания или жанр), запрашивает новое значение для выбранного параметра
 * и обновляет соответствующую запись в базе данных.
 *
 * @param db Указатель на объект базы данных SQLite.
 * @param book_info Указатель на структуру BOOK, содержащую информацию о книге.
 *
 * Функция выполняет следующие шаги:
 * 1. Запрашивает у пользователя параметр для изменения.
 * 2. Проверяет корректность ввода.
 * 3. Запрашивает новое значение для выбранного параметра.
 * 4. Формирует SQL-запрос для обновления записи.
 * 5. Подготавливает и выполняет SQL-запрос.
 * 6. Обрабатывает результат выполнения запроса и выводит сообщение о статусе операции.
 */
void red_info_book(sqlite3 *db, BOOK *book_info)
{

    // создадим текст запроса
    // std::string sql_red_info = "UPDATE ? SET ? = ? WHERE ? = ?";

    // создадим указатель на объект запроса
    sqlite3_stmt *stmt_red_info;

    // переменная для нового значения
    std::string new_value_str; // числового
    int new_value_int;         // буквенного

    std::string sql_red_info; // переменная для текста запроса

    std::cout << "Which parameter do you want to change ?\n";
    std::cout << "1 - title\n2 - author\n3 - year\n4 - genre\n...: ";

    int ch;

    // Чтение выбора пользователя и проверка корректности ввода
    while (true)
    {
        std::cin >> ch;

        if (std::cin.fail() || ch < 1 || ch > 4)
        {
            std::cin.clear();                                                   // Очистка флагов ошибок
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера
            std::cout << "Invalid input. Please enter a number between 1 and 4: ";
        }
        else
        {
            break; // Корректный ввод
        }
    }

    // Дальнейшая обработка выбора пользователя выбирается текст запроса
    switch (ch)
    {
    case 1: // изменить название книги

        std::cout << "input new title value: ";
        std::cin >> new_value_str;
        sql_red_info = "UPDATE " + book_info->table.second + " SET title = '" + new_value_str + "' WHERE id = " + std::to_string(book_info->id.second) + ";";
        break;

    case 2: // изменить автора книги

        std::cout << "input new author value: ";
        std::cin >> new_value_str;
        sql_red_info = "UPDATE " + book_info->table.second + " SET author = '" + new_value_str + "' WHERE id = " + std::to_string(book_info->id.second) + ";";
        break;

    case 3: // изменить год издания книги

        std::cout << "input new author value: ";
        while (true)
        {
            std::cin >> new_value_int;
            // проверка на то что это не строка
            if (std::cin.fail())
            {
                std::cin.clear();                                                   // Очистка флагов ошибок
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
                std::cout << "Invalid input: Please enter a number, not string.\n...: ";
            }
            else
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка остатка потока
                break;                                                              // Выход из цикла, если ввод корректный
            }
        }
        sql_red_info = "UPDATE " + book_info->table.second + " SET author = '" + std::to_string(new_value_int) + "' WHERE id = " + std::to_string(book_info->id.second) + ";";
        break;

    case 4: // изменить жанр книги

        std::cout << "input new genre value: ";
        std::cin >> new_value_str;
        sql_red_info = "UPDATE " + book_info->table.second + " SET genre = '" + new_value_str + "' WHERE id = " + std::to_string(book_info->id.second) + ";";
        break;

    default:

        std::cout << "shomething wrong\n";
        menu_ch = menu();                          // выходим в меню обратно
        jump_to_choice(menu_ch, db, chosen_table); // переходим к выбору

        break;
    }

    // подготавливаем запрос
    if (sqlite3_prepare_v2(db, sql_red_info.c_str(), -1, &stmt_red_info, nullptr) == SQLITE_OK)
    {
        // выполняем шаг
        if (sqlite3_step(stmt_red_info) == SQLITE_DONE)
        {
            std::cout << "The information was successfullu updated\n";
        }
        else
        {
            std::cerr << "Failed to update the information: " << sqlite3_errmsg(db) << std::endl;
            menu_ch = menu(); // выходим в меню обратно
            jump_to_choice(menu_ch, db, chosen_table);
        }
    }
    else
    {
        std::cerr << "SQL error in preparing statement: " << sqlite3_errmsg(db) << std::endl; // Выводим сообщение об ошибке подготовки запроса
        menu_ch = menu();                                                                     // выходим в меню обратно
        jump_to_choice(menu_ch, db, chosen_table);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* документационный комментарий */
void find_change_info_book(sqlite3 *db)
{
    BOOK book_main_struct; // создаем структуру о книги
    /* создадим переменную в которой будет храниться результат выполнения функции по
    получению списка таблиц */
    std::vector<std::string> vector_tables; // переменаая хранения наименований таблиц

    vector_tables = get_tables(db); // получаем список таблиц

    /* далее спросим у пользователя какую книгу он хочет найти по названию */
    std::string book_title; // для наименования искомой книги

    std::cout << "Enter the name of the book you want to find: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка остатка потока
    std::getline(std::cin, book_title);                                 // cчитываем данные

    /* создаем переменную для храниня пары */
    std::pair<std::string, std::string> pair_table_book; // пара таблица:книга

    pair_table_book = find_book(db, vector_tables, book_title); // выполняем функцию и заносим в переменную результат выполнения

    book_main_struct = book_info(db, pair_table_book); // функция вывода информации о книге

    /* тепрь даем пользователю выбрать действие над книгой */
    // 1 - редактировать информацию о книге
    // 2 - удалить книгу
    // 3 - вернуться в главное меню
    int choice; // переменная для выбора действия

    /* выводим пользвателю предложение на ввод выбора действия над книгой */
    std::cout << "1 - modify book's info\n2 - delete book\n3 - got to main menu\n...: "; // сам текст
    while (true)
    {
        std::cin >> choice;

        if (std::cin.fail() || choice < 1 || choice > 3)
        {
            std::cin.clear();                                                   // Очистка флагов ошибок
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
            std::cout << "Invalid input: Please enter a number between 1 and 3.\n...: ";
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка остатка потока
            break;                                                              // Выход из цикла, если ввод корректный
        }
    }

    switch (choice)
    {
    case 1: // редактировать информацию
        red_info_book(db, &book_main_struct);
        break;

    case 2: // удалить книгу
        std::cout << "are you sure?\n1 - yes\n2 - no\n...: " << std::endl;
        int ch; // переменная выбора

        // проверка на адекватность введенного значения
        while (true)
        {
            std::cin >> ch;                          // вводим
            if (std::cin.fail() || ch < 1 || ch > 2) // условие
            {
                std::cin.clear();                                                   // Очистка флагов ошибок
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
                std::cout << "Invalid input: Please enter a number between 1 and 2.\n...: ";
            }
            else
            {
                switch (ch) // обработчик выбора действия
                {
                case 1: // удалить

                    del_book(db, &book_main_struct);           // функция удаления
                    menu_ch = menu();                          // выходим в меню обратно
                    jump_to_choice(menu_ch, db, chosen_table); // переходим к выбору
                    break;

                case 2: // отменить

                    menu_ch = menu();                          // выходим в меню обратно
                    jump_to_choice(menu_ch, db, chosen_table); // переходим к выбору
                    break;
                }
            }
        }
        break;

    case 3: // вернуться в меню

        menu_ch = menu();                          // выходим в меню обратно
        jump_to_choice(menu_ch, db, chosen_table); // переходим к выбору
        break;

    default:
        menu_ch = menu();                          // выходим в меню обратно
        jump_to_choice(menu_ch, db, chosen_table); // переходим к выбору

        break;
    }

    menu_ch = menu();                          // выходим в меню обратно
    jump_to_choice(menu_ch, db, chosen_table); // переходим к выбору
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
