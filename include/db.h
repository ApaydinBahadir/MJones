#ifndef MJONES_DB_H
#define MJONES_DB_H


#include <sqlite3.h>

typedef struct SQLITE_MIGRATIONS{
  
}

sqlite3* create_db();
void free_db(sqlite3 *db);

#endif //MJONES_DB_H
