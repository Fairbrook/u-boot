#include <command.h>
#include <common.h>
#include <errno.h>
#include <dm.h>
#include <asm/gpio.h>

int name_to_gpio(const char *name) { return simple_strtoul(name, NULL, 10); }

static int do_custom(struct cmd_tbl *cmdtp, int flag, int argc,
                     char *const argv[]) {
  printf("Hello\n");
  char *gpio_names[] = {
      "gpio@4804c0006",
      "gpio@4804c0007",
      "gpio@4804c0002",
      "gpio@4804c0003",
  };
  unsigned int gpio_numbers[] = {0, 0, 0, 0};
  int ret = gpio_lookup_name(gpio_names[0], NULL, NULL, &gpio_numbers[0]);
  if (ret) {
    printf("GPIO: '%s' not found\n", gpio_names[0]);
    return cmd_process_error(cmdtp, ret);
  }
  // gpio = name_to_gpio(str_gpio);
  // if (gpio < 0)
  //   return 1;
  ret = gpio_request(gpio_numbers[0], "cmd_custom");
  if (ret && ret != -EBUSY) {
    printf("gpio: requesting pin %u failed\n", gpio_numbers[0]);
    return -1;
  }
  gpio_direction_output(gpio_numbers[0], 0);
  gpio_direction_output(gpio_numbers[0], 1);
  udelay(1000000);
  gpio_direction_output(gpio_numbers[0], 0);
 // for(int i=0;i<100;i++){
 //   gpio_direction_output(gpio_numbers[0], 1);

 // }

  if (ret != -EBUSY)
    gpio_free(gpio_numbers[0]);
  return 0;
}

U_BOOT_CMD(custom_gpio, 1, 0, do_custom, "help", "help");
