# image_manipulation

#for download iup file (in linux)

sudo apt update 

sudo apt install iup

#for setup iup (in linux)

wget "https://sourceforge.net/projects/iup/files/3.31/Linux%20Libraries/iup-3.31_Linux54_64_lib.tar.gz/download" -O iup-lib.tar.gz

mkdir iup_tmp

tar -zxvf iup-lib.tar.gz -C iup_tmp

#Move header files

sudo cp -r iup_tmp/include/* /usr/local/include/

#Move shared and static libraries

sudo cp iup_tmp/*.so /usr/local/lib/

sudo cp iup_tmp/*.a /usr/local/lib/

#Update the linker bindings

sudo ldconfig

#for creating the image manipulation app (in terminal of linux)

sudo apt update

sudo apt install make

cd project2

make clean

make
