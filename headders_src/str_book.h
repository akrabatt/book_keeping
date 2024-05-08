/* в этом файле мы создаем структуру книги */

#include <string>

typedef struct BOOK
{
    int id;             // пременная для идентификатора книги
    std::string title;  // пременная для названия книги
    std::string author; // пременная для автора книги
    int year;           // пременная для года выпуска книги
    std::string genre;  // пременная для жанра книги
};
