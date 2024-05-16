#include <iostream>
#include <cstdlib>
#include "headders_src/global_var.h"
#include "sql_src/sqlite3.h"
#include "headders_src/extern.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* функция меню стартовая, будет принимать аргумент выбора пользователя */
int menu() // функция будет возвращать целоцисленное значение выбора
{
    int choice;
    std::cout << "1. Add book" << std::endl;     // добавление книги
    std::cout << "2. Book's info" << std::endl;  // информация о книге (выдает информцию о книяе, так же дает возможность удалить книгу и изменить информацию о книге)
    std::cout << "3. Search book" << std::endl;  // поиск книги по названию
    std::cout << "4. Out books" << std::endl;    // вывод всех книг отсортированных по алфавиту, так же выводит в конце колличество книг
    std::cout << "5. Change table" << std::endl; // функция смены таблицы
    std::cout << "6. Delete table" << std::endl; // удаление таблицы
    std::cout << "7. Quit" << std::endl;         // выход из программы
    std::cout << std::endl;                      // пустая строка для красоты вывода меню
    std::cout << "Enter your choice: ";          // выбор пункта меню
    std::cin >> choice;
    std::cout << std::endl; // пустая строка для красоты вывода меню
    /* запускаем проверку адекватности выбора */
    if (choice < 1 || choice > 6) // если выбор не входит в диапазон 1-5
    {
        std::cout << std::endl;                           // пустая строка для красоты вывода меню
        std::cout << "!!! Wrong choice !!!" << std::endl; // выводим ошибку
        return menu();                                    // рекурсивно запускаем функцию меню
    }
    else // если все проавильно, то возвращаем значение
    {
        return choice; // возвращаем обратно значение
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* данная функция служит для переадресации из меню в функцию выбранным пользователем */
void jump_to_choice(int choice, sqlite3 *db, std::string ch_tb) // функция принимает переменную выбора полученную из menu(), указатель на БД, наименование выбранной таблицы
{
    switch (choice)
    {
        /* позиция смены таблицы на другую */
    case 5:             // позиция переключить на другую таблицу
        chosen_table = list_table(db); // функция по выводу списка таблиц
        menu_ch = menu(); // записывам в переменную возвращаемое значение
        if (menu_ch == 5) // если вызываем опять сами себя
        {
            return jump_to_choice(5, db, ch_tb); // рекурсивно возвращаемся в начало текущей функции
        }
        else
        {
            return jump_to_choice(menu_ch, db, chosen_table); // рекурсивно возвращаем функцию с выбранным значением
        }
        break;

        /* позиция удаления таблицы */
    case 6: // позиция удалить таблицу
        // return del_table(db, ch_tb); // переходим в функцию удаления таблицы
        del_table(db, ch_tb); // переходим в функцию удаления таблицы
        break;

        /* позиция выходи из программы */
    case 7:      // выход из программы
        exit(0); // стандартная функция передачи управления вашей ОС
        break;

    default:
        /* если выбрана неправильная цифро, то надо вернуть функцию */
        break;
    }
}