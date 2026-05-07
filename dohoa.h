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
    SetColor(0, 14); // Mau vang
    VeKhung(2, 5, 28, 23);
    gotoxy(13, 6); cout << "DANH MUC";
    gotoxy(3, 7); cout << "---------------------------";

    // 3. Khung Tren-Phai
    SetColor(0, 11);
    VeKhung(32, 1, 85, 3);
    
    // 4. Khung Duoi-Phai//
    SetColor(0, 7);
    VeKhung(32, 5, 85, 23);
}

void VeMenuChinh() {
    system("cls");
    SetColor(0, 11); 
    VeKhung(0, 0, 118, 28);
    
    SetColor(0, 14); 
    gotoxy(45, 3); cout << "HE THONG THI TRAC NGHIEM";
    gotoxy(2, 5); for(int i=0; i<116; i++) cout << (char)196;

    SetColor(0, 15); 
    gotoxy(40, 10); cout << "[ 1 ] - Bat dau thi trac nghiem";
    gotoxy(40, 12); cout << "[ 2 ] - Xem bang diem ket qua";
    gotoxy(40, 14); cout << "[ 0 ] - Thoat chuong trinh";

    SetColor(0, 8); 
    gotoxy(2, 26); for(int i=0; i<116; i++) cout << (char)196;
    gotoxy(4, 27); cout << "Huong dan: Nhap so tuong ung (0-2) va nhan Enter de chon.";
}

void VeManHinhThi(string monThi, string thoiGian, int cauHienTai, int tongCau) {
    system("cls");
    SetColor(0, 11);
    VeKhung(0, 0, 118, 28);

    gotoxy(3, 1); cout << "Mon thi: " << monThi;
    gotoxy(90, 1); cout << "Thoi gian con lai: " << thoiGian;
    gotoxy(2, 2); for(int i=0; i<116; i++) cout << (char)196;

    SetColor(0, 15);
    gotoxy(5, 5); cout << "Cau hoi " << cauHienTai << "/" << tongCau << ":";
    
    gotoxy(10, 12); cout << "[ A ] Dap an mau A";
    gotoxy(65, 12); cout << "[ C ] Dap an mau C";
    gotoxy(10, 15); cout << "[ B ] Dap an mau B";
    gotoxy(65, 15); cout << "[ D ] Dap an mau D";

    SetColor(0, 11);
    gotoxy(2, 25); for(int i=0; i<116; i++) cout << (char)196;
    gotoxy(4, 26); cout << "Dieu huong: [<-] Cau truoc | [->] Cau tiep | [A/B/C/D] Chon | [ESC] Nop bai";
}

void VeGiaoDienDoiMatKhau() {
	system("cls");
    int kWidth = 60;
    int kHeight = 12;
    int sX = (120 - kWidth) / 2;
    int sY = (30 - kHeight) / 2;

    SetColor(0, 12); 
    VeKhung(sX, sY, kWidth, kHeight);
    
    SetColor(0, 15);
    gotoxy(sX + 15, sY + 1); cout << "DOI MAT KHAU LAN DAU DANG NHAP";
    gotoxy(sX + 2, sY + 2); for(int i=0; i < kWidth - 3; i++) cout << (char)196;

    gotoxy(sX + 5, sY + 4); cout << "Mat khau hien tai: ";
    gotoxy(sX + 5, sY + 6); cout << "Mat khau moi     : ";
    gotoxy(sX + 5, sY + 8); cout << "Xac nhan moi     : ";
    
    SetColor(0, 14);
    gotoxy(sX + 2, sY + 10); cout << "(*) Mat khau moi phai khac mat khau mac dinh '123456'";
}

#endif