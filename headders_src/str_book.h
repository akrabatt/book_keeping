/* в этом файле мы создаем структуру книги */

#include <string>
#include <utility>

typedef struct BOOK
{
    std::pair<std::string, int> id;             // id пременная для идентификатора книги
    std::pair<std::string, std::string> title;  // пременная для названия книги
    std::pair<std::string, std::string> author; // пременная для автора книги
    std::pair<std::string, int> year;           // пременная для года выпуска книги
    std::pair<std::string, std::string> genre;  // пременная для жанра книги
};
// typedef struct BOOK
// {
//     int id;             // пременная для идентификатора книги
//     std::string title;  // пременная для названия книги
//     std::string author; // пременная для автора книги
//     int year;           // пременная для года выпуска книги
//     std::string genre;  // пременная для жанра книги
// };
