#include <command.h>
#include <common.h>
#include <dm.h>
#include <errno.h>
#include <asm/gpio.h>
#define long_delay 200000
#define short_delay 20000

int name_to_gpio(const char *name) { return simple_strtoul(name, NULL, 10); }

int lookup_names(char *names[], unsigned int gpios[], int namec) {
  int ret;
  for (int i = 0; i < namec; i++) {
    ret = gpio_lookup_name(names[i], NULL, NULL, &gpios[i]);
    if (ret) {
      printf("GPIO: '%s' not found\n", names[i]);
      return ret;
    }
  }
  return 0;
}

int request(unsigned int gpios[], int gpioc) {
  int ret;
  for (int i = 0; i < gpioc; i++) {
    printf("Requesting %d \n", gpios[i]);
    ret = gpio_request(gpios[i], "cmd_custom");
    if (ret && ret != -EBUSY) {
      return -1;
    }
  }
}

void set(unsigned int gpio) { gpio_direction_output(gpio, 1); }
void clear(unsigned int gpio) { gpio_direction_output(gpio, 0); }
void clear_all(unsigned int gpio[], int gpioc) {
  for (int i = 0; i < gpioc; i++) {
    clear(gpio[i]);
  }
}
void free_all(unsigned int gpio[], int gpioc) {
  for (int i = 0; i < gpioc; i++) {
    gpio_free(gpio[i]);
  }
}

static int do_custom(struct cmd_tbl *cmdtp, int flag, int argc,
                     char *const argv[]) {
  printf("Hello\n");
  int ret;
  char *gpio_names[] = {
      "gpio@4804c0006",  "gpio@4804c0007",  "gpio@4804c00013",
      "gpio@4804c00012", "gpio@44e0700026",
  };
  unsigned int gpios[] = {0, 0, 0, 0, 0};
  ret = lookup_names(gpio_names, gpios, 5);
  for (int i = 0; i < 5; i++) {
    printf("Found %d \n", gpios[i]);
  }
  if (ret) {
    return cmd_process_error(cmdtp, ret);
  }
  ret = request(gpios, 5);
  if (ret == -1) {
    printf("gpio: requesting pin %u failed\n", gpios[0]);
    return -1;
  }
  clear_all(gpios, 5);
  while (!tstc()) {
    set(gpios[2]);
    udelay(long_delay);
    set(gpios[1]);
    set(gpios[3]);
    udelay(short_delay);
    clear(gpios[2]);
    udelay(long_delay);
    set(gpios[0]);
    set(gpios[4]);
    udelay(short_delay);
    clear(gpios[1]);
    clear(gpios[3]);
    udelay(long_delay);

    udelay(long_delay);
    set(gpios[1]);
    set(gpios[3]);
    udelay(short_delay);
    clear(gpios[0]);
    clear(gpios[4]);
    udelay(long_delay);
    set(gpios[2]);
    udelay(short_delay);
    clear(gpios[1]);
    clear(gpios[3]);
  }
  getc();

  free_all(gpios, 5);
  return 0;
}

U_BOOT_CMD(custom_gpio, 1, 0, do_custom, "Custom GPIO Command",
           "Custom secuence of LEDs by using GPIO operations");
