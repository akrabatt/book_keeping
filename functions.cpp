#include <iostream>

/* функция меню стартовая, будет крутиться в бесконечном цикле */
int menu(int arg)
{
    int choice;
    std::cout << "1. Add book" << std::endl;    // добавление книги
    std::cout << "2. Book's info" << std::endl; // информация о книге (выдает информцию о книяе, так же дает возможность удалить книгу и изменить информацию о книге)
    std::cout << "3. Search book" << std::endl; // поиск книги по названию
    std::cout << "4. Out books" << std::endl;   // вывод всех книг отсортированных по алфавиту, так же выводит в конце колличество книг
    std::cout << "5. Quit" << std::endl;        // выход из программы
    std::cin >> choice;
    return choice;
}