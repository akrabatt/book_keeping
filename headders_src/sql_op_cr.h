/* этот файл служит для импорта по проекту функции по открытию или созданию базы данных */
#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

sqlite3 *open_or_create_db();

#endif /* DATABASE_H */