#include "application.h"
#include "pdfbtext.h"

#include <stdio.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
  if(fbtx_init("/dev/fb0"))
    return -1;

  // get resolution
  //printf("Resolution: %dx%d\n", buffer_get_width(), buffer_get_height());

  //buffer_clear(pixel_color(0xFF, 0xFF, 0xFF));
  buffer_clear(pixel_color(0x00, 0x00, 0x00));
  buffer_hline(0, 100, buffer_get_width() - 1, pixel_color(0xFF, 0x00, 0x00));
  fbtx_puts("Hi MadModder, my driver's kinda running!", pixel_color(0xFF, 0xFF, 0xFF));
  //volatile int anykey = getc(stdin);
  while(1);

  fbtx_deinit();
  return 0;
}
