#include <common.h>
#include <exports.h>

int custom(int argc, char *const argv[]) {
  int i;
  char name[100];
  char last_char;
  char index = '\0';

  /* Print the ABI version */
  app_startup(argv);
  printf("\n");
  printf("===== Custom standalone application ====\n");
  printf("=========== by Kevin Martinez ==========\n\n");

  printf("U-Boot version: %s\n", env_get("ver"));
  printf("Example expects ABI version %d\n", XF_VERSION);
  printf("Actual U-Boot ABI version %d\n\n", (int)get_version());

  printf("Cual es tu nombre? > ");
  last_char = '\0';
  while (last_char != '\n' && last_char != '\r') {
    while (!tstc())
      ;
    if (index >= 99) {
      index = 99;
      break;
    }
    last_char = getc();
    putc(last_char);
    name[index] = last_char;
    index++;
  }
  name[index - 1] = '\0';
  printf("\n");
  printf("Hola %s!\n", name);

  printf("Presiona una tecla para salir ... ");
  while (!tstc())
    ;
  /* consume input */
  (void)getc();

  printf("\n\n");
  return (0);
}
