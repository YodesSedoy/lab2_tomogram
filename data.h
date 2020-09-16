#ifndef DATA_H
#define DATA_H
#include <iostream>
#include <qstring.h>
#include <fstream>
#include <QDebug>

class Data {
private:
    float scaleX, scaleY, scaleZ;
    short* arr;
    int width, height, depth;
    short min, max;
public:
    Data() : width(0), height(0), depth(0), min(0), max(0), scaleX(0), scaleY(0), scaleZ(0) {
        arr = NULL;
    }
    ~Data() {
        delete arr;
    }
    short getMin() { return min; }
    short getMax() { return max; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    int getDepth() { return depth; }
    int readFile(QString fileName) {
        setlocale(0, "RUS");
        std::ifstream file;
        file.open(fileName.toStdString(), std::ios::binary | std::ios::in);
        if (!file.is_open()) {
            qDebug() << "File can't be open! \n";
            return 1;
        }
        int x, y, z;
        qDebug() << QString("Open");
        file.read((char*)&x, sizeof(int));
        file.read((char*)&y, sizeof(int));
        file.read((char*)&z, sizeof(int));
        if (x == 0 | y == 0 | z == 0) {
            qDebug() << QString("Wrong Data Size");
            file.close();
            return 2;
        }
        qDebug() << "width = " << x << ", height = " << y << ", depth = " << z;
        file.read((char*)&scaleX, sizeof(float));
        file.read((char*)&scaleY, sizeof(float));
        file.read((char*)&scaleZ, sizeof(float));
        qDebug() << "scaleX = " << scaleX << ", scaleY = " << scaleY << ", scaleZ = " << scaleZ;
        if (arr != NULL) { delete[] arr; }
        width = x;
        height = y;
        depth = z;
        long sizeArr = width * height * depth;
        arr = new short[sizeArr];
        file.read((char*)arr, sizeArr * sizeof(short));
        file.close();
        qDebug() << QString("Close");
        MinMax();
        return 0;
    }

    short& operator[] (const int index) {
        assert(index >= 0 && index < width * height * depth);
        return arr[index];
    }

    void MinMax() {
        min = arr[0];
        max = arr[0];
        for (int i = 0; i < depth * height*width; i++) {
            if (min > arr[i]) { min = arr[i]; }
            if (max < arr[i]) { max = arr[i]; }
        }
        qDebug() << "Min" << min << "Max" << max;
    }
};
#endif // DATA_H
