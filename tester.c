#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/extensions/XTest.h>

#include "config.h"

/* X11
   --- */
/* Special thanks to Adam for his example code on sending X11 events.

   http://www.doctort.org/adam/nerd-notes/x11-fake-keypress-event.html */

Display* x11_display;

/* enables the X module */
void x_on() {
  /* XXX read from environment variable? */
  x11_display = XOpenDisplay(0);
  if (!x11_display) {
    fputs("Could not start X module!\n", stderr);
    return;
  }
  fputs("X module on.\n", stderr);
}

/* disables the X module */
void x_off() {
  if (x11_display) {
    XCloseDisplay(x11_display);
    fputs("X module off.\n", stderr);
  }
}

/* checks whether the X module is turned on */
int x_is_on() {
  return x11_display != 0;
}

/* configurable stuff */
void all_on() {
  x_on();
}

void all_off() {
  x_off();
}

/* EVENT REACTIONS
   --------------- */

/* maximum number of buttons supported */
#define NUM_BUTTONS 3

#define RT_NOP	0		/* do nothing */
#define RT_X	1		/* do something in X */

#define RT_XKEY 0		/* emulate keyboard keys */

typedef struct _reactor_x11 {
  int type;
  KeySym keysym;
} reactor_x11;

/* holds reaction information for particular thingdoers */
typedef struct _reactor {
  int type;
  union _data {
    reactor_x11 x11;
  } data;
} reactor;

static reactor reactions[NUM_BUTTONS];

void reactor_do_x11(reactor* self, int on) {
  /* XKeyEvent event = x_synthesize_ke(on, self->data.x11.keysym); */
  XTestFakeKeyEvent(x11_display, XKeysymToKeycode(x11_display, self->data.x11.keysym), on, CurrentTime);
  XFlush(x11_display);
}

void reactor_do(reactor* self, int on) {
  switch (self->type) {
  case RT_NOP: break;
  case RT_X:
    reactor_do_x11(self, on);
    break;
  default:
    abort();
  }
}

/* OUR STUFF
   --------- */
static const char* FOOTPEDAL;

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

void init_config() {
  FOOTPEDAL = fpd_get_env("FOOTPEDAL");
  if (FOOTPEDAL == NULL) {
    fprintf(stderr, "FOOTPEDAL env not set!\n");
    exit(1);
  }
}

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

  init_config();

  /* nop out all reactors */
  memset((void*)&reactions, 0, sizeof(reactions));

  /* TODO delegate this to config/ipc */
  reactions[0].type	       = RT_X;
  reactions[0].data.x11.type   = RT_XKEY;
  reactions[0].data.x11.keysym = XK_Control_L;

  reactions[1].type	       = RT_X;
  reactions[1].data.x11.type   = RT_XKEY;
  reactions[1].data.x11.keysym = XK_Shift_L;

  reactions[2].type	       = RT_X;
  reactions[2].data.x11.type   = RT_XKEY;
  reactions[2].data.x11.keysym = XK_Super_R;

  /* header */
  printf("This is footpedald's proof of concept program.\n");
  printf("Not recommended for production use.\n");

  /* turn on modules */
  all_on();

  /* open and check for input */
  dev_input = fopen(FOOTPEDAL, "rb");
  if (dev_input == NULL) {
    fputs("Could not open HID device.\n", stderr);
    goto nohid;		  /* somebody saw this goto and died inside */
  }

  /* read user events */
  int bmask;			/* known button mask */
  while (ferror(dev_input) == 0) {
    if (fread((void*)&dblock, DATABLOCK_SIZE, 1, dev_input)) {
      int nmask;		/* next mask */
      nmask = mask_from_datablock(&dblock);
      /*print_button_banner(mask_from_datablock(&dblock));*/

      if ((bmask & BUTTON_LEFT) != (nmask & BUTTON_LEFT)) {
	reactor_do(&reactions[0], (nmask & BUTTON_LEFT));
      }
      if ((bmask & BUTTON_CENTER) != (nmask & BUTTON_CENTER)) {
	reactor_do(&reactions[1], (nmask & BUTTON_CENTER));
      }
      if ((bmask & BUTTON_RIGHT) != (nmask & BUTTON_RIGHT)) {
	reactor_do(&reactions[2], (nmask & BUTTON_RIGHT));
      }

      bmask = nmask;
    } else {
      fputs("Mutilated user event.\n", stderr);
    }
  }

  /* clean up after ourselves */
  fclose(dev_input);

 nohid:
  /* turn off modules */
  all_off();
}
