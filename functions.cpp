#include <iostream>
#include "headders_src/global_var.h"

/* функция меню стартовая, будет принимать аргумент выбора пользователя */
int menu()
{
    int choice;
    std::cout << "1. Add book" << std::endl;    // добавление книги
    std::cout << "2. Book's info" << std::endl; // информация о книге (выдает информцию о книяе, так же дает возможность удалить книгу и изменить информацию о книге)
    std::cout << "3. Search book" << std::endl; // поиск книги по названию
    std::cout << "4. Out books" << std::endl;   // вывод всех книг отсортированных по алфавиту, так же выводит в конце колличество книг
    std::cout << "5. Quit" << std::endl;        // выход из программы
    std::cout << std::endl;                     // пустая строка для красоты вывода меню
    std::cout << "Enter your choice: ";         // выбор пункта меню
    std::cin >> choice;
    std::cout << std::endl; // пустая строка для красоты вывода меню
    /* запускаем проверку адекватности выбора */
    if (choice < 1 || choice > 5) // если выбор не входит в диапазон 1-5
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
