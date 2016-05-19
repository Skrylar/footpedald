#include <stdio.h>

#define DEVICEPATH "/dev/usb/hiddev1"

#define DATABLOCK_SIZE 18
typedef char datablock[DATABLOCK_SIZE];

int main() {
  FILE* dev_input;
  datablock block;

  printf("This is footpedald's proof of concept program.\n");
  printf("Not recommended for production use.\n");

  /* open and check for input */
  dev_input = fopen(DEVICEPATH, "rb");
  if (dev_input == NULL) {
    fputs("Could not open HID device.\n", stderr);
  }

  /* read user events */
  while (ferror(dev_input) == 0) {
    if (fread((void*)&block, DATABLOCK_SIZE, 1, dev_input)) {
      fputs("User event.\n", stdout);
    } else {
      fputs("Mutilated user event.\n", stderr);
    }
  }

  /* clean up after ourselves */
  fclose(dev_input);
}
