# 🖼️ BMP Image Manipulation Software (C & IUP)

**Course:** Image Processing / C Programming Project

**Name:** Md. Shoyeb Uddin Shibli

**Roll:** 1847

**reg no.:** 2025-816-376

An interactive, desktop-based graphical **Image Manipulation Software** written in C99 using the **IUP (Portable User Interface)** library. This application provides custom pixel-level digital image processing capabilities for 24-bit uncompressed BMP (Bitmap) files without relying on external image processing libraries for pixel algorithms.

---

## 📌 System Architecture & High-Level Overview

The application is designed using a **modular C architecture**, separating user interface controls, file stream operations, and pixel manipulation logic into decoupled components:

- **`main.c`**: Handles IUP library initialization, UI layout assembly (Window, Canvas, VBox, Menu Bar), and event loop invocation.
- **`gui.c` / `gui.h`**: Manages UI action callbacks, dialog generation (`IupFileDlg`, `IupGetParam`), off-screen canvas redrawing via `IupDraw`, BGR-to-RGB color channel conversions, and state history buffering (Undo).
- **`image.c` / `image.h`**: Implements 24-bit uncompressed BMP file reading and writing algorithms, including packed binary header processing (`BMPFileHeader`, `BMPInfoHeader`) and 4-byte scanline padding calculations.

---

## 🎨 Features & Visual Demonstration

### 1. Main Canvas Interface (Loaded State)
Displays the application viewport holding a standard 24-bit BMP image centered inside the IUP canvas layout.

<p align="center">
  <img src="images/Screenshot_01_Main_Canvas.jpg" width="550" alt="Main Canvas Window">
</p>

---

### 2. Core Image Manipulation Operations

| Operation | Menu Item | Description & Applied Algorithmic Logic | Visual Output |
| :--- | :--- | :--- | :---: |
| **Grayscale** | `Edit -> Grayscale` | Converts color pixels to intensity using BT.601 weighted luminance: `gray = 0.299*R + 0.587*G + 0.114*B` | <img width="1920" height="913" alt="Screenshot_02_Grayscale" src="https://github.com/user-attachments/assets/6c0c05d8-67a9-4566-b058-465557a606d7" /> |
| **Brightness** | `Edit -> Brightness Adjustment` | Adds/subtracts user scalar `S` in [-255, 255] with clamping to [0, 255]: `C' = clamp(C + S)` |  <img width="1920" height="913" alt="Screenshot_03_Brightness" src="https://github.com/user-attachments/assets/0c6de67c-d84e-449c-a0dc-a7f9d8d09a66" /> |
| **Inversion** | `Edit -> Image Inversion` | Computes photographic negative values per RGB channel: `R' = 255 - R`, `G' = 255 - G`, `B' = 255 - B` | <img src="এখানে_ইনভার্সন_ছবির_লিংক" width="220"> |
| **Horizontal Flip** | `Edit -> Horizontal Flip` | Mirrors image horizontally across the vertical axis: `(x, y) <-> (W - 1 - x, y)` | <img src="এখানে_এইচ_ফ্লিপ_ছবির_লিংক" width="220"> |
| **Vertical Flip** | `Edit -> Vertical Flip` | Mirrors image vertically across the horizontal axis: `(x, y) <-> (x, H - 1 - y)` | <img src="এখানে_ভি_ফ্লিপ_ছবির_লিংক" width="220"> |
| **Rotate 90°** | `Edit -> Rotate 90 Degree` | Rotates canvas 90° clockwise by allocating swapped dimensions (H x W): `x' = H - 1 - y`, `y' = x` | <img src="এখানে_রোটেট_ছবির_লিংক" width="220"> |
| **Cropping** | `Edit -> Crop` | Extracts rectangular sub-region (X, Y, W, H) into a new image buffer | <img src="এখানে_ক্রপ_ছবির_লিংক" width="220"> |
| **3x3 Box Blur** | `Edit -> Blur` | Smooths pixels via spatial neighborhood averaging over 3x3 grid | <img src="এখানে_ব্লার_ছবির_লিংক" width="220"> |
| **Sharpening** | `Edit -> Sharpen` | Enhances high-frequency edge detail using 3x3 convolution kernel: `[0, -1, 0 / -1, 5, -1 / 0, -1, 0]` | <img src="এখানে_শার্পেন_ছবির_লিংক" width="220"> |

---

## 📊 Technical Feature Matrix

| Module / Category | Feature | Status | Technical Implementation Details |
| :--- | :--- | :---: | :--- |
| **File I/O** | 24-bit BMP Load | ✅ | Parses `BM` signature (`0x4D42`), bit depth (24 BPP), and compression mode (0). Handles bottom-up and top-down scanlines. |
| **File I/O** | 24-bit BMP Save | ✅ | Computes scanline byte padding (`(4 - (w * 3) % 4) % 4`), constructs valid binary headers, and writes raw BGR byte streams. |
| **GUI & State** | Centered Rendering | ✅ | Calculates view offsets dynamically in `canvas_redraw_cb` to center variable resolution images cleanly inside canvas boundaries. |
| **GUI & State** | Single-Level Undo | ✅ | Maintains an isolated heap buffer (`undo_pixels`) preserving image dimensions and pixel data prior to destructive transformations. |
| **Image Processing** | Spatial Filtering | ✅ | Custom 2D loop implementations for 3x3 spatial kernels (Blur & Sharpen Convolution) avoiding external filter libraries. |

---

## 🛠️ Build and Compilation

### Requirements
- **GCC Compiler** (C99 Standard)
- **IUP Toolkit Libraries** (`libiup`)
- **GNU Make**


### Execution Instructions
```bash
# 1. Clone the repository
git clone [https://github.com/your-username/bmp-image-editor.git](https://github.com/your-username/bmp-image-editor.git)
cd bmp-image-editor

# 2. Compile the application
make

# 3. Launch the software
./app

# 4. Clean compiled binaries
make clean
