#include <stdio.h>
#include <stdlib.h>
#include <iup.h>
#include <iupdraw.h>
#include "gui.h"
#include "image.h"


BMPPixel *img_pixels = NULL;
int img_w = 0;
int img_h = 0;

BMPPixel *undo_pixels = NULL;
int undo_w = 0;
int undo_h = 0;

Ihandle *display = NULL; 

int main(int argc, char **argv) {

    if (IupOpen(&argc, &argv) == IUP_ERROR) {
        fprintf(stderr, "Error initializing IUP library!\n");
        return 1;
    }

    // File Menu Items
    Ihandle *open_btn = IupItem("Open Image", NULL);
    Ihandle *save_btn = IupItem("Save As...", NULL);
    Ihandle *exit_btn = IupItem("Exit", NULL);

    Ihandle *file_menu = IupMenu(open_btn, save_btn, IupSeparator(), exit_btn, NULL);
    Ihandle *sub_file  = IupSubmenu("File", file_menu);

    // Edit Menu Items
    Ihandle *undo_btn   = IupItem("Undo", NULL);
    Ihandle *gray_btn   = IupItem("Grayscale", NULL);
    Ihandle *bright_btn = IupItem("Brightness Adjustment", NULL);
    Ihandle *invert_btn = IupItem("Image Inversion", NULL);
    Ihandle *hflip_btn  = IupItem("Horizontal Flip", NULL);
    Ihandle *vflip_btn  = IupItem("Vertical Flip", NULL);
    Ihandle *rotate_btn = IupItem("Rotate 90 Degree", NULL);
    Ihandle *crop_btn   = IupItem("Crop", NULL);
    Ihandle *blur_btn   = IupItem("Blur", NULL);
    Ihandle *sharpen_btn = IupItem("sharpen", NULL);

    Ihandle *edit_menu = IupMenu(
        undo_btn, IupSeparator(),
        gray_btn, bright_btn, invert_btn, hflip_btn, vflip_btn,
        rotate_btn, crop_btn, blur_btn, sharpen_btn, NULL
    );
    Ihandle *sub_edit = IupSubmenu("Edit", edit_menu);

    Ihandle *menu_bar = IupMenu(sub_file, sub_edit, NULL);

    // Canvas Creation
    Ihandle *canvas = IupCanvas(NULL);
    IupSetAttribute(canvas, "RASTERSIZE", "625x400");
    IupSetAttribute(canvas, "EXPAND", "YES");

    display = canvas; 

    IupSetCallback(canvas, "ACTION", (Icallback)canvas_redraw_cb);

    // Main Dialog Creation
    Ihandle *vbox = IupVbox(canvas, NULL);
    Ihandle *dlg = IupDialog(vbox);
    IupSetAttribute(dlg, "TITLE", "BMP Image Manipulation Software");
    IupSetAttributeHandle(dlg, "MENU", menu_bar);

    // File Menu Action Connections
    IupSetCallback(open_btn, "ACTION", (Icallback)open_image_cb);
    IupSetCallback(save_btn, "ACTION", (Icallback)save_image_cb);
    IupSetCallback(exit_btn, "ACTION", (Icallback)exit_cb);

    // Edit Menu Action Connections
    IupSetCallback(undo_btn,   "ACTION", (Icallback)undo_cb);
    IupSetCallback(gray_btn,   "ACTION", (Icallback)grayscale_cb);
    IupSetCallback(bright_btn, "ACTION", (Icallback)bright_cb);
    IupSetCallback(invert_btn, "ACTION", (Icallback)invert_cb);
    IupSetCallback(hflip_btn,  "ACTION", (Icallback)hflip_cb);
    IupSetCallback(vflip_btn,  "ACTION", (Icallback)vflip_cb);
    IupSetCallback(rotate_btn, "ACTION", (Icallback)rotate_cb);
    IupSetCallback(crop_btn,   "ACTION", (Icallback)crop_cb);
    IupSetCallback(blur_btn,   "ACTION", (Icallback)blur_cb);
    IupSetCallback(sharpen_btn, "ACTION", (Icallback)sharpen_cb);

    // Ensure the free of memory space while pressing exit
    IupSetCallback(dlg, "CLOSE_CB", (Icallback)exit_cb);

    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
    IupMainLoop();
    IupClose();

    return 0;
}
