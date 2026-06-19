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

    while (true) {
        int oldCursor = -1;
        int oldPage = -1;
        int forceRedraw = 1;
        bool isSelected = false;

        // Chon mon thi
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

        // Setup thong so
        if (isSelected) {
            MonHoc monChon = dsMH.data[cursor];
            CauHoi* khoCauHoi[MAX_CAU_HOI_MOI_MON] = {NULL};
            int tongCauHoiKho = 0;
            LayDanhSachCauHoiTheoMon(root, monChon.maMH, khoCauHoi, tongCauHoiKho);
            int gioiHanChon = (tongCauHoiKho < MAX_CAU_THI) ? tongCauHoiKho : MAX_CAU_THI;

            if (tongCauHoiKho == 0) {
                InThongBaoContent("Mon hoc nay chua co cau hoi!", 12, 45, 24);
                _getch();
                continue;
            }

            XoaVungContent();
            InTitle("SETUP CAU HINH THI TRAC NGHIEM");
            
            gotoxy(40, 10); cout << "Mon hoc: ";
            SetColor(0, 14); cout << monChon.tenMH; SetColor(0, 7);
            cout << " (Ngan hang co: " << tongCauHoiKho << " cau)";
            
            gotoxy(40, 12); cout << "Thoi gian thi (phut): "; 
            gotoxy(40, 14); cout << "So cau hoi          : ";

            gotoxy(35, 27); SetColor(0, 8); cout << "Nhap so | UP/DOWN: Di chuyen | ENTER: Xac nhan | ESC: Quay lai"; SetColor(0, 7);

            ShowCur(true);

            // Cac bien trang thai
            string time_str = "";
            string count_str = "";
            int testTime = 0, questionCount = 0;
            int currentField = 0; // 0: Thoi gian, 1: So cau
            bool isDone = false;
            bool isCancel = false;

            // Dieu huong
            while (!isDone && !isCancel) {
                if (currentField == 0) gotoxy(62 + time_str.length(), 12);
                else if (currentField == 1) gotoxy(62 + count_str.length(), 14);

                char ch = _getch();
                if (ch == 224 || ch == 0 || ch == -32) {
                    char ext = _getch();
                    if (ext == 72) { 
                        currentField = (currentField > 0) ? currentField - 1 : 1;
                    } else if (ext == 80) { 
                        currentField = (currentField < 1) ? currentField + 1 : 0;
                    }
                }
                else if (isprint(ch)) {
                    if (isdigit(ch)) {
                        if (currentField == 0 && time_str.length() < 3) {
                            time_str += ch; cout << ch;
                        } else if (currentField == 1 && count_str.length() < 3) {
                            count_str += ch; cout << ch;
                        }
                    }
                }
                else if (ch == 8) { 
                    if (currentField == 0 && !time_str.empty()) {
                        time_str.pop_back(); gotoxy(62 + time_str.length(), 12); cout << " \b";
                    } else if (currentField == 1 && !count_str.empty()) {
                        count_str.pop_back(); gotoxy(62 + count_str.length(), 14); cout << " \b";
                    }
                }
                else if (ch == 27) { 
                    isCancel = true;
                }
                else if (ch == 13) { 
                    if (currentField == 0) {
                        currentField = 1; 
                    } else {
                        if (time_str.empty() || count_str.empty()) {
                            InThongBaoContent("Vui long nhap day du thong so!", 12, 45, 24);
                            int waitKey = _getch();
                            if (waitKey == 224 || waitKey == 0 || waitKey == -32) _getch();
                            InThongBaoContent("                              ", 12, 45, 24);
                            continue;
                        }

                        testTime = stoi(time_str);
                        questionCount = stoi(count_str);

                        bool ok = true;
                        if (testTime <= 0 || testTime > 180) {
                            InThongBaoContent("Thoi gian thi phai tu 1 den 180 phut!", 12, 45, 24);
                            ok = false; currentField = 0;
                        } 
                        else if (questionCount <= 0 || questionCount > gioiHanChon) {
                            string msg = "So cau phai tu 1 den " + to_string(gioiHanChon) + " cau!";
                            InThongBaoContent(msg, 12, 45, 24);
                            ok = false; currentField = 1;
                        }

                        if (ok) {
                            isDone = true;
                        } else {
                            int waitKey = _getch();
                            if (waitKey == 224 || waitKey == 0 || waitKey == -32) _getch();
                            gotoxy(45, 24); cout << "                                             ";
                        }
                    }
                }
            }

            if (isCancel) {
                continue; 
            }

            ShowCur(false);

            // Fisher-Yates Shuffle xao de
            srand(time(0)); 
            for (int i = 0; i < tongCauHoiKho; i++) {
                int r = i + rand() % (tongCauHoiKho - i); 
                CauHoi* temp = khoCauHoi[i];
                khoCauHoi[i] = khoCauHoi[r];
                khoCauHoi[r] = temp;
            }

            CauHoi* deThi[MAX_CAU_THI] = {NULL};
            for (int i = 0; i < questionCount; i++) {
                deThi[i] = khoCauHoi[i];
            }
            
            gotoxy(40, 18); SetColor(0, 10); cout << "Ngan hang de thi da san sang!"; 
            gotoxy(40, 19); SetColor(0, 14); cout << "An phim bat ky de vao thi..."; SetColor(0, 7);
            _getch();
            
            TienHanhThiTracNghiem(sv, monChon, deThi, questionCount, testTime);
            return;
        }
    }
}

#endif