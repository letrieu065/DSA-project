#ifndef HAMCHINH_H
#define HAMCHINH_H

#include "ErrorCodes.h"
#include "khaibao.h"
#include "dohoa.h"

// Ham them lop
int KiemTraTrungMaLop(DanhSachLop &dsLop, char* maNhap) {
    for (int i = 0; i < dsLop.soLuong; i++) {
        if (strcmp(dsLop.nodes[i]->maLop, maNhap) == 0) {
            return i; 
        }
    }
    return -1; 
}

int themLop(DanhSachLop &dsLop, char* ma, char* ten) {
	if (dsLop.soLuong >= MAX_LOP) return ERR_LOP_FULL;
	
	if (KiemTraTrungMaLop(dsLop, ma) != -1) return ERR_LOP_DUPLICATE;
	
	int i = dsLop.soLuong;
    dsLop.nodes[i] = new Lop;
    
    strcpy(dsLop.nodes[i]->maLop, ma);
    strcpy(dsLop.nodes[i]->tenLop, ten);
    dsLop.nodes[i]->headSV = NULL;

    dsLop.soLuong++;
    return SUCCESS; 
}

int timViTriLop(DanhSachLop &dsLop, const char* maLop_nhap) {
    for (int i = 0; i < dsLop.soLuong; i++) {
        if (strcmp(dsLop.nodes[i]->maLop, maLop_nhap) == 0) {
            return i;
        }
    }
    return ERR_LOP_NOT_FOUND;
}

// In danh sach lop
void inDanhSachLop(DanhSachLop &dsLop, int x, int y) {	
	for (int i = 0; i < dsLop.soLuong; i++) {
		if (dsLop.nodes[i] != NULL) {
			char* ma = dsLop.nodes[i]->maLop;
			char* ten = dsLop.nodes[i]->tenLop;
			
			gotoxy(x, y + i); cout << i + 1 << ". " << ma << " - " << ten;
		}
	}
}

// Ham sua lop
int suaLop(DanhSachLop &dsLop, char* ma, char* tenMoi) {
    int index = timViTriLop(dsLop, ma);
    
    if (index == ERR_LOP_NOT_FOUND) {
        return ERR_LOP_NOT_FOUND;
    }
    
    strcpy(dsLop.nodes[index]->tenLop, tenMoi);
    
    return SUCCESS;
}

// Ham xoa lop
int xoaLop(DanhSachLop &dsLop, char* ma) {
	int index = timViTriLop(dsLop, ma);
	
	if (index == ERR_LOP_NOT_FOUND) {
        return ERR_LOP_NOT_FOUND;
    }
    
    if (dsLop.nodes[index]->headSV != NULL) {
        return ERR_LOP_HAS_SV; 
    }
    
    delete dsLop.nodes[index];
    
    for (int i = index; i < dsLop.soLuong - 1; i++) {
        dsLop.nodes[i] = dsLop.nodes[i + 1];
    }
    
    dsLop.soLuong--;
    
    return SUCCESS;
}

// Ham tiem kiem sinh vien
SinhVien* timKiemSinhVien(DanhSachLop &dsLop, char* maSV) {
    for (int i = 0; i < dsLop.soLuong; i++) {
        SinhVien* p = dsLop.nodes[i]->headSV;
        while (p != NULL) {
            if (strcmp(p->maSV, maSV) == 0) {
                return p; 
            }
            p = p->next;
        }
    }
    return NULL;
}

// Ham kiem tra trung ma sinh vien
int kiemTraTrungMaSinhVien(DanhSachLop &dsLop, char* maSV) {
	for (int i = 0; i < dsLop.soLuong; i++) {
		SinhVien* p = dsLop.nodes[i]->headSV;
		while (p != NULL) {
			if (strcmp(p->maSV, maSV) == 0) return ERR_SV_DUPLICATE;
			p = p->next;
		}
	}
	return SUCCESS;
}

// Ham them sinh vien
int themSinhVien(Lop* lop, SinhVien sv) {
	SinhVien * newNode = new SinhVien;
	if (newNode == NULL) return ERR_SV_FULL;
	
	*newNode = sv;
	
	newNode->next = lop->headSV;
	lop->headSV = newNode;
	
	return SUCCESS;
}

// Ham sua sinh vien

// Ham xoa sinh vien
int xoaSinhVien(Lop* lop, char* maSV) {
	SinhVien *p = lop->headSV;
	SinhVien* prev = NULL;
	
	while (p != NULL && strcmp(p->maSV, maSV) != 0) {
		prev = p;
		p = p->next;
	}
	
	if (p == NULL) return ERR_SV_NOT_FOUND;
	
	if (prev == NULL) lop->headSV = p->next;
	
	else prev->next = p->next;
	
	delete p;
    return SUCCESS;
}
#endif