#ifndef FLOWGV_H
#define FLOWGV_H

#include "khaibao.h"
#include "saveload.h"
#include "hamchinh.h"
#include "dohoa.h"
#include "ErrorCodes.h"
#include "hamtienich.h"

void flowGiaoVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root);
void flowQuanLyLop_SinhVien(DanhSachLop &dsLop);
void flowQuanLySinhVienCuaLop(DanhSachLop &dsLop, Lop* lopHienTai);
void flowQuanLyMonHoc_CauHoi(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root);
void flowQuanLyCauHoiCuaMon(const char* maMH,const char* tenMH ,PTR_CAUHOI &root);
void flowQuanLyDiemSV(DanhSachMonHoc &dsMH, DanhSachLop &dsLop);
void flowXemLaiBaiLam(const char* maSV, const char* maMH);
void flowThiThuGV(DanhSachMonHoc &dsMH, PTR_CAUHOI &root);
Lop* TimKiemLopThongMinh(DanhSachLop &dsLop);
SinhVien* TimKiemSinhVienThongMinh(Lop* lop, const char* maSV);
MonHoc* TimKiemMonHocThongMinh(DanhSachMonHoc &dsMH, const char* maMH);



void flowGiaoVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
    char menuChinh[5][50] = {"1. Quan Ly Lop", "2. Quan Ly Mon Hoc", "3. Quan Ly Diem", "4. Thi Thu", "5. Dang Xuat"};
    
    while (true) {
        XoaVungContent();
        SetupConsoleLayout();
        VeLayoutChinh("GIANG VIEN"); 
        
        int chon = MenuDong(menuChinh, 5, 4, 9); 
        
        if (chon == 0) flowQuanLyLop_SinhVien(dsLop);
        else if (chon == 1) flowQuanLyMonHoc_CauHoi(dsLop, dsMH, root);
        else if (chon == 2) flowQuanLyDiemSV(dsMH, dsLop);
        else if (chon == 3) flowThiThuGV(dsMH, root);
        else break; 
    }
}

void flowQuanLyLop_SinhVien(DanhSachLop &dsLop) {
    int cursor = 0;
    int itemsPerPage = 8;
    int oldCursor = -1;
    int oldPage = -1;
    int forceRedraw = 1;
    
    while (true) {
        ShowCur(false);
        int totalItems = dsLop.soLuong;
        int totalPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;
        int currentPage = (totalItems == 0) ? 0 : (cursor / itemsPerPage);

        if (forceRedraw || oldPage != currentPage) {
            XoaVungContent();
            InTitle("DANH SACH LOP HOC");
            gotoxy(105, 27); cout << "(Trang " << currentPage + 1 << "/" << totalPages << ")"; 
            SetColor(0, 7);
            
            gotoxy(35, 7); SetColor(0, 14);
            printf(" %-5s %-16s %-35s", "STT", "Ma Lop", "Ten Lop");
            SetColor(0, 7);

            int startIdx = currentPage * itemsPerPage;
            int endIdx = startIdx + itemsPerPage;
            if (endIdx > totalItems) {
                endIdx = totalItems;
            }
            
            if (totalItems == 0) {
                gotoxy(45, 10); cout << "(Chua co lop hoc nao)";
            }
            else {
                for (int i = startIdx; i < endIdx; i++) {
                    int toadoY = 9 + (i - startIdx) * 2; 
                    
                    if (i == cursor) {
                        SetColor(11, 0); 
                    } else {
                        SetColor(0, 7);  
                    }
                    
                    gotoxy(35, toadoY); 
                    
                    printf(" %-5d %-16s %-35s", i + 1, dsLop.nodes[i]->maLop, dsLop.nodes[i]->tenLop);
                }
            }

            gotoxy(35, 26); SetColor(0, 8);
            cout << "Up/Down: Chon | Left/Right: Chuyen trang | [ENTER] Xem SV"; 
            gotoxy(35, 27); SetColor(0, 11);
            cout << "[INS/I] Them | [E] Sua | [DEL] Xoa | [F] Tim Kiem  | [ESC] Thoat";
            SetColor(0, 7);
            oldPage = currentPage;
            oldCursor = cursor;
            forceRedraw = false; 
        }

        else if (oldCursor != cursor) {
            int startIdx = currentPage * itemsPerPage;
            
            if (oldCursor >= startIdx && oldCursor < startIdx + itemsPerPage) {
                int toadoY_old = 9 + (oldCursor - startIdx) * 2;
                SetColor(0, 7);
                gotoxy(35, toadoY_old);
                printf(" %-5d %-16s %-35s", oldCursor + 1, dsLop.nodes[oldCursor]->maLop, dsLop.nodes[oldCursor]->tenLop);
            }
            
            int toadoY_new = 9 + (cursor - startIdx) * 2;
            SetColor(11, 0);
            gotoxy(35, toadoY_new);
            printf(" %-5d %-16s %-35s", cursor + 1, dsLop.nodes[cursor]->maLop, dsLop.nodes[cursor]->tenLop);
            SetColor(0, 7);
            
            oldCursor = cursor;
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
            flowQuanLySinhVienCuaLop(dsLop, dsLop.nodes[cursor]);
            forceRedraw = 1; 
        }
        else if (ch == 27) { 
            break;
        } 
        else if (toupper(ch) == 'F') {
            if (dsLop.soLuong > 0) {
                Lop* lopDuocChon = TimKiemLopThongMinh(dsLop);
                if (lopDuocChon != NULL) {
                    flowQuanLySinhVienCuaLop(dsLop, lopDuocChon); 
                }
            } else {
                InThongBaoContent("Khong tim thay lop!", 12, 50, 26);
                _getch();
            }
            forceRedraw = 1;
        }
        else if (toupper(ch) == 'I') { 
            XoaVungContent();
            InTitle("THEM LOP HOC");
            
            gotoxy(40, 10); cout << "Nhap Ma Lop :                ";
            gotoxy(40, 12); cout << "Nhap Ten Lop:                               ";

            gotoxy(35, 26); SetColor(0, 8);
            cout << "ESC: de thoat | UP/DOWN: di chuyen | ENTER: xac nhan";
            SetColor(0, 7);

            string ma = "";
            string ten = "";

            int currentField = 0; // 0: Dang nhap ma, 1: Dang nhap ten
            bool isDone = false;
            bool isCancel = false;

            ShowCur(true);

            while (!isDone && !isCancel) {
                if (currentField == 0) gotoxy(55 + ma.length(), 10);
                else gotoxy(55 + ten.length(), 12);
                
                char ch = _getch();
                if (ch == 224 || ch == 0 || ch == -32) { 
                    char ext = _getch();
                    if (ext == 72) currentField = 0;
                    else if (ext == 80) currentField = 1; 
                } else if (ch == 27) { 
                    isCancel = true;
                } else if (isprint(ch)) {
                    if ((isalnum(ch) || ch == '-' || ch == '_') && currentField == 0 && ma.length() < MAX_MALOP) {
                        ma += toupper(ch); cout << (char)toupper(ch);
                    } else if (currentField == 1 && ten.length() < MAX_TEN) {
                        if (ch == ' ' && (ten.length() == 0 || ten.back() == ' ')) continue;
                        ten += toupper(ch); cout << (char)toupper(ch);
                    }
                } else if (ch == 8) { 
                    if (currentField == 0 && !ma.empty()) {
                        ma.pop_back(); gotoxy(55 + ma.length(), 10); cout << " \b";
                    } else if (currentField == 1 && !ten.empty()) {
                        ten.pop_back(); gotoxy(55 + ten.length(), 12); cout << " \b";
                    }
                } else if (ch == 13) { 
                    if (currentField == 0) {
                        while (!ma.empty() && ma.back() == ' ') ma.pop_back();
                        gotoxy(55, 10); cout << ma << "          ";
                        currentField = 1;
                    }
                    else {
                        while (!ma.empty() && ma.back() == ' ') ma.pop_back();
                        while (!ten.empty() && ten.back() == ' ') ten.pop_back();
                        gotoxy(55, 10); cout << ma << "          ";
                        gotoxy(55, 12); cout << ten << "          ";

                        if (ma.empty() || ten.empty()) {
                            InThongBaoContent("Vui long nhap day du thong tin!", 12, 50, 24);
                            _getch();
                            InThongBaoContent("                               ", 12, 50, 24);
                        }
                        else {
                            int res = themLop(dsLop, ma.c_str(), ten.c_str());
                            if (res == SUCCESS) {
                                InThongBaoContent("Them lop hoc thanh cong!", 10, 50, 24);
                                SaveLop(dsLop);
                                _getch(); 
                                isDone = true;
                            } else {
                                InThongBaoContent(layThongBaoLoi(res), 12, 50, 24);
                                _getch();
                                InThongBaoContent("                               ", 12, 50, 24);
                            }
                        }
                    }
                }
            }
            ShowCur(false);
            forceRedraw = 1;
        } 
        else if (toupper(ch) == 'E' && totalItems > 0) { 
            XoaVungContent();
            InTitle("SUA LOP HOC");
            gotoxy(35, 26); SetColor(0, 8);
            cout << "ESC: de thoat | UP/DOWN: di chuyen | ENTER: xac nhan";
            SetColor(0, 7);
            
            gotoxy(40, 10); cout << "Ma Lop : ";
            gotoxy(40, 12); cout << "Ten Lop: "; 
            
            string maCu = dsLop.nodes[cursor]->maLop;
            string tenCu = dsLop.nodes[cursor]->tenLop;
            string ma = maCu;
            string ten = tenCu;

            gotoxy(49, 10); cout << ma;
            gotoxy(49, 12); cout << ten;

            ShowCur(true);
            int currentField = 0; 
            bool isCancel = false;
            bool isDone = false;

            while (!isDone && !isCancel) {
                if (currentField == 0) gotoxy(49 + ma.length(), 10);
                else gotoxy(49 + ten.length(), 12);
                
                char ch = _getch();
                if (ch == 224 || ch == 0 || ch == -32) { 
                    char ext = _getch();
                    if (ext == 72) currentField = 0;
                    else if (ext == 80) currentField = 1; 
                } else if (ch == 27) { 
                    isCancel = true;
                } else if (isprint(ch)) {
                    if ((isalnum(ch) || ch == '-' || ch == '_') && currentField == 0 && ma.length() < MAX_MALOP) {
                        ma += toupper(ch); cout << (char)toupper(ch);
                    } else if (currentField == 1 && ten.length() < MAX_TEN) {
                        if (ch == ' ' && (ten.length() == 0 || ten.back() == ' ')) continue;
                        ten += toupper(ch); cout << (char)toupper(ch);
                    }
                } else if (ch == 8) { 
                    if (currentField == 0 && !ma.empty()) {
                        ma.pop_back(); gotoxy(49 + ma.length(), 10); cout << " \b";
                    } else if (currentField == 1 && !ten.empty()) {
                        ten.pop_back(); gotoxy(49 + ten.length(), 12); cout << " \b";
                    }
                } else if (ch == 13) {
                    if (currentField == 0) {
                        while (!ma.empty() && ma.back() == ' ') ma.pop_back();
                        gotoxy(49, 10); cout << ma << "          ";
                        currentField = 1;
                    }
                    else {
                        while (!ma.empty() && ma.back() == ' ') ma.pop_back();
                        while (!ten.empty() && ten.back() == ' ') ten.pop_back();
                        gotoxy(49, 10); cout << ma << "          ";
                        gotoxy(49, 12); cout << ten << "          ";

                        if (ma.empty() || ten.empty()) {
                            InThongBaoContent("Vui long nhap day du thong tin!", 12, 50, 24);
                            _getch();
                            InThongBaoContent("                               ", 12, 50, 24);
                        }
                        else if (ma == maCu && ten == tenCu) {
                            InThongBaoContent("Khong co thay doi nao.", 8, 50, 24);
                            _getch();
                            isCancel = true;
                        }
                        else {
                            int res = suaLop(dsLop, maCu.c_str(), ma.c_str(), ten.c_str());
                            if (res == SUCCESS) {
                                InThongBaoContent("Sua lop hoc thanh cong!", 10, 50, 24);
                                SaveLop(dsLop);
                                _getch(); 
                                isDone = true;
                            }
                            else {
                                InThongBaoContent(layThongBaoLoi(res), 12, 50, 24);
                                _getch();
                                InThongBaoContent("                               ", 12, 50, 24);
                            }
                        }
                    }
                }
            }
            ShowCur(false);
            forceRedraw = 1;
        }

        else if ((ch == 83 || toupper(ch) == 'D') && totalItems > 0) {
            gotoxy(40, 25); SetColor(0, 14); 
            string thongBao = "Xac nhan xoa lop: " + string(dsLop.nodes[cursor]->maLop) + " ";
            cout << thongBao;
            if (XacNhan(40 + thongBao.length(), 25) == 'Y') {
                if (KiemTraLopCoSinhVien(dsLop, dsLop.nodes[cursor]->maLop) == ERR_LOP_HAS_SV) {
                    gotoxy(40, 25); cout << "                                             ";
                    InThongBaoContent("Khong the xoa: lop co sinh vien!", 12, 40, 25);
                } else {
                    int res = xoaLop(dsLop, dsLop.nodes[cursor]->maLop);
                    if (res == SUCCESS) {
                        if (cursor >= dsLop.soLuong && cursor > 0) cursor--;
                        SaveLop(dsLop);
                        gotoxy(40, 25); cout << "                                             ";
                        InThongBaoContent("Da xoa lop thanh cong!", 10, 40, 25);
                    } else {
                        gotoxy(40, 25); cout << "                                             ";
                        InThongBaoContent(layThongBaoLoi(res), 12, 40, 25);
                    }
                }
            } else {
                gotoxy(40, 25); cout << "                                             ";
                InThongBaoContent("Da huy thao tac xoa!", 10, 40, 25);
            }
            _getch(); 
            forceRedraw = 1;
        }
    }
}

Lop* TimKiemLopThongMinh(DanhSachLop &dsLop) {
    string tuKhoa = ""; 
    int cursor = 0;

    while (true) {
        XoaVungContent();
        
        InTitle("TIM KIEM LOP HOC");
        gotoxy(35, 8); SetColor(0, 14); cout << "Nhap tu khoa de loc: ";
        SetColor(0, 15); cout << tuKhoa << "_"; 

        gotoxy(35, 11); SetColor(0, 14); printf(" %-5s %-15s %-30s", "STT", "Ma Lop", "Ten Lop");

        string tuKhoaLow = ToLower(tuKhoa);

        Lop* dsLoc[500]; 
        int sttGoc[500];
        int soLuongLoc = 0;

        for (int i = 0; i < dsLop.soLuong; i++) {
            string maLopLow = ToLower(dsLop.nodes[i]->maLop);
            string tenLopLow = ToLower(dsLop.nodes[i]->tenLop);
            
            if (tuKhoa.empty() || maLopLow.find(tuKhoaLow) != string::npos || tenLopLow.find(tuKhoaLow) != string::npos) {
                dsLoc[soLuongLoc] = dsLop.nodes[i];
                sttGoc[soLuongLoc] = i + 1;
                soLuongLoc++;
            }
        }

        if (soLuongLoc == 0) cursor = 0;
        else if (cursor >= soLuongLoc) cursor = soLuongLoc - 1;
        else if (cursor < 0) cursor = 0;

        int yPos = 13;
        int maxDong = 12;

        int batDau = (cursor / maxDong) * maxDong;
        int ketThuc = batDau + maxDong;
        if (ketThuc > soLuongLoc) ketThuc = soLuongLoc;

        for (int i = batDau; i < ketThuc; i++) {
            gotoxy(35, yPos);

            if (i == cursor) {
                SetColor(15, 0);
            } else {
                SetColor(0, 7);
            }

            string ma = string(dsLoc[i]->maLop).substr(0, 15);
            string ten = string(dsLoc[i]->tenLop).substr(0, 30);
            
            printf(" %-5d %-15s %-30s", sttGoc[i], ma.c_str(), ten.c_str());
            
            SetColor(0, 7);
            yPos++;
        }
        
        if (soLuongLoc == 0) {
            gotoxy(52, 14); SetColor(0, 12); cout << "(Khong tim thay lop hoc nao)";
        }

        gotoxy(35, 26); SetColor(0, 8); cout << "Go chu de loc | Phim mui ten LEN/XUONG de chon lop";
        gotoxy(35, 27); SetColor(0, 11); cout << "ENTER: Mo lop | BACKSPACE: Xoa | ESC: Thoat";
        SetColor(0, 7);

        int ch = _getch();

        if (ch == 0 || ch == 224) {
            ch = _getch();
            if (ch == 72) {
                if (cursor > 0) cursor--;
            } 
            else if (ch == 80) {
                if (cursor < soLuongLoc - 1) cursor++;
            }
            continue;
        }
        else if (ch == 27) {
            return NULL; 
        }
        else if (ch == 8) {
            if (!tuKhoa.empty()) {
                tuKhoa.pop_back(); 
                cursor = 0;
            }
        }
        else if (ch == 13) {
            if (soLuongLoc > 0) {
                return dsLoc[cursor]; 
            }
        }
        else if (ch >= 32 && ch <= 126) {
            if (ch == 32 && tuKhoa.empty()) continue; 
            
            if (tuKhoa.length() < 20) { 
                tuKhoa += (char)ch;
                cursor = 0;
            }
        }
    }
}

SinhVien* TimKiemSinhVienTrongLopThongMinh(Lop* lop) {
    if (lop == NULL || lop->headSV == NULL) return NULL;
    
    string tuKhoa = "";
    int cursor = 0;

    SinhVien* arrSV[100];
    int totalSV = 0;
    for (SinhVien* p = lop->headSV; p != NULL; p = p->next) {
        arrSV[totalSV++] = p;
    }

    while (true) {
        XoaVungContent();
        
        InTitle("TIM KIEM SINH VIEN TRONG LOP");
        gotoxy(35, 8); SetColor(0, 14); cout << "Nhap (Ma SV/Ho va ten): ";
        SetColor(0, 15); cout << tuKhoa << "_"; 
        
        gotoxy(35, 11); SetColor(0, 14); 
        printf(" %-5s %-15s %-30s %-10s", "STT", "Ma SV", "Ho va Ten", "Phai");
        string tuKhoaLow = ToLower(tuKhoa);
        
        SinhVien* dsLoc[100]; 
        int sttGoc[100];
        int soLuongLoc = 0;

        for (int i = 0; i < totalSV; i++) {
            string maSVLow = ToLower(arrSV[i]->maSV);
            string hoTenLow = ToLower(string(arrSV[i]->ho) + " " + string(arrSV[i]->ten));
            if (tuKhoa.empty() || maSVLow.find(tuKhoaLow) != string::npos || hoTenLow.find(tuKhoaLow) != string::npos) {
                dsLoc[soLuongLoc] = arrSV[i];
                sttGoc[soLuongLoc] = i + 1;
                soLuongLoc++;
            }
        }
        
        if (soLuongLoc == 0) cursor = 0;
        else if (cursor >= soLuongLoc) cursor = soLuongLoc - 1;
        else if (cursor < 0) cursor = 0;

        int yPos = 13;
        int maxDong = 12; 
        
        int batDau = (cursor / maxDong) * maxDong;
        int ketThuc = batDau + maxDong;
        if (ketThuc > soLuongLoc) ketThuc = soLuongLoc;

        for (int i = batDau; i < ketThuc; i++) {
            gotoxy(35, yPos);
            
            if (i == cursor) SetColor(15, 0);
            else SetColor(0, 7); 
            
            string phai = (dsLoc[i]->phai == 1) ? "Nam" : "Nu";
            string hoTen = string(dsLoc[i]->ho) + " " + string(dsLoc[i]->ten);
            if (hoTen.length() > 30) hoTen = hoTen.substr(0, 27) + "...";
            
            printf(" %-5d %-15s %-30s %-10s", sttGoc[i], dsLoc[i]->maSV, hoTen.c_str(), phai.c_str());
            
            SetColor(0, 7); 
            yPos++;
        }

        gotoxy(35, 27); SetColor(0, 11); cout << "ENTER: Chon | BACKSPACE: Xoa | ESC: Thoat";
        SetColor(0, 7);
        
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch(); 
            if (ch == 72) { if (cursor > 0) cursor--; } 
            else if (ch == 80) { if (cursor < soLuongLoc - 1) cursor++; }
            continue; 
        }
        else if (ch == 27) return NULL;
        else if (ch == 8) {
            if (!tuKhoa.empty()) {
                tuKhoa.pop_back(); 
                cursor = 0; 
            }
        }
        else if (ch == 13) { 
            if (soLuongLoc > 0) return dsLoc[cursor]; 
        }
        else if (ch >= 32 && ch <= 126) { 
            if (ch == 32 && tuKhoa.empty()) continue; 
            if (tuKhoa.length() < 25) { 
                tuKhoa += (char)ch;
                cursor = 0; 
            }
        }
    }
}

void flowQuanLySinhVienCuaLop(DanhSachLop &dsLop, Lop* lopHienTai) {
    int cursor = 0;
    int itemsPerPage = 8;
    int oldCursor = -1;
    int oldPage = -1;
    int forceRedraw = 1;
    
    while (true) {
        ShowCur(false);
        int totalItems = demSoSV(lopHienTai); 
        int totalPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;
        int currentPage = (totalItems == 0) ? 0 : (cursor / itemsPerPage);
        
        // 1. VẼ LẠI TOÀN BỘ KHI CHUYỂN TRANG HOẶC CÓ LỆNH (Thêm/Sửa/Xóa/Tìm)
        if (forceRedraw || currentPage != oldPage) {
            XoaVungContent();
            InTitle("DANH SACH SINH VIEN");
            InSmallTitle(string(lopHienTai->maLop) + " - " + string(lopHienTai->tenLop));
            gotoxy(105, 27); SetColor(0, 10); cout << "(Trang " << currentPage + 1 << "/" << totalPages << ")"; 
            
            gotoxy(33, 7); SetColor(0, 14);
            printf(" %-5s %-15s %-25s %-10s", "STT", "Ma SV", "Ho va Ten", "Gioi Tinh"); 
            SetColor(0, 7);
            
            SinhVien* p = lopHienTai->headSV;
            int startIdx = currentPage * itemsPerPage;
            int endIdx = startIdx + itemsPerPage;
            if (endIdx > totalItems) endIdx = totalItems;
            
            // Trượt con trỏ đến vị trí phần tử đầu tiên của trang
            for (int i = 0; i < startIdx && p != NULL; i++) {
                p = p->next;
            }
            
            if (totalItems == 0) {
                gotoxy(50, 10); SetColor(0, 12); cout << "(Lop nay chua co sinh vien nao)"; SetColor(0, 7);
            } else {
                for (int i = startIdx; i < endIdx && p != NULL; i++) {
                    int toadoY = 9 + (i - startIdx) * 2;
                    
                    if (i == cursor) SetColor(11, 0); 
                    else SetColor(0, 7);  
                    
                    gotoxy(33, toadoY); 
                    string hoTen = string(p->ho) + " " + string(p->ten);
                    string phaiStr = (p->phai) ? "Nam" : "Nu";
                    
                    printf(" %-5d %-15s %-25s %-10s", i + 1, p->maSV, hoTen.c_str(), phaiStr.c_str());
                    p = p->next; 
                }
            }
            SetColor(0, 7);
            
            gotoxy(35, 26); SetColor(0, 8);
            cout << "Up/Down: Chon | Left/Right: Chuyen trang"; 
            gotoxy(35, 27); SetColor(0, 11);
            cout << "[INS/I] Them | [E] Sua | [DEL] Xoa | [F] Tim Kiem  | [ESC] Thoat";
            SetColor(0, 7);

            oldPage = currentPage;
            oldCursor = cursor;
            forceRedraw = false;
        }
        // 2. CHỈ VẼ LẠI 2 DÒNG (DÒNG CŨ VÀ DÒNG MỚI) KHI DI CHUYỂN LÊN XUỐNG
        else if (oldCursor != cursor) {
            int startIdx = currentPage * itemsPerPage;

            // --- XÓA HIGHLIGHT DÒNG CŨ ---
            if (oldCursor >= startIdx && oldCursor < startIdx + itemsPerPage) {
                SinhVien* pOld = lopHienTai->headSV;
                for (int i = 0; i < oldCursor && pOld != NULL; i++) pOld = pOld->next;
                
                if (pOld != NULL) {
                    int toadoY_old = 9 + (oldCursor - startIdx) * 2;
                    SetColor(0, 7); // Trả về màu chữ bình thường
                    gotoxy(33, toadoY_old);
                    string hoTenOld = string(pOld->ho) + " " + string(pOld->ten);
                    string phaiStrOld = (pOld->phai) ? "Nam" : "Nu";
                    printf(" %-5d %-15s %-25s %-10s", oldCursor + 1, pOld->maSV, hoTenOld.c_str(), phaiStrOld.c_str());
                }
            }

            // --- TÔ HIGHLIGHT DÒNG MỚI ---
            SinhVien* pNew = lopHienTai->headSV;
            for (int i = 0; i < cursor && pNew != NULL; i++) pNew = pNew->next;
            
            if (pNew != NULL) {
                int toadoY_new = 9 + (cursor - startIdx) * 2;
                SetColor(11, 0); // Đổi thành màu Highlight
                gotoxy(33, toadoY_new);
                string hoTenNew = string(pNew->ho) + " " + string(pNew->ten);
                string phaiStrNew = (pNew->phai) ? "Nam" : "Nu";
                printf(" %-5d %-15s %-25s %-10s", cursor + 1, pNew->maSV, hoTenNew.c_str(), phaiStrNew.c_str());
                SetColor(0, 7);
            }

            oldCursor = cursor;
        }
        
        int ch = _getch();
        bool isDelete = false;
        
        if (ch == 224 || ch == 0 || ch == -32) {
            int extKey = _getch();
            if (extKey == 72 && cursor > 0) cursor--; 
            else if (extKey == 80 && cursor < totalItems - 1) cursor++; 
            else if (extKey == 75 && currentPage > 0) cursor = (currentPage - 1) * itemsPerPage; 
            else if (extKey == 77 && currentPage < totalPages - 1) cursor = (currentPage + 1) * itemsPerPage; 
            else if (extKey == 83) isDelete = true; 
        } 
        
        else if (toupper(ch) == 'D' && totalItems > 0) {
            SinhVien* p = lopHienTai->headSV;
            for (int i = 0; i < cursor && p != NULL; i++) {
                p = p->next;
            }

            if (p != NULL) {
                if (p->headDiem != NULL) {
                    InThongBaoContent("Loi: SV nay da co diem thi, khong the xoa!", 12, 40, 25);
                    _getch();
                    InThongBaoContent("                                          ", 12, 40, 25);
                    continue; 
                }

                gotoxy(40, 25); SetColor(0, 14); 
                string thongBao = "Xac nhan xoa sinh vien: " + string(p->maSV) + " (Y/N)? ";
                cout << thongBao;
                
                if (XacNhan(40 + thongBao.length(), 25) == 'Y') {
                    int Res = xoaSinhVien(lopHienTai, p->maSV);
                    if (Res == SUCCESS) {
                        if (cursor >= totalItems - 1 && cursor > 0) cursor--;
                        SaveLop(dsLop);
                        gotoxy(40, 25); cout << "                                                            ";
                        InThongBaoContent("Da xoa sinh vien thanh cong!", 10, 40, 25);
                    } else {
                        gotoxy(40, 25); cout << "                                                            ";
                        InThongBaoContent(layThongBaoLoi(Res), 12, 40, 25);
                    }
                } else {
                    gotoxy(40, 25); cout << "                                                            ";
                    InThongBaoContent("Da huy thao tac xoa!", 8, 40, 25);
                }
                
                _getch(); 
                gotoxy(40, 25); cout << "                                                            ";
                forceRedraw = 1; 
            }
        }

        else if (ch == 27) break;
        
        else if (toupper(ch) == 'F') {
            if (lopHienTai->headSV != NULL) {
                SinhVien* svDuocChon = TimKiemSinhVienTrongLopThongMinh(lopHienTai);
                if (svDuocChon != NULL) {
                    InThongBaoContent("Ban dang chon Sinh vien: " + string(svDuocChon->maSV) + " - " + string(svDuocChon->ho) + " " + string(svDuocChon->ten), 10, 40, 26);
                    _getch();
                }
            } else {
                _getch();
            }
            forceRedraw = 1;
        }
        else if (toupper(ch) == 'I') { 
            XoaVungContent();
            InTitle("THEM SINH VIEN");
            InSmallTitle(string(lopHienTai->maLop) + " - " + string(lopHienTai->tenLop));
            
            gotoxy(40, 10); cout << "Ma SV:                 ";
            gotoxy(40, 12); cout << "Ho   :                               ";
            gotoxy(40, 14); cout << "Ten  :                 ";
            gotoxy(40, 16); cout << "Phai : ";

            gotoxy(35, 26); SetColor(0, 8);
            cout << "ESC: Thoat | UP/DOWN: Di chuyen | [SPACE]: Doi gioi tinh | ENTER: Xac nhan";
            SetColor(0, 7);
            
            string ma = "";
            string ho = "";
            string ten = "";
            bool phai = true; 

            gotoxy(47, 16); cout << "NAM";

            int currentField = 0; // 0: Ma, 1: Ho, 2: Ten, 3: Phai
            bool isDone = false;
            bool isCancel = false;

            ShowCur(true);

            while (!isDone && !isCancel) {
                if (currentField == 0) gotoxy(47 + ma.length(), 10);
                else if (currentField == 1) gotoxy(47 + ho.length(), 12);
                else if (currentField == 2) gotoxy(47 + ten.length(), 14);
                else if (currentField == 3) {
                    ShowCur(false);
                    gotoxy(47, 16); 
                }
                if (currentField != 3) ShowCur(true);

                char ch = _getch();
                if (ch == 224 || ch == 0 || ch == -32) { 
                    char ext = _getch();
                    if (ext == 72) { // UP
                        currentField--;
                        if (currentField < 0) currentField = 3;
                    } else if (ext == 80) {
                        currentField++;
                        if (currentField > 3) currentField = 0;
                    }
                } 
                else if (ch == 32 && currentField == 3) { 
                    phai = !phai;
                    gotoxy(47, 16);
                    cout << (phai ? "NAM" : "NU") << "   ";
                }
                else if (ch == 27) { 
                    isCancel = true;
                } 
                else if (isprint(ch)) {
                    if ((isalnum(ch) || ch == '-' || ch == '_') && currentField == 0 && ma.length() < MAX_MASV) {
                        ma += toupper(ch); cout << (char)toupper(ch);
                    } else if (currentField == 1 && ho.length() < MAX_TEN) {
                        if (ch == ' ' && (ho.length() == 0 || ho.back() == ' ')) continue;
                        ho += toupper(ch); cout << (char)toupper(ch);
                    } else if (currentField == 2 && ten.length() < MAX_TEN) {
                        if (ch == ' ' && (ten.length() == 0 || ten.back() == ' ')) continue;
                        ten += toupper(ch); cout << (char)toupper(ch);
                    }
                } 
                else if (ch == 8) { 
                    if (currentField == 0 && !ma.empty()) {
                        ma.pop_back(); gotoxy(47 + ma.length(), 10); cout << " \b";
                    } else if (currentField == 1 && !ho.empty()) {
                        ho.pop_back(); gotoxy(47 + ho.length(), 12); cout << " \b";
                    } else if (currentField == 2 && !ten.empty()) {
                        ten.pop_back(); gotoxy(47 + ten.length(), 14); cout << " \b";
                    }
                } 
                else if (ch == 13) { 
                    if (currentField < 3) {
                        if (currentField == 0) {
                            while (!ma.empty() && ma.back() == ' ') ma.pop_back();
                            gotoxy(47, 10); cout << ma << "          ";
                        } else if (currentField == 1) {
                            while (!ho.empty() && ho.back() == ' ') ho.pop_back();
                            gotoxy(47, 12); cout << ho << "          ";
                        } else if (currentField == 2) {
                            while (!ten.empty() && ten.back() == ' ') ten.pop_back();
                            gotoxy(47, 14); cout << ten << "          ";
                        }
                        currentField++; 
                    } else {
                        while (!ma.empty() && ma.back() == ' ') ma.pop_back();
                        while (!ho.empty() && ho.back() == ' ') ho.pop_back();
                        while (!ten.empty() && ten.back() == ' ') ten.pop_back();
                        gotoxy(47, 10); cout << ma << "          ";
                        gotoxy(47, 12); cout << ho << "          ";
                        gotoxy(47, 14); cout << ten << "          ";

                        if (ma.empty() || ho.empty() || ten.empty()) {
                            InThongBaoContent("Vui long nhap day du thong tin!", 12, 45, 24);
                            _getch();
                            InThongBaoContent("                               ", 12, 45, 24); 
                        } 
                        else if (kiemTraTrungMaSinhVien(dsLop, ma.c_str()) == ERR_SV_DUPLICATE) {
                            InThongBaoContent("Ma SV da ton tai trong he thong!", 12, 45, 24);
                            _getch();
                            InThongBaoContent("                                ", 12, 45, 24);
                            currentField = 0; 
                        } 
                        else {
                            isDone = true; 
                        }
                    }
                }
            }

            if (isDone) {
                SinhVien svMoi;
                snprintf(svMoi.maSV, sizeof(svMoi.maSV), "%s", ma.c_str());
                snprintf(svMoi.ho, sizeof(svMoi.ho), "%s", ho.c_str());
                snprintf(svMoi.ten, sizeof(svMoi.ten), "%s", ten.c_str());
                svMoi.phai = phai;
                strcpy(svMoi.password, "123456"); 
                svMoi.headDiem = NULL;

                int res = themSinhVien(lopHienTai, svMoi);
                if (res == SUCCESS) {
                    InThongBaoContent("Them sinh vien thanh cong!", 10, 45, 24);
                    SaveLop(dsLop); 
                } else {
                    InThongBaoContent("Loi khi them sinh vien!", 12, 45, 24);
                }
                _getch();
            }
            ShowCur(false);
            forceRedraw = 1;
        }
        
        else if (toupper(ch) == 'E' && totalItems > 0) {
            SinhVien* svTarget = lopHienTai->headSV;
            for (int i = 0; i < cursor && svTarget != NULL; i++) {
                svTarget = svTarget->next;
            }

            if (svTarget != NULL) {
                XoaVungContent();
                InTitle("SUA SINH VIEN");
                InSmallTitle(string(lopHienTai->maLop) + " - " + string(lopHienTai->tenLop));

                gotoxy(40, 10); cout << "Ma SV:                 ";
                gotoxy(40, 12); cout << "Ho   :                               ";
                gotoxy(40, 14); cout << "Ten  :                 ";
                gotoxy(40, 16); cout << "Phai : ";

                gotoxy(35, 26); SetColor(0, 8);
                cout << "ESC: Thoat | UP/DOWN: Di chuyen | [SPACE]: Doi gioi tinh | ENTER: Xac nhan";
                SetColor(0, 7);

                string maCu = svTarget->maSV;
                string ma = svTarget->maSV;
                string ho = svTarget->ho;
                string ten = svTarget->ten;
                bool phai = svTarget->phai;

                gotoxy(47, 10); cout << ma;
                gotoxy(47, 12); cout << ho;
                gotoxy(47, 14); cout << ten;
                gotoxy(47, 16); cout << (phai ? "NAM" : "NU");

                int currentField = 0; // 0: Ma, 1: Ho, 2: Ten, 3: Phai
                bool isDone = false;
                bool isCancel = false;

                ShowCur(true);

                while (!isDone && !isCancel) {
                    if (currentField == 0) gotoxy(47 + ma.length(), 10);
                    else if (currentField == 1) gotoxy(47 + ho.length(), 12);
                    else if (currentField == 2) gotoxy(47 + ten.length(), 14);
                    else if (currentField == 3) {
                        ShowCur(false);
                        gotoxy(47, 16); 
                    }
                    if (currentField != 3) ShowCur(true);

                    char ch = _getch();
                    if (ch == 224 || ch == 0 || ch == -32) { 
                        char ext = _getch();
                        if (ext == 72) { // UP
                            currentField--;
                            if (currentField < 0) currentField = 3;
                        } else if (ext == 80) {
                            currentField++;
                            if (currentField > 3) currentField = 0;
                        }
                    } 
                    else if (ch == 32 && currentField == 3) { 
                        phai = !phai;
                        gotoxy(47, 16);
                        cout << (phai ? "NAM" : "NU") << "   ";
                    }
                    else if (ch == 27) { 
                        isCancel = true;
                    } 
                    else if (isprint(ch)) {
                        if ((isalnum(ch) || ch == '-' || ch == '_') && currentField == 0 && ma.length() < MAX_MASV) {
                            ma += toupper(ch); cout << (char)toupper(ch);
                        } else if (currentField == 1 && ho.length() < MAX_TEN) {
                            if (ch == ' ' && (ho.length() == 0 || ho.back() == ' ')) continue;
                            ho += toupper(ch); cout << (char)toupper(ch);
                        } else if (currentField == 2 && ten.length() < MAX_TEN) {
                            if (ch == ' ' && (ten.length() == 0 || ten.back() == ' ')) continue;
                            ten += toupper(ch); cout << (char)toupper(ch);
                        }
                    } 
                    else if (ch == 8) { 
                        if (currentField == 0 && !ma.empty()) {
                            ma.pop_back(); gotoxy(47 + ma.length(), 10); cout << " \b";
                        } else if (currentField == 1 && !ho.empty()) {
                            ho.pop_back(); gotoxy(47 + ho.length(), 12); cout << " \b";
                        } else if (currentField == 2 && !ten.empty()) {
                            ten.pop_back(); gotoxy(47 + ten.length(), 14); cout << " \b";
                        }
                    } else if (ch == 13) { 
                        if (currentField < 3) {
                            if (currentField == 0) {
                                while (!ma.empty() && ma.back() == ' ') ma.pop_back();
                                gotoxy(47, 10); cout << ma << "          ";
                            } else if (currentField == 1) {
                                while (!ho.empty() && ho.back() == ' ') ho.pop_back();
                                gotoxy(47, 12); cout << ho << "          ";
                            } else if (currentField == 2) {
                                while (!ten.empty() && ten.back() == ' ') ten.pop_back();
                                gotoxy(47, 14); cout << ten << "          ";
                            }
                            currentField++; 
                        } else {
                            while (!ma.empty() && ma.back() == ' ') ma.pop_back();
                            while (!ho.empty() && ho.back() == ' ') ho.pop_back();
                            while (!ten.empty() && ten.back() == ' ') ten.pop_back();
                            gotoxy(47, 10); cout << ma << "          ";
                            gotoxy(47, 12); cout << ho << "          ";
                            gotoxy(47, 14); cout << ten << "          ";

                            if (ma.empty() || ho.empty() || ten.empty()) {
                                InThongBaoContent("Vui long nhap day du thong tin!", 12, 45, 24);
                                _getch();
                                InThongBaoContent("                               ", 12, 45, 24); 
                            }
                            else if (ma == maCu && ho == string(svTarget->ho) && ten == string(svTarget->ten) && phai == svTarget->phai) {
                                InThongBaoContent("Khong co thay doi nao.", 8, 45, 24);
                                _getch();
                                isCancel = true;
                            } 
                            else if (kiemTraTrungMaSinhVien(dsLop, ma.c_str()) == ERR_SV_DUPLICATE && ma != svTarget->maSV) {
                                InThongBaoContent("Ma SV da ton tai trong he thong!", 12, 45, 24);
                                _getch();
                                InThongBaoContent("                                ", 12, 45, 24);
                                currentField = 0; 
                            } 
                            else {
                                isDone = true; 
                            }
                        }
                    }
                }
                if (isDone) {
                    SinhVien svSua;
                    snprintf(svSua.maSV, sizeof(svSua.maSV), "%s", ma.c_str());
                    snprintf(svSua.ho, sizeof(svSua.ho), "%s", ho.c_str());
                    snprintf(svSua.ten, sizeof(svSua.ten), "%s", ten.c_str());
                    svSua.phai = phai;
                    strcpy(svSua.password, svTarget->password); 
                    svSua.headDiem = svTarget->headDiem; 

                    int res = suaSinhVien(lopHienTai, maCu.c_str(), svSua);
                    if (res == SUCCESS) {
                        InThongBaoContent("Sua sinh vien thanh cong!", 10, 45, 24);
                        SaveLop(dsLop); 
                    } else {
                        InThongBaoContent("Loi khi sua sinh vien!", 12, 45, 24);
                    }
                    _getch();
                }
            }
            ShowCur(false);
            forceRedraw = 1;
        }        
    }
}

MonHoc* TimKiemMonHocThongMinh(DanhSachMonHoc &dsMH) {
    string tuKhoa = "";
    int cursor = 0;

    while (true) {
        XoaVungContent();
        InTitle("TIM KIEM MON HOC");
        gotoxy(35, 8); SetColor(0, 14); cout << "Nhap (Ma MH/Ten MH): ";
        SetColor(0, 15); cout << tuKhoa << "_"; 
        
        gotoxy(35, 11); SetColor(0, 14); 

        printf(" %-5s %-15s %-35s %-10s", "STT", "Ma MH", "Ten Mon Hoc", "Tin Chi");
        
        string tuKhoaLow = ToLower(tuKhoa);
        
        MonHoc* dsLoc[500]; 
        int sttGoc[500];
        int soLuongLoc = 0;

        for (int i = 0; i < dsMH.soLuong; i++) {
            string maMHLow = ToLower(dsMH.data[i].maMH);
            string tenMHLow = ToLower(dsMH.data[i].tenMH);
            
            if (tuKhoa.empty() || maMHLow.find(tuKhoaLow) != string::npos || tenMHLow.find(tuKhoaLow) != string::npos) {
                dsLoc[soLuongLoc] = &dsMH.data[i]; 
                sttGoc[soLuongLoc] = i + 1;
                soLuongLoc++;
            }
        }
        
        if (soLuongLoc == 0) cursor = 0;
        else if (cursor >= soLuongLoc) cursor = soLuongLoc - 1;
        else if (cursor < 0) cursor = 0;

        int yPos = 13;
        int maxDong = 12;
        
        int batDau = (cursor / maxDong) * maxDong;
        int ketThuc = batDau + maxDong;
        if (ketThuc > soLuongLoc) ketThuc = soLuongLoc;

        for (int i = batDau; i < ketThuc; i++) {
            gotoxy(35, yPos);
            
            if (i == cursor) SetColor(15, 0);
            else SetColor(0, 7); 

            string ma = string(dsLoc[i]->maMH);
            if (ma.length() > 15) ma = ma.substr(0, 15);
            
            string ten = string(dsLoc[i]->tenMH);
            if (ten.length() > 35) ten = ten.substr(0, 32) + "...";
            
            printf(" %-5d %-15s %-35s %-10d", sttGoc[i], ma.c_str(), ten.c_str(), dsLoc[i]->soTinChi);
            
            SetColor(0, 7); 
            yPos++;
        }

        gotoxy(35, 27); SetColor(0, 11); cout << "ENTER: Chon | BACKSPACE: Xoa | ESC: Thoat";
        SetColor(0, 7);
        
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch(); 
            if (ch == 72) { if (cursor > 0) cursor--; } 
            else if (ch == 80) { if (cursor < soLuongLoc - 1) cursor++; } 
            continue; 
        }
        else if (ch == 27) return NULL; 
        else if (ch == 8) { 
            if (!tuKhoa.empty()) {
                tuKhoa.pop_back(); 
                cursor = 0; 
            }
        }
        else if (ch == 13) { 
            if (soLuongLoc > 0) return dsLoc[cursor]; 
        }
        else if (ch >= 32 && ch <= 126) { 
            if (ch == 32 && tuKhoa.empty()) continue; 
            if (tuKhoa.length() < 25) { 
                tuKhoa += (char)ch;
                cursor = 0; 
            }
        }
    }
}

void flowQuanLyMonHoc_CauHoi(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
    XoaVungContent();
    int cursor = 0;
    int itemsPerPage = 8;

    int oldCursor = -1;
    int oldPage = -1;
    int forceRedraw = 1;
    
    while (true) {
        ShowCur(false);
        int totalItems = dsMH.soLuong;
        int totalPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;
        int currentPage = (totalItems == 0) ? 0 : (cursor / itemsPerPage);

        if (forceRedraw || currentPage != oldPage) {
            XoaVungContent();
            InTitle("DANH SACH MON HOC");
            gotoxy(105, 27);  SetColor(0, 10); cout << "(Trang " << currentPage + 1 << "/" << totalPages << ")";

            gotoxy(35, 8); SetColor(0, 14);
            printf("%-5s %-15s %-35s %-5s", "STT", "Ma Mon", "Ten Mon Hoc", "STC");
            SetColor(0, 7);
            int startIdx = currentPage * itemsPerPage;
            int endIdx = (startIdx + itemsPerPage > totalItems) ? totalItems : (startIdx + itemsPerPage);

            if (totalItems == 0) {
                SetColor(0, 12);
                gotoxy(55, 12); cout << "Danh sach mon hoc dang trong!";
                gotoxy(55, 13); cout << "(Vui long them mon hoc truoc)";
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
            cout << "Up/Down: Chon | Left/Right: Chuyen trang | [ENTER] Xem cau hoi"; 
            gotoxy(35, 27); SetColor(0, 11);
            cout << "[INS/I] Them | [E] Sua | [DEL] Xoa | [F] Tim Kiem  | [ESC] Thoat";
            SetColor(0, 7);

            oldPage = currentPage;
            oldCursor = cursor;
            forceRedraw = false;
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
            flowQuanLyCauHoiCuaMon(dsMH.data[cursor].maMH,dsMH.data[cursor].tenMH, root);
            forceRedraw = true;
        }
        else if (ch == 27) break;

        else if (toupper(ch) == 'I') {
            XoaVungContent();
            gotoxy(100, 27); cout << "ESC: de thoat";
            InTitle("THEM MON HOC");
            gotoxy(40, 10); cout << "Ma Mon:";
            gotoxy(40, 12); cout << "Ten Mon Hoc:";
            gotoxy(40, 14); cout << "So Tin Chi:";
            int xNhap = 54;
            string ma = "", ten = "", stc_str = "";
            int currentField = 0;
            bool isDone = false;
            bool isCancel = false;

            ShowCur(true);
            while (!isDone && !isCancel) {
                int keyLog = 0;
                
                if (currentField == 0) {
                    ma = NhapCoDieuHuong(xNhap, 10, 15, 1, ma, keyLog);
                } else if (currentField == 1) {
                    ten = NhapCoDieuHuong(xNhap, 12, 35, 1, ten, keyLog);
                } else if (currentField == 2) {
                    stc_str = NhapCoDieuHuong(xNhap, 14, 2, 1, stc_str, keyLog);
                }
                
                if (keyLog == 27) {
                    InThongBaoContent("Da huy thao tac them mon hoc!", 14, 50, 26);
                    _getch();
                    isCancel = true;
                } 
                else if (keyLog == 72) {
                    if (currentField > 0) currentField--;
                } 
                else if (keyLog == 80) {
                    if (currentField < 2) currentField++;
                } 
                else if (keyLog == 13) {
                    if (currentField < 2) {
                        if (currentField == 0) {
                            while (!ma.empty() && ma.back() == ' ') ma.pop_back();
                            gotoxy(xNhap, 10); cout << ma << "          ";
                        } else if (currentField == 1) {
                            while (!ten.empty() && ten.back() == ' ') ten.pop_back();
                            gotoxy(xNhap, 12); cout << ten << "          ";
                        }
                        currentField++;
                    } else {
                        while (!ma.empty() && ma.back() == ' ') ma.pop_back();
                        while (!ten.empty() && ten.back() == ' ') ten.pop_back();
                        while (!stc_str.empty() && stc_str.back() == ' ') stc_str.pop_back();

                        gotoxy(xNhap, 10); cout << ma << "          ";
                        gotoxy(xNhap, 12); cout << ten << "          ";
                        gotoxy(xNhap, 14); cout << stc_str << "   ";

                        if (ma.empty() || ten.empty() || stc_str.empty()) {
                            InThongBaoContent("Vui long nhap day du thong tin!", 12, 45, 26);
                            _getch(); 
                            gotoxy(45, 26); cout << "                                     ";
                            continue;
                        }

                        for (int i = 0; i < ma.length(); i++) ma[i] = toupper(ma[i]);

                        if (kiemTraTrungMaMonHoc(dsMH, (char*)ma.c_str())) {
                            InThongBaoContent("Loi: Ma mon hoc da ton tai!", 12, 50, 26);
                            _getch(); 
                            gotoxy(50, 26); cout << "                                ";
                            currentField = 0;
                            continue;
                        }
                        int stc = atoi(stc_str.c_str());
                        if (stc <= 0 || stc > 10) {
                            InThongBaoContent("So tin chi phai tu 1 den 10!", 12, 45, 26);
                            _getch(); 
                            gotoxy(45, 26); cout << "                                ";
                            currentField = 2;
                            continue;
                        }

                        ShowCur(false);
                        MonHoc mhMoi;
                        strcpy(mhMoi.maMH, ma.c_str());
                        strcpy(mhMoi.tenMH, ten.c_str());
                        mhMoi.soTinChi = stc;
                        int res = themMonHoc(dsMH, &mhMoi);
                        if (res == SUCCESS) {
                            SaveMonHoc(dsMH);
                            KiemTraVaTaoFileCauHoi(mhMoi.maMH, mhMoi.tenMH);
                            InThongBaoContent("Them mon hoc thanh cong!", 10, 50, 26);
                            cursor = dsMH.soLuong - 1;
                            _getch();
                            isDone = true;
                        } else {
                            InThongBaoContent(layThongBaoLoi(res), 12, 50, 26);
                            _getch();
                        }
                    }
                }
            }
            ShowCur(false);
            forceRedraw = true;
        }   

        else if (toupper(ch) == 'E' && totalItems > 0) { 
            XoaVungContent();
            gotoxy(100, 27); cout << "ESC: de thoat";
            InTitle("SUA MON HOC");
        
            gotoxy(40, 10); cout << "Ma mon hoc    : " << dsMH.data[cursor].maMH;
            gotoxy(40, 12); cout << "Ten mon moi   : ";
            gotoxy(40, 14); cout << "So tin chi moi: ";
        
            int xNhap = 56;
            string tenMoi = dsMH.data[cursor].tenMH;

            char buffer[10];
            itoa(dsMH.data[cursor].soTinChi, buffer, 10);
            string stcMoi_str = string(buffer);
            
            int currentField = 0;
            bool isDone = false;
            gotoxy(xNhap, 12); cout << tenMoi;
            gotoxy(xNhap, 14); cout << stcMoi_str;
            ShowCur(true);
            while (!isDone) {
                int keyLog = 0;
                if (currentField == 0) {
                    tenMoi = NhapCoDieuHuong(xNhap, 12, 35, 1, tenMoi, keyLog);
                } else if (currentField == 1) {
                    stcMoi_str = NhapCoDieuHuong(xNhap, 14, 2, 1, stcMoi_str, keyLog);
                }

                if (keyLog == 27) {
                    InThongBaoContent("Da huy thao tac sua mon hoc!", 14, 50, 26);
                    _getch();
                    isDone = true;
                } 
                else if (keyLog == 72) {
                    if (currentField > 0) currentField--;
                } 
                else if (keyLog == 80) {
                    if (currentField < 1) currentField++;
                } 
                else if (keyLog == 13) {
                    if (currentField < 1) {
                        if (currentField == 0) {
                            while (!tenMoi.empty() && tenMoi.back() == ' ') tenMoi.pop_back();
                            gotoxy(xNhap, 12); cout << tenMoi << "          ";
                        }
                        currentField++;
                    } else {
                        while (!tenMoi.empty() && tenMoi.back() == ' ') tenMoi.pop_back();
                        while (!stcMoi_str.empty() && stcMoi_str.back() == ' ') stcMoi_str.pop_back();
                        
                        gotoxy(xNhap, 12); cout << tenMoi << "          ";
                        gotoxy(xNhap, 14); cout << stcMoi_str << "   ";

                        if (tenMoi.empty() || stcMoi_str.empty()) {
                            InThongBaoContent("Khong duoc de trong thong tin!", 12, 45, 26);
                            _getch(); 
                            gotoxy(45, 26); cout << "                                     ";
                            continue;
                        }
                        int stcMoi = atoi(stcMoi_str.c_str());
                        if (stcMoi <= 0 || stcMoi > 10) {
                            InThongBaoContent("So tin chi phai tu 1 den 10!", 12, 45, 26);
                            _getch(); 
                            gotoxy(45, 26); cout << "                                ";
                            currentField = 1;
                            continue;
                        }
                        ShowCur(false);
                        int res = suaMonHoc(dsMH, dsMH.data[cursor].maMH, tenMoi.c_str(), stcMoi);
                        if (res == SUCCESS) {
                            SaveMonHoc(dsMH);
                            InThongBaoContent("Cap nhat thanh cong!", 10, 50, 26);
                            _getch();
                            isDone = true;
                        } else {
                            InThongBaoContent(layThongBaoLoi(res), 12, 50, 26);
                            _getch();
                        }
                    }
                }
            }
            ShowCur(false);
            forceRedraw = true;
        }
        else if ((ch == 83 || toupper(ch) == 'D') && totalItems > 0) {
            gotoxy(40, 25); SetColor(0, 14); 
            string thongBao = "Xac nhan xoa mon: " + string(dsMH.data[cursor].tenMH) + " ";
            cout << thongBao;
            if (XacNhan(40 + thongBao.length(), 25) == 'Y') {
                if (KiemTraMonHocDaDuocThi(dsLop, dsMH.data[cursor].maMH)) {
                    gotoxy(40, 25); cout << "                                             ";
                    InThongBaoContent("Khong the xoa: mon hoc da co diem thi!", 12, 40, 25);
                } else {
                    int res = xoaMonHoc(dsMH, dsMH.data[cursor].maMH, root);
                    if (res == SUCCESS) {
                        if (cursor >= dsMH.soLuong && cursor > 0) cursor--;
                        SaveMonHoc(dsMH);
                        gotoxy(40, 25); cout << "                                             ";
                        InThongBaoContent("Da xoa mon hoc thanh cong!", 10, 40, 25);
                    } else {
                        gotoxy(40, 25); cout << "                                             ";
                        InThongBaoContent(layThongBaoLoi(res), 12, 40, 25);
                    }
                }
            } else {
                gotoxy(40, 25); cout << "                                             ";
                InThongBaoContent("Da huy thao tac xoa!", 10, 40, 25);
            }
            _getch(); 
            forceRedraw = true;
        }
        else if (toupper(ch) == 'F') {
            if (totalItems > 0) {
                MonHoc* monDuocChon = TimKiemMonHocThongMinh(dsMH);
                if (monDuocChon != NULL) {
                    flowQuanLyCauHoiCuaMon(monDuocChon->maMH, monDuocChon->tenMH, root); 
                }
            } else {
                InThongBaoContent("Danh sach mon hoc dang trong!", 12, 50, 25);
                _getch();
            }
            forceRedraw = true;
        }
    }
}
void flowQuanLyCauHoiCuaMon(const char* maMH, const char* tenMH, PTR_CAUHOI &root) {
    KiemTraVaTaoFileCauHoi(maMH, tenMH);
    
    int currentPage = 0;
    int itemsPerPage = 1; 

    while (true) {
        ShowCur(false);
        XoaVungContent();
        InTitle("DANH SACH CAU HOI");
        InSmallTitle("MON: " + string(tenMH) + " - MA: " + string(maMH));

        CauHoi* dsTam[1000];
        int totalItems = 0;
        LayDanhSachCauHoiTheoMon(root, maMH, dsTam, totalItems);

        int totalPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;
        if (currentPage >= totalPages) currentPage = totalPages - 1;
        if (currentPage < 0) currentPage = 0;

        gotoxy(103, 27); SetColor(0, 10);
        cout << "(Trang " << currentPage + 1 << "/" << totalPages << ") ";
        SetColor(0, 7);

        if (totalItems == 0) {
            SetColor(0, 12);
            gotoxy(55, 12); cout << "Mon nay chua co cau hoi nao!";
        } else {
            int i = currentPage; 
            if (i >= 0 && i < totalItems) {
                int yPos = 6;
                int startX = 35;
                int idLen = strlen(dsTam[i]->id);
                int idPadding = idLen + 3;
                int contentX = startX + idPadding;
                
                SetColor(0, 8);
                gotoxy(35, yPos );
                cout << "Cau: " << (currentPage + 1) << " / " << totalItems;

                SetColor(0, 7);
                cout << " - ID: ";
        
                SetColor(0, 14);
                cout << "[" << dsTam[i]->id << "] ";

                SetColor(0, 11);
                gotoxy(35, yPos + 1); cout << "Noi dung: ";
            
                SetColor(0, 7);
                InNoiDungCanLe(dsTam[i]->noiDung, 46, yPos + 1, 63); 
        
                gotoxy(35, yPos + 6);  cout << "A."; InNoiDungCanLe(dsTam[i]->a, 39, yPos + 6, 75);
                gotoxy(35, yPos + 9);  cout << "B."; InNoiDungCanLe(dsTam[i]->b, 39, yPos + 9, 75);
                gotoxy(35, yPos + 12); cout << "C."; InNoiDungCanLe(dsTam[i]->c, 39, yPos + 12, 75);
                gotoxy(35, yPos + 15); cout << "D."; InNoiDungCanLe(dsTam[i]->d, 39, yPos + 15, 75);
                
                gotoxy(35, yPos + 18); SetColor(0, 11);
                cout << ">> Dap an dung: " << dsTam[i]->dapAn;
                
            }
        }
        
        gotoxy(35, 26); SetColor(0, 8);
        cout << "Left/Right: Chuyen cau hoi"; 
        gotoxy(35, 27); SetColor(0, 11);
        cout << "[INS/I] Them | [E] Sua | [DEL] Xoa | [ESC] Thoat";
        SetColor(0, 7);
        
        int ch = _getch();
        if (ch == 224 || ch == 0 || ch == -32) {
            ch = _getch();
            if (ch == 75 && currentPage > 0) currentPage--;     
            else if (ch == 77 && currentPage < totalItems - 1) currentPage++; 
        }
        else if (ch == 27) break; 
         
        // --- Logic Them (I) ---
        else if (toupper(ch) == 'I') {
            char dsDapAn[] = {'A', 'B', 'C', 'D'};
            XoaVungContent();
            ShowCur(true);
            while (_kbhit()) _getch();
            InTitle("THEM CAU HOI");
            InSmallTitle("MON: " + string(tenMH) + " - MA: " + string(maMH));
            gotoxy(35, 8);  cout << "Noi Dung   : ";
            gotoxy(35, 14); cout << "A. ";
            gotoxy(35, 16); cout << "B. ";
            gotoxy(35, 18); cout << "C. ";
            gotoxy(35, 20); cout << "D. ";
            gotoxy(35, 22); cout << "Dap An Dung: ";
            for (int i = 0; i < 4; i++) {
                gotoxy(48 + (i * 6), 22);
                SetColor(0, 7);
                cout << " " << dsDapAn[i] << " ";
            }

            gotoxy(35, 26); SetColor(0, 8);
            cout << "ESC: Thoat | UP/DOWN: Di chuyen | ENTER: Xac nhan";
            SetColor(0, 7);
            
            
            SetColor(0, 7);
 
            // Du lieu tam cua form
            string t_ND = "", t_A = "", t_B = "", t_C = "", t_D = "";
            char t_DA = 'A';
 
            int field = 0;   // 0=ND, 1=A, 2=B, 3=C, 4=D, 5=DapAn
            int keyLog = 0;
            bool exitForm = false;
            bool huyBo = false;
 
            while (!exitForm) {

                if (field == 5) {
                    ShowCur(false);
                } else {
                    ShowCur(true);
                }

                // Highlight label cua field hien tai
                SetColor(0, 11);
                switch (field) {
                    case 0: gotoxy(35, 8);  cout << "Noi Dung   :"; break;
                    case 1: gotoxy(35, 14); cout << "A."; break;
                    case 2: gotoxy(35, 16); cout << "B."; break;
                    case 3: gotoxy(35, 18); cout << "C."; break;
                    case 4: gotoxy(35, 20); cout << "D."; break;
                    case 5: gotoxy(35, 22); cout << "Dap An Dung:"; break;
                }
                SetColor(0, 7);
 
                // Nhap field hien tai
                switch (field) {
                    case 0: t_ND = xuongdongGioiHan(48, 8,  67, 4, 244, (t_ND.empty() ? NULL : (char*)t_ND.c_str()), &keyLog); break;
                    case 1: t_A  = xuongdongGioiHan(38, 14, 75, 2, 153, (t_A.empty()  ? NULL : (char*)t_A.c_str()),  &keyLog); break;
                    case 2: t_B  = xuongdongGioiHan(38, 16, 75, 2, 153, (t_B.empty()  ? NULL : (char*)t_B.c_str()),  &keyLog); break;
                    case 3: t_C  = xuongdongGioiHan(38, 18, 75, 2, 153, (t_C.empty()  ? NULL : (char*)t_C.c_str()),  &keyLog); break;
                    case 4: t_D  = xuongdongGioiHan(38, 20, 75, 2, 153, (t_D.empty()  ? NULL : (char*)t_D.c_str()),  &keyLog); break;
                    case 5:
                        t_DA = ChonDapAn(48, 22, &keyLog);
                        break;
                }
 
                // Tat highlight label
                SetColor(0, 7);
                switch (field) {
                    case 0: gotoxy(35, 8);  cout << "Noi Dung   :"; break;
                    case 1: gotoxy(35, 14); cout << "A."; break;
                    case 2: gotoxy(35, 16); cout << "B."; break;
                    case 3: gotoxy(35, 18); cout << "C."; break;
                    case 4: gotoxy(35, 20); cout << "D."; break;
                    case 5: gotoxy(35, 22); cout << "Dap An Dung:"; break;
                }
 
                // Xu ly ket qua phim
                if (keyLog == 27) {
                    gotoxy(40, 24); SetColor(0, 14); cout << "Ban co chac muon huy them cau hoi?";
                    if (XacNhan(76, 24) == 'Y') {
                        huyBo = true;
                        exitForm = true;
                    } else {
                        gotoxy(40, 24); cout << "                                                  ";
                    }
                    continue;
                }
 
                if (keyLog == 72) {          // Up: chuyen len field truoc
                    field = (field > 0) ? field - 1 : 5;
                }
                else if (keyLog == 80 || keyLog == 13) {  // Down hoac Enter
                    if (field == 5 && keyLog == 13) {
                        // Validate truoc khi luu
                        bool ok = true;
                        if (t_ND.empty()) {
                            InThongBaoContent("Noi dung khong duoc de trong!", 12, 45, 25);
                            ok = false; field = 0; 
                        } else if (t_A.empty()) {
                            InThongBaoContent("Dap an A khong duoc de trong!", 12, 45, 25);
                            ok = false; field = 1; 
                        } else if (t_B.empty()) {
                            InThongBaoContent("Dap an B khong duoc de trong!", 12, 45, 25);
                            ok = false; field = 2; 
                        } else if (t_C.empty()) {
                            InThongBaoContent("Dap an C khong duoc de trong!", 12, 45, 25);
                            ok = false; field = 3; 
                        } else if (t_D.empty()) {
                            InThongBaoContent("Dap an D khong duoc de trong!", 12, 45, 25);
                            ok = false; field = 4; 
                        }
 
                        if (ok) {
                            CauHoi* newNode = new CauHoi();
                            TaoIdCauHoi(newNode->id, maMH, root); // Truyền root vào để tự sinh và check trùng
                            newNode->dapAn = t_DA;
                            newNode->left = newNode->right = NULL;
                            strcpy(newNode->maMH, maMH);
                            strcpy(newNode->noiDung, t_ND.c_str());
                            strcpy(newNode->a, t_A.c_str());
                            strcpy(newNode->b, t_B.c_str());
                            strcpy(newNode->c, t_C.c_str());
                            strcpy(newNode->d, t_D.c_str());
 
                            root = ThemCauHoi(root, newNode);
                            LuuFileCauHoiTheoMon(maMH, tenMH, root);
                            InThongBaoContent("Them cau hoi thanh cong!", 10, 45, 25);
                            exitForm = true;
                        } else {
                            _getch();
                            gotoxy(45, 25); cout << "                                        ";
                        }
                    } else {
                        field = (field < 5) ? field + 1 : 0;
                    }
                }
            }
            if (!huyBo) _getch();
        }
        
        
        // --- Logic Sua (E) ---

         else if (toupper(ch) == 'E') {
            if (totalItems > 0) {
                CauHoi* q = dsTam[currentPage];
 
                XoaVungContent();
                ShowCur(true);
                while (_kbhit()) _getch();
                SetColor(0, 11);
                InTitle("SUA CAU HOI");
                InSmallTitle("MON: " + string(tenMH) + " - MA: " + string(maMH));
 
                // Load du lieu cu vao bien tam
                string t_ND = q->noiDung, t_A = q->a, t_B = q->b, t_C = q->c, t_D = q->d;
                char t_DA = q->dapAn;
 
                // In nhan cac truong
                SetColor(0, 7);
                gotoxy(35, 8);  cout << "Noi Dung   : ";
                gotoxy(35, 14); cout << "A. ";
                gotoxy(35, 16); cout << "B. ";
                gotoxy(35, 18); cout << "C. ";
                gotoxy(35, 20); cout << "D. ";
                gotoxy(35, 22); cout << "Dap An Dung: ";
                gotoxy(35, 25); SetColor(0, 8);
                cout << "Up/Down/Enter: Chuyen truong | Enter o Dap An: Luu | ESC: Huy";
                SetColor(0, 7);
 
                // Render toan bo du lieu cu ngay tu dau
                InNoiDungCanLe(t_ND, 48, 8,  67);
                InNoiDungCanLe(t_A,  38, 14, 75);
                InNoiDungCanLe(t_B,  38, 16, 75);
                InNoiDungCanLe(t_C,  38, 18, 75);
                InNoiDungCanLe(t_D,  38, 20, 75);
                {
                    char dsDapAn[] = {'A','B','C','D'};
                    for (int i = 0; i < 4; i++) {
                        gotoxy(48 + i * 6, 22);
                        if (dsDapAn[i] == t_DA) { SetColor(0, 11); cout << "[" << dsDapAn[i] << "]"; }
                        else                     { SetColor(0, 7);  cout << " " << dsDapAn[i] << " "; }
                    }
                    SetColor(0, 7);
                }
 
                int field = 0, keyLog = 0;
                bool exitForm = false;
 
                while (!exitForm) {
                    // Highlight label field dang active
                    SetColor(0, 11);
                    switch (field) {
                        case 0: gotoxy(35, 8);  cout << "Noi Dung   :"; break;
                        case 1: gotoxy(35, 14); cout << "A."; break;
                        case 2: gotoxy(35, 16); cout << "B."; break;
                        case 3: gotoxy(35, 18); cout << "C."; break;
                        case 4: gotoxy(35, 20); cout << "D."; break;
                        case 5: gotoxy(35, 22); cout << "Dap An Dung:"; break;
                    }
                    SetColor(0, 7);
 
                    // Nhap: dung xuongdongGioiHan (pre-load + word-wrap chinh xac + cursor cuoi)
                    switch (field) {
                        case 0: t_ND = xuongdongGioiHan(48, 8,  67, 4, 244, (char*)t_ND.c_str(), &keyLog); break;
                        case 1: t_A  = xuongdongGioiHan(38, 14, 75, 2, 153, (char*)t_A.c_str(),  &keyLog); break;
                        case 2: t_B  = xuongdongGioiHan(38, 16, 75, 2, 153, (char*)t_B.c_str(),  &keyLog); break;
                        case 3: t_C  = xuongdongGioiHan(38, 18, 75, 2, 153, (char*)t_C.c_str(),  &keyLog); break;
                        case 4: t_D  = xuongdongGioiHan(38, 20, 75, 2, 153, (char*)t_D.c_str(),  &keyLog); break;
                        case 5:
                            t_DA = ChonDapAn(48, 22, &keyLog);
                            break;
                    }
 
                    // Tat highlight label
                    SetColor(0, 7);
                    switch (field) {
                        case 0: gotoxy(35, 8);  cout << "Noi Dung   :"; break;
                        case 1: gotoxy(35, 14); cout << "A."; break;
                        case 2: gotoxy(35, 16); cout << "B."; break;
                        case 3: gotoxy(35, 18); cout << "C."; break;
                        case 4: gotoxy(35, 20); cout << "D."; break;
                        case 5: gotoxy(35, 22); cout << "Dap An Dung:"; break;
                    }
 
                    // Xu ly phim
                    if (keyLog == 27) {
                        gotoxy(40, 24); SetColor(0, 14); cout << "Ban co chac muon huy sua cau hoi?";
                        if (XacNhan(76, 24) == 'Y') {
                            exitForm = true;
                        } else {
                            gotoxy(40, 24); cout << "                                                  ";
                        }
                        continue;
                    }
 
                    if (keyLog == 72) {          // Up
                        field = (field > 0) ? field - 1 : 5;
                    }
                    else if (keyLog == 80 || keyLog == 13) {  // Down hoac Enter
                        if (field == 5 && keyLog == 13) {
                            // Validate
                            bool ok = true;
                            if (t_ND.empty()) {
                                InThongBaoContent("Noi dung khong duoc de trong!", 12, 45, 25);
                                ok = false; field = 0;
                            } else if (t_A.empty()) {
                                InThongBaoContent("Dap an A khong duoc de trong!", 12, 45, 25);
                                ok = false; field = 1;
                            } else if (t_B.empty()) {
                                InThongBaoContent("Dap an B khong duoc de trong!", 12, 45, 25);
                                ok = false; field = 2;
                            } else if (t_C.empty()) {
                                InThongBaoContent("Dap an C khong duoc de trong!", 12, 45, 25);
                                ok = false; field = 3;
                            } else if (t_D.empty()) {
                                InThongBaoContent("Dap an D khong duoc de trong!", 12, 45, 25);
                                ok = false; field = 4;
                            }
 
                            if (ok) {
                                strcpy(q->noiDung, t_ND.c_str());
                                strcpy(q->a, t_A.c_str());
                                strcpy(q->b, t_B.c_str());
                                strcpy(q->c, t_C.c_str());
                                strcpy(q->d, t_D.c_str());
                                q->dapAn = t_DA;
                                LuuFileCauHoiTheoMon(maMH, tenMH, root);
                                InThongBaoContent(" CAP NHAT THANH CONG! ", 10, 55, 24);
                                Sleep(1500);
                                exitForm = true;
                            } else {
                                _getch();
                                gotoxy(45, 25); cout << "                                        ";
                            }
                        } else {
                            field = (field < 5) ? field + 1 : 0;
                        }
                    }
                }
                continue;
            }
        }
        
        else if (toupper(ch) == 'D') {
            if (totalItems > 0) { 
                gotoxy(65, 25); 
                SetColor(0, 14);  cout << "Xac nhan xoa cau hoi ";
                
                if (XacNhan(88, 25) == 'Y') {
                    root = XoaCauHoi(root, dsTam[currentPage]->id);
                    
                    LuuFileCauHoiTheoMon(maMH, tenMH, root);
                    gotoxy(65, 25); 
                    SetColor(0, 10);
                    cout << "Da xoa cau hoi!                      "; 

                    if (currentPage > 0) {
                        currentPage--; 
                    } 

                    Sleep(800); 
                } 
                else { 
                    gotoxy(65, 25); 
                    SetColor(0, 12);
                    cout << "Da huy thao tac!                                 "; 
                    Sleep(800);
                }
            }
        }
    }
}

void flowQuanLyDiemSV(DanhSachMonHoc &dsMH, DanhSachLop &dsLop) {
    if (dsMH.soLuong == 0 || dsLop.soLuong == 0) {
        InThongBaoContent("He thong chua co du lieu Lop hoac Mon Hoc!", 12, 45, 15);
        _getch(); SetColor(0, 7);
        return;
    }
    int cursorMH = 0;
    int itemsPerPage = 8;
    bool forceRedrawMH = true;
    int oldCursorMH = -1;
    int oldPageMH = -1;

    // ================= MÀN HÌNH 1: CHỌN MÔN HỌC =================
    while (true) {
        int totalMH = dsMH.soLuong;
        int totalPagesMH = (totalMH == 0) ? 1 : (totalMH + itemsPerPage - 1) / itemsPerPage;
        int currentPageMH = (totalMH == 0) ? 0 : cursorMH / itemsPerPage;

        if (forceRedrawMH || currentPageMH != oldPageMH) {
            XoaVungContent();
            InTitle("QUAN LY DIEM - CHON MON HOC");
            gotoxy(105, 27); SetColor(0, 10); cout << "(Trang " << currentPageMH + 1 << "/" << totalPagesMH << ")";
            
            gotoxy(35, 8); SetColor(0, 14);
            printf("%-5s %-15s %-35s", "STT", "Ma Mon", "Ten Mon Hoc");
            SetColor(0, 7);
            
            int startMH = currentPageMH * itemsPerPage;
            int endMH = (startMH + itemsPerPage > totalMH) ? totalMH : (startMH + itemsPerPage);

            for (int i = startMH; i < endMH; i++) {
                int toadoY = 10 + (i - startMH) * 2;
                if (i == cursorMH) SetColor(11, 0); else SetColor(0, 7);
                gotoxy(35, toadoY);
                printf("%-5d %-15s %-35s", i + 1, dsMH.data[i].maMH, dsMH.data[i].tenMH);
            }
            SetColor(0, 7);
            gotoxy(35, 26); SetColor(0, 8); cout << "Up/Down: Chon | Left/Right: Chuyen trang | [ENTER] Chon mon"; 
            gotoxy(35, 27); SetColor(0, 11); cout << "[ESC] Thoat";
            
            oldPageMH = currentPageMH;
            oldCursorMH = cursorMH;
            forceRedrawMH = false;
        }
        else if (oldCursorMH != cursorMH) {
            int startMH = currentPageMH * itemsPerPage;
            if (oldCursorMH >= startMH && oldCursorMH < startMH + itemsPerPage) {
                SetColor(0, 7); gotoxy(35, 10 + (oldCursorMH - startMH) * 2);
                printf("%-5d %-15s %-35s", oldCursorMH + 1, dsMH.data[oldCursorMH].maMH, dsMH.data[oldCursorMH].tenMH);
            }
            SetColor(11, 0); gotoxy(35, 10 + (cursorMH - startMH) * 2);
            printf("%-5d %-15s %-35s", cursorMH + 1, dsMH.data[cursorMH].maMH, dsMH.data[cursorMH].tenMH);
            oldCursorMH = cursorMH;
            SetColor(0, 7);
        }

        int ch = _getch();
        if (ch == 224 || ch == 0 || ch == -32) {
            ch = _getch();
            if (ch == 72 && cursorMH > 0) cursorMH--;
            else if (ch == 80 && cursorMH < totalMH - 1) cursorMH++;
            else if (ch == 75 && currentPageMH > 0) cursorMH = (currentPageMH - 1) * itemsPerPage;
            else if (ch == 77 && currentPageMH < totalPagesMH - 1) cursorMH = (currentPageMH + 1) * itemsPerPage;
        }
        else if (ch == 27) break;
        else if (ch == 13) {

            // ================= MÀN HÌNH 2: CHỌN LỚP HỌC =================
            int cursorLop = 0;
            int oldCursorLop = -1;
            int oldPageLop = -1;
            bool forceRedrawLop = true;
            
            while (true) {
                int totalLop = dsLop.soLuong;
                int totalPagesLop = (totalLop == 0) ? 1 : (totalLop + itemsPerPage - 1) / itemsPerPage;
                int currentPageLop = (totalLop == 0) ? 0 : cursorLop / itemsPerPage;
                
                if (forceRedrawLop || currentPageLop != oldPageLop) {
                    XoaVungContent();
                    InTitle("QUAN LY DIEM - CHON LOP HOC");
                    InSmallTitle("MON DA CHON: " + string(dsMH.data[cursorMH].tenMH) + " (" + string(dsMH.data[cursorMH].maMH) + ")");
                    
                    gotoxy(105, 27); SetColor(0, 10); cout << "(Trang " << currentPageLop + 1 << "/" << totalPagesLop << ")";
                    
                    gotoxy(35, 8); SetColor(0, 14);
                    printf("%-5s %-16s %-35s", "STT", "Ma Lop", "Ten Lop");
                    SetColor(0, 7);

                    int startLop = currentPageLop * itemsPerPage;
                    int endLop = (startLop + itemsPerPage > totalLop) ? totalLop : (startLop + itemsPerPage);

                    for (int i = startLop; i < endLop; i++) {
                        int toadoY = 10 + (i - startLop) * 2;
                        if (i == cursorLop) SetColor(11, 0); else SetColor(0, 7);
                        gotoxy(35, toadoY);
                        printf("%-5d %-16s %-35s", i + 1, dsLop.nodes[i]->maLop, dsLop.nodes[i]->tenLop);
                    }
                    SetColor(0, 7);
                    gotoxy(35, 26); SetColor(0, 8); cout << "Up/Down: Chon | Left/Right: Chuyen trang | [ENTER] Xem bang diem"; 
                    gotoxy(35, 27); SetColor(0, 11); cout << "[ESC] Quay lai chon Mon Hoc"; SetColor(0, 7);

                    oldPageLop = currentPageLop;
                    oldCursorLop = cursorLop;
                    forceRedrawLop = false;
                }
                else if (oldCursorLop != cursorLop) {
                    int startLop = currentPageLop * itemsPerPage;
                    if (oldCursorLop >= startLop && oldCursorLop < startLop + itemsPerPage) {
                        SetColor(0, 7); gotoxy(35, 10 + (oldCursorLop - startLop) * 2);
                        printf("%-5d %-16s %-35s", oldCursorLop + 1, dsLop.nodes[oldCursorLop]->maLop, dsLop.nodes[oldCursorLop]->tenLop);
                    }
                    SetColor(11, 0); gotoxy(35, 10 + (cursorLop - startLop) * 2);
                    printf("%-5d %-16s %-35s", cursorLop + 1, dsLop.nodes[cursorLop]->maLop, dsLop.nodes[cursorLop]->tenLop);
                    oldCursorLop = cursorLop;
                    SetColor(0, 7);
                }

                int ch2 = _getch();
                if (ch2 == 224 || ch2 == 0 || ch2 == -32) {
                    ch2 = _getch();
                    if (ch2 == 72 && cursorLop > 0) cursorLop--;
                    else if (ch2 == 80 && cursorLop < totalLop - 1) cursorLop++;
                    else if (ch2 == 75 && currentPageLop > 0) cursorLop = (currentPageLop - 1) * itemsPerPage;
                    else if (ch2 == 77 && currentPageLop < totalPagesLop - 1) cursorLop = (currentPageLop + 1) * itemsPerPage;
                }
                else if (ch2 == 27) {
                    forceRedrawMH = true; 
                    break; 
                }
                else if (ch2 == 13) {

                    // ================= MÀN HÌNH 3: BẢNG ĐIỂM SINH VIÊN =================
                    Lop* lopChon = dsLop.nodes[cursorLop];
                    string targetMaMH = string(dsMH.data[cursorMH].maMH);

                    SinhVien* arrSV[1000];
                    int totalSV = 0;
                    for (SinhVien* p = lopChon->headSV; p != NULL; p = p->next) {
                        arrSV[totalSV++] = p;
                    }

                    int cursorSV = 0;
                    int itemsPerPageSV = 8;
                    int oldCursorSV = -1;
                    int oldPageSV = -1;
                    bool forceRedrawSV = true;

                    while (true) {
                        ShowCur(false);
                        int totalPagesSV = (totalSV == 0) ? 1 : (totalSV + itemsPerPageSV - 1) / itemsPerPageSV;
                        int currentPageSV = (totalSV == 0) ? 0 : (cursorSV / itemsPerPageSV);

                        if (forceRedrawSV || currentPageSV != oldPageSV) {
                            XoaVungContent();
                            InTitle("BANG DIEM SINH VIEN");
                            InSmallTitle("MON: " + string(dsMH.data[cursorMH].tenMH) + " | LOP: " + string(lopChon->tenLop));
                            
                            gotoxy(105, 27); SetColor(0, 10); cout << "(Trang " << currentPageSV + 1 << "/" << totalPagesSV << ")";
                            
                            gotoxy(35, 8); SetColor(0, 14);
                            printf("%-5s %-15s %-35s %-10s", "STT", "Ma SV", "Ho va Ten", "Diem Thi");
                            SetColor(0, 7);
                            
                            int startSV = currentPageSV * itemsPerPageSV;
                            int endSV = (startSV + itemsPerPageSV > totalSV) ? totalSV : (startSV + itemsPerPageSV);

                            if (totalSV == 0) {
                                SetColor(0, 12); gotoxy(50, 12); cout << "Lop nay hien tai chua co sinh vien nao!"; SetColor(0, 7);
                            } else {
                                for (int i = startSV; i < endSV; i++) {
                                    int toadoY = 10 + (i - startSV) * 2;
                                    if (i == cursorSV) SetColor(11, 0); else SetColor(0, 7);
                                    gotoxy(35, toadoY);
                                    
                                    string hoTen = string(arrSV[i]->ho) + " " + string(arrSV[i]->ten);
                                    if(hoTen.length() > 32) hoTen = hoTen.substr(0, 29) + "...";

                                    DiemThi* d = arrSV[i]->headDiem;
                                    bool found = false; float score = 0;
                                    while (d != NULL) {
                                        if (string(d->maMH) == targetMaMH) { found = true; score = d->diem; break; }
                                        d = d->next;
                                    }
                                    if (found) printf("%-5d %-15s %-35s %-10.1f", i + 1, arrSV[i]->maSV, hoTen.c_str(), score);
                                    else printf("%-5d %-15s %-35s %-10s", i + 1, arrSV[i]->maSV, hoTen.c_str(), "Chua thi");
                                }
                            }
                            SetColor(0, 7);
                            gotoxy(35, 26); SetColor(0, 8); cout << "Up/Down: Chon | Left/Right: Chuyen trang";
                            gotoxy(35, 27); SetColor(0, 11); cout << "[ENTER] Xem bai lam chi tiet | [ESC] Quay lai chon Lop"; SetColor(0, 7);

                            oldPageSV = currentPageSV;
                            oldCursorSV = cursorSV;
                            forceRedrawSV = false;
                        }
                        else if (oldCursorSV != cursorSV) {
                            int startSV = currentPageSV * itemsPerPageSV;

                            if (oldCursorSV >= startSV && oldCursorSV < startSV + itemsPerPageSV) {
                                SetColor(0, 7); gotoxy(35, 10 + (oldCursorSV - startSV) * 2);
                                string hoTenOld = string(arrSV[oldCursorSV]->ho) + " " + string(arrSV[oldCursorSV]->ten);
                                if(hoTenOld.length() > 32) hoTenOld = hoTenOld.substr(0, 29) + "...";
                                
                                DiemThi* d = arrSV[oldCursorSV]->headDiem;
                                bool found = false; float score = 0;
                                while (d) { if (string(d->maMH) == targetMaMH) { found = true; score = d->diem; break; } d = d->next; }

                                if (found) printf("%-5d %-15s %-35s %-10.1f", oldCursorSV + 1, arrSV[oldCursorSV]->maSV, hoTenOld.c_str(), score);
                                else printf("%-5d %-15s %-35s %-10s", oldCursorSV + 1, arrSV[oldCursorSV]->maSV, hoTenOld.c_str(), "Chua thi");
                            }
                            
                            SetColor(11, 0); gotoxy(35, 10 + (cursorSV - startSV) * 2);
                            string hoTenNew = string(arrSV[cursorSV]->ho) + " " + string(arrSV[cursorSV]->ten);
                            if(hoTenNew.length() > 32) hoTenNew = hoTenNew.substr(0, 29) + "...";
                            
                            DiemThi* d = arrSV[cursorSV]->headDiem;
                            bool found = false; float score = 0;
                            while (d) { if (string(d->maMH) == targetMaMH) { found = true; score = d->diem; break; } d = d->next; }

                            if (found) printf("%-5d %-15s %-35s %-10.1f", cursorSV + 1, arrSV[cursorSV]->maSV, hoTenNew.c_str(), score);
                            else printf("%-5d %-15s %-35s %-10s", cursorSV + 1, arrSV[cursorSV]->maSV, hoTenNew.c_str(), "Chua thi");

                            oldCursorSV = cursorSV;
                            SetColor(0, 7);
                        }

                        int ch3 = _getch();
                        if (ch3 == 224 || ch3 == 0 || ch3 == -32) {
                            ch3 = _getch();
                            if (ch3 == 72 && cursorSV > 0) cursorSV--;
                            else if (ch3 == 80 && cursorSV < totalSV - 1) cursorSV++;
                            else if (ch3 == 75 && currentPageSV > 0) cursorSV = (currentPageSV - 1) * itemsPerPageSV;
                            else if (ch3 == 77 && currentPageSV < totalPagesSV - 1) cursorSV = (currentPageSV + 1) * itemsPerPageSV;
                        }
                        else if (ch3 == 27) {
                            forceRedrawLop = true;
                            break;
                        }
                        else if (ch3 == 13 && totalSV > 0) {
                            DiemThi* d = arrSV[cursorSV]->headDiem;
                            bool found = false;
                            while (d != NULL) {
                                if (string(d->maMH) == targetMaMH) { found = true; break; }
                                d = d->next;
                            }
                            if (!found) {
                                InThongBaoContent("Sinh vien nay chua thi mon: " + string(dsMH.data[cursorMH].tenMH), 12, 40, 25);
                                _getch();
                                InThongBaoContent("                                                              ", 12, 40, 25);
                            } else {
                                flowXemLaiBaiLam(arrSV[cursorSV]->maSV, targetMaMH.c_str());
                            }
                            forceRedrawSV = true; 
                        }
                    }
                }
            }
        }
    }
}

void flowXemLaiBaiLam(const char* maSV, const char* maMH) {
    XoaVungContent();
    string path = "data/bailam/" + string(maSV) + " " + string(maMH) + ".txt";
    ifstream fIn(path.c_str());

    if (!fIn.is_open()) {
        InThongBaoContent("KHONG TIM THAY BAI LAM!", 12, 45, 15);
        _getch(); return;
    }

    TrangChiTiet dsCau[200];

    int idx = -1;
    string line;
    while (getline(fIn, line)) {
        while(!line.empty() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n')) 
            line.pop_back();
        
        if (line.empty()) continue;

        string temp = line;
        for(int i=0; i<temp.length(); i++) temp[i] = toupper(temp[i]);

        if (temp.find("CAU") != string::npos) idx++;

        if (idx >= 0 && idx < 200 && dsCau[idx].soDong < 35) {
            dsCau[idx].noiDungCau[dsCau[idx].soDong++] = line;
        }
    }
    fIn.close();

    int tongSoCau = idx + 1;
    int cur = 0;
    while (true) {
        XoaVungContent();
        SetColor(0, 11);
        InTitle("CHI TIET BAI LAM - " + string(maSV) + " | " + string(maMH));
        gotoxy(35, 6); cout << "Mon: " << maMH << " | Cau: " << cur + 1 << "/" << tongSoCau;
        int yPos = 9; 

        for (int i = 0; i < dsCau[cur].soDong; i++) {
            string s = dsCau[cur].noiDungCau[i];
            if (s.find("CAU") != string::npos || s.find("Cau") != string::npos) SetColor(0, 11);
            else if (s.find("Dap an dung") != string::npos) SetColor(0, 10);
            else if (s.find("Sinh vien chon") != string::npos) SetColor(0, 14);
            else SetColor(0, 7);
            
        InTextWrap(s, 35, yPos, 75);
        
        yPos++;
        if (yPos > 25) break;
        }
        SetColor(0, 8);
        gotoxy(35, 27); cout << "<- ->: Chuyen cau | ESC: Thoat";
        int phim = _getch();
        if (phim == 224) {
            phim = _getch();
            if (phim == 77 && cur < tongSoCau - 1) cur++;
            else if (phim == 75 && cur > 0) cur--;
        } else if (phim == 27) break;
    }
}

void flowThiThuGV(DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
    int cursor = 0;
    int itemsPerPage = 8;

    // Vòng lặp tổng bọc bên ngoài để xử lý khi bấm ESC từ màn hình nhập thông số quay lại chọn môn
    while (true) {
        int oldCursor = -1;
        int oldPage = -1;
        int forceRedraw = 1;
        bool isSelected = false;

        // VÒNG LẶP 1: GIAO DIỆN CHỌN MÔN HỌC (PARTIAL REDRAW KHÔNG NHÁY MÀN HÌNH)
        while (true) {
            ShowCur(false);
            int totalItems = dsMH.soLuong;
            int totalPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;
            int currentPage = (totalItems == 0) ? 0 : (cursor / itemsPerPage);
            
            if (forceRedraw || currentPage != oldPage) {
                XoaVungContent();
                InTitle("THI THU - CHON MON HOC");
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
                cout << "[ENTER] Chon mon thi thu | [ESC] Thoat";
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
            else if (ch == 27) return; // Nhấn ESC thoát hẳn ra Menu chính
        }

        // VÒNG LẶP 2: SETUP THÔNG SỐ CẤU HÌNH THI THỬ
        if (isSelected) {
            MonHoc monChon = dsMH.data[cursor];
            CauHoi* khoCauHoi[2000] = {NULL};
            int tongCauHoiKho = 0;
            LayDanhSachCauHoiTheoMon(root, monChon.maMH, khoCauHoi, tongCauHoiKho);

            if (tongCauHoiKho == 0) {
                InThongBaoContent("Mon hoc nay chua co cau hoi de test!", 12, 45, 24);
                _getch();
                continue; // Quay lại vòng lặp ngoài để chọn môn khác
            }

            XoaVungContent();
            InTitle("SETUP CAU HINH THI THU (GV)");
            
            gotoxy(40, 10); cout << "Mon hoc: ";
            SetColor(0, 14); cout << monChon.tenMH; SetColor(0, 7);
            cout << " (Ngan hang co: " << tongCauHoiKho << " cau)";
            
            int testTime = 0, questionCount = 0;
            gotoxy(40, 12); cout << "Thoi gian thi (phut): "; 
            gotoxy(40, 14); cout << "So cau hoi          : ";

            gotoxy(35, 27); SetColor(0, 8); cout << "Nhap so | ENTER: Xac nhan | ESC: Quay lai danh sach mon"; SetColor(0, 7);

            ShowCur(true);

            // Nhập Thời gian
            while (true) {
                gotoxy(63, 12); testTime = NhapSo(-1, 3, 1, 180); 
                if (testTime == INPUT_CANCELLED) break; 
                if (testTime > 0) break;
                InThongBaoContent("Thoi gian phai > 0!", 12, 70, 12);
                Sleep(800); gotoxy(70, 12); cout << "                   ";   
            }
            if (testTime == INPUT_CANCELLED) continue; // Quay lại chọn môn

            // Nhập Số câu
            while (true) {
                gotoxy(63, 14); questionCount = NhapSo(-1, 4, 1, tongCauHoiKho);
                if (questionCount == INPUT_CANCELLED) break; 
                if (questionCount > 0 && questionCount <= tongCauHoiKho) break;
                
                InThongBaoContent("So cau vuot qua ngan hang!", 12, 70, 14);
                _getch(); gotoxy(63, 14); cout << "                                 ";
            }
            if (questionCount == INPUT_CANCELLED) continue; // Quay lại chọn môn

            ShowCur(false);

            // Xáo trộn đề bài (Fisher-Yates Shuffle)
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
            gotoxy(40, 19); SetColor(0, 14); cout << "An phim bat ky de vao phong thi..."; SetColor(0, 7);
            _getch();
            
            // Khởi tạo sinh viên giả lập
            SinhVien svGiaoVien;
            strcpy(svGiaoVien.maSV, "GV_TEST");
            strcpy(svGiaoVien.ho, "Giao");
            strcpy(svGiaoVien.ten, "Vien");
            svGiaoVien.phai = true; 
            svGiaoVien.headDiem = NULL;

            // Tiến hành thi
            TienHanhThiTracNghiem(&svGiaoVien, monChon, deThi, questionCount, testTime);

            // Dọn dẹp con trỏ danh sách liên kết điểm sau khi thi xong để tránh rò rỉ RAM
            DiemThi* d = svGiaoVien.headDiem;
            while (d != NULL) {
                DiemThi* temp = d;
                d = d->next;
                delete temp; 
            }
            return; // Thi thử kết thúc hoàn toàn, quay về Menu chính của GV
        }
    }
}

#endif