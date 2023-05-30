#include "graphics.h"

struct minix_mem_range mr, m2;

vbe_mode_info_t mode_info;

unsigned bytesPerPixel;
uint16_t xRes, yRes;

int (set_graphics_mode)(uint16_t mode) {

  memset(&mode_info, 0, sizeof(mode_info));
  if (vbe_get_mode_info(mode, &mode_info)) return 1;

  bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  xRes = mode_info.XResolution;
  yRes = mode_info.YResolution;

  unsigned int vram_base = mode_info.PhysBasePtr;
  unsigned int vram_size = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;

  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size;  

  if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) {
    panic("sys_privctl (ADD_MEM) failed\n");
    return 1;
  }

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED) {
    panic("couldn't map video memory");
    return 1;
  }
  
  return 0;
}

int (init_graphics_mode)(uint16_t mode) {
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86));
  reg86.intno = 0x10;
  reg86.ah = 0x4F;
  reg86.al = 0x02;
  reg86.bx = mode | BIT(14);
  if (sys_int86(&reg86) != 0) {
      printf("Set graphic mode failed\n");
      return 1;
  }
  return 0;
}

int normalize_color(uint32_t color, uint32_t *new_color) {
  if (mode_info.BitsPerPixel == 32) {
    *new_color = color;
  } else {
    *new_color = color & (BIT(mode_info.BitsPerPixel) - 1);
  }
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned i = 0 ; i < len ; i++)   
    if (vg_draw_pixel(x+i, y, color) != 0) return 1;
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for(unsigned i = 0; i < height ; i++)
    if (vg_draw_hline(x, y+i, width, color) != 0) return 1;
  return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if(x > mode_info.XResolution || y > mode_info.YResolution) return 1;
  unsigned BytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int index = (mode_info.XResolution * y + x) * BytesPerPixel;
  if (memcpy(&video_mem[index], &color, BytesPerPixel) == NULL) return 1;
  return 0;
}

uint32_t (indexed_color)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t no_rectangles) {
  return (first + (row*no_rectangles + col) * step) % (1 << mode_info.BitsPerPixel);
}

uint32_t (direct_color)(uint32_t red, uint32_t green, uint32_t blue) {
  return blue | (green << mode_info.GreenFieldPosition) | (red << mode_info.RedFieldPosition);
}

uint32_t (R_First)(uint32_t first) {
  return (first >> mode_info.RedFieldPosition) & ((1 << mode_info.RedMaskSize) - 1);
}

uint32_t (G_First)(uint32_t first) {
  return (first >> mode_info.GreenFieldPosition) & ((1 << mode_info.GreenMaskSize) - 1);
}

uint32_t (B_First)(uint32_t first) {
  return (first >> mode_info.BlueFieldPosition) & ((1 << mode_info.BlueMaskSize) - 1);
}

uint32_t (R)(unsigned int w, uint8_t step, uint32_t first) {
  return (R_First(first) + w * step) % (1 << mode_info.RedMaskSize);
}

uint32_t (G)(unsigned int h, uint8_t step, uint32_t first) {
  return (G_First(first) + h * step) % (1 << mode_info.GreenMaskSize);
}

uint32_t (B)(unsigned int w, unsigned int h, uint8_t step, uint32_t first) {
  return (B_First(first) + (w + h) * step) % (1 << mode_info.BlueMaskSize);
}

int deleteXpm(xpm_image_t img, uint16_t x, uint16_t y){
  for(unsigned int i=0; i<img.height; i++){
    for(unsigned int j= 0; j<img.width; j++){
       vg_draw_pixel(x + j, y + i, 0);
    }
  }
  return 0;
}
