#include "settings.h"
#include <stdio.h>

int main(void)
{
  char *deneme = get_file_path();

  settings settings;
  settings_init(&settings, get_file_path());

  // if (parse_setting("/mnt/sda1/MJones/deneme", &settings) != 0)
  // {
  //   fprintf(stderr, "config okunamadı (dosya yolu doğru mu? klasör değil dosya olmalı)\n");
  //   return 1;
  // }

  // for (size_t i = 0; i < settings.count; i++)
  // {
  //   printf("%s\n", settings.items[i].value);
  //   printf("%d\n", settings.count);
  // }

  // settings_free(&settings);
  return 0;
}
