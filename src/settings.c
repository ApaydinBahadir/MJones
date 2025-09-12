#include <settings.h>
#include <string.h>
#include <stdio.h>

const char *settings_get_home(void)
{
  static int inited = 0;
  static char *cached = NULL;
  if (!inited)
  {
    const char *h = getenv("HOME");
#ifdef _WIN32
    if (!h)
      h = getenv("USERPROFILE");
#endif
    if (h)
    {
      cached = (char *)malloc(strlen(h) + 1);
      if (cached)
        strcpy(cached, h);
    }
    inited = 1;
  }
  return cached;
}

char *getFilePath()
{
  const char *home = settings_get_home();
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

settings *parseSetting(char *path)
{
  // char *path = getFilePath();

  FILE *settings_pointer;
  settings_pointer = fopen(path, "r");
  if (settings_pointer == NULL)
  {
    printf("The file is not opened.");
    return NULL;
  }

  char line[256];

  while (fgets(line, 256, settings_pointer))
  {
    printf("%s", line);
  }

  settings setting;

  return &setting;
}

char parseLine(char *xx)
{
}