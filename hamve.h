#ifndef HAMVE_H
#define HAMVE_H

#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

// Ham chuyen toa do
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Ham doi mau
void SetColor(int backgound_color, int text_color) {
    HANDLE hDefaultColor = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hDefaultColor, (backgound_color << 4) | text_color);
}

// Ham ve khung
void VeKhung(int x, int y, int width, int height) {
    for (int i = x; i <= x + width; i++) {
        gotoxy(i, y); cout << (char)196; 
        gotoxy(i, y + height); cout << (char)196; 
    }
    for (int i = y; i <= y + height; i++) {
        gotoxy(x, i); cout << (char)179; 
        gotoxy(x + width, i); cout << (char)179; 
    }
    gotoxy(x, y); cout << (char)218;
    gotoxy(x + width, y); cout << (char)191;
    gotoxy(x, y + height); cout << (char)192;
    gotoxy(x + width, y + height); cout << (char)217;
}


// Ham in chuoi gioi han
int InChuoiGioiHan(string s, int x, int y, int maxCols) {
    int curX = x, curY = y;
    int startY = y;

    for (int i = 0; i < s.length(); i++) {
        if (curX >= x + maxCols) {
            curX = x;
            curY++;
        }

        gotoxy(curX, curY);
        cout << s[i];
        curX++;
    }

    return curY - startY + 1; 
}

#endif
