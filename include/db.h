#ifndef MJONES_DB_H
#define MJONES_DB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

typedef struct SQLITE_MIGRATIONS
{

} SQLITE_MIGRATION;

sqlite3 *create_db();
void close_db(sqlite3 *db);

#endif // MJONES_DB_H
