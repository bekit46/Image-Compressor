#include<iostream>
#include<string>
#include<fstream>

using namespace std;

class Pixel // pixel pair class keeps pixel repeat and value of this repeat
{
    int repeat;
    char pixelValue;

    public: // basic functions of pixel class
        Pixel();
        Pixel(int repeat, char pixelValue);
        ~Pixel();

        void setRepeat(int repeat);
        void setValue(char value);
        int getRepeat();
        char getValue();
};

Pixel::Pixel()
{
    //Default copy constructor
    this->repeat = 0;
    this->pixelValue = '\0';
}

Pixel::Pixel(int repeat, char pixelValue)
{
    //Modified Constructor
    this->repeat = repeat;
    this->pixelValue = pixelValue;
}

Pixel::~Pixel()
{
    //Default Destructor but doesn't do anything special
    this->repeat = 0;
    this->pixelValue = '\0';
}

void Pixel::setRepeat(int repeat)
{
    this->repeat = repeat;
}

void Pixel::setValue(char value)
{
    this->pixelValue = value;
}

int Pixel::getRepeat()
{
    return this->repeat;
}

char Pixel::getValue()
{
    return this->pixelValue;
}
//********************************END OF THE CLASS FUNCTIONS**************************************

void RLE_compressor(string file_name, char* temp, int size, int& counter) // Run Length Encoding Compressor
{
    counter = 1;
    for(int i = 0; i < size-1; i++)
    {
        if(temp[i] != temp[i+1])
        {
            counter++;
        }
    }

    Pixel* image = new Pixel[counter];
    int repeat = 1;
    int index = 0;

    for(int i = 0; i < size; i++)
    {
        if(temp[i] == temp[i+1])
        {
            repeat ++;
        }
        else
        {
            image[index].setRepeat(repeat);
            image[index].setValue(temp[i]);
            index++;
            repeat = 1;
        }
    }

    
    ofstream output(file_name, ios::binary);
    if(output.is_open())
    {
        for(int i = 0; i < counter; i++)
        {
            int repeat = image[i].getRepeat();
            char pixelValue = image[i].getValue();

            output.write((char*)&repeat, sizeof(int));
            output.write(&pixelValue, sizeof(char));
        }
        output.close();
    }
    else
        cerr << "File couldn't open (version error 1.0)";

    delete[] image;
}

void RLE_decompressor(string fileName, int num_of_obj, int rows, int cols, int max) //Run lenght decoding decompressor
{
    ifstream file(fileName + "_compressed.bkt", ios::binary | ios::ate); //get values from compressed document
    if(file.is_open())
    {
        int filesize = file.tellg();
        char* mBlock = new char[filesize];
        file.seekg(0, ios::beg);
        file.read(mBlock, filesize);
        char* temp = mBlock;
    

        ofstream decompressed(fileName + "_decompressed.pgm", ios::binary); //new decompressed version of image
        if(decompressed.is_open())
        {
            decompressed << "P5\n" << "#Decompressed by using BKT codes\n" << cols << " " << rows << "\n" << max << "\n"; // adds imporan information and comments into the pgm file

            for(int i = 0; i < num_of_obj; i++)
            {
                int repeataiton = *((int*)temp);
                temp += sizeof(int);
                
                for(int j = 0; j < repeataiton; j++)
                {
                    decompressed.write(temp, sizeof(char));
                }
                temp += sizeof(char);
            }
        }
        else
            cerr << "File couldn't open (version error 2.0)";
        delete[] mBlock;
    }
    else
        cerr << "File couldn't open (version error 2.1)";
}

int strToint(string line) // basic string to integer converter
{
    int ans = 0;
    for(int i = 0; i < line.length(); i++)
    {
        ans *= 10;
        ans += line[i] - '0';
    }
    return ans;
}

void widthheight(string line, int& width, int& height) // takes width and height from comment line of the original pgm file
{
    string w, h;
    int index = 0;
    while(line[index] != ' ')
    {
        w += line[index];
        index ++;
    }
    index ++;
    while(line[index] != '\0')
    {
        h += line[index];
        index ++;
    }
    width = strToint(w);
    height = strToint(h);
}

char* imagedetails(string fileName, int& rows, int& cols, int& max) // read image details of the pgm file untill reaching unreadable pixel values.
{
    rows = 0; 
    cols = 0; 
    string line ="";

    ifstream pgm(fileName, ios::binary | ios::ate);
    if(pgm.is_open())
    {
        int filesize = pgm.tellg();
        pgm.seekg(0, ios::beg);

        getline(pgm,line);// read the first line : P5
        if(line[1] != '5')
        {
            cerr <<"(version error 3.0)"<< endl;
            return 0;
        }
        cout <<"Version : "<< line << endl;
        
        getline(pgm,line);// read the second line : comment
        cout <<"Comment : "<< line << endl;

        getline(pgm,line);
        widthheight(line, cols, rows);
        cout << cols <<" columns and "<< rows <<" rows"<< endl;
        
        getline(pgm,line);
        max = strToint(line);
        cout<< "Max value: " << max << "\n";

        int current = pgm.tellg();
        char* mBlock = new char[filesize - current];
        pgm.read(mBlock, (filesize - current));

        pgm.close();
        return mBlock; // when reach to the pixel values return the as a char* array
    }
    else
    {
        cerr << "File couldn't open (version error 3.1)";
        return nullptr;
    }
}