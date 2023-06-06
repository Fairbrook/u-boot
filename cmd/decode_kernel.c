#include <command.h>
#include <common.h>
#define KERNEL_SIZE 6848552

int do_decode_kernel(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[]) {
  if (argc != 4) {
    return CMD_RET_USAGE;
  }
  char *rest;
  char *start_addr_str = argv[1];
  char *dest_addr_str = argv[2];
  char *key_str = argv[3];
  unsigned long start = strtoul(start_addr_str, &rest, 16);
  if (rest[0] != '\0') {
    printf("Bad start address format %s\n", start_addr_str);
    return -1;
  }
  unsigned long dest = strtoul(dest_addr_str, &rest, 16);
  if (rest[0] != '\0') {
    printf("Bad output address format %s\n", dest_addr_str);
    return -1;
  }
  char key = strtoul(key_str, &rest, 16);
  if (rest[0] != '\0') {
    printf("Bad key format %s\n", key_str);
    return -1;
  }
  printf("Decoding from 0x%x to 0x%x with key 0x%X", (int)start, (int)dest, key);
  char *start_addr = (char *)start;
  char *dest_addr = (char *)dest;
  for (int i = 0; i < KERNEL_SIZE; i++) {
    dest_addr[i] = start_addr[i] ^ key;
  }
  return 0;
}

U_BOOT_CMD(decode_kernel, 4, 0, do_decode_kernel, "Decode a zImage Kernel",
           "[start_addr dest_address key]\n"
           "    - Decode the kernel located in start_addr with the key and\n"
           "      places it in dest_address. Uses an XOR decode.\n"
           "      All the inputs are read in hex");
