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
    XoaVungContent();
    InTitle("SETUP CAU HINH THI TRAC NGHIEM");
    gotoxy(40, 10); cout << "Chon mon hoc: "; 
    SetColor(11, 0); cout << " [ CHON ] "; SetColor(0, 7); 
    gotoxy(40, 12); cout << "Thoi gian thi (phut): "; 
    gotoxy(40, 14); cout << "So cau hoi: ";  
    
    gotoxy(35, 27); SetColor(0, 8); cout << "ENTER: Bat dau chon mon | ESC: Thoat"; SetColor(0, 7);
    ShowCur(false);
    while (true) {
        char ch = _getch();
        if (ch == 13) break;
        else if (ch == 27) return;
    }
    
    int cursor = 0;
    int itemsPerPage = 8;

    // Vòng lặp tổng bọc bên ngoài (Thay thế cho lệnh goto)
    while (true) {
        int oldCursor = -1;
        int oldPage = -1;
        int forceRedraw = 1;
        bool isSelected = false;

        // VÒNG LẶP 1: CHỌN MÔN HỌC (PARTIAL REDRAW)
        while (true) {
            ShowCur(false);
            int totalItems = dsMH.soLuong;
            int totalPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;
            int currentPage = (totalItems == 0) ? 0 : (cursor / itemsPerPage);
            
            if (forceRedraw || currentPage != oldPage) {
                XoaVungContent();
                InTitle("CHON MON HOC DE THI");
                gotoxy(105, 27);  SetColor(0, 10); cout << "(Trang " << currentPage + 1 << "/" << totalPages << ")";

                gotoxy(35, 8); SetColor(0, 14);
                printf("%-5s %-15s %-35s %-5s", "STT", "Ma Mon", "Ten Mon Hoc", "STC");
                SetColor(0, 7);

                int startIdx = currentPage * itemsPerPage;
                int endIdx = (startIdx + itemsPerPage > totalItems) ? totalItems : (startIdx + itemsPerPage);

                if (totalItems == 0) {
                    SetColor(0, 12);
                    gotoxy(50, 12); cout << "Danh sach mon hoc dang trong!";
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
                cout << "Up/Down: Chon | Left/Right: Chuyen trang";
                gotoxy(35, 27); SetColor(0, 11);
                cout << "[ENTER] Chon mon thi | [ESC] Thoat";
                SetColor(0, 7);

                oldPage = currentPage;
                oldCursor = cursor;
                forceRedraw = 0;
            }
            else if (oldCursor != cursor) {
                int startIdx = currentPage * itemsPerPage;
                if (oldCursor >= startIdx && oldCursor < startIdx + itemsPerPage) {
                    int toadoY_old = 10 + (oldCursor - startIdx) * 2;
                    SetColor(0, 7);
                    gotoxy(35, toadoY_old);
                    printf("%-5d %-15s %-35s %-5d", oldCursor + 1, dsMH.data[oldCursor].maMH, dsMH.data[oldCursor].tenMH, dsMH.data[oldCursor].soTinChi);
                }
                int toadoY_new = 10 + (cursor - startIdx) * 2;
                SetColor(11, 0);
                gotoxy(35, toadoY_new);
                printf("%-5d %-15s %-35s %-5d", cursor + 1, dsMH.data[cursor].maMH, dsMH.data[cursor].tenMH, dsMH.data[cursor].soTinChi);
                oldCursor = cursor;
                SetColor(0, 7);
            }

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
            else if (ch == 27) return; // Thoát hẳn
        }

        // VÒNG LẶP 2: SETUP THÔNG SỐ SAU KHI ĐÃ CHỌN MÔN
        if (isSelected) {
            MonHoc monChon = dsMH.data[cursor];
            CauHoi* khoCauHoi[2000] = {NULL};
            int tongCauHoiKho = 0;
            LayDanhSachCauHoiTheoMon(root, monChon.maMH, khoCauHoi, tongCauHoiKho);

            if (tongCauHoiKho == 0) {
                InThongBaoContent("Mon hoc nay chua co cau hoi!", 12, 45, 24);
                _getch();
                continue; // Lặp lại vòng ngoài cùng để chọn lại môn
            }

            XoaVungContent();
            InTitle("SETUP CAU HINH THI TRAC NGHIEM");
            
            gotoxy(40, 10); cout << "Mon hoc: ";
            SetColor(0, 14); cout << monChon.tenMH; SetColor(0, 7);
            cout << " (Ngan hang co: " << tongCauHoiKho << " cau)";
            
            int testTime = 0, questionCount = 0;
            gotoxy(40, 12); cout << "Thoi gian thi (phut): "; 
            gotoxy(40, 14); cout << "So cau hoi          : ";

            gotoxy(35, 27); SetColor(0, 8); cout << "Nhap so | ENTER: Xac nhan | ESC: Quay lai chon mon khac"; SetColor(0, 7);

            ShowCur(true);

            // Nhập Thời gian
            while (true) {
                gotoxy(63, 12); testTime = NhapSo(-1, 3, 1, 180); // Tối đa 180 phút
                if (testTime == INPUT_CANCELLED) break; // Bấm ESC
                if (testTime > 0) break;
                InThongBaoContent("Thoi gian phai > 0!", 12, 70, 12);
                Sleep(800); gotoxy(70, 12); cout << "                   ";   
            }
            if (testTime == INPUT_CANCELLED) {
                continue; // Quay lại bảng chọn môn nếu ấn ESC
            }

            // Nhập Số câu
            while (true) {
                gotoxy(63, 14); questionCount = NhapSo(-1, 4, 1, tongCauHoiKho);
                if (questionCount == INPUT_CANCELLED) break; // Bấm ESC
                if (questionCount > 0 && questionCount <= tongCauHoiKho) break;
                
                InThongBaoContent("So cau vuot qua ngan hang!", 12, 70, 14);
                _getch(); gotoxy(63, 14); cout << "                                 ";
            }
            if (questionCount == INPUT_CANCELLED) {
                continue; // Quay lại bảng chọn môn
            }

            ShowCur(false);

            // Fisher-Yates Shuffle xáo trộn đề
            srand(time(0)); 
            for (int i = 0; i < tongCauHoiKho; i++) {
                int r = i + rand() % (tongCauHoiKho - i); 
                swap(khoCauHoi[i], khoCauHoi[r]); 
            }
            CauHoi* deThi[1000] = {NULL};
            for (int i = 0; i < questionCount; i++) {
                deThi[i] = khoCauHoi[i];
            }
            
            gotoxy(40, 18); SetColor(0, 10); cout << "Ngan hang de da san sang!"; 
            gotoxy(40, 19); SetColor(0, 14); cout << "An phim bat ky de vao thi..."; SetColor(0, 7);
            _getch();
            
            TienHanhThiTracNghiem(sv, monChon, deThi, questionCount, testTime);
            return; // Thi xong thì thoát luôn luồng này
        }
    }
}

#endif