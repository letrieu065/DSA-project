#ifndef FLOW_H
#define FLOW_H
#include "khaibao.h"
#include "hamtienich.h"
#include "saveload.h"
#include "hamchinh.h"
#include "dohoa.h"
#include "ErrorCodes.h"

int nextCauHoiId = 1;



int flowDangNhap(DanhSachLop &dsLop);
void flowGiaoVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root);
void flowQuanLyLop_SinhVien(DanhSachLop &dsLop);
void flowQuanLySinhVienCuaLop(DanhSachLop &dsLop, Lop* lopHienTai);
//void flowQuanLyLop(DanhSachLop &dsLop);
//void flowQuanLySinhVien(DanhSachLop &dsLop);
void flowQuanLyMonHoc(DanhSachMonHoc &dsMH);
void flowQuanLyCauHoi(PTR_CAUHOI &root);

void flowChinh(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
    while (true) {
        int role = flowDangNhap(dsLop); 
        
        if (role == 1) {
            flowGiaoVien(dsLop, dsMH, root);
        }
        else if (role == 2) {
        	
        }
    }
}

int flowDangNhap(DanhSachLop &dsLop) {
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
        string pass = NhapMatKhau(); 
        int passCheck = xacThucMatKhau(role, pass, svHienTai);

        if (passCheck == SUCCESS) return role;
        else {
            InThongBaoContent(layThongBaoLoi(passCheck), 12, 45, 18);
            _getch();
            continue;
        }
    }
}

void flowGiaoVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root) {
    char menuChinh[4][50] = {"1. Quan Ly Lop", "2. Quan Ly Mon Hoc", "3. Quan Ly Cau Hoi", "4. Dang Xuat "};
    
    while (true) {
        SetupConsoleLayout();
        VeLayoutChinh("GIANG VIEN"); 
        
        int chon = MenuDong(menuChinh, 4, 4, 9); 
        
        //if (chon == 0) flowQuanLyLop(dsLop);
        if (chon == 0) flowQuanLyLop_SinhVien(dsLop);
        else if (chon == 1) flowQuanLyMonHoc(dsMH);
        else if (chon == 2) flowQuanLyCauHoi(root);
        else break; 
    }
}

/*void flowQuanLyLop(DanhSachLop &dsLop) {
    char menuCon[5][50] = {"1. Them Lop", "2. Xem Danh Sach", "3. Sua Lop", "4. Xoa Lop", "5. Quay Lai"};
    
    while (true) {
        XoaVungContent(); 
        gotoxy(60, 2); SetColor(0, 10); cout << "QUAN LY LOP HOC"; SetColor(0, 7);

        int chon = MenuDong(menuCon, 5, 4, 9); 
        
        if (chon == -1 || chon == 4) break; 
        
        if (chon == 0) {
			while (true) {
				XoaVungContent();
				gotoxy(100, 27); cout << "ESC: de thoat";
	            gotoxy(55, 7); SetColor(0, 11); cout << "--- THEM LOP MOI ---"; SetColor(0, 7);
	            gotoxy(40, 10); cout << "Nhap Ma Lop :                ";
	            gotoxy(40, 12); cout << "Nhap Ten Lop:                               ";
	            
	            gotoxy(55, 10); string ma = NhapMa(MAX_MALOP); if (ma == "") break;
	            gotoxy(55, 12); string ten = NhapTen(); if (ten == "") break;
	            
	            int res = themLop(dsLop, (char*)ma.c_str(), (char*)ten.c_str());
	            
	            if (res == SUCCESS) {
	                InThongBaoContent("Them lop thanh cong!", 10, 50, 26);
	            } else {
	                InThongBaoContent(layThongBaoLoi(res), 12, 50, 26);
	            }
	            _getch();
			} 
        }
        
        else if (chon == 1) {
        	XoaVungContent();
        	gotoxy(55, 7); SetColor(0, 11); cout << "--- DANH SACH LOP ---"; SetColor(0, 7);
        	inDanhSachLop(dsLop, 40, 12);
        	_getch();
		}
		
		else if (chon == 2) {
			while (true) {
				XoaVungContent();
				gotoxy(100, 27); cout << "ESC: de thoat";
				gotoxy(55, 7); SetColor(0, 11); cout << "--- SUA LOP ---"; SetColor(0, 7);
	        	gotoxy(40, 10); cout << "Nhap Ma Lop :                "; 
	            gotoxy(40, 12); cout << "Nhap Ten Lop:                               ";
	            
	            gotoxy(55, 10); string ma = NhapMa(MAX_MALOP); if (ma == "") break;
	            gotoxy(55, 12); string ten = NhapTen(); if (ten == "") break;
	            
	            int index = suaLop(dsLop, (char*)ma.c_str(), (char*)ten.c_str());
	            
				if (index == ERR_LOP_NOT_FOUND) {
	        		InThongBaoContent(layThongBaoLoi(index), 12, 50, 26);
	    		}
	    		else {
	        		InThongBaoContent("Cap nhat ten lop thanh cong!", 10, 50, 26);
	        		SaveLop(dsLop); 
	    		}
	    		_getch(); 
			}
		}
		
		else if (chon == 3) {
			while (true) {
				XoaVungContent();
				gotoxy(100, 27); cout << "ESC: de thoat";
	        	gotoxy(55, 7); SetColor(0, 11); cout << "--- XOA LOP ---"; SetColor(0, 7);
	        	gotoxy(40, 10); cout << "Nhap Ma Lop :                ";
	        	gotoxy(55, 10); string ma = NhapMa(MAX_MALOP); if (ma == "") break;
	        	
	        	int res = xoaLop(dsLop, (char*)ma.c_str());
	        	
	        	if (res == ERR_LOP_NOT_FOUND) {
	        		InThongBaoContent(layThongBaoLoi(res), 12, 50, 26);
	    		}
	    		else if (res == ERR_LOP_HAS_SV) {
	    			InThongBaoContent(layThongBaoLoi(res), 12, 50, 26);
				}
	    		else {
	        		InThongBaoContent("Xoa lop thanh cong!", 10, 50, 26);
	        		SaveLop(dsLop); 
	    		}
	    		_getch(); 
			}
		}
    }
}

void flowQuanLySinhVien(DanhSachLop &dsLop) {
    char menuCon[5][50] = {"1. Them Sinh Vien", "2. Sua Sinh Vien", "3. Xoa Sinh Vien", "4. Xem Danh Sach", "5. Quay Lai"};
    
    while (true) {
        XoaVungContent();
        gotoxy(60, 2); SetColor(0, 10); cout << "QUAN LY SINH VIEN"; SetColor(0, 7);
        
        int chon = MenuDong(menuCon, 5, 4, 9);
        if (chon == -1 || chon == 4) break; 

        XoaVungContent();
        if (chon == 0) {
        	while (true) {
        		XoaVungContent();
				gotoxy(100, 27); cout << "ESC: de thoat";
	            gotoxy(55, 7); SetColor(0, 11); cout << "--- THEM SINH VIEN MOI ---"; SetColor(0, 7);
	            
	            gotoxy(40, 9); cout << "Nhap Ma lop:                ";
	            gotoxy(54, 9); string maLop = NhapMa(MAX_MALOP); if (maLop == "") break;
	            
	            int index = timViTriLop(dsLop, maLop.c_str());
	            if (index == -1) {
	                InThongBaoContent(layThongBaoLoi(ERR_LOP_NOT_FOUND), 12, 50, 26);
	                _getch();
	                continue;
	            }
	
	            while (true) {
				    XoaVungContent();
				    gotoxy(40, 7); SetColor(0, 14); cout << "Lop: " << dsLop.nodes[index]->tenLop; SetColor(0, 7);
				    gotoxy(40, 8); SetColor(0, 8); cout << "(Nhan ESC hoac de trong Ma SV de quay lai)"; SetColor(0, 7);
				
				    while(kbhit()) _getch(); 
				
				    gotoxy(40, 11); cout << "Ma SV:                 ";
				    gotoxy(50, 11); string ma = NhapMa(MAX_MASV); 
				    
				    if (ma == "" || ma.empty()) break; 
				
				    if (kiemTraTrungMaSinhVien(dsLop, ma.c_str()) == ERR_SV_DUPLICATE) {
				        InThongBaoContent("Loi: Ma SV da ton tai!", 12, 50, 26);
				        _getch();
				        continue;
				    }
	
			    gotoxy(40, 13); cout << "Ho   :                               ";
			    gotoxy(48, 13); string ho = NhapTen();
			    
			    gotoxy(40, 15); cout << "Ten  :                 ";
			    gotoxy(48, 15); string ten = NhapTen();
			
			    gotoxy(40, 17); cout << "Phai :       (Nam/Nu)";
				bool phai = NhapGioiTinh(48, 17);
			
			    SinhVien svMoi;
			    snprintf(svMoi.maSV, sizeof(svMoi.maSV), "%s", ma.c_str());
			    snprintf(svMoi.ho, sizeof(svMoi.ho), "%s", ho.c_str());
			    snprintf(svMoi.ten, sizeof(svMoi.ten), "%s", ten.c_str());
			    svMoi.phai = phai;
			    strcpy(svMoi.password, "123456");
			    svMoi.headDiem = NULL;
			
			    int res = themSinhVien(dsLop.nodes[index], svMoi);
			    
			    if (res == SUCCESS) {
			        InThongBaoContent("Them thanh cong! Tiep tuc...", 10, 50, 26);
			        SaveLop(dsLop); 
			    } else {
			        InThongBaoContent("Loi cap phat bo nho!", 12, 50, 26);
			    }
			    _getch();
				}
			}
        }
    else if (chon == 1) {
    	while (true) {
    		XoaVungContent();
			gotoxy(100, 27); cout << "ESC: de thoat";
			gotoxy(55, 7); SetColor(0, 11); cout << "--- SUA SINH VIEN ---"; SetColor(0, 7);
		    gotoxy(40, 9);  cout << "Nhap Ma lop:                ";
		    gotoxy(54, 9);  string maLop = NhapMa(MAX_MALOP); if (maLop == "") break;
		
		    int index = timViTriLop(dsLop, maLop.c_str());
		    if (index == -1) {
		        InThongBaoContent(layThongBaoLoi(ERR_LOP_NOT_FOUND), 12, 50, 26);
		        _getch();
		        continue;
		    }
		
		    gotoxy(40, 11); cout << "Nhap Ma SV can sua:                ";
		    gotoxy(60, 11); string maSV = NhapMa(MAX_MASV);
		
		    SinhVien* svCu = timKiemSinhVien(dsLop.nodes[index], maSV.c_str());
		    if (svCu == NULL) {
		        InThongBaoContent(layThongBaoLoi(ERR_SV_NOT_FOUND), 12, 50, 26);
		        _getch();
		        continue;
		    }
		
			gotoxy(40, 13); cout << "Ho hien tai   : " << svCu->ho;
			gotoxy(40, 14); cout << "Ten hien tai  : " << svCu->ten;
			gotoxy(40, 15); cout << "Phai hien tai : " << (svCu->phai == 0 ? "Nam" : "Nu");		
			
			gotoxy(40, 17); cout << "Ho moi   (Enter de giu nguyen): ";
			gotoxy(72, 17); string ho = NhapChuoiChu(); 
			
			gotoxy(40, 18); cout << "Ten moi  (Enter de giu nguyen): ";
			gotoxy(72, 18); string ten = NhapChuoiChu();
			
			gotoxy(40, 19); cout << "Phai moi (Enter de giu nguyen): ";
			bool phaiMoi = NhapGioiTinh(72, 19); 
			
			SinhVien svMoi;
			strncpy(svMoi.ho, ho.c_str(), sizeof(svMoi.ho) - 1);
			strncpy(svMoi.ten, ten.c_str(), sizeof(svMoi.ten) - 1);
			svMoi.phai = phaiMoi;
			
			int res = suaSinhVien(dsLop.nodes[index], maSV.c_str(), svMoi);
			
			if (res == SUCCESS) {
			    InThongBaoContent("Sua thanh cong!", 10, 50, 26);
			    SaveLop(dsLop); 
			} else {
			    InThongBaoContent(layThongBaoLoi(res), 12, 50, 26);
			}
			_getch();
		}
	}
	else if (chon == 2) {
		while (true) {
			XoaVungContent();
			gotoxy(100, 27); cout << "ESC: de thoat";
			gotoxy(55, 7); SetColor(0, 11); cout << "--- XOA SINH VIEN ---"; SetColor(0, 7);
		    gotoxy(40, 9);  cout << "Nhap Ma lop:                ";
		    gotoxy(54, 9);  string maLop = NhapMa(MAX_MALOP); if (maLop == "") break;
		
		    int index = timViTriLop(dsLop, maLop.c_str());
		    if (index == -1) {
		        InThongBaoContent(layThongBaoLoi(ERR_LOP_NOT_FOUND), 12, 50, 26);
		        _getch();
		        continue;
		    }
		
		    gotoxy(40, 11); cout << "Nhap Ma SV can xoa:                ";
		    gotoxy(60, 11); string maSV = NhapMa(MAX_MASV);
		
		    SinhVien* sv = timKiemSinhVien(dsLop.nodes[index], maSV.c_str());
		    if (sv == NULL) {
		        InThongBaoContent(layThongBaoLoi(ERR_SV_NOT_FOUND), 12, 50, 26);
		        _getch();
		        continue;
		    }
		
		    gotoxy(40, 13); cout << "Sinh vien: " << sv->ho << " " << sv->ten;
		    gotoxy(40, 15); cout << "Ban co chac muon xoa? (y/n): ";
		    char confirm = _getch();
		    if (confirm != 'y' && confirm != 'Y') {
		        InThongBaoContent("Da huy thao tac xoa.", 14, 50, 26);
		        _getch();
		        continue;
		    }
		
		    int res = xoaSinhVien(dsLop.nodes[index], maSV.c_str());
		    if (res == SUCCESS) {
		        InThongBaoContent("Xoa thanh cong!", 10, 50, 26);
		        SaveLop(dsLop);
		    } else {
		        InThongBaoContent(layThongBaoLoi(res), 12, 50, 26);
		    }
		    _getch();
		}
	}
	else if (chon == 3) {
		while (true) {
			gotoxy(40, 9); cout << "Nhap Ma lop:                ";
	        gotoxy(54, 9); string maLop = NhapMa(MAX_MALOP); if (maLop == "") break;
	
	        int index = timViTriLop(dsLop, maLop.c_str());
	
	            if (index == -1) {
	                InThongBaoContent(layThongBaoLoi(ERR_LOP_NOT_FOUND), 12, 50, 26);
					}
	            else {
	                XoaVungContent();
	                gotoxy(55, 7); SetColor(0, 11); cout << "--- DANH SACH SINH VIEN ---"; SetColor(0, 7);
	
	                inDanhSachSinhVien(dsLop.nodes[index], 42, 10);
				}
	            _getch();	
			}
		}
	}
}
*/

void flowQuanLyMonHoc(DanhSachMonHoc &dsMH) {
	char menuCon[5][50] = {"1. Them Mon Hoc", "2. Sua Mon", "3. Xoa Mon", "4. Xem Danh Sach", "5. Quay Lai"};
    
    while (true) {
        XoaVungContent();
        gotoxy(60, 2); SetColor(0, 10); cout << "QUAN LY SINH VIEN"; SetColor(0, 7);
        
        int chon = MenuDong(menuCon, 5, 4, 9);
        if (chon == -1 || chon == 4) break;
		
		if (chon == 1) {
			while (true) {
				XoaVungContent();
				gotoxy(100, 27); cout << "ESC: de thoat";
				gotoxy(55, 7); SetColor(0, 11); cout << "--- THEM MON HOC ---"; SetColor(0, 7);
				gotoxy(40, 10); cout << "Nhap Ma Mon Hoc :                               ";
	            gotoxy(40, 12); cout << "Nhap Ten Mon Hoc:                               ";
	            gotoxy(40, 14); cout << "Nhap so tin chi :                               ";
	            
	            gotoxy(55, 10); string ma = NhapMa(MAX_MAMH); if (ma == "") break;
	            gotoxy(55, 12); string ten = NhapTen(); if (ten == "") break;
	            gotoxy(55, 14); int stc = NhapSo();
			}
		} 

        if (chon == -1 || chon == 4) break; 
	}	
}

void flowQuanLyCauHoi(PTR_CAUHOI &root) {
    char menuCon[5][50] = {
        "1. Them Cau Hoi",
        "2. Hieu Chinh Cau Hoi",
        "3. Xoa Cau Hoi",
        "4. Xem Danh Sach",
        "5. Quay Lai"
    };

    while (true) {
        XoaVungContent();
        gotoxy(60, 2); SetColor(0, 10); cout << "QUAN LY CAU HOI"; SetColor(0, 7);

        int chon = MenuDong(menuCon, 5, 4, 9);
        if (chon == -1 || chon == 4) break;

        // --------------------------------------------------
        // THEM CAU HOI
        // --------------------------------------------------
        if (chon == 0) {
            XoaVungContent();
            gotoxy(55, 6); SetColor(0, 11); cout << "--- THEM CAU HOI MOI ---"; SetColor(0, 7);

            // Nhap ma mon hoc
            gotoxy(35, 8);  cout << "Ma Mon Hoc : [              ]";
            gotoxy(35, 10); cout << "Noi Dung   : [                                        ]";
            gotoxy(35, 12); cout << "A. [                    ]";
            gotoxy(35, 13); cout << "B. [                    ]";
            gotoxy(35, 14); cout << "C. [                    ]";
            gotoxy(35, 15); cout << "D. [                    ]";
            gotoxy(35, 17); cout << "Dap An Dung (A/B/C/D): [  ]";

            gotoxy(50, 8);  string maMH    = NhapMa(MAX_MAMH);
            gotoxy(50, 10); string noiDung = NhapTen();
            gotoxy(39, 12); string pA      = NhapTen();
            gotoxy(39, 13); string pB      = NhapTen();
            gotoxy(39, 14); string pC      = NhapTen();
            gotoxy(39, 15); string pD      = NhapTen();
            gotoxy(59, 17); string dapStr  = NhapMa(1);
            char dapAn = toupper(dapStr[0]);

            // Tao node moi
            CauHoi* newNode = new CauHoi();
            newNode->id     = nextCauHoiId++;
            newNode->dapAn  = dapAn;
            newNode->left   = NULL;
            newNode->right  = NULL;
            strcpy(newNode->maMH,    maMH.c_str());
            strcpy(newNode->noiDung, noiDung.c_str());
            strcpy(newNode->a, pA.c_str());
            strcpy(newNode->b, pB.c_str());
            strcpy(newNode->c, pC.c_str());
            strcpy(newNode->d, pD.c_str());

            root = ThemCauHoi(root, newNode);

            InThongBaoContent("Them cau hoi thanh cong! ID = " + to_string(newNode->id), 10, 45, 26);
            _getch();
        }

        // --------------------------------------------------
        // HIEU CHINH CAU HOI
        // --------------------------------------------------
        else if (chon == 1) {
            XoaVungContent();
            gotoxy(55, 6); SetColor(0, 11); cout << "--- HIEU CHINH CAU HOI ---"; SetColor(0, 7);

            gotoxy(35, 8); cout << "Nhap ID can sua: [     ]";
            gotoxy(53, 8);
            int id = NhapSo();

            CauHoi* node = TimCauHoi(root, id);
            if (node == NULL) {
                InThongBaoContent("Khong tim thay cau hoi ID = " + to_string(id), 12, 45, 26);
                _getch();
                continue;
            }

            // Hien thi du lieu cu
            gotoxy(35, 10); SetColor(0, 14); cout << "Du lieu cu:"; SetColor(0, 7);
            gotoxy(37, 11); cout << "ND: " << node->noiDung;
            gotoxy(37, 12); cout << "A." << node->a << " B." << node->b;
            gotoxy(37, 13); cout << "C." << node->c << " D." << node->d;
            gotoxy(37, 14); cout << "Dap an: " << node->dapAn;

            // Nhap du lieu moi
            gotoxy(35, 16); SetColor(0, 11); cout << "Nhap moi:"; SetColor(0, 7);
            gotoxy(35, 17); cout << "Noi Dung: [                                      ]";
            gotoxy(35, 18); cout << "A. [              ]";
            gotoxy(35, 19); cout << "B. [              ]";
            gotoxy(35, 20); cout << "C. [              ]";
            gotoxy(35, 21); cout << "D. [              ]";
            gotoxy(35, 22); cout << "Dap An (A/B/C/D): [  ]";

            gotoxy(45, 17); string nd  = NhapTen();
            gotoxy(39, 18); string nA  = NhapTen();
            gotoxy(39, 19); string nB  = NhapTen();
            gotoxy(39, 20); string nC  = NhapTen();
            gotoxy(39, 21); string nD  = NhapTen();
            gotoxy(54, 22); string dStr = NhapMa(1);
            char dapAn = toupper(dStr[0]);

            HieuChinhCauHoi(root, id,
                nd.c_str(), nA.c_str(), nB.c_str(),
                nC.c_str(), nD.c_str(), dapAn);

            InThongBaoContent("Cap nhat cau hoi ID = " + to_string(id) + " thanh cong!", 10, 45, 26);
            _getch();
        }

        // --------------------------------------------------
        // XOA CAU HOI
        // --------------------------------------------------
        else if (chon == 2) {
            XoaVungContent();
            gotoxy(55, 6); SetColor(0, 11); cout << "--- XOA CAU HOI ---"; SetColor(0, 7);

            gotoxy(35, 8); cout << "Nhap ID can xoa: [     ]";
            gotoxy(53, 8);
            int id = NhapSo();

            if (TimCauHoi(root, id) == NULL) {
                InThongBaoContent("Khong tim thay cau hoi ID = " + to_string(id), 12, 45, 26);
            } else {
                root = XoaCauHoi(root, id);
                InThongBaoContent("Da xoa cau hoi ID = " + to_string(id), 10, 45, 26);
            }
            _getch();
        }

        // --------------------------------------------------
        // XEM DANH SACH (In-order theo ID)
        // --------------------------------------------------
        else if (chon == 3) {
            XoaVungContent();
            gotoxy(55, 6); SetColor(0, 11); cout << "--- DANH SACH CAU HOI (tang dan theo ID) ---"; SetColor(0, 7);

            if (root == NULL) {
                InThongBaoContent("Chua co cau hoi nao trong he thong.", 12, 45, 26);
            } else {
                int soThuTu = 0;
                InOrderCauHoi(root, soThuTu, 34, 8);
            }
            _getch();
        }
    }
}

// TEST
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
            
            gotoxy(40, 10); cout << "Ma Lop hien tai: " << dsLop.nodes[cursor]->maLop;
            gotoxy(40, 12); cout << "Nhap Ten Lop moi:                               ";
            
            gotoxy(58, 12); string tenMoi = NhapTen(); if (tenMoi == "") continue;
            
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
            gotoxy(47, 10); string ma = NhapMa(MAX_MASV); if (ma == "") continue;
            
            if (kiemTraTrungMaSinhVien(dsLop, ma.c_str()) == ERR_SV_DUPLICATE) {
                InThongBaoContent("Loi: Ma SV nay da ton tai trong he thong!", 12, 45, 26);
                _getch(); continue;
            }

            gotoxy(40, 12); cout << "Ho   :                               ";
            gotoxy(47, 12); string ho = NhapTen(); if (ho == "") continue;
            
            gotoxy(40, 14); cout << "Ten  :                 ";
            gotoxy(47, 14); string ten = NhapTen(); if (ten == "") continue;
        
            gotoxy(40, 16); cout << "Phai :       Nam/Nu";
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
        
        if ((toupper(ch) == 'E' || isDelete) && totalItems > 0) {
            SinhVien* svTarget = lopHienTai->headSV;
            for (int i = 0; i < cursor && svTarget != NULL; i++) {
                svTarget = svTarget->next;
            }
            
            if (svTarget == NULL) continue; 
            
            if (toupper(ch) == 'E') {
                XoaVungContent();
                gotoxy(100, 27); cout << "ESC: de thoat";
                gotoxy(55, 7); SetColor(0, 11); cout << "--- SUA SINH VIEN ---"; SetColor(0, 7);
                
                gotoxy(40, 10); cout << "Dang sua SV: " << svTarget->maSV;
                gotoxy(40, 12); cout << "Ho hien tai  : " << svTarget->ho;
                gotoxy(40, 13); cout << "Ten hien tai : " << svTarget->ten;
                
                gotoxy(40, 15); cout << "Ho moi  (Enter de giu nguyen): ";
                gotoxy(71, 15); string hoMoi = NhapChuoiChu(); 
                
                gotoxy(40, 16); cout << "Ten moi (Enter de giu nguyen): ";
                gotoxy(71, 16); string tenMoi = NhapChuoiChu();
                
                gotoxy(40, 17); cout << "Phai moi (Nam/Nu)            : ";
                bool phaiMoi = NhapGioiTinh(71, 17); 
                
                SinhVien svMoiData;
                strncpy(svMoiData.ho, hoMoi.c_str(), sizeof(svMoiData.ho));
                strncpy(svMoiData.ten, tenMoi.c_str(), sizeof(svMoiData.ten));
                svMoiData.phai = phaiMoi;
                
                int res = suaSinhVien(lopHienTai, svTarget->maSV, svMoiData);
                if (res == SUCCESS) {
                    InThongBaoContent("Cap nhat thong tin thanh cong!", 10, 45, 26);
                    SaveLop(dsLop); 
                }
                _getch();
            }
            
            if (isDelete) {
                XoaVungContent();
                gotoxy(55, 7); SetColor(0, 11); cout << "--- XOA SINH VIEN ---"; SetColor(0, 7);
                
                gotoxy(40, 10); cout << "Ban co chac muon xoa SV: ";
                SetColor(0, 12); cout << svTarget->ho << " " << svTarget->ten << " (" << svTarget->maSV << ")"; SetColor(0, 7);
                cout << "? (y/n): ";
                
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

#endif