#ifndef DOHOA_H
#define DOHOA_H

#include <windows.h>

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

// Ham set font chu
void SetFontSize(int size) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;                   
    cfi.dwFontSize.Y = size;                
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;             
    wcscpy(cfi.FaceName, L"Consolas");      
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}

// Ham set console
void SetupConsoleLayout() {
    system("mode con cols=120 lines=30"); 
}

// Ham ve khung
void VeLayoutChinh(string tenNguoiDung) {
    system("cls");
    
    // 1. Khung Tren-Trai
    SetColor(0, 11);
    VeKhung(2, 1, 28, 3);
    gotoxy(4, 2); cout << "User: " << tenNguoiDung;
    
    // 2. Khung Duoi-Trai
    SetColor(0, 14); // Màu vàng
    VeKhung(2, 5, 28, 23);
    gotoxy(10, 6); cout << "DANH MUC";
    gotoxy(3, 7); cout << "--------------------------";

    // 3. Khung Tren-Phai
    SetColor(0, 11);
    VeKhung(32, 1, 85, 3);
    
    // 4. Khung Duoi-Phai
    SetColor(0, 7);
    VeKhung(32, 5, 85, 23);
}
#endif