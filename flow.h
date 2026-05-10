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

void flowSinhVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root, SinhVien* sv);
void flowXemThongTinSinhVien(SinhVien* sv);
void flowXemDiemSinhVien(SinhVien* sv, DanhSachMonHoc &dsMH);
void flowThiTracNghiem(SinhVien* sv, DanhSachMonHoc &dsMH, PTR_CAUHOI &root);

void flowChinh(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
	CapNhatSTT(root);
    while (true) {
        SinhVien* svHienTai = NULL;
        int role = flowDangNhap(dsLop, svHienTai); 
        
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
    char menuChinh[4][50] = {"1. Quan Ly Lop", "2. Quan Ly Mon Hoc", "3. Dang Xuat "};
    
    while (true) {
        SetupConsoleLayout();
        VeLayoutChinh("GIANG VIEN"); 
        
        int chon = MenuDong(menuChinh, 3, 4, 9); 
        
        if (chon == 0) flowQuanLyLop_SinhVien(dsLop);
        else if (chon == 1) flowQuanLyMonHoc_CauHoi(dsMH, root);
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
        //else if (chon == 1) flowXemDiemSinhVien(sv, dsMH);
        else if (chon == 2) flowThiTracNghiem(sv, dsMH, root);
        else break; 
    }
}

void flowQuanLyMonHoc_CauHoi(DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
    int cursor = 0;
    int itemsPerPage = 8;
    
    gotoxy(63, 2); SetColor(0, 11); cout << "QUAN LY CAU HOI";

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
            gotoxy(53, 13); cout << "(Vui long them mon hoc truoc)";
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

        int ch = _getch();
        if (ch == 224 || ch == 0 || ch == -32) {
            ch = _getch();
            if (ch == 72 && cursor > 0) cursor--;
            else if (ch == 80 && cursor < totalItems - 1) cursor++;
            else if (ch == 75 && currentPage > 0) cursor = (currentPage - 1) * itemsPerPage;
            else if (ch == 77 && currentPage < totalPages - 1) cursor = (currentPage + 1) * itemsPerPage;
        }
        else if (ch == 13 && totalItems > 0)
            flowQuanLyCauHoiCuaMon(dsMH.data[cursor].maMH,dsMH.data[cursor].tenMH, root);
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
            gotoxy(xNhap, 14); int stc = NhapSo();
            if (stc <= 0) {
            	InThongBaoContent("Da huy thao tac them mon hoc", 14, 56, 26);
            	_getch();
			continue;
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
		    
		    if (stcMoi == -1) {
		        ShowCur(false); InThongBaoContent("Da huy!", 14, 50, 26);
		        _getch(); continue;
		    }
		
		    strcpy(dsMH.data[cursor].tenMH, tenMoi.c_str());
		    dsMH.data[cursor].soTinChi = stcMoi;
		    
		    ShowCur(false);
		    SaveMonHoc(dsMH);
		    InThongBaoContent("Cap nhat thanh cong!", 10, 50, 26);
		    _getch();
		}
        else if ((ch == 83 || toupper(ch) == 'D') && totalItems > 0) {
            XoaVungContent();
            string thongBao = "XAC NHAN XOA MON: " + string(dsMH.data[cursor].tenMH);
            
            if (XacNhanMenu(40, 12, thongBao)) {
                for (int i = cursor; i < dsMH.soLuong - 1; i++) {
                    dsMH.data[i] = dsMH.data[i + 1];
                }
                dsMH.soLuong--;
                
                if (cursor >= dsMH.soLuong && cursor > 0) cursor--;
            
                SaveMonHoc(dsMH);
                InThongBaoContent("Da xoa mon hoc!", 10, 50, 26);
            } else {
                InThongBaoContent("Da huy xoa!", 14, 50, 26);
            }
            _getch();
        }
    }
}

void flowQuanLyCauHoiCuaMon(const char* maMH, const char* tenMH, PTR_CAUHOI &root) {
	
	KiemTraVaTaoFileCauHoi(maMH, tenMH);
	
    int currentPage = 0;
    int itemsPerPage = 2; 

    while (true) {
        ShowCur(false);
        XoaVungContent();

        // 1. Lọc danh sách câu hỏi của môn này
        CauHoi* dsTam[1000];
        int totalItems = 0;
        LayDanhSachCauHoiTheoMon(root, maMH, dsTam, totalItems);

        int totalPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;
        if (currentPage >= totalPages) currentPage = totalPages - 1;
        if (currentPage < 0) currentPage = 0;

        // 2. Hiển thị tiêu đề môn học
        gotoxy(55, 6); SetColor(0, 11);
        cout << "MON: " << tenMH << " - MA: " << maMH;
        gotoxy(105, 27); SetColor(0, 10);
        cout << "(Trang " << currentPage + 1 << "/" << totalPages << ")";
        SetColor(0, 7);

        // 3. Hiển thị danh sách câu hỏi (nếu có)
        if (totalItems == 0) {
            SetColor(0, 12);
            gotoxy(55, 12); cout << "Mon nay chua co cau hoi nao!";
        } else {
            int startIdx = currentPage * itemsPerPage;
            int endIdx = (startIdx + itemsPerPage > totalItems) ? totalItems : (startIdx + itemsPerPage);

                
            for (int i = startIdx; i < endIdx; i++) {
    			int yPos = 8 + (i - startIdx) * 9;
                
                int startX = 35;
                int idLen = strlen(dsTam[i]->id);
                int idPadding = idLen + 3;
				int contentX = startX + idPadding;
				
                SetColor(0, 14);
                gotoxy(startX, yPos);     cout << "[" << dsTam[i]->id<<"] ";
                
				SetColor(0, 7);
				InNoiDungCanLe(dsTam[i]->noiDung, contentX, yPos, 65); 

                gotoxy(35, yPos + 3); cout << "A:"; InNoiDungCanLe(dsTam[i]->a, 39, yPos + 3, 70);
                gotoxy(35, yPos + 4); cout << "B:"; InNoiDungCanLe(dsTam[i]->b, 39, yPos + 4, 70);
                gotoxy(35, yPos + 5); cout << "C:"; InNoiDungCanLe(dsTam[i]->c, 39, yPos + 5, 70);
                gotoxy(35, yPos + 6); cout << "D:"; InNoiDungCanLe(dsTam[i]->d, 39, yPos + 6, 70);
                gotoxy(35, yPos + 7); SetColor(0, 11);
                cout << ">> Dap an: " << dsTam[i]->dapAn;
                SetColor(0, 8); cout << " ---------------------------------------";
            }
        }

        gotoxy(35, 26); SetColor(0, 8);
        cout << "Left/Right: Chuyen trang";
        gotoxy(35, 27); SetColor(0, 11);
        cout << "[INS/I] Them | [E] Sua | [DEL] Xoa | [ESC] Thoat";
        SetColor(0, 7);

        int ch = _getch();
        if (ch == 224 || ch == 0 || ch == -32) {
            ch = _getch();
            if (ch == 75 && currentPage > 0) currentPage--;     
            else if (ch == 77 && currentPage < totalPages - 1) currentPage++; 
        }
        else if (ch == 27) break; 
         
        // --- Logic Them (I) ---
        else if (toupper(ch) == 'I') {
            XoaVungContent();
            ShowCur(true);
            while (_kbhit()) _getch();
            gotoxy(57, 6); SetColor(0, 11); cout << "--- THEM CAU HOI MOI ---"; SetColor(0, 7);
            gotoxy(35, 8);  cout << "Ma Mon Hoc : " << maMH;
            gotoxy(35, 10); cout << "Noi Dung   : ";
            gotoxy(35, 16); cout << "A. ";
            gotoxy(35, 18); cout << "B. ";
            gotoxy(35, 20); cout << "C. ";
            gotoxy(35, 22); cout << "D. ";
            gotoxy(35, 24); cout << "Dap An Dung: ";

            gotoxy(50, 10); string noiDung = xuongdongGioiHan(50, 10, 65, 4, 800); if (noiDung == "") continue;
            gotoxy(39, 16); string pA = xuongdongGioiHan(39, 16, 70, 2, 200); if (pA == "") continue;
            gotoxy(39, 18); string pB = xuongdongGioiHan(39, 18, 70, 2, 200); if (pB == "") continue;
            gotoxy(39, 20); string pC = xuongdongGioiHan(39, 20, 70, 2, 200); if (pC == "") continue;
            gotoxy(39, 22); string pD = xuongdongGioiHan(39, 22, 70, 2, 200); if (pD == "") continue;
            char dapAn = ChonDapAn(60, 24); if (dapAn == '\0') continue;

            CauHoi* newNode = new CauHoi();
            TaoIdCauHoi(newNode->id, maMH, nextCauHoiSoThuTu++);
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
			
		    XoaVungContent();		
		    ShowCur(true);	
				
		    gotoxy(55, 6); SetColor(0, 11); cout << "--- HIEU CHINH CAU HOI ---"; SetColor(0, 7);				    						
		    gotoxy(35, 8); cout << "Nhap ID can sua: ";		
		    string idStr = NhapMa(24); 		
		    if (idStr == "") continue;
		   		
		    CauHoi* node = TimCauHoi(root, idStr.c_str());
				
		    if (node == NULL || strcmp(node->maMH, maMH) != 0) {		
		        InThongBaoContent("Khong tim thay ID trong mon nay!", 12, 45, 26);		
		        _getch(); continue;		
		    }
				
		    XoaVungContent();
		
		    gotoxy(57, 6); SetColor(0, 11); cout << "--- SUA NOI DUNG CAU HOI ---"; SetColor(0, 7);				    		
		    gotoxy(35, 8);  cout << "Ma : " << "[" << node->id <<"]";
			gotoxy(35, 10); cout << "Noi Dung   : ";
			
		    gotoxy(35, 16); cout << "A. " ;		
		    gotoxy(35, 18); cout << "B. " ;		
		    gotoxy(35, 20); cout << "C. " ;		
		    gotoxy(35, 22); cout << "D. " ;		
		    gotoxy(35, 24); cout << "Dap An Dung (A/B/C/D): " ;
				    
				
		    gotoxy(50, 10); string ndMoi = xuongdongGioiHan(50, 10, 65, 4, 800, node->noiDung); if (ndMoi == "") continue;		
		    gotoxy(39, 16); string aMoi = xuongdongGioiHan(39, 16, 70, 2, 200, node->a); if (aMoi == "") continue;		
		    gotoxy(39, 18); string bMoi = xuongdongGioiHan(39, 18, 70, 2, 200, node->b); if (bMoi == "") continue;		
		    gotoxy(39, 20); string cMoi = xuongdongGioiHan(39, 20, 70, 2, 200, node->c); if (cMoi == "") continue;		
		    gotoxy(39, 22); string dMoi = xuongdongGioiHan(39, 22, 70, 2, 200, node->d); if (dMoi == "") continue;		    
		
		    char daMoi = ChonDapAn(60, 24); if (daMoi == '\0') continue;	
		    // Bước 4: Xác nhận và lưu	
		    gotoxy(45, 26); SetColor(0, 11);		
		    cout << "An [ENTER] de luu thay doi, [ESC] de huy.";	
		    int confirm = _getch();
		
		    if (confirm == 13) { // Phím Enter
		
		        strcpy(node->noiDung, ndMoi.c_str());
		        strcpy(node->a, aMoi.c_str());
		        strcpy(node->b, bMoi.c_str());
		        strcpy(node->c, cMoi.c_str());
		        strcpy(node->d, dMoi.c_str());
		
		        if(daMoi != '\0') node->dapAn = daMoi;
		        LuuFileCauHoiTheoMon(maMH, tenMH, root);
		        InThongBaoContent("Cap nhat thanh cong!", 10, 45, 27);		
		    } else {		
		        InThongBaoContent("Da huy thay doi.", 12, 45, 27);
		    }
		    _getch();
		}
		
		else if (toupper(ch) == 'D') {
	            XoaVungContent();
	            ShowCur(true);
	            gotoxy(55, 6); SetColor(0, 11); cout << "--- XOA CAU HOI ---"; SetColor(0, 7);
	            gotoxy(35, 8); cout << "Nhap ID can xoa : ";
	            
	            string idStr = NhapMa(24);
	            if (idStr == "") continue;
	        
	            CauHoi* node = TimCauHoi(root, idStr.c_str());
	            if (node == NULL || strcmp(node->maMH, maMH) != 0) {
	                InThongBaoContent("ID khong ton tai trong mon nay!", 12, 45, 26);
	            } else {
	                root = XoaCauHoi(root, idStr.c_str());
	                LuuFileCauHoiTheoMon(maMH, tenMH, root);
	                XoaFileNeuTrong(maMH, tenMH, root);
	                InThongBaoContent("Da xoa cau hoi!", 10, 45, 26);
	            }
	            _getch();
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
        gotoxy(105, 25); cout << "(Trang " << currentPage + 1 << "/" << totalPages << ")"; 
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
        cout << "Up/Down: Chon | Left/Right: Chuyen trang";
        gotoxy(35, 27); SetColor(0, 11);
        cout << "[INS/I] Them | [E] Sua | [DEL] Xoa | [ENTER] Xem SV | [ESC] Thoat";
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
        cout << "[INS/I] Them | [E] Sua | [DEL] Xoa | [ESC] Thoat";
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
		    // 1. Tìm node mục tiêu
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

/*void flowXemDiemSinhVien(SinhVien* sv) {
    XoaVungContent();
    gotoxy(60, 2); SetColor(0, 11); cout << "DIEM SO SINH VIEN"; SetColor(0, 7);
    
    if (sv->headDiem == NULL) {
        gotoxy(50, 12); cout << "(Sinh vien chua co diem mon hoc)";
    } else {
        gotoxy(35, 8); SetColor(0, 14);
        printf(" %-5s %-15s %-10s", "STT", "Ma Mon Hoc", "Diem");
        SetColor(0, 7);
        
        Diem* p = sv->headDiem;
        int stt = 1;
        while (p != NULL) {
            int toadoY = 10 + (stt - 1) * 2;
            gotoxy(35, toadoY);
            printf(" %-5d %-15s %-10.2f", stt, p->maMH, p->diemSo);
            p = p->next;
            stt++;
        }
    }
    
    _getch();
}
*/

void flowThiTracNghiem(SinhVien* sv, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
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
            if (testTime == -1) { ShowCur(false); return; }
            if (testTime > 0) break;
            InThongBaoContent("Thoi gian phai > 0!", 12, 70, 12);
            Sleep(800); gotoxy(70, 12); cout << "                   ";   
        }

        while (true) {
                gotoxy(63, 14); questionCount = NhapSo();
                if (questionCount == -1) { ShowCur(false); return; } 
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
