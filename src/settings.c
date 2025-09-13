#include <settings.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#ifdef _WIN32
#include <direct.h> // _mkdir
#define PATH_SEP1 '\\'
#define PATH_SEP2 '/'
static int make_dir(const char *p) { return _mkdir(p); } // 0 ok, -1 var ya da hata
#else
#include <unistd.h>
#include <errno.h>
#define PATH_SEP1 '/'
#define PATH_SEP2 '/'
static int make_dir(const char *p) { return mkdir(p, 0700); } // 0 ok, -1 var ya da hata
#endif

static char *sdup(const char *string)
{
  size_t string_size = strlen(string) + 1;
  char *return_string = (char *)malloc(string_size);
  if (return_string)
    memcpy(return_string, string, string_size);
  return return_string;
}

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

char *get_file_path(void)
{
#ifdef _WIN32
  /* Tercihen %LOCALAPPDATA%, yoksa %APPDATA%, o da yoksa USERPROFILE\AppData\Local */
  const char *base = getenv("LOCALAPPDATA");
  if (!base || !*base)
    base = getenv("APPDATA");

  if (base && *base)
  {
    size_t len = strlen(base) + strlen("\\MJones\\") + strlen(settingsFileName) + 1;
    char *path = (char *)malloc(len);
    if (!path)
      return NULL;
    snprintf(path, len, "%s\\MJones\\%s", base, settingsFileName);
    return path;
  }
  else
  {
    const char *up = getenv("USERPROFILE");
    if (!up || !*up)
      return NULL;
    size_t len = strlen(up) + strlen("\\AppData\\Local\\MJones\\") + strlen(settingsFileName) + 1;
    char *path = (char *)malloc(len);
    if (!path)
      return NULL;
    snprintf(path, len, "%s\\AppData\\Local\\MJones\\%s", up, settingsFileName);
    return path;
  }
#else
  /* XDG: $XDG_CONFIG_HOME/MJones/settings; yoksa ~/.config/MJones/settings */
  const char *xdg = getenv("XDG_CONFIG_HOME");
  if (xdg && *xdg)
  {
    size_t len = strlen(xdg) + strlen("/MJones/") + strlen(settingsFileName) + 1;
    char *path = (char *)malloc(len);
    if (!path)
      return NULL;
    snprintf(path, len, "%s/MJones/%s", xdg, settingsFileName);
    return path;
  }
  else
  {
    const char *home = settings_get_home();
    if (!home || !*home)
      return NULL;
    size_t len = strlen(home) + strlen("/.config/MJones/") + strlen(settingsFileName) + 1;
    char *path = (char *)malloc(len);
    if (!path)
      return NULL;
    snprintf(path, len, "%s/.config/MJones/%s", home, settingsFileName);
    return path;
  }
#endif
}

void settings_init(settings *settings, char *settings_file)
{

  FILE *settings_file_path;
  settings_file_path = fopen(settings_file, "w+");

  if (settings_file_path == NULL)
  {
    fprintf(settings_file_path,
            "DB_PATH=%s\nDB_FILENAME=%s\n",
            settings_file,
            settingsFileName);
  }

  settings->items = NULL;
  settings->count = 0;
}

void settings_free(settings *settings)
{
  if (!settings)
    return;
  for (size_t i = 0; i < settings->count; i++)
  {
    free(settings->items[i].key);
    free(settings->items[i].value);
  }
  free(settings->items);
  settings->items = NULL;
  settings->count = 0;
}

void settings_add(settings *settings, const char *key, const char *value)
{

  settings_item *np = (settings_item *)realloc(settings->items, (settings->count + 1) * sizeof *np);
  if (!np)
    return -1;
  settings->items = np;

  settings->items[settings->count].key = sdup(key);
  settings->items[settings->count].value = sdup(value);
  if (!settings->items[settings->count].key || !settings->items[settings->count].value)
    return;
  settings->count++;
  return;
}

void *parse_setting(settings *settings)
{
  char *path = get_file_path();

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
    parse_line(line, settings);
  }

  return NULL;
}

void parse_line(char *line, settings *s)
{
  if (!line || !s)
    return;

  size_t line_size = strlen(line);
  while (line_size && (line[line_size - 1] == '\n' || line[line_size - 1] == '\r' || isspace((unsigned char)line[line_size - 1])))
    line[--line_size] = '\0';

  char *parsed_line = line;
  while (*parsed_line && isspace((unsigned char)*parsed_line))
    parsed_line++;
  if (*parsed_line == '\0' || *parsed_line == '#' || *parsed_line == ';' || (parsed_line[0] == '/' && parsed_line[1] == '/'))
    return;

  // ayraç (sadece ':')
  char *seperator = strchr(parsed_line, ':');
  if (!seperator)
    return;

  // sol - sağ
  char *left = parsed_line;
  char *right = seperator + 1;

  // solun son boşluklarını sil ve NUL koy
  char *left_parsed = seperator - 1;
  while (left_parsed >= left && isspace((unsigned char)*left_parsed))
    left_parsed--;
  *(left_parsed + 1) = '\0';

  while (*right && isspace((unsigned char)*right))
    right++;

  if (strcmp(left, "DB_PATH") == 0 || strcmp(left, "DB_NAME") == 0)
    settings_add(s, left, right);
}