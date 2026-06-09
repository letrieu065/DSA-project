#ifndef FLOW_H
#define FLOW_H

#include "khaibao.h"
#include "saveload.h"
#include "hamchinh.h"
#include "dohoa.h"
#include "ErrorCodes.h"
#include "hamtienich.h"

int flowDangNhap(DanhSachLop &dsLop, SinhVien* &svLogined);

void flowGiaoVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root);
void flowQuanLyLop_SinhVien(DanhSachLop &dsLop);
void flowQuanLySinhVienCuaLop(DanhSachLop &dsLop, Lop* lopHienTai);
void flowQuanLyMonHoc_CauHoi(DanhSachMonHoc &dsMH, PTR_CAUHOI &root);
void flowQuanLyCauHoiCuaMon(const char* maMH,const char* tenMH ,PTR_CAUHOI &root);
void flowQuanLyDiemSV(DanhSachMonHoc &dsMH, DanhSachLop &dsLop);

void flowSinhVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root, SinhVien* sv);
void flowXemThongTinSinhVien(SinhVien* sv);
void flowXemDiemSinhVien(SinhVien* sv, DanhSachMonHoc &dsMH);
void flowThiTracNghiem(SinhVien* sv, DanhSachMonHoc &dsMH, PTR_CAUHOI &root, DanhSachLop &dsLop);
void flowXemLaiBaiLam(const char* maSV, const char* maMH);

void flowChinh(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
	CapNhatSTT(root);
    while (true) {
        SinhVien* svHienTai = NULL;
        int role = flowDangNhap(dsLop, svHienTai); 
        
        if (role == 0) break;
        if (role == 1) {
            flowGiaoVien(dsLop, dsMH, root);
        }
        else if (role == 2) {
        	flowSinhVien(dsLop, dsMH, root, svHienTai);
        }
    }
}

int flowDangNhap(DanhSachLop &dsLop, SinhVien* &svLogined) {
    while (true) {
        system("cls");
    	cout << "TAB: hien mat khau";
    	
        SetColor(0, 7);
        gotoxy(51, 10); cout << "DANG NHAP HE THONG";
        VeKhung(37, 11, 46, 8);
        
        gotoxy(40, 13); cout << "Tai khoan:                          ";
        gotoxy(40, 16); cout << "Mat khau :                          ";

        gotoxy(52, 13);
        string user = NhapMa(15);

        if (user == "") {
            gotoxy(38, 26); SetColor(0, 14); cout << "Ban co chac muon thoat khong?";
            char xn = XacNhan(70, 26);
            gotoxy(38, 26); cout << "                                                    ";
            if (xn == 'Y') {
                return 0;  
            }
            continue;
        }

        SinhVien* svHienTai = NULL;
        int role = kiemTraTaiKhoan(dsLop, user, svHienTai);

        if (role != 1 && role != 2) {
            InThongBaoContent(layThongBaoLoi(role), 12, 43, 18); 
            _getch();
            continue;
        }

        gotoxy(52, 16);
        string pass = NhapMatKhau(52, 16, 47, 18); 
        int passCheck = xacThucMatKhau(role, pass, svHienTai);

        if (passCheck == SUCCESS) {
            if (role == 2 && strcmp(svHienTai->password, "123456") == 0) {
                changePass(dsLop, svHienTai);
            }
            svLogined = svHienTai;
            return role; 
        }
        else {
            InThongBaoContent(layThongBaoLoi(passCheck), 12, 45, 18);
            _getch();
            continue;
        }
    }
}

void flowGiaoVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
    char menuChinh[4][50] = {"1. Quan Ly Lop", "2. Quan Ly Mon Hoc", "3. Xem Diem Sinh Vien", "4. Dang Xuat "};
    
    while (true) {
        SetupConsoleLayout();
        VeLayoutChinh("GIANG VIEN"); 
        
        int chon = MenuDong(menuChinh, 4, 4, 9); 
        
        if (chon == 0) flowQuanLyLop_SinhVien(dsLop);
        else if (chon == 1) flowQuanLyMonHoc_CauHoi(dsMH, root);
        else if (chon == 2) flowQuanLyDiemSV(dsMH, dsLop);
        else break; 
    }
}

void flowSinhVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root, SinhVien* sv) {
    char menuChinh[4][50] = {"1. Xem thong tin", "2. Xem diem", "3. Thi trac nghiem", "4. Dang xuat"};
    string info = string(sv->ho) + " " + string(sv->ten);

	while (true) {
        SetupConsoleLayout();
        VeLayoutChinh((char*)info.c_str(), (char*)sv->maSV); 
        
        int chon = MenuDong(menuChinh, 4, 4, 9); 
        
        if (chon == 0) flowXemThongTinSinhVien(sv);
        else if (chon == 1) flowXemDiemSinhVien(sv, dsMH);
        else if (chon == 2) flowThiTracNghiem(sv, dsMH, root, dsLop);
        else break; 
    }
}

void flowQuanLyMonHoc_CauHoi(DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
    XoaTitle();
    InTitle("QUAN LY MON HOC - CAU HOI");
    int cursor = 0;
    int itemsPerPage = 8;

    int oldCursor = -1;
    int oldPage = -1;
    int forceRedraw = 0;
    
    while (true) {
        ShowCur(false);
        int totalItems = dsMH.soLuong;
        int totalPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;
        int currentPage = (totalItems == 0) ? 0 : (cursor / itemsPerPage);

        if (forceRedraw || currentPage != oldPage) {
            XoaVungContent();
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
            cout << "[ENTER] Quan ly cau hoi cua mon";
            gotoxy(35, 27); SetColor(0, 11);
            cout << "[INS/I] Them | [E] Sua | [DEL] Xoa | [ESC] Thoat";
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
            gotoxy(59, 7); SetColor(0, 11); cout << "--- THEM MON HOC MOI ---"; SetColor(0, 7);

            gotoxy(40, 10); cout << "Ma Mon:";
            gotoxy(40, 12); cout << "Ten Mon Hoc:";
            gotoxy(40, 14); cout << "So Tin Chi:";

            int xNhap = 54;
			ShowCur(true);
            gotoxy(xNhap, 10); 
			string ma = NhapMa(MAX_MAMH); 
            if (ma == "") {
            	InThongBaoContent("Da  huy thao tac them mon hoc!", 10, 56, 26);
            	_getch();
            continue;
			}

            if (kiemTraTrungMaMonHoc(dsMH, (char*)ma.c_str())) {
    			InThongBaoContent("Loi: Ma mon hoc da ton tai!", 12, 56, 26);
    			_getch();
    		continue;
			}
			ShowCur(true);
            gotoxy(xNhap, 12); 
			string ten = NhapTen(); 
			if (ten == ""){
				InThongBaoContent("Da huy thao tac them mon hoc", 14, 56, 26);
				_getch();
			continue;
			}
			ShowCur(true);
            gotoxy(xNhap, 14); 
            int stc = NhapSo(-1, 2, 1, 10);
            if (stc == INPUT_CANCELLED) {
                InThongBaoContent("Da huy!", 14, 50, 26);
                _getch(); continue;
            }
			
			ShowCur(false);
			
            MonHoc mhMoi;
            strcpy(mhMoi.maMH, ma.c_str());
            strcpy(mhMoi.tenMH, ten.c_str());
            mhMoi.soTinChi = stc;
        
            if (dsMH.soLuong >= MAX_MON_HOC) {
    			InThongBaoContent("Danh sach mon hoc da day!", 12, 50, 26);
    			_getch();
    		continue;
			}	
			
			dsMH.data[dsMH.soLuong++] = mhMoi;
			
            SaveMonHoc(dsMH);
            InThongBaoContent("Them mon hoc thanh cong!", 10, 58, 26);
            _getch();
            cursor = dsMH.soLuong - 1;
            forceRedraw = true;
		}   

        else if (toupper(ch) == 'E' && totalItems > 0) { 
		    XoaVungContent();
		    gotoxy(100, 27); cout << "ESC: de thoat";
		    gotoxy(55, 7); SetColor(0, 11); cout << "--- SUA MON HOC ---"; SetColor(0, 7);
		    
		    gotoxy(40, 10); cout << "Ma mon hoc    : " << dsMH.data[cursor].maMH;
		    gotoxy(40, 12); cout << "Ten mon moi   :                               ";
		    gotoxy(40, 14); cout << "So tin chi moi:     ";
		
		    gotoxy(57, 12); cout << dsMH.data[cursor].tenMH;
		    gotoxy(57, 14); cout << dsMH.data[cursor].soTinChi;
		
		    ShowCur(true);
		    
		    gotoxy(57, 12); 
		    string tenMoi = NhapTen(dsMH.data[cursor].tenMH); 
		    if (tenMoi == "") {
		        ShowCur(false); InThongBaoContent("Da huy!", 14, 50, 26);
		        _getch(); continue;
		    }
		
		    gotoxy(57, 14); cout << "    "; 
		    gotoxy(57, 14); 
		    int stcMoi = NhapSo(dsMH.data[cursor].soTinChi);
		    
		    if (stcMoi == INPUT_CANCELLED) {
                InThongBaoContent("Da huy!", 14, 50, 26);
                _getch(); continue;
            }
		
		    strcpy(dsMH.data[cursor].tenMH, tenMoi.c_str());
		    dsMH.data[cursor].soTinChi = stcMoi;
		    
		    ShowCur(false);
		    SaveMonHoc(dsMH);
		    InThongBaoContent("Cap nhat thanh cong!", 10, 50, 26);
		    _getch();
            forceRedraw = true;
		}
        else if ((ch == 83 || toupper(ch) == 'D') && totalItems > 0) {
            gotoxy(40, 25); SetColor(0, 14); 
            string thongBao = "Xac nhan xoa mon: " + string(dsMH.data[cursor].tenMH) + " ";
            cout << thongBao;
            
            if (XacNhan(40 + thongBao.length(), 25) == 'Y') {

                for (int i = cursor; i < dsMH.soLuong - 1; i++) {
                    dsMH.data[i] = dsMH.data[i + 1];
                }
                dsMH.soLuong--;
                
                if (cursor >= dsMH.soLuong && cursor > 0) cursor--;
            
                SaveMonHoc(dsMH);
                gotoxy(40, 25); cout << "                                             ";
                InThongBaoContent("Da xoa mon hoc thanh cong!", 10, 40, 25);
            } else {
            	gotoxy(40, 25); cout << "                                             ";
                InThongBaoContent("Da huy thao tac xoa!", 10, 40, 25);
            }
            
            _getch(); 
            forceRedraw = true;
        }
    }
}

void flowQuanLyCauHoiCuaMon(const char* maMH, const char* tenMH, PTR_CAUHOI &root) {
    XoaSmallTitle();
    InSmallTitle("MON: " + string(tenMH) + " - MA: " + string(maMH));
	
	KiemTraVaTaoFileCauHoi(maMH, tenMH);
	
    int currentPage = 0;
    int itemsPerPage = 1; 

    while (true) {
        ShowCur(false);
        XoaVungContent();

        CauHoi* dsTam[1000];
        int totalItems = 0;
        LayDanhSachCauHoiTheoMon(root, maMH, dsTam, totalItems);

        int totalPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;
        if (currentPage >= totalPages) currentPage = totalPages - 1;
        if (currentPage < 0) currentPage = 0;

        gotoxy(105, 27); SetColor(0, 10);
        cout << "(Trang " << currentPage + 1 << "/" << totalPages << ")";
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
		
		        SetColor(0, 14);
		        gotoxy(startX, yPos + 1); cout << "[" << dsTam[i]->id << "] ";
		    
		        SetColor(0, 7);
		        InNoiDungCanLe(dsTam[i]->noiDung, contentX, yPos + 1, 63); 
		
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
		    // Chuyển câu hỏi (trang)
		    if (ch == 75 && currentPage > 0) currentPage--;     
		    else if (ch == 77 && currentPage < totalItems - 1) currentPage++; 
		}
        else if (ch == 27) break; 
         
        // --- Logic Them (I) ---
        else if (toupper(ch) == 'I') {
            XoaVungContent();
            ShowCur(true);
            while (_kbhit()) _getch();
            gotoxy(57, 6); SetColor(0, 11); cout << "--- THEM CAU HOI MOI ---"; SetColor(0, 7);
            gotoxy(35, 8); cout << "Noi Dung   : ";
            gotoxy(35, 14); cout << "A. ";
            gotoxy(35, 16); cout << "B. ";
            gotoxy(35, 18); cout << "C. ";
            gotoxy(35, 20); cout << "D. ";
            gotoxy(35, 22); cout << "Dap An Dung: ";

            gotoxy(50, 8); string noiDung = xuongdongGioiHan(50, 8, 63, 4, 230); if (noiDung == "") continue;
            gotoxy(39, 14); string pA 	  = xuongdongGioiHan(39, 14, 75, 2, 148); if (pA == "") continue;
            gotoxy(39, 16); string pB     = xuongdongGioiHan(39, 16, 75, 2, 148); if (pB == "") continue;
            gotoxy(39, 28); string pC     = xuongdongGioiHan(39, 18, 75, 2, 148); if (pC == "") continue;
            gotoxy(39, 20); string pD     = xuongdongGioiHan(39, 20, 75, 2, 148); if (pD == "") continue;
            char dapAn = ChonDapAn(60, 22); if (dapAn == '\0') continue;

            CauHoi* newNode = new CauHoi();
            TaoIdCauHoi(newNode->id, maMH, getNextSTT(maMH)++);
            newNode->dapAn = dapAn;
            newNode->left = newNode->right = NULL;
            strcpy(newNode->maMH, maMH);
            strcpy(newNode->noiDung, noiDung.c_str());
            strcpy(newNode->a, pA.c_str());
            strcpy(newNode->b, pB.c_str());
            strcpy(newNode->c, pC.c_str());
            strcpy(newNode->d, pD.c_str());

            root = ThemCauHoi(root, newNode);
            
			LuuFileCauHoiTheoMon(maMH, tenMH, root);
			InThongBaoContent("Them cau hoi thanh cong!", 10, 45, 26);
        
            _getch();
        }
        
		// --- Logic Sua (E) ---

        else if (toupper(ch) == 'E') {
		    if (totalItems > 0) {
		        CauHoi* q = dsTam[currentPage];
		        
		        // 1. Chuẩn bị giao diện
		        XoaVungContent(); 
		        SetColor(0, 11);
		        InGiuaKhung("--- SUA NOI DUNG CAU HOI ---", 30, 6, 88);
		
		        string t_ND = q->noiDung, t_A = q->a, t_B = q->b, t_C = q->c, t_D = q->d;
		        char t_DA = q->dapAn;
		
		        SetColor(0, 15);
		        gotoxy(35, 8);  cout << "Noi Dung   : ";
		        gotoxy(35, 14); cout << "A. ";
		        gotoxy(35, 16); cout << "B. ";
		        gotoxy(35, 18); cout << "C. ";
		        gotoxy(35, 20); cout << "D. ";
		        gotoxy(35, 22); cout << "Dap An Dung: ";
		                
		        int field = 0, keyLog = 0;
		        bool exitForm = false;
		
		        while (!exitForm) {
		            SetColor(0, 7); 
		            switch (field) {
		                case 0: t_ND = NhapCoDieuHuong(50, 8, 65, 4, t_ND, keyLog); break;
		                case 1: t_A  = NhapCoDieuHuong(39, 14, 75, 2, t_A, keyLog); break;
		                case 2: t_B  = NhapCoDieuHuong(39, 16, 75, 2, t_B, keyLog); break;
		                case 3: t_C  = NhapCoDieuHuong(39, 18, 75, 2, t_C, keyLog); break;
		                case 4: t_D  = NhapCoDieuHuong(39, 20, 75, 2, t_D, keyLog); break;
		                case 5: 
		                    t_DA = ChonDapAn(50, 22); 
		                    keyLog = 13; // Sau khi chọn xong, tự động hiểu là nhấn Enter để lưu
		                    break;
		            }
		
		            // --- XỬ LÝ NHẤN ESC: HỎI XÁC NHẬN TRƯỚC KHI THOÁT ---
		            if (keyLog == 27) {
		                gotoxy(40, 24); SetColor(0, 14); cout << "Ban co chac muon thoat khong?";
		                if (XacNhan(82, 24) == 'Y') {
		                    exitForm = true;
		                } else {

		                    gotoxy(40, 24); cout << "                                             ";
		                    continue; 
		                }
		            }
		            
		            if (keyLog == 72) {
		                field = (field > 0) ? field - 1 : 5;
		            }
		            else if (keyLog == 80 || keyLog == 13) { 
		                if (field == 5 && keyLog == 13) {

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
Lop* TimKiemLopThongMinh(DanhSachLop &dsLop) {
    string tuKhoa = ""; 
    int cursor = 0;

    while (true) {
        XoaVungContent();
        
        gotoxy(58, 5); SetColor(0, 10); cout << "--- TIM KIEM LOP HOC THONG MINH ---";
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
	
void flowQuanLyLop_SinhVien(DanhSachLop &dsLop) {
    int cursor = 0;
    int itemsPerPage = 8;
    
    while (true) {
        int totalItems = dsLop.soLuong;
        int totalPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;
        int currentPage = (totalItems == 0) ? 0 : (cursor / itemsPerPage);
        
        XoaVungContent();
        gotoxy(62, 6); SetColor(0, 10); 
        cout << "DANH SACH LOP HOC";
        gotoxy(105, 27); cout << "(Trang " << currentPage + 1 << "/" << totalPages << ")"; 
        SetColor(0, 7);
        
        gotoxy(35, 8); SetColor(0, 14);
        cout << "STT   Ma Lop          Ten Lop";
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
                int toadoY = 10 + (i - startIdx) * 2; 
                
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
        cout << "[INS/I] Them | [E] Sua | [DEL] Xoa | [F| Tim Kiem  | [ESC] Thoat";
        SetColor(0, 7);
        
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
        }
        else if (toupper(ch) == 'I') { 
            XoaVungContent();
            gotoxy(100, 27); cout << "ESC: de thoat";
            gotoxy(55, 7); SetColor(0, 11); cout << "--- THEM LOP MOI ---"; SetColor(0, 7);
            
            gotoxy(40, 10); cout << "Nhap Ma Lop :                ";
            gotoxy(40, 12); cout << "Nhap Ten Lop:                               ";
            
            gotoxy(55, 10); string ma = NhapMa(MAX_MALOP); if (ma == "") continue;
            if (KiemTraTrungMaLop(dsLop, (char*)ma.c_str()) != -1) {
            	InThongBaoContent(layThongBaoLoi(ERR_LOP_DUPLICATE), 12, 50, 26);
            	_getch();
            	continue;
			}
            gotoxy(55, 12); string ten = NhapTen(); if (ten == "") continue;
            
            int res = themLop(dsLop, (char*)ma.c_str(), (char*)ten.c_str());
            
            if (res == SUCCESS) {
                InThongBaoContent("Them lop thanh cong!", 10, 50, 26);
                SaveLop(dsLop); 
                cursor = dsLop.soLuong - 1; 
            } else {
                InThongBaoContent(layThongBaoLoi(res), 12, 50, 26);
            }
            _getch();
        } 
        else if (toupper(ch) == 'E' && totalItems > 0) { 
		    XoaVungContent();
		    gotoxy(100, 27); cout << "ESC: de thoat";
		    gotoxy(55, 7); SetColor(0, 11); cout << "--- SUA LOP ---"; SetColor(0, 7);
		    
		    gotoxy(40, 10); cout << "Ma Lop : " << dsLop.nodes[cursor]->maLop;
		    gotoxy(40, 12); cout << "Ten Lop: "; 
		    
		    string tenCu = dsLop.nodes[cursor]->tenLop;
		    string tenMoi = NhapTen(tenCu); 
		    
		    if (tenMoi == "" || tenMoi == tenCu) {
		        InThongBaoContent("Khong co thay doi nao.", 8, 50, 26);
		        _getch();
		        continue;
		    }
		    
		    int res = suaLop(dsLop, dsLop.nodes[cursor]->maLop, (char*)tenMoi.c_str());
		    
		    if (res == SUCCESS) {
		        InThongBaoContent("Cap nhat ten lop thanh cong!", 10, 50, 26);
		        SaveLop(dsLop); 
		    } else {
		        InThongBaoContent(layThongBaoLoi(res), 12, 50, 26);
		    }
		    _getch();
		}
        else if ((ch == 83 || toupper(ch) == 'D') && totalItems > 0) { 
            XoaVungContent();
            gotoxy(55, 7); SetColor(0, 11); cout << "--- XOA LOP ---"; SetColor(0, 7);
            
            gotoxy(40, 10); cout << "Ban co chac muon xoa lop ";
            SetColor(0, 12); cout << dsLop.nodes[cursor]->maLop; SetColor(0, 7);
            cout << "? (y/n): ";
            
            char confirm = _getch();
            if (confirm == 'y' || confirm == 'Y') {
                int res = xoaLop(dsLop, dsLop.nodes[cursor]->maLop);
                if (res == SUCCESS) {
                    InThongBaoContent("Xoa lop thanh cong!", 10, 50, 26);
                    SaveLop(dsLop);
                    if (cursor >= dsLop.soLuong && cursor > 0) cursor--; 
                } else {
                    InThongBaoContent(layThongBaoLoi(res), 12, 50, 26);
                }
                _getch();
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
        
        gotoxy(52, 5); SetColor(0, 10); cout << "--- TIM KIEM SINH VIEN TRONG LOP ---";
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
    
    while (true) {
        int totalItems = demSoSV(lopHienTai); 
        int totalPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;
        int currentPage = (totalItems == 0) ? 0 : (cursor / itemsPerPage);
        
        XoaVungContent();
        gotoxy(52, 6); SetColor(0, 10); 
        cout << "DANH SACH SINH VIEN LOP: " << lopHienTai->maLop;
        gotoxy(105, 25); cout << "(Trang " << currentPage + 1 << "/" << totalPages << ")"; 
        SetColor(0, 7);
        
        gotoxy(33, 8); SetColor(0, 14);
        printf(" %-5s %-15s %-25s %-10s", "STT", "Ma SV", "Ho va Ten", "Gioi Tinh"); 
        SetColor(0, 7);
        
        SinhVien* p = lopHienTai->headSV;
        int startIdx = currentPage * itemsPerPage;
        int endIdx = startIdx + itemsPerPage;
        if (endIdx > totalItems) endIdx = totalItems;
        
        for (int i = 0; i < startIdx && p != NULL; i++) {
            p = p->next;
        }
        
        if (totalItems == 0) {
            gotoxy(50, 10); cout << "(Lop nay chua co sinh vien nao)";
        } else {
            for (int i = startIdx; i < endIdx && p != NULL; i++) {
                int toadoY = 10 + (i - startIdx) * 2;
                
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
        cout << "[INS/I] Them | [E] Sua | [F] Tim Kiem | [DEL] Xoa | [ESC] Thoat";
        SetColor(0, 7);
        
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
        
        else if (toupper(ch) == 'D') isDelete = true;
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
            
        }
        else if (toupper(ch) == 'I') { 
            XoaVungContent();
            gotoxy(100, 27); cout << "ESC: de thoat";
            gotoxy(55, 7); SetColor(0, 11); cout << "--- THEM SINH VIEN MOI ---"; SetColor(0, 7);
            
            gotoxy(40, 10); cout << "Ma SV:                 ";
            gotoxy(40, 12); cout << "Ho   :                               ";
            gotoxy(40, 14); cout << "Ten  :                 ";
            gotoxy(40, 16); cout << "Phai :       ";
            
            gotoxy(47, 10); string ma = NhapMa(MAX_MASV); if (ma == "") continue;
            
            if (kiemTraTrungMaSinhVien(dsLop, ma.c_str()) == ERR_SV_DUPLICATE) {
                InThongBaoContent("Loi: Ma SV nay da ton tai trong he thong!", 12, 45, 26);
                _getch(); continue;
            }

            gotoxy(47, 12); string ho = NhapTen(); if (ho == "") continue;
            gotoxy(47, 14); string ten = NhapTen(); if (ten == "") continue;
            bool phai = NhapGioiTinh(47, 16);
        
            SinhVien svMoi;
            snprintf(svMoi.maSV, sizeof(svMoi.maSV), "%s", ma.c_str());
            snprintf(svMoi.ho, sizeof(svMoi.ho), "%s", ho.c_str());
            snprintf(svMoi.ten, sizeof(svMoi.ten), "%s", ten.c_str());
            svMoi.phai = phai;
            strcpy(svMoi.password, "123456"); 
            svMoi.headDiem = NULL;
        
            int res = themSinhVien(lopHienTai, svMoi);
            if (res == SUCCESS) {
                InThongBaoContent("Them sinh vien thanh cong!", 10, 45, 26);
                SaveLop(dsLop); 
                cursor = 0; 
            }
            _getch();
        } 
        
		else if ((toupper(ch) == 'E' || isDelete) && totalItems > 0) {

		    SinhVien* svTarget = lopHienTai->headSV;
		    for (int i = 0; i < cursor && svTarget != NULL; i++) {
		        svTarget = svTarget->next;
		    }
		
		    if (svTarget != NULL) {
		        if (toupper(ch) == 'E') {
		            XoaVungContent();
		            gotoxy(100, 27); cout << "ESC: de thoat";
		            gotoxy(55, 7); SetColor(0, 11); cout << "--- SUA SINH VIEN ---"; SetColor(0, 7);
		            
		            gotoxy(40, 10); cout << "Dang sua SV  : " << svTarget->maSV;
		            gotoxy(40, 12); cout << "Ho           :                               ";
		            gotoxy(40, 13); cout << "Ten          :                ";
		            gotoxy(40, 14); cout << "Phai (Nam/Nu):      ";
		
		            gotoxy(57, 12); cout << svTarget->ho;
		            gotoxy(57, 13); cout << svTarget->ten;
		            gotoxy(57, 14); cout << (svTarget->phai ? "Nam" : "Nu");
		
		            ShowCur(true);
		
		            gotoxy(57, 12); 
		            string hoMoi = NhapTen(svTarget->ho); 
		            if (hoMoi == "") { ShowCur(false); return; } 
		
		            gotoxy(57, 13);
		            string tenMoi = NhapTen(svTarget->ten);
		            if (tenMoi == "") { ShowCur(false); return; }
		
		            ShowCur(false);
		            bool phaiMoi = NhapGioiTinh(57, 14, svTarget->phai); 
		            
		            SinhVien svMoiData;
		            strcpy(svMoiData.ho, hoMoi.c_str());
		            strcpy(svMoiData.ten, tenMoi.c_str());
		            svMoiData.phai = phaiMoi;
		
		            int res = suaSinhVien(lopHienTai, svTarget->maSV, svMoiData);
		            if (res == SUCCESS) {
		                InThongBaoContent("Cap nhat thong tin thanh cong!", 10, 45, 26);
		                SaveLop(dsLop); 
		            }
		            _getch();
		        }
		        
		        else if (isDelete) {
		            XoaVungContent();
		            gotoxy(55, 7); SetColor(0, 11); cout << "--- XOA SINH VIEN ---"; SetColor(0, 7);
		            
		            gotoxy(40, 10); cout << "Ban co chac muon xoa SV: ";
		            SetColor(0, 12); cout << svTarget->ho << " " << svTarget->ten;
		            SetColor(0, 7); cout << " (" << svTarget->maSV << ")? (y/n): ";
		            
		            char confirm = _getch();
		            if (confirm == 'y' || confirm == 'Y') {
		                int res = xoaSinhVien(lopHienTai, svTarget->maSV);
		                if (res == SUCCESS) {
		                    InThongBaoContent("Xoa sinh vien thanh cong!", 10, 45, 26);
		                    SaveLop(dsLop);
		                    
		                    int newTotal = demSoSV(lopHienTai);
		                    if (cursor >= newTotal && cursor > 0) cursor--; 
		                }
		                _getch();
		            }
		        }
		    }
		}
	}
}

void flowXemThongTinSinhVien(SinhVien* sv) {
    XoaVungContent();
    gotoxy(65, 2); SetColor(0, 11); cout << "THONG TIN SINH VIEN"; SetColor(0, 7);
    
    gotoxy(40, 10); cout << "Ma SV: " << sv->maSV;
    gotoxy(40, 12); cout << "Ho va Ten: " << sv->ho << " " << sv->ten;
    gotoxy(40, 14); cout << "Gioi Tinh: " << (sv->phai ? "Nam" : "Nu");
    
    _getch();
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
void flowQuanLyDiemSV(DanhSachMonHoc &dsMH, DanhSachLop &dsLop) {
    if (dsMH.soLuong == 0 || dsLop.soLuong == 0) {
        InThongBaoContent("He thong chua co du lieu!", 12, 45, 15);
        _getch(); SetColor(0, 7);
        return;
    }
    
    int cursorMH = 0;
    int itemsPerPage = 8;
    
    while (true) {
        int totalMH = dsMH.soLuong;
        int totalPagesMH = (totalMH + itemsPerPage - 1) / itemsPerPage;
        if (totalPagesMH == 0) totalPagesMH = 1; 
        int currentPageMH = cursorMH / itemsPerPage;

        XoaVungContent();
        gotoxy(56, 6); SetColor(0, 11); cout << "--- XEM DIEM MON HOC ---";
        gotoxy(95, 27); SetColor(0, 10); cout << "(Trang " << currentPageMH + 1 << "/" << totalPagesMH << ")";
        gotoxy(35, 26); SetColor(0, 8); cout << "Nhan ESC de quay lai";
        gotoxy(35, 8); SetColor(0, 14); cout << "Vui long chon mon hoc:";
        
        int startMH = currentPageMH * itemsPerPage;
        int endMH = (startMH + itemsPerPage > totalMH) ? totalMH : (startMH + itemsPerPage);

        for (int i = startMH; i < endMH; i++) {
            int toadoY = 10 + (i - startMH) * 2;
            if (i == cursorMH) SetColor(11, 0); else SetColor(0, 7);
            gotoxy(38, toadoY);
            cout << dsMH.data[i].maMH << " - " << dsMH.data[i].tenMH;
        }
        SetColor(0, 7);

        int ch = _getch();
        if (ch == 224 || ch == 0) {
            ch = _getch();
            if (ch == 72 && cursorMH > 0) cursorMH--;
            else if (ch == 80 && cursorMH < totalMH - 1) cursorMH++;
            else if (ch == 75 && currentPageMH > 0) cursorMH = (currentPageMH - 1) * itemsPerPage;
            else if (ch == 77 && currentPageMH < totalPagesMH - 1) cursorMH = (currentPageMH + 1) * itemsPerPage;
        }
        else if (ch == 27) break;
        else if (ch == 13) {
            
            int cursorLop = 0;
            while (true) {
                int totalLop = dsLop.soLuong;
                int totalPagesLop = (totalLop + itemsPerPage - 1) / itemsPerPage;
                int currentPageLop = cursorLop / itemsPerPage;
                
                XoaVungContent();
                gotoxy(56, 6); SetColor(0, 11); cout << "Mon: " << dsMH.data[cursorMH].tenMH;
                gotoxy(95, 27); SetColor(0, 10); cout << "(Trang Lop: " << currentPageLop + 1 << "/" << totalPagesLop << ")";
                gotoxy(35, 26); SetColor(0, 8); cout << "ESC: Quay lai chon Mon | ENTER: Xem diem";
                gotoxy(35, 8); SetColor(0, 14); cout << "2. Vui long chon lop:";

                int startLop = currentPageLop * itemsPerPage;
                int endLop = (startLop + itemsPerPage > totalLop) ? totalLop : (startLop + itemsPerPage);

                for (int i = startLop; i < endLop; i++) {
                    int toadoY = 10 + (i - startLop) * 2;
                    if (i == cursorLop) SetColor(11, 0); else SetColor(0, 7);
                    gotoxy(38, toadoY);
                    cout << dsLop.nodes[i]->maLop << " - " << dsLop.nodes[i]->tenLop;
                }
                SetColor(0, 7);

                int ch2 = _getch();
                if (ch2 == 224 || ch2 == 0) {
                    ch2 = _getch();
                    if (ch2 == 72 && cursorLop > 0) cursorLop--;
                    else if (ch2 == 80 && cursorLop < totalLop - 1) cursorLop++;
                    else if (ch2 == 75 && currentPageLop > 0) cursorLop = (currentPageLop - 1) * itemsPerPage;
                    else if (ch2 == 77 && currentPageLop < totalPagesLop - 1) cursorLop = (currentPageLop + 1) * itemsPerPage;
                }
                else if (ch2 == 27) break;
                else if (ch2 == 13) {
                    XoaVungContent();
                    Lop* lopChon = dsLop.nodes[cursorLop];
                    string targetMaMH = string(dsMH.data[cursorMH].maMH);

                    SetColor(0, 11);
                    InTitle("BANG DIEM LOP: " + string(lopChon->tenLop));
                    InSmallTitle("Mon: " + string(dsMH.data[cursorMH].tenMH));

                    int yHeader = 9;
                    SetColor(0, 14);
                    gotoxy(38, yHeader); cout << "STT";
                    gotoxy(48, yHeader); cout << "MA SV";
                    gotoxy(70, yHeader); cout << "HO VA TEN";
                    gotoxy(105, yHeader); cout << "DIEM";
                    
                    SetColor(0, 7);
                    int count = 0;
                    int y = yHeader;
                    SinhVien* sv = lopChon->headSV;

                    if (sv == NULL) {
                        SetColor(0, 12);
                        gotoxy(40, 15); cout << "Lop nay hien tai chua co sinh vien nao!";
                    } else {
                        while (sv != NULL) {
                            count++; y++;
                            gotoxy(38, y); cout << count;
                            gotoxy(48, y); cout << sv->maSV;
                            gotoxy(70, y); cout << string(sv->ho) + " " + string(sv->ten);

                            DiemThi* d = sv->headDiem;
                            bool found = false;
                            float score = 0;
                            while (d != NULL) {
                                if (string(d->maMH) == targetMaMH) {
                                    found = true; score = d->diem;
                                    break;
                                }
                                d = d->next;
                            }

                            gotoxy(105, y);
                            if (found) cout << score; 
                            else { 
                                SetColor(0, 12); cout << "Chua thi"; SetColor(0, 7); 
                            }

                            sv = sv->next;
                            
                            if (y >= 23 && sv != NULL) {
                                SetColor(0, 8);
                                gotoxy(35, 26); cout << "Nhan phim bat ky de xem tiep danh sach...";
                                _getch();
                                XoaVungContent();

                                SetColor(0, 11); gotoxy(55, 6); cout << "BANG DIEM LOP: " << lopChon->tenLop;
                                SetColor(0, 14); gotoxy(38, 9); cout << "STT"; gotoxy(48, 9); cout << "MA SV"; 
                                gotoxy(70, 9); cout << "HO VA TEN"; gotoxy(105, 9); cout << "DIEM";
                                SetColor(0, 7);
                                y = 9;
                            }
                        }
                        
                        SetColor(0, 14);
                        gotoxy(38, 25);
                    }
                    _getch();
                }
            }
        }
    }
}
void flowXemDiemSinhVien(SinhVien* sv, DanhSachMonHoc &dsMH) {
    if (dsMH.soLuong == 0) {
        InThongBaoContent("He thong chua co mon hoc nao!", 12, 45, 15);
        _getch(); return;
    }
    
    int itemsPerPage = 7; 
    int cursor = 0; 

    while (true) {
        XoaVungContent();
        int totalPages = (dsMH.soLuong + itemsPerPage - 1) / itemsPerPage;
        if (totalPages == 0) totalPages = 1;
        int currentPage = cursor / itemsPerPage;

        SetColor(0, 11);
        gotoxy(60, 2); cout << "--- BANG DIEM CA NHAN ---";
        gotoxy(55, 6); cout << "Sinh vien: " << sv->ho << " " << sv->ten << " (" << sv->maSV << ")";

        SetColor(0, 14);
        int yHeader = 9;
        gotoxy(38, yHeader); cout << "STT";
        gotoxy(46, yHeader); cout << "MA MON";
        gotoxy(60, yHeader); cout << "TEN MON HOC";
        gotoxy(105, yHeader); cout << "DIEM";
        int start = currentPage * itemsPerPage;
        int end = (start + itemsPerPage > dsMH.soLuong) ? dsMH.soLuong : (start + itemsPerPage);
        int yPos = 11;

        for (int i = start; i < end; i++) {
            if (i == cursor) SetColor(11, 0);
            else SetColor(0, 7);

            gotoxy(38, yPos); cout << i + 1;
            gotoxy(46, yPos); cout << dsMH.data[i].maMH;
            gotoxy(60, yPos); cout << dsMH.data[i].tenMH;

            DiemThi* pDiem = sv->headDiem;
            bool daThi = false;
            while (pDiem != NULL) {
                if (strcmp(pDiem->maMH, dsMH.data[i].maMH) == 0) {
                    gotoxy(105, yPos); cout << pDiem->diem;
                    daThi = true;
                    break;
                }
                pDiem = pDiem->next;
            }
            if (!daThi) { gotoxy(105, yPos); cout << "_"; }
            yPos += 2;
        }
        SetColor(0, 10);
        gotoxy(95, 27); cout << "(Trang " << currentPage + 1 << "/" << totalPages << ")";
        SetColor(0, 8);
        gotoxy(35, 27); cout << "ENTER: Chi tiet | ESC: Thoat";
        int ch = _getch();
        if (ch == 224 || ch == 0) {
            ch = _getch();
            if (ch == 80 && cursor < dsMH.soLuong - 1) cursor++;
            else if (ch == 72 && cursor > 0) cursor--;
            else if (ch == 77 && (currentPage + 1) < totalPages)
                cursor = (currentPage + 1) * itemsPerPage;
            else if (ch == 75 && currentPage > 0)
                cursor = (currentPage - 1) * itemsPerPage;
        }
        else if (ch == 13) {
             flowXemLaiBaiLam(sv->maSV, dsMH.data[cursor].maMH);
        }
        else if (ch == 27) {
            break;
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
		gotoxy(60, 5); cout << "--- CHI TIET BAI LAM ---";
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
#endif