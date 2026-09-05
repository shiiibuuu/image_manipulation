#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iup.h>
#include <iupdraw.h>
#include "gui.h"
#include "image.h"

// IUP image handle
static Ihandle *img_handle = NULL;

//1. backup helper function of Undo file save
static void save_undo_state(void) {
    if (!img_pixels || img_w <= 0 || img_h <= 0) return;

    if (undo_pixels) free(undo_pixels);
    undo_w = img_w;
    undo_h = img_h;

    undo_pixels = (BMPPixel *)malloc(img_w * img_h * sizeof(BMPPixel));
    if (!undo_pixels) return;

    memcpy(undo_pixels, img_pixels, img_w * img_h * sizeof(BMPPixel));
}

//2. function of refreshing canvas
void update_canvas_image(void) {
    if (!img_pixels || img_w <= 0 || img_h <= 0) return;

    // alocate 3 byte buffer for rgb
    unsigned char *rgb_buffer = (unsigned char *)malloc(img_w * img_h * 3);
    if (!rgb_buffer) return;

    for (int y = 0; y < img_h; y++) {
        for (int x = 0; x < img_w; x++) {
            int src_idx = (img_h - 1 - y) * img_w + x;
            int dst_idx = (y * img_w + x) * 3;

            rgb_buffer[dst_idx + 0] = img_pixels[src_idx].r;
            rgb_buffer[dst_idx + 1] = img_pixels[src_idx].g;
            rgb_buffer[dst_idx + 2] = img_pixels[src_idx].b;
        }
    }

    if (img_handle) IupDestroy(img_handle);
    img_handle = IupImageRGB(img_w, img_h, rgb_buffer);
    free(rgb_buffer);

    IupSetHandle("CURRENT_IMG", img_handle);
    if (display) IupUpdate(display);
}

//3. Open Image Callback
int open_image_cb(Ihandle *self) {
    (void)self;

    Ihandle *filedlg = IupFileDlg();
    IupSetAttribute(filedlg, "DIALOGTYPE", "OPEN");
    IupSetAttribute(filedlg, "FILTER", "*.bmp");
    IupSetAttribute(filedlg, "FILTERTITLE", "BMP Image Files");

    IupPopup(filedlg, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(filedlg, "STATUS") != -1) {
        char *filename = IupGetAttribute(filedlg, "VALUE");

        if (load_bmp(filename)) {
            update_canvas_image();
        } else {
            IupMessage("Error", "Failed to load BMP file! Please select a valid 24-bit BMP image.");
        }
    }

    IupDestroy(filedlg);
    return IUP_DEFAULT;
}

//4. Canvas Redraw Callback
int canvas_redraw_cb(Ihandle *self) {
    IupDrawBegin(self);
    IupDrawParentBackground(self);
    
    if (img_handle) {
        int canvas_w, canvas_h;
      
        IupGetIntInt(self, "DRAWSIZE", &canvas_w, &canvas_h);


        int center_x = (canvas_w - img_w) / 2;
        int center_y = (canvas_h - img_h) / 2;


        if (center_x < 0) center_x = 0;
        if (center_y < 0) center_y = 0;

    
        IupDrawImage(self, "CURRENT_IMG", center_x, center_y, 0, 0);
    }
    
    IupDrawEnd(self);
    return IUP_DEFAULT;
}

//5. Undo Callback
int undo_cb(Ihandle *self) {
    (void)self;

    if (!undo_pixels || undo_w <= 0 || undo_h <= 0) {
        IupMessage("Info", "Nothing to undo!");
        return IUP_DEFAULT;
    }

    if (img_pixels) free(img_pixels);
    img_w = undo_w;
    img_h = undo_h;

    img_pixels = (BMPPixel *)malloc(img_w * img_h * sizeof(BMPPixel));
    if (!img_pixels) return IUP_DEFAULT;

    memcpy(img_pixels, undo_pixels, img_w * img_h * sizeof(BMPPixel));

    update_canvas_image();
    return IUP_DEFAULT;
}

//6. Brightness Adjustment Callback
int bright_cb(Ihandle *self) {
    (void)self;
    if (!img_pixels || img_w <= 0 || img_h <= 0){
        IupMessage("Error", "No image loaded to adjust brightness!");
        return IUP_DEFAULT;
    }

    int amount = 30;
    if (!IupGetParam("Brightness Adjustment", NULL, NULL, "Amount (-255 to 255): %i\n", &amount, NULL)) {
        return IUP_DEFAULT;
    }

    save_undo_state();

    for (int i = 0; i < img_h * img_w; i++) {
    
        int r = img_pixels[i].r + amount;
        int g = img_pixels[i].g + amount;
        int b = img_pixels[i].b + amount;

        img_pixels[i].r = (uint8_t)(r < 0 ? 0 : (r > 255 ? 255 : r));
        img_pixels[i].g = (uint8_t)(g < 0 ? 0 : (g > 255 ? 255 : g));
        img_pixels[i].b = (uint8_t)(b < 0 ? 0 : (b > 255 ? 255 : b));
    }

    update_canvas_image();
    return IUP_DEFAULT;
}


//7. Rotate Callback (90 Degrees Clockwise)
int rotate_cb(Ihandle *self) {
    (void)self;
    if (!img_pixels || img_w <= 0 || img_h <= 0){
        IupMessage("Error", "No image loaded to rotate!");
        return IUP_DEFAULT;
    }

    save_undo_state();

    int new_w = img_h;
    int new_h = img_w;
    BMPPixel *new_pixels = (BMPPixel *)malloc(new_w * new_h * sizeof(BMPPixel));
    if (!new_pixels) return IUP_DEFAULT;

    for (int y = 0; y < img_h; y++) {
        for (int x = 0; x < img_w; x++) {
            int src_idx = y * img_w + x;
            int dst_x = img_h - 1 - y;
            int dst_y = x;
            int dst_idx = dst_y * new_w + dst_x;

            new_pixels[dst_idx] = img_pixels[src_idx];
        }
    }

    free(img_pixels);
    img_pixels = new_pixels;
    img_w = new_w;
    img_h = new_h;

    update_canvas_image();
    return IUP_DEFAULT;
}

//8. Crop Callback
int crop_cb(Ihandle *self) {
    (void)self;
    if (!img_pixels || img_w <= 0 || img_h <= 0){
        IupMessage("Error", "No image loaded to crop!");
        return IUP_DEFAULT;
    }

    int crop_x = 0, crop_y = 0, crop_w = img_w / 2, crop_h = img_h / 2;
 /*   if (!IupGetParam("Crop Image", NULL, NULL,
                    "Start X: %i\nStart Y: %i\nWidth: %i\nHeight: %i\n",
                    &crop_x, &crop_y, &crop_w, &crop_h, NULL)) {
        return IUP_DEFAULT;
    }
*/
    
    if (!IupGetParam("Crop Image", NULL, NULL,
                    "Start X:               %i\nStart Y:               %i\nWidth:               %i\nHeight:               %i\n",
                    &crop_x, &crop_y, &crop_w, &crop_h, NULL)) {
        return IUP_DEFAULT;
    }

    if (crop_x < 0) crop_x = 0;
    if (crop_y < 0) crop_y = 0;
    if (crop_x >= img_w || crop_y >= img_h || crop_w <= 0 || crop_h <= 0) {
        IupMessage("Error", "Invalid Crop Region!");
        return IUP_DEFAULT;
    }
    if (crop_x + crop_w > img_w) crop_w = img_w - crop_x;
    if (crop_y + crop_h > img_h) crop_h = img_h - crop_y;

    save_undo_state();

    BMPPixel *new_pixels = (BMPPixel *)malloc(crop_w * crop_h * sizeof(BMPPixel));
    if (!new_pixels) return IUP_DEFAULT;

    for (int y = 0; y < crop_h; y++) {
        for (int x = 0; x < crop_w; x++) {
            int src_idx = (crop_y + y) * img_w + (crop_x + x);
            int dst_idx = y * crop_w + x;
            new_pixels[dst_idx] = img_pixels[src_idx];
        }
    }

    free(img_pixels);
    img_pixels = new_pixels;
    img_w = crop_w;
    img_h = crop_h;

    update_canvas_image();
    return IUP_DEFAULT;
}


//9. Blur Callback (3x3 Box Blur)
int blur_cb(Ihandle *self) {
    (void)self;
    if (!img_pixels || img_w <= 0 || img_h <= 0){
        IupMessage("Error", "No image loaded to blur!");
        return IUP_DEFAULT;
    }
    save_undo_state();

    BMPPixel *output = (BMPPixel *)malloc(img_w * img_h * sizeof(BMPPixel));
    if (!output) return IUP_DEFAULT;

    for (int y = 0; y < img_h; y++) {
        for (int x = 0; x < img_w; x++) {
            int sum_r = 0, sum_g = 0, sum_b = 0;
            int count = 0;

            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < img_w && ny >= 0 && ny < img_h) {
                        int idx = ny * img_w + nx;
                        sum_r += img_pixels[idx].r;
                        sum_g += img_pixels[idx].g;
                        sum_b += img_pixels[idx].b;
                        count++;
                    }
                }
            }

            int out_idx = y * img_w + x;
            output[out_idx].r = (uint8_t)(sum_r / count);
            output[out_idx].g = (uint8_t)(sum_g / count);
            output[out_idx].b = (uint8_t)(sum_b / count);
        }
    }

    free(img_pixels);
    img_pixels = output;

    update_canvas_image();
    return IUP_DEFAULT;
}

//10. Grayscale Callback
int grayscale_cb(Ihandle *self) {
    (void)self;
    if (!img_pixels || img_w <= 0 || img_h <= 0){
        IupMessage("Error", "No image loaded to convert to grayscale!");
        return IUP_DEFAULT;
    }
    save_undo_state();

    for (int i = 0; i < img_h * img_w; i++) {
        uint8_t gray = (uint8_t)(0.299 * img_pixels[i].r + 0.587 * img_pixels[i].g + 0.114 * img_pixels[i].b);
        img_pixels[i].r = img_pixels[i].g = img_pixels[i].b = gray;
    }

    update_canvas_image();
    return IUP_DEFAULT;
}

//11. Horizontal Flip Callback
int hflip_cb(Ihandle *self) {
    (void)self;
    if (!img_pixels || img_w <= 0 || img_h <= 0){
        IupMessage("Error", "No image loaded to flip horizontally!");
        return IUP_DEFAULT;
    }
    save_undo_state();

    int width = img_w;
    int height = img_h;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width / 2; x++) {
            int src_idx = y * width + x;
            int dst_idx = y * width + (width - 1 - x);

            BMPPixel temp = img_pixels[src_idx];
            img_pixels[src_idx] = img_pixels[dst_idx];
            img_pixels[dst_idx] = temp;
        }
    }

    update_canvas_image();
    return IUP_DEFAULT;
}

//12. Vertical Flip Callback
int vflip_cb(Ihandle *self) {
    (void)self;
    if (!img_pixels || img_w <= 0 || img_h <= 0){
        IupMessage("Error", "No image loaded to flip vertically!");
        return IUP_DEFAULT;
    }
    save_undo_state();

    int width = img_w;
    int height = img_h;

    for (int y = 0; y < height / 2; y++) {
        for (int x = 0; x < width; x++) {
            int src_idx = y * width + x;
            int dst_idx = (height - 1 - y) * width + x;

            BMPPixel temp = img_pixels[src_idx];
            img_pixels[src_idx] = img_pixels[dst_idx];
            img_pixels[dst_idx] = temp;
        }
    }

    update_canvas_image();
    return IUP_DEFAULT;
}

//13. Invert Color Callback
int invert_cb(Ihandle *self) {
    (void)self;
    if (!img_pixels || img_w <= 0 || img_h <= 0){
        IupMessage("Error", "No image loaded to invert!");
        return IUP_DEFAULT;
    }
    save_undo_state();

    for (int i = 0; i < img_h * img_w; i++) {
        img_pixels[i].r = 255 - img_pixels[i].r;
        img_pixels[i].g = 255 - img_pixels[i].g;
        img_pixels[i].b = 255 - img_pixels[i].b;
    }

    update_canvas_image();
    return IUP_DEFAULT;
}

//14. Exit Callback
int exit_cb(Ihandle *self) {
    (void)self;

    if (img_pixels) {
        free(img_pixels);
        img_pixels = NULL;
    }
    if (undo_pixels) {
        free(undo_pixels);
        undo_pixels = NULL;
    }
    if (img_handle) {
        IupDestroy(img_handle);
        img_handle = NULL;
    }

    return IUP_CLOSE;
}

// Save Image Callback
int save_image_cb(Ihandle *self) {
    (void)self;
    if (!img_pixels || img_w <= 0 || img_h <= 0) {
        IupMessage("Error", "No image available to save!");
        return IUP_DEFAULT;
    }

    Ihandle *filedlg = IupFileDlg();
    IupSetAttribute(filedlg, "DIALOGTYPE", "SAVE");
    IupSetAttribute(filedlg, "FILTER", "*.bmp");
    IupSetAttribute(filedlg, "FILTERTITLE", "BMP Image Files");
    IupSetAttribute(filedlg, "EXTDEFAULT", "bmp");

    IupPopup(filedlg, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(filedlg, "STATUS") != -1) {
        char *filename = IupGetAttribute(filedlg, "VALUE");

        if (save_bmp(filename)) {
            IupMessage("Success", "Image saved successfully!");
        } else {
            IupMessage("Error", "Could not create file!");
        }
    }

    IupDestroy(filedlg);
    return IUP_DEFAULT;
}
