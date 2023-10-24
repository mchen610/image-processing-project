#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Header
{
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};

void print_header(Header &header)
{
    cout << int(header.idLength) << '\n';
    cout << int(header.colorMapType) << '\n';
    cout << int(header.dataTypeCode) << '\n';
    cout << int(header.colorMapOrigin) << '\n';
    cout << int(header.colorMapLength) << '\n';
    cout << int(header.colorMapDepth) << '\n';
    cout << int(header.xOrigin) << '\n';
    cout << int(header.yOrigin) << '\n';
    cout << int(header.width) << '\n';
    cout << int(header.height) << '\n';
    cout << int(header.bitsPerPixel) << '\n';
    cout << int(header.imageDescriptor) << '\n';
}

void readHeader(ifstream &file, Header &header)
{
    file.seekg(0);

    file.read(&header.idLength, sizeof(header.idLength));
    file.read(&header.colorMapType, sizeof(header.colorMapType));
    file.read(&header.dataTypeCode, sizeof(header.dataTypeCode));
    file.read(reinterpret_cast<char *>(&header.colorMapOrigin), sizeof(header.colorMapOrigin));
    file.read(reinterpret_cast<char *>(&header.colorMapLength), sizeof(header.colorMapLength));
    file.read(&header.colorMapDepth, sizeof(header.colorMapDepth));
    file.read(reinterpret_cast<char *>(&header.xOrigin), sizeof(header.xOrigin));
    file.read(reinterpret_cast<char *>(&header.yOrigin), sizeof(header.yOrigin));
    file.read(reinterpret_cast<char *>(&header.width), sizeof(header.width));
    file.read(reinterpret_cast<char *>(&header.height), sizeof(header.height));
    file.read(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
    file.read(&header.imageDescriptor, sizeof(header.imageDescriptor));
}

void writeHeader(ofstream &file, Header &header)
{
    file.seekp(0);

    file.write(&header.idLength, sizeof(header.idLength));
    file.write(&header.colorMapType, sizeof(header.colorMapType));
    file.write(&header.dataTypeCode, sizeof(header.dataTypeCode));
    file.write(reinterpret_cast<char *>(&header.colorMapOrigin), sizeof(header.colorMapOrigin));
    file.write(reinterpret_cast<char *>(&header.colorMapLength), sizeof(header.colorMapLength));
    file.write(&header.colorMapDepth, sizeof(header.colorMapDepth));
    file.write(reinterpret_cast<char *>(&header.xOrigin), sizeof(header.xOrigin));
    file.write(reinterpret_cast<char *>(&header.yOrigin), sizeof(header.yOrigin));
    file.write(reinterpret_cast<char *>(&header.width), sizeof(header.width));
    file.write(reinterpret_cast<char *>(&header.height), sizeof(header.height));
    file.write(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
    file.write(&header.imageDescriptor, sizeof(header.imageDescriptor));
}

void scale(const string &fileName, const string &newFileName, int factor, int offset = -1)
{
    ifstream readFile(fileName, ios::binary);
    Header header;
    readHeader(readFile, header);

    int size = header.width * header.bitsPerPixel/8 * header.height;

    ofstream writeFile(newFileName, ios::binary);
    writeHeader(writeFile, header);
    unsigned char currentByte;
    for (int i = 0; i < size; i++)
    {
        readFile.read(reinterpret_cast<char *>(&currentByte), sizeof(currentByte));
        if (i % 3 == offset || offset == -1)
        {
            int newValue = static_cast<int>(currentByte) * factor;
            newValue = (newValue > 255) ? 255 : newValue;
            unsigned char newByte = static_cast<unsigned char>(newValue);
            writeFile.write(reinterpret_cast<char *>(&newByte), sizeof(newByte));
        }
        else
        {
            writeFile.write(reinterpret_cast<char *>(&currentByte), sizeof(currentByte));
        }
    }

    readFile.close();
    writeFile.close();
}

void add_flat(const string &fileName, const string &newFileName, int factor, int offset = -1)
{
    ifstream readFile(fileName, ios::binary);
    Header header;
    readHeader(readFile, header);

    int size = header.width * header.bitsPerPixel/8 * header.height;

    ofstream writeFile(newFileName, ios::binary);
    writeHeader(writeFile, header);
    unsigned char currentByte;
    for (int i = 0; i < size; i++)
    {
        readFile.read(reinterpret_cast<char *>(&currentByte), sizeof(currentByte));
        if (i % 3 == offset || offset == -1)
        {
            int newValue = static_cast<int>(currentByte) + factor;
            newValue = (newValue > 255) ? 255 : newValue;
            unsigned char newByte = static_cast<unsigned char>(newValue);
            writeFile.write(reinterpret_cast<char *>(&newByte), sizeof(newByte));
        }
        else
        {
            writeFile.write(reinterpret_cast<char *>(&currentByte), sizeof(currentByte));
        }
    }

    readFile.close();
    writeFile.close();
}

void multiply(const string &fileName1, const string &fileName2, const string &newFileName)
{
    ifstream file1(fileName1, ios::binary);
    ifstream file2(fileName2, ios::binary);
    ofstream newFile(newFileName, ios::binary);

    Header header;
    readHeader(file1, header);
    readHeader(file2, header);
    writeHeader(newFile, header);

    int size = header.width * header.bitsPerPixel / 8 * header.height;

    for (int i = 0; i < size; i++)
    {
        unsigned char byte1;
        unsigned char byte2;
        file1.read(reinterpret_cast<char *>(&byte1), sizeof(byte1));
        file2.read(reinterpret_cast<char *>(&byte2), sizeof(byte2));
        float norm1 = static_cast<int>(byte1) / 255.0;
        float norm2 = static_cast<int>(byte2) / 255.0;
        float newValue = norm1 * norm2 * 255 + 0.5f;
        unsigned char newByte = static_cast<unsigned char>(newValue);
        newFile.write(reinterpret_cast<char *>(&newByte), sizeof(newByte));
    }
    file1.close();
    file2.close();
    newFile.close();
}

void screen(const string &fileName1, const string &fileName2, const string &newFileName)
{
    ifstream file1(fileName1, ios::binary);
    ifstream file2(fileName2, ios::binary);
    ofstream newFile(newFileName, ios::binary);

    Header header;
    readHeader(file1, header);
    readHeader(file2, header);
    writeHeader(newFile, header);

    int size = header.width * header.bitsPerPixel / 8 * header.height;

    for (int i = 0; i < size; i++)
    {
        unsigned char byte1;
        unsigned char byte2;
        file1.read(reinterpret_cast<char *>(&byte1), sizeof(byte1));
        file2.read(reinterpret_cast<char *>(&byte2), sizeof(byte2));
        float norm1 = static_cast<int>(byte1) / 255.0;
        float norm2 = static_cast<int>(byte2) / 255.0;
        float newValue = (1 - (1 - norm1) * (1 - norm2)) * 255 + 0.5f;
        unsigned char newByte = static_cast<unsigned char>(newValue);
        newFile.write(reinterpret_cast<char *>(&newByte), sizeof(newByte));
    }
    file1.close();
    file2.close();
    newFile.close();
}

void overlay(const string &fileName1, const string &fileName2, const string &newFileName)
{
    ifstream file1(fileName1, ios::binary);
    ifstream file2(fileName2, ios::binary);
    ofstream newFile(newFileName, ios::binary);

    Header header;
    readHeader(file1, header);
    readHeader(file2, header);
    writeHeader(newFile, header);

    int size = header.width * header.bitsPerPixel / 8 * header.height;

    for (int i = 0; i < size; i++)
    {

            unsigned char byte1;
            unsigned char byte2;
            file1.read(reinterpret_cast<char *>(&byte1), sizeof(byte1));
            file2.read(reinterpret_cast<char *>(&byte2), sizeof(byte2));
            float norm1 = static_cast<int>(byte1) / 255.0;
            float norm2 = static_cast<int>(byte2) / 255.0;

            float newValue;
            if (norm2 <= 0.5)
            {
                newValue = 2 * norm1 * norm2 * 255 + 0.5f;
            }
            else
            {
                newValue = (1 - 2 * (1 - norm1) * (1 - norm2)) * 255 + 0.5f;
            }
            unsigned char newByte = static_cast<unsigned char>(newValue);
            newFile.write(reinterpret_cast<char *>(&newByte), sizeof(newByte));
    }
    file1.close();
    file2.close();
    newFile.close();
}

void subtract(const string &fileName1, const string &fileName2, const string &newFileName)
{

    ifstream file1(fileName1, ios::binary);
    ifstream file2(fileName2, ios::binary);
    ofstream newFile(newFileName, ios::binary);

    Header header;
    readHeader(file1, header);
    readHeader(file2, header);
    writeHeader(newFile, header);

    int size = header.width * header.bitsPerPixel/8 * header.height;

    unsigned char byte1;
    unsigned char byte2;
    unsigned char newByte;
    for (int i = 0; i < size; i++)
    {

        file1.read(reinterpret_cast<char *>(&byte1), sizeof(byte1));
        file2.read(reinterpret_cast<char *>(&byte2), sizeof(byte2));
        int value1 = static_cast<int>(byte1);
        int value2 = static_cast<int>(byte2);
        int newValue = value1 - value2;
        newValue = (newValue < 0) ? 0 : newValue;
        newByte = static_cast<unsigned char>(newValue);
        newFile.write(reinterpret_cast<char *>(&newByte), sizeof(newByte));
    }
    file1.close();
    file2.close();
    newFile.close();
}

void add(const string &fileName1, const string &fileName2, const string &newFileName, const int offset = -1)
{

    ifstream file1(fileName1, ios::binary);
    ifstream file2(fileName2, ios::binary);
    ofstream newFile(newFileName, ios::binary);

    Header header;
    readHeader(file1, header);
    readHeader(file2, header);
    writeHeader(newFile, header);

    int size = header.width * header.bitsPerPixel / 8 * header.height;

    for (int i = 0; i < size; i++)
    {
        unsigned char byte1;
        unsigned char byte2;
        unsigned char newByte;
        if (offset == -1 || i % 3 == offset)
        {
            file1.read(reinterpret_cast<char *>(&byte1), sizeof(byte1));
            file2.read(reinterpret_cast<char *>(&byte2), sizeof(byte2));
            int value1 = static_cast<int>(byte1);
            int value2 = static_cast<int>(byte2);
            int newValue = value1 + value2;
            newValue = (newValue > 255) ? 255 : newValue;
            newByte = static_cast<unsigned char>(newValue);
            newFile.write(reinterpret_cast<char *>(&newByte), sizeof(newByte));
        }
        else
        {
            file1.seekg(sizeof(byte1), ios::cur);
            file2.seekg(sizeof(byte2), ios::cur);
            newFile.seekp(sizeof(newByte), ios::cur);
        }
    }
    file1.close();
    file2.close();
    newFile.close();
}

void rotate(const string &fileName, const string &newFileName)
{
    ifstream readFile(fileName, ios::binary);

    Header header;
    readHeader(readFile, header);

    int size = header.width * header.bitsPerPixel/8 * header.height;
    unsigned char *bytes = new unsigned char[size];

    for (int i = 0; i < size; i++)
    {
        readFile.read(reinterpret_cast<char *>(&bytes[i]), sizeof(bytes[i]));
    }

    readFile.close();

    ofstream writeFile(newFileName, ios::binary);
    writeHeader(writeFile, header);

    for (int i = size - 3; i >= 0; i -= 3)
    {
        for (int j = 0; j < 3; j++)
        {
            writeFile.write(reinterpret_cast<char *>(&bytes[i + j]), sizeof(bytes[i + j]));
        }
    }

    writeFile.close();
    delete[] bytes;
}

void load_channel(const string &fileName, const string &newFileName, int offset)
{
    ifstream readFile(fileName, ios::binary);
    Header header;
    readHeader(readFile, header);

    int size = header.width * header.bitsPerPixel/8 * header.height;

    ofstream writeFile(newFileName, ios::binary);
    writeHeader(writeFile, header);
    unsigned char currentByte;
    readFile.seekg(offset, ios::cur);
    for (int i = offset; i < size; i+=3)
    {
        readFile.read(reinterpret_cast<char *>(&currentByte), sizeof(currentByte));

        for(int j = 0; j < 3; j++) {
            writeFile.write(reinterpret_cast<char *>(&currentByte), sizeof(currentByte));
        }
        readFile.seekg(2, ios::cur);
    }

    readFile.close();
    writeFile.close();
}

void combine_rgb(const string &redFileName, const string &greenFileName, const string &blueFileName, const string &newFileName)
{
    ifstream redFile(redFileName, ios::binary);
    ifstream greenFile(greenFileName, ios::binary);
    ifstream blueFile(blueFileName, ios::binary);
    Header header;
    readHeader(redFile, header);
    readHeader(greenFile, header);
    readHeader(blueFile, header);

    ofstream newFile(newFileName, ios::binary);
    writeHeader(newFile, header);

    int size = header.width * header.bitsPerPixel/8 * header.height;

    unsigned char newByte;
    for (int i = 0; i < size; i++)
    {
        if (i % 3 == 0)
        {
            blueFile.read(reinterpret_cast<char *>(&newByte), sizeof(newByte));
            newFile.write(reinterpret_cast<char *>(&newByte), sizeof(newByte));
            greenFile.seekg(sizeof(newByte), ios::cur);
            redFile.seekg(sizeof(newByte), ios::cur);
        }
        else if (i % 3 == 1)
        {
            greenFile.read(reinterpret_cast<char *>(&newByte), sizeof(newByte));
            newFile.write(reinterpret_cast<char *>(&newByte), sizeof(newByte));
            blueFile.seekg(sizeof(newByte), ios::cur);
            redFile.seekg(sizeof(newByte), ios::cur);
        }
        else if (i % 3 == 2)
        {
            redFile.read(reinterpret_cast<char *>(&newByte), sizeof(newByte));
            newFile.write(reinterpret_cast<char *>(&newByte), sizeof(newByte));
            greenFile.seekg(sizeof(newByte), ios::cur);
            blueFile.seekg(sizeof(newByte), ios::cur);
        }
    }
    redFile.close();
    greenFile.close();
    blueFile.close();
    newFile.close();
}

void print_diff(const string &file_id)
{
    string fileName1 = "examples/EXAMPLE_part" + file_id + ".tga";
    string fileName2 = "output/part" + file_id + ".tga";

    ifstream file1(fileName1, ios::binary);
    ifstream file2(fileName2, ios::binary);

    file1.seekg(0, ios::end);
    int size = static_cast<int>(file1.tellg());
    file1.seekg(0);

    unsigned char byte1;
    unsigned char byte2;
    cout << file_id << '\n';
    for (int i = 0; i < 200; i++)
    {
        file1.read(reinterpret_cast<char *>(&byte1), sizeof(byte1));
        file2.read(reinterpret_cast<char *>(&byte2), sizeof(byte2));

        if (byte1 != byte2)
        {
            cout << int(byte1) << ", " << int(byte2) << ", " << i << '\n';
        }
    }
    cout << "Success!" << '\n';
}

void test() {
    multiply("input/layer1.tga", "input/pattern1.tga", "output/part1.tga");

    subtract("input/car.tga", "input/layer2.tga", "output/part2.tga");

    multiply("input/layer1.tga", "input/pattern2.tga", "output/temp.tga");
    screen("output/temp.tga", "input/text.tga", "output/part3.tga");
    remove("output/temp.tga");

    multiply("input/layer2.tga", "input/circles.tga", "output/temp.tga");
    subtract("output/temp.tga", "input/pattern2.tga", "output/part4.tga");
    remove("output/temp.tga");

    overlay("input/layer1.tga", "input/pattern1.tga", "output/part5.tga");

    add_flat("input/car.tga", "output/part6.tga", 200, 1);

    scale("input/car.tga", "output/temp.tga", 4, 2);
    scale("output/temp.tga", "output/part7.tga", 0, 0);
    remove("output/temp.tga");

    load_channel("input/car.tga", "output/part8_r.tga", 2);
    load_channel("input/car.tga", "output/part8_g.tga", 1);
    load_channel("input/car.tga", "output/part8_b.tga", 0);

    combine_rgb("input/layer_red.tga", "input/layer_green.tga", "input/layer_blue.tga", "output/part9.tga");

    rotate("input/text2.tga", "output/part10.tga");
}

int main()
{
    test();
    return 0;
}