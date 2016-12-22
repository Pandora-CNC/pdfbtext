#ifndef PDFBTEXT_H
  #define PDFBTEXT_H

  #include <stdint.h>
  #include <linux/fb.h>

  uint8_t fbtx_init(char fbdev[]);
  uint8_t fbtx_deinit(void);
  uint8_t fbtx_putc(char c, uint32_t color);
  uint8_t fbtx_puts(char str[], uint32_t color);

  void buffer_clear(uint32_t color);
  void buffer_hline(uint32_t x, uint32_t y, uint32_t length, uint32_t color);
  uint32_t buffer_get_width(void);
  uint32_t buffer_get_height(void);

  void pixel_set(uint32_t x, uint32_t y, uint32_t color);
  uint32_t pixel_color(uint8_t r, uint8_t g, uint8_t b);
  uint32_t pixel_color_vinfo(uint8_t r, uint8_t g, uint8_t b, struct fb_var_screeninfo *vinfo);


#endif
