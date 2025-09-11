#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static void die_sql(sqlite3 *db, const char *ctx, int rc)
{
  fprintf(stderr, "[sqlite] %s failed: rc=%d, msg=%s\n", ctx, rc, db ? sqlite3_errmsg(db) : "(no db)");
  if (db)
    sqlite3_close(db);

  exit(1);
}

static char *build_home_path(const char *filename)
{
  const char *home = getenv("HOME");
  if (!home || !*home)
    return NULL;
  size_t len = strlen(home) + 1 /* slash */ + strlen(filename) + 1 /* NUL */;
  char *path = (char *)malloc(len);
  if (!path)
    return NULL;
  snprintf(path, len, "%s/%s", home, filename);
  return path;
}

sqlite3 *create_db()
{
  char *dbpath = build_home_path("MJones.db");

  sqlite3 *db = NULL;
  int rc = sqlite3_open(dbpath, &db);
  if (rc != SQLITE_OK)
    die_sql(db, "open", rc);

  sqlite3_busy_timeout(db, 2000);

  rc = sqlite3_exec(db, "PRAGMA journal_mode=WAL; PRAGMA foreign_keys=ON;", NULL, NULL, NULL);
  if (rc != SQLITE_OK)
    die_sql(db, "pragmas", rc);

  return db;
}
