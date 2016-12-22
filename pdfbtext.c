#include "pdfbtext.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "font.h"

struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;
int fb_fd;
long screensize;
uint8_t *fbp;
uint32_t xpos, ypos;

uint8_t fbtx_init(char fbdev[])
{
  fb_fd = open(fbdev, O_RDWR);

	// Get variable screen information
	ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
	vinfo.grayscale = 0;
	vinfo.bits_per_pixel = 32;
  vinfo.vmode = FB_VMODE_NONINTERLACED;

	ioctl(fb_fd, FBIOPUT_VSCREENINFO, &vinfo);
	ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);

	ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo);

	screensize = vinfo.yres_virtual * finfo.line_length;

	fbp = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, (off_t)0);

  xpos = 0;
  ypos = 0;

  return 0;
}

uint8_t fbtx_deinit(void)
{

  return 0;
}

uint8_t fbtx_putc(char c, uint32_t color)
{
  if(c > 32 && c < 127) {
    int columns = bigfont_lengths[c - 33];
    int start = bigfont_starts[c - 33];

    int column;
    for(column = 0; column < columns; column++) {
      uint16_t scanline = bigfont_scanlines[start + column];

      int line;
      for(line = 0; line < 16; line++) {
        if((scanline >> (15 - line)) & 0x1)
          pixel_set(xpos + column, ypos + line, color);
      }
    }

    xpos += columns;
  } else if(c == 32) {
    xpos += 5;
  } else
    return 1;

  return 0;
}

uint8_t fbtx_puts(char str[], uint32_t color)
{
  int ptr;
  for(ptr = 0; ptr < strlen(str); ptr++)
    fbtx_putc(str[ptr], color);

  return 0;
}

void buffer_clear(uint32_t color)
{
  int x,y;
  for (x=0; x<vinfo.xres; x++) {
		for (y=0; y<vinfo.yres; y++)
		  pixel_set(x,y,color);
  }
}

void buffer_hline(uint32_t x, uint32_t y, uint32_t length, uint32_t color)
{
  uint32_t xend = x + length;
  for(; x <= xend; x++)
    pixel_set(x, y, color);
}

uint32_t buffer_get_width(void) {
  return vinfo.xres;
}

uint32_t buffer_get_height(void) {
  return vinfo.yres;
}

void pixel_set(uint32_t x, uint32_t y, uint32_t color)
{
  //if(x % 2) // pixel is odd; e.g. 1,3,5 => we need to shift it
    //*((uint32_t*)(fbp + ((x+vinfo.xoffset + (vinfo.xres / 2)) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length))) = color;
  //else  pixel is even; e.g. 0,2,4
    *((uint32_t*)(fbp + ((x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length))) = color;
}

uint32_t pixel_color(uint8_t r, uint8_t g, uint8_t b)
{
  return (r<<vinfo.red.offset) | (g<<vinfo.green.offset) | (b<<vinfo.blue.offset);
}

uint32_t pixel_color_vinfo(uint8_t r, uint8_t g, uint8_t b, struct fb_var_screeninfo *vinfo)
{
	return (r<<vinfo->red.offset) | (g<<vinfo->green.offset) | (b<<vinfo->blue.offset);
}
