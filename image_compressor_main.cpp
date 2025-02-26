#include<iostream>
#include<string>
#include<fstream>
#include"image_compressor_header.h"

using namespace std;

int main()
{
    int rows, cols, max, size, num_of_obj;
    string file_name;
    string compressed_fn;

    cout << "Wellcome to BKT Image Compressor Terminal\n\nPlease enter the file name you want to compress: ";
    cin >> file_name;
    
    char* pixels = imagedetails(file_name + ".pgm", rows, cols, max);
    size = rows*cols;

    if(pixels != nullptr)
    {
        RLE_compressor(file_name + "_compressed.bkt", pixels, size, num_of_obj);

        RLE_decompressor(file_name, num_of_obj, rows, cols, max);
        
        cout << "\nProcess is completed succesfully";
    }
    else
        cout << "\nProcess couldn't complete";

    delete[] pixels;
    return 0;  
}