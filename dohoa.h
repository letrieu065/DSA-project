
#ifndef DOHOA_H
#define DOHOA_H

#include <windows.h>
#include <iomanip>
#include "khaibao.h"
#include "hamve.h"  

using namespace std;

void InNoiDungCanLe(string noiDung, int X, int Y, int maxW);
void ShowCur(bool CursorVisibility);


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

    gotoxy(3, 1);
    cout << "MSSV:   ";
    SetColor(0, 14); cout << sv->maSV;
    SetColor(0, 11); gotoxy(3, 2);
    cout << "Ho ten: ";
    SetColor(0, 14); cout << sv->ho << " " << sv->ten;

    // ✅ Thêm placeholder counter bên phải row 1
    SetColor(0, 11); gotoxy(85, 1); cout << "Da lam: 0/" << tongSoCau;

    SetColor(0, 11);
    gotoxy(40, 1); cout << "Mon thi: ";
    SetColor(0, 15); cout << monThi;
    SetColor(0, 11);
    gotoxy(40, 2); cout << "Tong so cau: ";
    SetColor(0, 15); cout << tongSoCau;

    SetColor(0, 11);
    gotoxy(1, 3);
    for (int i = 0; i < 117; i++) cout << (char)196;

    gotoxy(1, 25);
    for (int i = 0; i < 117; i++) cout << (char)196;

    // ✅ Bỏ legend màu, chỉ giữ navigation hints
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

// Hàm 1: vẽ mini map tại row 23
void VeMiniMap(char dapAnDaChon[], int soCau, int cauHienTai) {
    int mapY   = 22;
    int startX = 2;
    int totalW = 116;

    int boxW = totalW / soCau;
    if (boxW < 3) boxW = 3;
    if (boxW > 9) boxW = 9;

    gotoxy(startX, mapY);
    for (int i = 0; i < totalW; i++) cout << " ";

    for (int i = 0; i < soCau; i++) {
        int x = startX + i * boxW;
        if (x + boxW > startX + totalW) break;

        bool answered  = (dapAnDaChon[i] != ' ');
        bool isCurrent = (i == cauHienTai);

        gotoxy(x, mapY);

        if (isCurrent)     SetColor(14, 0);
        else if (answered) SetColor(2,  0);
        else               SetColor(4,  15);

        // ✅ Chỉ hiện số, không có ký tự thêm
        string label = to_string(i + 1);
        int pad  = (boxW - (int)label.size()) / 2;
        int rpad = boxW - pad - (int)label.size();
        for (int p = 0; p < pad;  p++) cout << " ";
        cout << label;
        for (int p = 0; p < rpad; p++) cout << " ";

        SetColor(0, 7);
    }
}

// Hàm 2: cập nhật counter "Đã làm X/Y" trên header
void CapNhatCounter(char dapAnDaChon[], int soCau) {
    int daDone = 0;
    for (int i = 0; i < soCau; i++)
        if (dapAnDaChon[i] != ' ') daDone++; // ✅ ' ' không phải '\0'

    gotoxy(85, 1);
    SetColor(0, 11); cout << "Da lam: ";
    SetColor(0, daDone == soCau ? 10 : 14);
    cout << daDone << "/" << soCau << "   ";
    SetColor(0, 7);
}

void HienThiKetQuaThi(int soCauDung, int soCau, float diemThi) {
    system("cls"); // Hoặc XoaVungContent() tùy vào việc bạn muốn xóa sạch cả màn hình hay chỉ vùng nội dung
    ShowCur(false);

    // Kích thước khung thông báo
    int kWidth = 46;
    int kHeight = 10;
    
    // Tự động canh giữa dựa trên SetupConsoleLayout (120x30)
    int sX = (120 - kWidth) / 2;
    int sY = (30 - kHeight) / 2;

    // 1. Vẽ viền khung
    SetColor(0, 10); // Viền xanh lá cây (Success)
    VeKhung(sX, sY, kWidth, kHeight);
    
    // 2. Tiêu đề
    SetColor(0, 10);
    gotoxy(sX + 12, sY + 2); cout << "DA NOP BAI THANH CONG!";
    
    // Đường gạch ngang phân cách (Tùy chọn cho đẹp)
    gotoxy(sX + 2, sY + 3); 
    for(int i = 0; i < kWidth - 3; i++) cout << (char)196;

    // 3. Nội dung điểm số
    SetColor(0, 14); // Vàng
    gotoxy(sX + 12, sY + 5); cout << "So cau dung : " << soCauDung << " / " << soCau;

    SetColor(0, 11); // Xanh dương nhạt nổi bật
    gotoxy(sX + 12, sY + 7); cout << "DIEM CUA BAN: " << fixed << setprecision(2) << diemThi;

    // 4. Hướng dẫn thoát
    SetColor(0, 8); // Xám
    gotoxy(sX + 6, sY + kHeight - 1); cout << "Nhan [ENTER] hoac [ESC] de quay ve";

    // Trả lại màu mặc định
    SetColor(0, 7);

    // Vòng lặp chờ thoát
    while (true) {
        char ch = _getch();
        if (ch == 13 || ch == 27) { // 13: ENTER, 27: ESC
            break;
        }
    }
}

#endif