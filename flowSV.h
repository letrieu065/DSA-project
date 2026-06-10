#ifndef FLOWSV_H
#define FLOWSV_H

#include "khaibao.h"
#include "saveload.h"
#include "hamchinh.h"
#include "dohoa.h"
#include "ErrorCodes.h"
#include "hamtienich.h"

void flowSinhVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root, SinhVien* sv);
void flowXemThongTinSinhVien(SinhVien* sv);
void flowXemDiemSinhVien(SinhVien* sv, DanhSachMonHoc &dsMH);
void flowThiTracNghiem(SinhVien* sv, DanhSachMonHoc &dsMH, PTR_CAUHOI &root, DanhSachLop &dsLop);
void flowXemLaiBaiLam(const char* maSV, const char* maMH);


void flowSinhVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root, SinhVien* sv) {
    char menuChinh[2][50] = {"1. Thi trac nghiem", "2. Dang xuat"};
    string info = string(sv->ho) + " " + string(sv->ten);

	while (true) {
        SetupConsoleLayout();
        VeLayoutChinh((char*)info.c_str(), (char*)sv->maSV); 
        
        int chon = MenuDong(menuChinh, 2, 4, 9); 
        
        if (chon == 0) flowThiTracNghiem(sv, dsMH, root, dsLop);
        else break; 
    }
}

void flowThiTracNghiem(SinhVien* sv, DanhSachMonHoc &dsMH, PTR_CAUHOI &root, DanhSachLop &dsLop) {
    int cursor = 0;
    int itemsPerPage = 8;
    bool isSelected = false;

    XoaVungContent();
    gotoxy(60, 2); SetColor(0, 11); cout << "SETUP CAU HINH THI TRAC NGHIEM"; SetColor(0, 7);
    gotoxy(40, 12); cout << "Thoi gian thi (phut): "; 
    gotoxy(40, 14); cout << "So cau hoi: ";  
    gotoxy(40, 10); cout << "Chon mon hoc: "; 
    SetColor(11, 0); 
    cout << " [ CHON ] "; 
    SetColor(0, 7); 
    gotoxy(57, 10);
    ShowCur(false);
    while (true)
    {
        char ch = _getch();
        if (ch == 13) break;
        else if (ch == 27) return;
        else continue;
    }
    
    chon_lai:
    while (true) {
        ShowCur(false);
        int totalItems = dsMH.soLuong;
        int totalPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;
        int currentPage = (totalItems == 0) ? 0 : (cursor / itemsPerPage);
        XoaVungContent();
        gotoxy(61, 6); SetColor(0, 11); cout << "--- CHON MON HOC ---";
        gotoxy(105, 27);  SetColor(0, 10); cout << "(Trang " << currentPage + 1 << "/" << totalPages << ")";

        gotoxy(35, 8); SetColor(0, 14);
        printf("%-5s %-15s %-35s %-5s", "STT", "Ma Mon", "Ten Mon Hoc", "STC");
        SetColor(0, 7);

        int startIdx = currentPage * itemsPerPage;
        int endIdx = (startIdx + itemsPerPage > totalItems) ? totalItems : (startIdx + itemsPerPage);

        if (totalItems == 0) {
            SetColor(0, 12);
            gotoxy(55, 12); cout << "Danh sach mon hoc dang trong!";
            SetColor(0, 7);
        } else {
            for (int i = startIdx; i < endIdx; i++) {
                int toadoY = 10 + (i - startIdx) * 2;
                if (i == cursor) SetColor(11, 0);
                else SetColor(0, 7);
                gotoxy(35, toadoY);
                printf("%-5d %-15s %-35s %-5d", i + 1, dsMH.data[i].maMH, dsMH.data[i].tenMH, dsMH.data[i].soTinChi);
            }
        }
        gotoxy(35, 26); SetColor(0, 8);
        cout << "[ENTER] Chon mon thi | Left/Right: Chuyen trang | [ESC] Thoat";
        int ch = _getch();
        if (ch == 224 || ch == 0 || ch == -32) {
            ch = _getch();
            if (ch == 72 && cursor > 0) cursor--;
            else if (ch == 80 && cursor < totalItems - 1) cursor++;
            else if (ch == 75 && currentPage > 0) cursor = (currentPage - 1) * itemsPerPage;
            else if (ch == 77 && currentPage < totalPages - 1) cursor = (currentPage + 1) * itemsPerPage;
        }
        else if (ch == 13 && totalItems > 0) {
            isSelected = true;
            break;
        }
        else if (ch == 27) return;
    }

    if (isSelected) {
        MonHoc monChon = dsMH.data[cursor];
        CauHoi* khoCauHoi[2000] = {NULL};
        int tongCauHoiKho = 0;
        LayDanhSachCauHoiTheoMon(root, monChon.maMH, khoCauHoi, tongCauHoiKho);

        if (tongCauHoiKho == 0) {
            XoaVungContent();
            InThongBaoContent("Mon hoc nay chua co cau hoi!", 12, 45, 15);
            _getch();
            isSelected = false; 
            goto chon_lai;
        }

        XoaVungContent();
        gotoxy(60, 2); SetColor(0, 11); cout << "SETUP CAU HINH THI TRAC NGHIEM"; SetColor(0, 7);
        gotoxy(40, 10); cout << "Mon hoc: ";
        SetColor(0, 14); cout << monChon.tenMH; SetColor(0, 7);
        cout << " (Ngan hang co: " << tongCauHoiKho << " cau)";
        ShowCur(true);
        int testTime = 0, questionCount = 0;
        gotoxy(40, 12); cout << "Thoi gian thi (phut): "; 
        gotoxy(40, 14); cout << "So cau hoi          :     ";

        while (true) {
            gotoxy(63, 12); testTime = NhapSo();
            if (testTime == INPUT_CANCELLED) { ShowCur(false); return; }
            if (testTime > 0) break;
            InThongBaoContent("Thoi gian phai > 0!", 12, 70, 12);
            Sleep(800); gotoxy(70, 12); cout << "                   ";   
        }

        while (true) {
                gotoxy(63, 14); questionCount = NhapSo();
                if (questionCount == INPUT_CANCELLED) { ShowCur(false); return; }
                if (questionCount > 0 && questionCount <= tongCauHoiKho) break;
                
                InThongBaoContent("So cau vuot qua ngan hang!", 12, 70, 14);
                _getch(); gotoxy(63, 14); cout << "                                 ";
        }
        srand(time(0)); 
        for (int i = 0; i < tongCauHoiKho; i++) {
            int r = i + rand() % (tongCauHoiKho - i); 
            swap(khoCauHoi[i], khoCauHoi[r]); 
        }
        CauHoi* deThi[1000] = {NULL};
        for (int i = 0; i < questionCount; i++) {
            deThi[i] = khoCauHoi[i];
        }
        InThongBaoContent("Ngan hang de da san sang! An phim bat ky de vao thi...", 10, 35, 20);
        _getch();
        TienHanhThiTracNghiem(sv, monChon, deThi, questionCount, testTime);

    }
    
}

#endif