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
| **Grayscale** | `Edit -> Grayscale` | Converts color pixels to intensity using BT.601 weighted luminance: <br> `gray = 0.299*R + 0.587*G + 0.114*B` | <img src="images/Screenshot_02_Grayscale.jpg" width="220" alt="Grayscale"> |
| **Brightness** | `Edit -> Brightness Adjustment` | Adds/subtracts user scalar `S` in [-255, 255] with clamping to [0, 255]: <br> `C' = clamp(C + S)` | <img src="images/Screenshot_03_Brightness.jpg" width="220" alt="Brightness"> |
| **Inversion** | `Edit -> Image Inversion` | Computes photographic negative values per RGB channel: <br> `R' = 255 - R`, `G' = 255 - G`, `B' = 255 - B` | <img src="images/Screenshot_04_Inversion.jpg" width="220" alt="Inversion"> |
| **Horizontal Flip** | `Edit -> Horizontal Flip` | Mirrors image horizontally across the vertical axis: <br> `(x, y) <-> (W - 1 - x, y)` | <img src="images/Screenshot_05_HFlip.jpg" width="220" alt="Horizontal Flip"> |
| **Vertical Flip** | `Edit -> Vertical Flip` | Mirrors image vertically across the horizontal axis: <br> `(x, y) <-> (x, H - 1 - y)` | <img src="images/Screenshot_06_VFlip.jpg" width="220" alt="Vertical Flip"> |
| **Rotate 90°** | `Edit -> Rotate 90 Degree` | Rotates canvas 90° clockwise by allocating swapped dimensions (H x W): <br> `x' = H - 1 - y`, `y' = x` | <img src="images/Screenshot_07_Rotate.jpg" width="220" alt="Rotate 90"> |
| **Cropping** | `Edit -> Crop` | Extracts rectangular sub-region (X, Y, W, H) into a new image buffer | <img src="images/Screenshot_08_Crop.png" width="220" alt="Crop"> |
| **3x3 Box Blur** | `Edit -> Blur` | Smooths pixels via spatial neighborhood averaging over 3x3 grid | <img src="images/Screenshot_09_Blur.jpg" width="220" alt="Blur"> |
| **Sharpening** | `Edit -> Sharpen` | Enhances high-frequency edge detail using 3x3 convolution kernel: <br> `[0, -1, 0 / -1, 5, -1 / 0, -1, 0]` | <img src="images/Screenshot_10_Sharpen.jpg" width="220" alt="Sharpen"> |

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
make clean# 🖼️ BMP Image Manipulation Software (C & IUP)

An interactive, desktop-based graphical **Image Manipulation Software** written in C99 using the **IUP (Portable User Interface)** library[cite: 1, 2, 7]. This application provides custom pixel-level digital image processing capabilities for 24-bit uncompressed BMP (Bitmap) files without relying on external image processing libraries for pixel algorithms[cite: 1].

---

## 📌 System Architecture & High-Level Overview

The application is designed using a **modular C architecture**, separating user interface controls, file stream operations, and pixel manipulation logic into decoupled components[cite: 1, 6, 7]:

- **`main.c`**: Handles IUP library initialization, UI layout assembly (Window, Canvas, VBox, Menu Bar), and event loop invocation[cite: 6].
- **`gui.c` / `gui.h`**: Manages UI action callbacks, dialog generation (`IupFileDlg`, `IupGetParam`), off-screen canvas redrawing via `IupDraw`, BGR-to-RGB color channel conversions, and state history buffering (Undo)[cite: 2, 3].
- **`image.c` / `image.h`**: Implements 24-bit uncompressed BMP file reading and writing algorithms, including packed binary header processing (`BMPFileHeader`, `BMPInfoHeader`) and 4-byte scanline padding calculations[cite: 4, 5].

---

## 🎨 Features & Visual Demonstration

### 1. Main Canvas Interface (Loaded State)
Displays the application viewport holding a standard $24$-bit BMP image centered inside the IUP canvas layout[cite: 2, 6].

![Main Canvas Window](images/Screenshot_01_Main_Canvas.jpg)

---

### 2. Core Image Manipulation Operations

| Operation | Menu Item | Description & Applied Algorithmic Logic | Visual Output |
| :--- | :--- | :--- | :--- |
| **Grayscale** | `Edit -> Grayscale` | Converts color pixels to intensity using BT.601 weighted luminance: <br> $gray = 0.299R + 0.587G + 0.114B$[cite: 2] | ![Grayscale](images/Screenshot_02_Grayscale.jpg) |
| **Brightness** | `Edit -> Brightness Adjustment` | Adds/subtracts user scalar $S \in [-255, 255]$ with clamping to $[0, 255]$: <br> $C' = \text{clamp}(C + S)$[cite: 2] | ![Brightness](images/Screenshot_03_Brightness.jpg) |
| **Inversion** | `Edit -> Image Inversion` | Computes photographic negative values per RGB channel: <br> $R' = 255 - R, \ G' = 255 - G, \ B' = 255 - B$[cite: 2] | ![Inversion](images/Screenshot_04_Inversion.jpg) |
| **Horizontal Flip** | `Edit -> Horizontal Flip` | Mirrors image horizontally across the vertical axis: <br> $(x, y) \leftrightarrow (W - 1 - x, y)$[cite: 2] | ![Horizontal Flip](images/Screenshot_05_HFlip.jpg) |
| **Vertical Flip** | `Edit -> Vertical Flip` | Mirrors image vertically across the horizontal axis: <br> $(x, y) \leftrightarrow (x, H - 1 - y)$[cite: 2] | ![Vertical Flip](images/Screenshot_06_VFlip.jpg) |
| **Rotate 90°** | `Edit -> Rotate 90 Degree` | Rotates canvas 90° clockwise by allocating swapped dimensions ($H \times W$): <br> $x' = H - 1 - y, \quad y' = x$[cite: 2] | ![Rotate 90](images/Screenshot_07_Rotate.jpg) |
| **Cropping** | `Edit -> Crop` | Extracts rectangular sub-region $(X, Y, W, H)$ into a new image buffer[cite: 2] | ![Crop](images/Screenshot_08_Crop.png) |
| **3x3 Box Blur** | `Edit -> Blur` | Smooths pixels via spatial neighborhood averaging over $3 \times 3$ grid[cite: 2] | ![Blur](images/Screenshot_09_Blur.jpg) |
| **Sharpening** | `Edit -> Sharpen` | Enhances high-frequency edge detail using $3 \times 3$ convolution matrix: <br> $K = \begin{bmatrix} 0 & -1 & 0 \\ -1 & 5 & -1 \\ 0 & -1 & 0 \end{bmatrix}$[cite: 1] | ![Sharpen](images/Screenshot_10_Sharpen.jpg) |

---

## 📊 Technical Feature Matrix

| Module / Category | Feature | Status | Technical Implementation Details |
| :--- | :--- | :---: | :--- |
| **File I/O** | 24-bit BMP Load | ✅ | Parses `BM` signature (`0x4D42`), bit depth (24 BPP), and compression mode (0)[cite: 4]. Handles bottom-up and top-down scanlines[cite: 4]. |
| **File I/O** | 24-bit BMP Save | ✅ | Computes scanline byte padding (`(4 - (w * 3) % 4) % 4`), constructs valid binary headers, and writes raw BGR byte streams[cite: 4, 5]. |
| **GUI & State** | Centered Rendering | ✅ | Calculates view offsets dynamically in `canvas_redraw_cb` to center variable resolution images cleanly inside canvas boundaries[cite: 2]. |
| **GUI & State** | Single-Level Undo | ✅ | Maintains an isolated heap buffer (`undo_pixels`) preserving image dimensions and pixel data prior to destructive transformations[cite: 2, 6]. |
| **Image Processing** | Spatial Filtering | ✅ | Custom 2D loop implementations for $3 \times 3$ spatial kernels (Blur & Sharpen Convolution) avoiding external filter libraries[cite: 1, 2]. |

---

## 🛠️ Build and Compilation

### Requirements
- **GCC Compiler** (C99 Standard)[cite: 7]
- **IUP Toolkit Libraries** (`libiup`)[cite: 7]
- **GNU Make**[cite: 7]

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
