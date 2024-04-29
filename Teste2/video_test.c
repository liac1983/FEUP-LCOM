enum lpv_dir_t {
    lpv_hor, // horizontal line
    lpv_vert // vertical line
};

static lpv_mode_info_t vmi;
static char *video_mem;
static unsigned h_res;
static unsigned v_res;
static unsigned bits_per_pixel;
static unsigned bytes_per_pixel;
static unsigned int vram_size;
static unsigned int vram_base;

int pp_test_line(uint8_t mode, enum lpv_dir_t dir, uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint32_t delay) {
    vg_init(mode);
    uint32_t new_color;

    normalize_color(mode, color, &new_color);

    switch(dir) {
        case lpv_hor:
            if (vg_draw_hline(x,y,len,new_color)) return 1;
            break;
        case lpv_vert:
            if (vg_draw_vline(x,y,len,new_color)) return 1;
            break;
        default:    
            break;
    }
    sleep(delay);

    if (vg_exit()) return 1;

    return 0;

}

void *(vg_init)(uint16_t mode) {
    if (lpv_get_mode_info(mode, &vmi)) return NULL;

    h_res = vmi.x_res;
    v_res = vmi.y_res;

    bits_per_pixel = vmi.bpp;
    bytes_per_pixel = (bits_per_pixel + 7) / 8;

    vram_base = vmi.phys_addr;
    vram_size = bytes_per_pixel * h_res * v_res;

    video_mem = video_map_phys(vram_base, vram_size);

    if (lpv_set_mode(mode)) return NULL;

    return video_mem;
}

void (normalize_color)(uint16_t mode, uint32_t color, uint32_t *new_color) {
    if (mode == 0) {
        return;
    }
    if (mode == 1 || mode == 2 || mode == 4) {
        *new_color = color;
    }
    if (mode == 3) {
        *new_color = color & 0x001F3F1F;
    }

}


int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if (x >= h_res || y >= v_res)
        return 1;
    
    unsigned int pod = (x+y*h_res) * bytes_per_pixel;
    memcpy(video_mem + pos, &color, bytes_per_pixel);

    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (uint16_t i = 0; i < len; i++)
        if (draw_pixel(x+i, y, color))
            return 1;
    return 0;
}


int (vg_draw_vline)(uint16_t x, unt16_t y, uint16_t len, uint32_t color) {
    for (uint16_t i = 0; i < len; i++) 
        if (draw_pixel(x,y+i,color))
            return 1;
    return 0;
}

