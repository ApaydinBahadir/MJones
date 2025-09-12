#ifndef MJONES_SETTINGS_H
#define MJONES_SETTINGS_H
#include <stdlib.h>

const char *settings_get_home(void);
static const char *settingsFileName = "settings.json";

typedef struct settings
{
  char *key;
  char *value;
} settings;

char *getFilePath();

settings *parseSetting(char *path);

#endif // MJONES_SETTINGS_H