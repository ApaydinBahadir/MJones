#include <settings.h>
#include <string.h>
#include <stdio.h>

static char *getFilePath()
{
  if (!home || !*home)
  {
    printf("Home environment couldn't finded.");
    return NULL;
  }

  size_t len = strlen(home) + 1 + strlen(settingsFileName) + 1;
  char *path = (char *)malloc(len);
  if (!path)
  {
    printf("Path couldn't finded.");
    return NULL;
  }
  snprintf(path, len, "%s/%s", home, settingsFileName);
  return path;
}

settings *parseSetting()
{
  char *path = getFilePath();

  FILE *settings_pointer;
  settings_pointer = fopen(path, "r");
  if (settings_pointer == NULL)
  {
    printf("The file is not opened.");
    return;
  }
}