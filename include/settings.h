#ifndef MJONES_SETTINGS_H
#define MJONES_SETTINGS_H
#include <stdlib.h>

const char *settings_get_home(void);
static const char *settingsFileName = "settings";

typedef struct settings_item
{
  char *key;
  char *value;
} settings_item;

typedef struct
{
  settings_item *items;
  size_t count;
} settings;

void settings_init(settings *settings, char *settings_file);
void settings_free(settings *settings);
void settings_add(settings *settings, const char *key, const char *value);

char *get_file_path();

void *parse_setting(settings *settings);

void parse_line(char *line, settings *settings);

#endif // MJONES_SETTINGS