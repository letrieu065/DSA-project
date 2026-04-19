#ifndef HAMCHINH_H
#define HAMCHINH_H

#include "ErrorCodes.h"
#include "khaibao.h"
#include "dohoa.h"
int demSoSinhVien(Lop* lop);
int themMonHoc(DanhSachMonHoc &dsMH, MonHoc* mh);
int suaMonHoc(DanhSachMonHoc &dsMH, const char* ma, const char* tenMoi, int tinChiMoi);
int xoaMonHoc(DanhSachMonHoc &dsMH, const char* ma, PTR_CAUHOI root);
int timViTriMonHoc(DanhSachMonHoc &dsMH, const char* ma);
bool kiemTraTrungMaMonHoc(DanhSachMonHoc &dsMH, const char* ma);
bool kiemTraMonHocDaCoCauHoi(PTR_CAUHOI root, const char* maMH);
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
SinhVien* timKiemSinhVien(Lop* lop, const char* maSV) {
    if (lop == NULL) return NULL;

    SinhVien* p = lop->headSV;

    while (p != NULL) {
        if (strcmp(p->maSV, maSV) == 0)
            return p;
        p = p->next;
    }

    return NULL;
}
SinhVien* timKiemSinhVien(DanhSachLop &dsLop, const char* maSV) {
    for (int i = 0; i < dsLop.soLuong; i++) {
    	if (dsLop.nodes[i] == NULL) continue;
    	
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
int kiemTraTrungMaSinhVien(DanhSachLop &dsLop, const char* maSV) {
	if (timKiemSinhVien(dsLop, maSV) !=NULL) {
		return ERR_SV_DUPLICATE;
	}
	return SUCCESS;
}
// Ham them sinh vien
int themSinhVien(Lop* lop, SinhVien sv) {
	if (lop == NULL) return ERR_LOP_NOT_FOUND;
	SinhVien * newNode = new SinhVien;
	
	*newNode = sv;
	
	newNode->next = lop->headSV;
	lop->headSV = newNode;
	
	return SUCCESS;
}
// Ham sua sinh vien
int suaSinhVien(Lop* lop, const char* maSV, SinhVien svMoi) {
	if (lop == NULL) return ERR_LOP_NOT_FOUND;
	
	SinhVien* p = lop ->headSV;
	
	while (p != NULL) {
		if (strcmp(p->maSV, maSV) == 0) {
			if (strlen(svMoi.ho) > 0)
			    snprintf(p->ho, sizeof(p->ho), "%s", svMoi.ho);
			
			if (strlen(svMoi.ten) > 0)
			    snprintf(p->ten, sizeof(p->ten), "%s", svMoi.ten);
			
			if (strlen(svMoi.phai) > 0)
			    snprintf(p->phai, sizeof(p->phai), "%s", svMoi.phai);
			
			return SUCCESS;
		}
		p = p->next;
	}
	return ERR_SV_NOT_FOUND;
}
// Ham xoa sinh vien
int xoaSinhVien(Lop* lop, const char* maSV) {
	if (lop == NULL) return ERR_LOP_NOT_FOUND;
	
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
int demSoSV(Lop* lop) {
    if (lop == NULL) return 0;

    int dem = 0;
    SinhVien* p = lop->headSV;

    while (p != NULL) {
        dem++;
        p = p->next;
    }

    return dem;
}
//Them mon hoc
int themMonHoc(DanhSachMonHoc &dsMH, MonHoc* mh) {
    if (mh == NULL) return ERR_MH_INVALID_DATA;

    if (dsMH.soLuong >= MAX_MON_HOC) 
        return ERR_MH_FULL;
    
    if (strlen(mh->maMH) == 0 || strlen(mh->tenMH) == 0 || mh->soTinChi <= 0)
        return ERR_MH_INVALID_DATA;

    if (kiemTraTrungMaMonHoc(dsMH, mh->maMH)) 
        return ERR_MH_DUPLICATE;
        

	dsMH.data[dsMH.soLuong++] = *mh;
     
    return SUCCESS;
}
//Sua mon hoc
int suaMonHoc(DanhSachMonHoc &dsMH, const char* ma, const char* tenMoi, int tinChiMoi) {
    int i = timViTriMonHoc(dsMH, ma);
    
    if (i == -1) return ERR_MH_NOT_FOUND;
    
    if (tenMoi != NULL && strlen(tenMoi) > 0) {
    	snprintf(dsMH.data[i].tenMH, sizeof(dsMH.data[i].tenMH), "%s", tenMoi);	
	}
	if (tinChiMoi != -1) {
		if (tinChiMoi <= 0) return ERR_MH_INVALID_DATA;
		dsMH.data[i].soTinChi = tinChiMoi;
	}
	return SUCCESS;
}
//Xoa mon hoc
int xoaMonHoc(DanhSachMonHoc &dsMH, const char* ma, PTR_CAUHOI root) {
    int idx = timViTriMonHoc(dsMH, ma);
    if (idx == -1) return ERR_MH_NOT_FOUND;
    
    if (kiemTraMonHocDaCoCauHoi (root, ma)) {
    	return ERR_MH_HAS_QUESTION;
	}
    for (int i = idx; i < dsMH.soLuong - 1; i++)
        dsMH.data[i] = dsMH.data[i+1];
    
	dsMH.soLuong--;
    
	return SUCCESS;
}
//Tim kiem mon hoc
int timViTriMonHoc(DanhSachMonHoc &dsMH, const char* ma) {
    for (int i = 0; i < dsMH.soLuong; i++)
        if (strcmp(dsMH.data[i].maMH, ma) == 0) return i;
    return -1;
}

bool kiemTraTrungMaMonHoc(DanhSachMonHoc &dsMH, const char* ma) {
    return timViTriMonHoc(dsMH, ma) != -1;
}
bool kiemTraMonHocDaCoCauHoi(PTR_CAUHOI root, const char* maMH) {
	if (root == NULL ) return false;
	
	if (strcmp(root -> maMH, maMH) == 0) return true;
	
	return kiemTraMonHocDaCoCauHoi(root->left, maMH) ||
	       kiemTraMonHocDaCoCauHoi(root->right, maMH);
}
#endif