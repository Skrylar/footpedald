#include <stdio.h>

#define DEVICEPATH "/dev/usb/hiddev1"

/* size of individual datablocks */
#define DATABLOCK_SIZE 24
typedef char datablock[DATABLOCK_SIZE];

/* offsets to bytes we are interested in */
#define OFF_LEFT 4
#define OFF_CENTER 12
#define OFF_RIGHT 20

/* bit flags for which button is down */
#define BUTTON_LEFT (1 << 0)
#define BUTTON_CENTER (1 << 1)
#define BUTTON_RIGHT (1 << 2)

int mask_from_datablock(datablock* dblock) {
  int ret = 0;
  if ((*dblock)[OFF_LEFT] != 0) {
    ret |= BUTTON_LEFT;
  }
  if ((*dblock)[OFF_CENTER] != 0) {
    ret |= BUTTON_CENTER;
  }
  if ((*dblock)[OFF_RIGHT] != 0) {
    ret |= BUTTON_RIGHT;
  }
  return ret;
}

void print_button_banner(int buttons) {
  fputs("Left ", stdout);
  if ((buttons & BUTTON_LEFT) != 0) {
    fputs("ON\t", stdout);
  } else {
    fputs("OFF\t", stdout);
  }

  fputs("Center ", stdout);
  if ((buttons & BUTTON_CENTER) != 0) {
    fputs("ON\t", stdout);
  } else {
    fputs("OFF\t", stdout);
  }

  fputs("Right ", stdout);
  if ((buttons & BUTTON_RIGHT) != 0) {
    fputs("ON\t", stdout);
  } else {
    fputs("OFF\t", stdout);
  }

  fputs("\n", stdout);
}

int main() {
  FILE* dev_input;		/* fd to read events from */
  datablock dblock;		/* block to store packets */
  int button_states = 0;	/* current button states */

  /* header */
  printf("This is footpedald's proof of concept program.\n");
  printf("Not recommended for production use.\n");

  /* open and check for input */
  dev_input = fopen(DEVICEPATH, "rb");
  if (dev_input == NULL) {
    fputs("Could not open HID device.\n", stderr);
  }

  /* read user events */
  while (ferror(dev_input) == 0) {
    if (fread((void*)&dblock, DATABLOCK_SIZE, 1, dev_input)) {
      print_button_banner(mask_from_datablock(&dblock));
    } else {
      fputs("Mutilated user event.\n", stderr);
    }
  }

  /* clean up after ourselves */
  fclose(dev_input);
}
