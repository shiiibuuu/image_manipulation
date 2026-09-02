#ifndef GUI_H
#define GUI_H

#include <iup.h>

// Canvas Redraw & Canvas Handle Ref
extern Ihandle *display;

void update_canvas_image(void);

// Main File Callback Prototypes
int open_image_cb(Ihandle *self);
int save_image_cb(Ihandle *self);
int exit_cb(Ihandle *self);
int undo_cb(Ihandle *self);
int canvas_redraw_cb(Ihandle *self);

// Processing Callback Prototypes
int grayscale_cb(Ihandle *self);
int hflip_cb(Ihandle *self);
int vflip_cb(Ihandle *self);
int invert_cb(Ihandle *self);
int bright_cb(Ihandle *self);
int rotate_cb(Ihandle *self);
int crop_cb(Ihandle *self);
int blur_cb(Ihandle *self);

#endif // GUI_H