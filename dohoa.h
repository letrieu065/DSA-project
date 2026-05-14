
#ifndef DOHOA_H
#define DOHOA_H

#include <windows.h>
#include "hamchinh.h"

using namespace std;

void InNoiDungCanLe(string noiDung, int X, int Y, int maxW);

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
void VeLayoutChinh(string tenNguoiDung, string maso = "") {
    system("cls");
    
    // 1. Khung Tren-Trai
    SetColor(0, 11);
    VeKhung(2, 1, 28, 3);
    gotoxy(4, 2); cout << "User: " << tenNguoiDung;
    if (!maso.empty()) {
        gotoxy(4, 3); cout << "ID  : " << maso;
    }
    // 2. Khung Duoi-Trai
    SetColor(0, 14); 
    VeKhung(2, 5, 28, 23);
    gotoxy(13, 6); cout << "DANH MUC";
    gotoxy(3, 7); cout << "---------------------------";

    // 3. Khung Tren-Phai
    SetColor(0, 11);
    VeKhung(32, 1, 85, 3);
    
    // 4. Khung Duoi-Phai
    SetColor(0, 7);
    VeKhung(32, 5, 85, 23);
}

// UI thi trac nghiem
void VeKhungPhongThi(SinhVien* sv, string monThi, int tongSoCau) {
    system("cls");
    SetColor(0, 11); 
    VeKhung(0, 0, 118, 28); 

    gotoxy(3, 1); cout << "MSSV: ";
    SetColor(0, 14); cout << sv->maSV; 
    
    SetColor(0, 11); gotoxy(30, 1); cout << "Ho ten: ";
    SetColor(0, 14); cout << sv->ho << " " << sv->ten;

    SetColor(0, 11);
    gotoxy(3, 2); cout << "Mon thi: ";
    SetColor(0, 15); cout << monThi; 

    SetColor(0, 11);
    gotoxy(60, 2); cout << "Tong so cau: ";
    SetColor(0, 15); cout << tongSoCau;

    SetColor(0, 11);
    gotoxy(1, 3); 
    for(int i=0; i<116; i++) cout << (char)196; 

    gotoxy(2, 25); 
    for(int i=0; i<116; i++) cout << (char)196; 
    
    gotoxy(4, 26); 
    SetColor(0, 8);
    cout << "Dieu huong: [<-] Cau truoc | [->] Cau tiep | [A/B/C/D] Chon | [ESC] Nop bai";
    SetColor(0, 7);
}

void InDongHo(int soGiay) {
    int phut = soGiay / 60;
    int giay = soGiay % 60;
    gotoxy(85, 2); SetColor(0, 11); cout << "Thoi gian con lai: ";
    SetColor(0, 12); // Màu đỏ cho nổi bật
    printf("%02d:%02d", phut, giay);
    SetColor(0, 7);
}

void HienThiCauHoiThi(CauHoi* cau, int cauSo, int tongSoCau, char dapAnDaChon) {
    // Xóa ruột khung (Y từ 4 đến 23) để in câu mới
    for (int i = 4; i <= 23; i++) {
        gotoxy(2, i); cout << "                                                                                                                  ";
    }

    // In Nội dung câu hỏi
    gotoxy(5, 5); SetColor(0, 14); cout << "Cau " << cauSo << " / " << tongSoCau << ": "; SetColor(0, 15);
    InNoiDungCanLe(cau->noiDung, 5, 6, 110); 

    // In Đáp án A, B, C, D
    if (dapAnDaChon == 'A') SetColor(11, 0); else SetColor(0, 15);
    gotoxy(5, 12); cout << "[ A ]"; SetColor(0, 7); InNoiDungCanLe(cau->a, 12, 12, 45);

    if (dapAnDaChon == 'B') SetColor(11, 0); else SetColor(0, 15);
    gotoxy(5, 18); cout << "[ B ]"; SetColor(0, 7); InNoiDungCanLe(cau->b, 12, 18, 45);

    if (dapAnDaChon == 'C') SetColor(11, 0); else SetColor(0, 15);
    gotoxy(60, 12); cout << "[ C ]"; SetColor(0, 7); InNoiDungCanLe(cau->c, 67, 12, 45);

    if (dapAnDaChon == 'D') SetColor(11, 0); else SetColor(0, 15);
    gotoxy(60, 18); cout << "[ D ]"; SetColor(0, 7); InNoiDungCanLe(cau->d, 67, 18, 45);

    // In thanh Nút bấm trạng thái
    gotoxy(35, 23); SetColor(0, 11); cout << "DAP AN DA CHON: ";
    char dsDapAn[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++) {
        gotoxy(53 + i * 12, 23);
        if (dapAnDaChon == dsDapAn[i]) {
            SetColor(12, 14); cout << " ( " << dsDapAn[i] << " ) "; // Nổi bần bật khi chọn
        } else {
            SetColor(0, 8); cout << " [ " << dsDapAn[i] << " ] ";   // Chìm đi khi chưa chọn
        }
    }
    SetColor(0, 7);
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