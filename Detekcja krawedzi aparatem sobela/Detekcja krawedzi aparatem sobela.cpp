#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cmath>
#include<algorithm>
#include<vector>

using namespace std;

short w1[3][3] = {
    {1,2,1},
    {0,0,0},
    {-1,-2,-1}
};

short w2[3][3] = {
    {2,1,0},
    {1,0,-1},
    {0,-1,-2}
};

short w3[3][3] = {
    {1,0,-1},
    {2,0,-2},
    {1,0,-1}
};

short w4[3][3] = {
    {0,-1,-2},
    {1, 0, -1},
    {2,1,0}
};

struct FileHeader {  //informacje o pliku
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
};

struct PictureHeader { //informacje o obrazku
    unsigned int biSize;
    unsigned int biWidth;
    unsigned int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    unsigned int biXPelsPerMeter;
    unsigned int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
};

struct KoloryRGB {
    short R;
    short G;
    short B;
};


struct BMP {
    FileHeader info;
    PictureHeader obraz;
    KoloryRGB** kolor;
    KoloryRGB** kr;
};

int odczytajFH(fstream& plik, BMP& bitmapa) {
    plik.read(reinterpret_cast<char*>(&bitmapa.info.bfType), 2);
    plik.read(reinterpret_cast<char*>(&bitmapa.info.bfSize), 4);
    plik.read(reinterpret_cast<char*>(&bitmapa.info.bfReserved1), 2);
    plik.read(reinterpret_cast<char*>(&bitmapa.info.bfReserved2), 2);
    plik.read(reinterpret_cast<char*>(&bitmapa.info.bfOffBits), 4);
    return plik.tellg();
}

int odczytajPH(fstream& plik, BMP& bitmapa) {
    plik.read(reinterpret_cast<char*>(&bitmapa.obraz.biSize), 4);
    plik.read(reinterpret_cast<char*>(&bitmapa.obraz.biWidth), 4);
    plik.read(reinterpret_cast<char*>(&bitmapa.obraz.biHeight), 4);
    plik.read(reinterpret_cast<char*>(&bitmapa.obraz.biPlanes), 2);
    plik.read(reinterpret_cast<char*>(&bitmapa.obraz.biBitCount), 2);
    plik.read(reinterpret_cast<char*>(&bitmapa.obraz.biCompression), 4);
    plik.read(reinterpret_cast<char*>(&bitmapa.obraz.biSizeImage), 4);
    plik.read(reinterpret_cast<char*>(&bitmapa.obraz.biXPelsPerMeter), 4);
    plik.read(reinterpret_cast<char*>(&bitmapa.obraz.biYPelsPerMeter), 4);
    plik.read(reinterpret_cast<char*>(&bitmapa.obraz.biClrUsed), 4);
    plik.read(reinterpret_cast<char*>(&bitmapa.obraz.biClrImportant), 4);
    return plik.tellg();
}

int zapiszFH(fstream& kopia, BMP& bitmapa) {
    kopia.write(reinterpret_cast<char*>(&bitmapa.info.bfType), 2);
    kopia.write(reinterpret_cast<char*>(&bitmapa.info.bfSize), 4);
    kopia.write(reinterpret_cast<char*>(&bitmapa.info.bfReserved1), 2);
    kopia.write(reinterpret_cast<char*>(&bitmapa.info.bfReserved2), 2);
    kopia.write(reinterpret_cast<char*>(&bitmapa.info.bfOffBits), 4);
    return kopia.tellg();
}

int zapiszPH(fstream& kopia, BMP& bitmapa) {
    kopia.write(reinterpret_cast<char*>(&bitmapa.obraz.biSize), 4);
    kopia.write(reinterpret_cast<char*>(&bitmapa.obraz.biWidth), 4);
    kopia.write(reinterpret_cast<char*>(&bitmapa.obraz.biHeight), 4);
    kopia.write(reinterpret_cast<char*>(&bitmapa.obraz.biPlanes), 2);
    kopia.write(reinterpret_cast<char*>(&bitmapa.obraz.biBitCount), 2);
    kopia.write(reinterpret_cast<char*>(&bitmapa.obraz.biCompression), 4);
    kopia.write(reinterpret_cast<char*>(&bitmapa.obraz.biSizeImage), 4);
    kopia.write(reinterpret_cast<char*>(&bitmapa.obraz.biXPelsPerMeter), 4);
    kopia.write(reinterpret_cast<char*>(&bitmapa.obraz.biYPelsPerMeter), 4);
    kopia.write(reinterpret_cast<char*>(&bitmapa.obraz.biClrUsed), 4);
    kopia.write(reinterpret_cast<char*>(&bitmapa.obraz.biClrImportant), 4);
    return kopia.tellg();
}

void odczytajbitmape(fstream& plik, BMP& bitmapa, int padding) {
    plik.seekg(bitmapa.info.bfOffBits, ios::beg);
    for (int i = 0; i < bitmapa.obraz.biHeight; i++)
    {

        for (int j = 0; j < bitmapa.obraz.biWidth; j++)
        {
            int pixel = 0;
            plik.read(reinterpret_cast<char*>(&pixel), 1);
            bitmapa.kolor[j][i].B = (char)pixel;

            plik.read(reinterpret_cast<char*>(&pixel), 1);
            bitmapa.kolor[j][i].G = pixel;

            plik.read(reinterpret_cast<char*>(&pixel), 1);
            bitmapa.kolor[j][i].R = pixel;
        }
        plik.seekg(padding, ios::cur);
    }

}

void krawedz(BMP& bitmapa, int padding) { //stworzenie czarnego obrazu o 2 wiekszego (krawedzie)
    for (int i = 0; i < bitmapa.obraz.biWidth + 2; i++)
    {

        for (int j = 0; j < bitmapa.obraz.biHeight + 2; j++)
        {
            bitmapa.kr[i][j].R = 0;
            bitmapa.kr[i][j].G = 0;
            bitmapa.kr[i][j].B = 0;

        }
    }
    for (int i = 1; i < bitmapa.obraz.biWidth + 1; i++)
    {
        for (int j = 1; j < bitmapa.obraz.biHeight + 1; j++)
        {
            bitmapa.kr[i][j].R = bitmapa.kolor[i - 1][j - 1].R;
            bitmapa.kr[i][j].G = bitmapa.kolor[i - 1][j - 1].G;
            bitmapa.kr[i][j].B = bitmapa.kolor[i - 1][j - 1].B;
        }
    }
}

void sobel(BMP& bitmapa) {
    KoloryRGB** nowekolory = new KoloryRGB * [bitmapa.obraz.biWidth];
    for (int i = 0; i < bitmapa.obraz.biWidth; i++)
        nowekolory[i] = new KoloryRGB[bitmapa.obraz.biHeight];

    for (int i = 0; i < bitmapa.obraz.biHeight; i++) {
        for (int j = 0; j < bitmapa.obraz.biWidth; j++) {
            int sumasoblar = 0;
            int sumasoblag = 0;
            int sumasoblab = 0;

            for (int a = -1; a <= 1; a++) {
                for (int b = -1; b <= 1; b++) {
                    int indeksx1 = j + a + 1;
                    int indeksy1 = i + b + 1;

                    int indeksx2 = a + 1;
                    int indeksy2 = b + 1;

                    sumasoblar += bitmapa.kr[indeksx1][indeksy1].R * w1[indeksx2][indeksy2];
                    sumasoblag += bitmapa.kr[indeksx1][indeksy1].G * w1[indeksx2][indeksy2];
                    sumasoblab += bitmapa.kr[indeksx1][indeksy1].B * w1[indeksx2][indeksy2];

                    sumasoblar += bitmapa.kr[indeksx1][indeksy1].R * w2[indeksx2][indeksy2];
                    sumasoblag += bitmapa.kr[indeksx1][indeksy1].G * w2[indeksx2][indeksy2];
                    sumasoblab += bitmapa.kr[indeksx1][indeksy1].B * w2[indeksx2][indeksy2];

                    sumasoblar += bitmapa.kr[indeksx1][indeksy1].R * w3[indeksx2][indeksy2];
                    sumasoblag += bitmapa.kr[indeksx1][indeksy1].G * w3[indeksx2][indeksy2];
                    sumasoblab += bitmapa.kr[indeksx1][indeksy1].B * w3[indeksx2][indeksy2];

                    sumasoblar += bitmapa.kr[indeksx1][indeksy1].R * w4[indeksx2][indeksy2];
                    sumasoblag += bitmapa.kr[indeksx1][indeksy1].G * w4[indeksx2][indeksy2];
                    sumasoblab += bitmapa.kr[indeksx1][indeksy1].B * w4[indeksx2][indeksy2];
                }
            }

            sumasoblar /= 8 * 4;
            sumasoblag /= 8 * 4;
            sumasoblab /= 8 * 4;

            sumasoblar = abs(sumasoblar);
            sumasoblag = abs(sumasoblag);
            sumasoblab = abs(sumasoblab);

            if (sumasoblar > 255)
                sumasoblar = 255;
            if (sumasoblag > 255)
                sumasoblag = 255;
            if (sumasoblab > 255)
                sumasoblab = 255;

            nowekolory[j][i].R = sumasoblar;
            nowekolory[j][i].G = sumasoblag;
            nowekolory[j][i].B = sumasoblab;
        }
    }

    for (int i = 0; i < bitmapa.obraz.biWidth; i++) {
        for (int j = 0; j < bitmapa.obraz.biHeight; j++) {
            bitmapa.kolor[i][j].R = nowekolory[i][j].R;
            bitmapa.kolor[i][j].G = nowekolory[i][j].G;
            bitmapa.kolor[i][j].B = nowekolory[i][j].B;
        }
    }
}

void zapiszbitmape(fstream& kopia, BMP& bitmapa, int padding) {
    kopia.seekg(bitmapa.info.bfOffBits, ios::beg);
    for (int i = 0; i < bitmapa.obraz.biHeight; i++)
    {

        for (int j = 0; j < bitmapa.obraz.biWidth; j++)
        {
            int pixel = bitmapa.kolor[j][i].B;
            kopia.write(reinterpret_cast<char*>(&pixel), 1);

            pixel = bitmapa.kolor[j][i].G;
            kopia.write(reinterpret_cast<char*>(&pixel), 1);

            pixel = bitmapa.kolor[j][i].R;
            kopia.write(reinterpret_cast<char*>(&pixel), 1);

        }
        kopia.seekg(padding, ios::cur);
    }

    int test = 0;
    kopia.write(reinterpret_cast<char*>(&test), 1);
}



void wypiszFH(fstream& plik, BMP& bitmapa) {
    cout << "File Header: " << endl;
    cout << dec << "Type : " << bitmapa.info.bfType << endl;
    cout << dec << "Size: " << bitmapa.info.bfSize << endl;
    cout << dec << "Reserved1: " << bitmapa.info.bfReserved1 << endl;
    cout << dec << "Reserved2: " << bitmapa.info.bfReserved2 << endl;
    cout << dec << "OffBits: " << bitmapa.info.bfOffBits << endl;

}

void wypiszPH(fstream& plik, BMP& bitmapa) {
    cout << "Picture Header: " << endl;
    cout << dec << "Size : " << bitmapa.obraz.biSize << endl;
    cout << dec << "Width : " << bitmapa.obraz.biWidth << endl;
    cout << dec << "Height: " << bitmapa.obraz.biHeight << endl;
    cout << dec << "Planes " << bitmapa.obraz.biPlanes << endl;
    cout << dec << "BitCount " << bitmapa.obraz.biBitCount << endl;
    cout << dec << "Compression: " << bitmapa.obraz.biCompression << endl;
    cout << dec << "Size: " << bitmapa.obraz.biSizeImage << endl;
    cout << dec << "XPelsPerMater: " << bitmapa.obraz.biXPelsPerMeter << endl;
    cout << dec << "YPelsPerMater: " << bitmapa.obraz.biYPelsPerMeter << endl;
    cout << dec << "ClrUsed: " << bitmapa.obraz.biClrUsed << endl;
    cout << dec << "ClrImportant: " << bitmapa.obraz.biClrImportant << endl;
    cout << endl << endl;
}
;

int main() {
    string nazwapliku;
    cout << "Podaj nazwe pliku: ";
    cin >> nazwapliku;
    BMP bitmapa;

    fstream plik;
    plik.open(nazwapliku, ios::in | ios::binary);


    if (!plik) { //sprawdzenie, czy plik zostal otwarty
        cout << "Plik nie zostal otwarty\n";
    }
    cout << "Plik wczytany" << endl;

    odczytajFH(plik, bitmapa);
    odczytajPH(plik, bitmapa);
    wypiszPH(plik, bitmapa);
    wypiszFH(plik, bitmapa);
    int padding = bitmapa.obraz.biWidth % 4;

    bitmapa.kolor = new KoloryRGB * [bitmapa.obraz.biWidth];
    for (int i = 0; i < bitmapa.obraz.biWidth; i++)
        bitmapa.kolor[i] = new KoloryRGB[bitmapa.obraz.biHeight];
    odczytajbitmape(plik, bitmapa, padding);

    bitmapa.kr = new KoloryRGB * [bitmapa.obraz.biWidth + 2];
    for (int i = 0; i < bitmapa.obraz.biWidth + 2; i++)
        bitmapa.kr[i] = new KoloryRGB[bitmapa.obraz.biHeight + 2];

    krawedz(bitmapa, padding);
    sobel(bitmapa);

    plik.close();

    fstream kopia;
    string nazwakopii;
    cout << "Jak chcesz nazwac kopie? : ";
    cin >> nazwakopii;
    kopia.open(nazwakopii, ios::out | ios::binary);
    zapiszFH(kopia, bitmapa);
    zapiszPH(kopia, bitmapa);
    zapiszbitmape(kopia, bitmapa, padding);
    kopia.close();
}
