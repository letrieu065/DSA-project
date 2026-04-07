#ifndef FLOW_H
#define FLOW_H
#include "khaibao.h"
#include "hamtienich.h"
#include "saveload.h"
#include "hamchinh.h"
#include "dohoa.h"
#include "ErrorCodes.h"

void hienLoi(int errorCode);

int flowDangNhap(DanhSachLop &dsLop);

void flowQuanLyLop(DanhSachLop &dsLop);
void flowQuanLySinhVien(DanhSachLop &dsLop);
void flowGiaoVien(DanhSachLop &dsLop, DanhSachMonHoc &dsMH, PTR_CAUHOI &root);

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
    	if (isCapsLockOn()) {
    		gotoxy(45, 20); cout << "CapsLock dang bat";
		}
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
            InThongBaoContent(layThongBaoLoi(role), 12, 45, 18); 
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
    char menuChinh[5][50] = {"1. Quan Ly Lop", "2. Quan Ly Sinh Vien", "3. Quan Ly Mon Hoc", "4. Quan Ly Cau Hoi", "5. Dang Xuat"};
    
    while (true) {
        SetupConsoleLayout();
        VeLayoutChinh("GIANG VIEN"); 
        
        int chon = MenuDong(menuChinh, 5, 4, 9); 
        
        if (chon == 0) flowQuanLyLop(dsLop);
        else if (chon == 1) flowQuanLySinhVien(dsLop);
        else if (chon == 2) ;
        else if (chon == 3) ;
        else break; 
    }
}

void flowQuanLyLop(DanhSachLop &dsLop) {
    char menuCon[5][50] = {"1. Them Lop", "2. Xem Danh Sach", "3. Sua Lop", "4. Xoa Lop", "5. Quay Lai"};
    
    while (true) {
        XoaVungContent(); 
        gotoxy(60, 2); SetColor(0, 10); cout << "QUAN LY LOP HOC"; SetColor(0, 7);

        int chon = MenuDong(menuCon, 5, 4, 9); 
        
        if (chon == -1 || chon == 4) break; 
        
        if (chon == 0) { 
            XoaVungContent();
            gotoxy(55, 7); SetColor(0, 11); cout << "--- THEM LOP MOI ---"; SetColor(0, 7);
            
            gotoxy(40, 10); cout << "Nhap Ma Lop :                ";
            gotoxy(40, 12); cout << "Nhap Ten Lop:                               ";
            
            gotoxy(55, 10); string ma = NhapMa(MAX_MALOP);
            gotoxy(55, 12); string ten = NhapTen();
            
            int res = themLop(dsLop, (char*)ma.c_str(), (char*)ten.c_str());
            
            if (res == SUCCESS) {
                InThongBaoContent("Them lop thanh cong!", 10, 50, 26);
            } else {
                InThongBaoContent(layThongBaoLoi(res), 12, 50, 26);
            }
            _getch();
        }
        
        else if (chon == 1) {
        	XoaVungContent();
        	gotoxy(55, 7); SetColor(0, 11); cout << "--- DANH SACH LOP ---"; SetColor(0, 7);
        	inDanhSachLop(dsLop, 40, 12);
        	_getch();
		}
		
		else if (chon == 2) {
			XoaVungContent();
        	gotoxy(55, 7); SetColor(0, 11); cout << "--- SUA LOP ---"; SetColor(0, 7);
        	gotoxy(40, 10); cout << "Nhap Ma Lop :                ";
            gotoxy(40, 12); cout << "Nhap Ten Lop:                               ";
            
            gotoxy(55, 10); string ma = NhapMa(MAX_MALOP);
            gotoxy(55, 12); string ten = NhapTen();
            
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
		
		else if (chon == 3) {
			XoaVungContent();
        	gotoxy(55, 7); SetColor(0, 11); cout << "--- XOA LOP ---"; SetColor(0, 7);
        	gotoxy(40, 10); cout << "Nhap Ma Lop :                ";
        	gotoxy(55, 10); string ma = NhapMa(MAX_MALOP);
        	
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

void flowQuanLySinhVien(DanhSachLop &dsLop) {
    char menuCon[5][50] = {"1. Them Sinh Vien", "2. Sua Sinh Vien", "3. Xoa Sinh Vien", "4. Xem Danh Sach", "5. Quay Lai"};
    
    while (true) {
        XoaVungContent();
        gotoxy(60, 2); SetColor(0, 10); cout << "QUAN LY SINH VIEN"; SetColor(0, 7);
        
        int chon = MenuDong(menuCon, 5, 4, 9);
        if (chon == -1 || chon == 4) break; 

        XoaVungContent();
        if (chon == 0) { // --- CHỨC NĂNG NHẬP LIÊN TỤC ---
            gotoxy(55, 7); SetColor(0, 11); cout << "--- THEM SINH VIEN MOI ---"; SetColor(0, 7);
            
            // Bước 1: Hỏi xem muốn nhập vào lớp nào
            gotoxy(40, 9); cout << "Nhap Ma lop: [               ]";
            gotoxy(54, 9); string maLop = NhapMa(MAX_MALOP);
            
            int index = timViTriLop(dsLop, maLop.c_str());
            if (index == -1) {
                InThongBaoContent(layThongBaoLoi(ERR_LOP_NOT_FOUND), 12, 50, 26);
                _getch();
                continue;
            }

            // Bước 2: Vòng lặp nhập liên tục cho lớp đã chọn
            while (true) {
                XoaVungContent();
                gotoxy(40, 7); SetColor(0, 14); cout << "Lop: " << dsLop.nodes[index]->tenLop; SetColor(0, 7);
                gotoxy(40, 8); cout << "(De trong Ma SV de dung nhap)";

                // Khởi tạo struct SV tạm để hứng dữ liệu từ bàn phím
                SinhVien svMoi;
                
                gotoxy(40, 11); cout << "Ma SV: [               ]";
                gotoxy(48, 11); string ma = NhapMa(MAX_MASV);
                
                if (ma == "") break; // Thoát vòng lặp nhập liên tục nếu để trống mã

                // Kiểm tra trùng mã toàn trường trước khi cho nhập tiếp
                if (kiemTraTrungMaSinhVien(dsLop, (char*)ma.c_str())) {
                    InThongBaoContent("Loi: Ma SV da ton tai!", 12, 50, 26);
                    _getch();
                    continue;
                }

                // Nhập các thông tin còn lại
                gotoxy(40, 13); cout << "Ho   : [                              ]";
                gotoxy(40, 15); cout << "Ten  : [               ]";
                gotoxy(40, 17); cout << "Phai : [     ] (Nam/Nu)";

                gotoxy(48, 13); string ho = NhapTen();
                gotoxy(48, 15); string ten = NhapTen();
                gotoxy(48, 17); string phai = NhapTen();

                // Đổ dữ liệu vào struct tạm
                strcpy(svMoi.maSV, ma.c_str());
                strcpy(svMoi.ho, ho.c_str());
                strcpy(svMoi.ten, ten.c_str());
                strcpy(svMoi.phai, phai.c_str());
                strcpy(svMoi.password, "123456"); // Pass mặc định
                svMoi.headDiem = NULL;

                // GỌI HÀM CỦA BẠN Ở ĐÂY
                int res = themSinhVien(dsLop.nodes[index], svMoi);
                
                if (res == SUCCESS) {
                    InThongBaoContent("Them thanh cong! Tiep tuc...", 10, 50, 26);
                    SaveLop(dsLop); // Lưu lại file sau mỗi lần thêm
                }
                _getch();
            }
        }
        // ... (Sua, Xoa, Xem)
    }
}

#endif