#ifndef MJONES_SETTINGS_H
#define MJONES_SETTINGS_H
#include <stdlib.h>

const char *home = getenv("HOME");
static const char *settingsFileName = "settings.json";

typedef struct settings
{
  char *key;
  char *value;
} settings;

static char *getFilePath();

settings *parseSetting();

#endif // MJONES_SETTINGS_H