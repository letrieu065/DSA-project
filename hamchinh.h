#ifndef HAMCHINH_H
#define HAMCHINH_H

#include "ErrorCodes.h"
#include "khaibao.h"
#include "hamve.h"

// Ham lop
int KiemTraTrungMaLop(DanhSachLop &dsLop, char* maNhap);
int KiemTraTrungTenLop(DanhSachLop &dsLop, char* tenNhap);
int themLop(DanhSachLop &dsLop, char* ma, char* ten);
int timViTriLop(DanhSachLop &dsLop, const char* maLop_nhap);
int suaLop(DanhSachLop &dsLop, char* ma, char* tenMoi);
int xoaLop(DanhSachLop &dsLop, char* ma);

// Ham sinh vien
SinhVien* timKiemSinhVien(Lop* lop, const char* maSV);
SinhVien* timKiemSinhVien(DanhSachLop &dsLop, const char* maSV);
int kiemTraTrungMaSinhVien(DanhSachLop &dsLop, const char* maSV);
int themSinhVien(Lop* lop, SinhVien sv);
int suaSinhVien(Lop* lop, const char* maSV, SinhVien svMoi);
int xoaSinhVien(Lop* lop, const char* maSV);
int demSoSV(Lop* lop);

// Ham mon hoc
int themMonHoc(DanhSachMonHoc &dsMH, MonHoc* mh);
int suaMonHoc(DanhSachMonHoc &dsMH, const char* ma, const char* tenMoi, int tinChiMoi);
int xoaMonHoc(DanhSachMonHoc &dsMH, const char* ma, PTR_CAUHOI root);
int timViTriMonHoc(DanhSachMonHoc &dsMH, const char* ma);
bool kiemTraTrungMaMonHoc(DanhSachMonHoc &dsMH, const char* ma);
bool kiemTraMonHocDaCoCauHoi(PTR_CAUHOI root, const char* maMH);

// Ham cau hoi
int& getNextSTT(const char* maMH);
void TaoIdCauHoi(char* bufOut, const char* maMH, int soThuTu);
PTR_CAUHOI ThemCauHoi(PTR_CAUHOI root, CauHoi* node);
CauHoi* TimCauHoi(PTR_CAUHOI root, const char* id);
CauHoi* LayNhoNhat(CauHoi* node);
PTR_CAUHOI XoaCauHoi(PTR_CAUHOI root, const char* id);
void CapNhatSTT(PTR_CAUHOI root);
void InOrderCauHoi(PTR_CAUHOI root, int &y, int x);

DiemThi* saoChepDanhSachDiem(DiemThi* src); 

using namespace std;

int KiemTraTrungMaLop(DanhSachLop &dsLop, char* maNhap) {
    for (int i = 0; i < dsLop.soLuong; i++) {
        if (strcmp(dsLop.nodes[i]->maLop, maNhap) == 0) {
            return i; 
        }
    }
    return -1; 
}

int KiemTraTrungTenLop(DanhSachLop &dsLop, char* tenNhap) {
    for (int i = 0; i < dsLop.soLuong; i++) {
        if (strcmp(dsLop.nodes[i]->tenLop, tenNhap) == 0) {
            return i; 
        }
    }
    return -1; 
}

int themLop(DanhSachLop &dsLop, char* ma, char* ten) {
	if (dsLop.soLuong >= MAX_LOP) return ERR_LOP_FULL;
	
	if (KiemTraTrungMaLop(dsLop, ma) != -1) return ERR_LOP_DUPLICATE;
	
	if (KiemTraTrungTenLop(dsLop, ten) != -1) return ERR_LOP_DUPLICATE_NAME;
	
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

int suaLop(DanhSachLop &dsLop, char* ma, char* tenMoi) {
    int index = timViTriLop(dsLop, ma);
    
    if (index == ERR_LOP_NOT_FOUND) {
        return ERR_LOP_NOT_FOUND;
    }
    
    strcpy(dsLop.nodes[index]->tenLop, tenMoi);
    
    return SUCCESS;
}

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

int kiemTraTrungMaSinhVien(DanhSachLop &dsLop, const char* maSV) {
	if (timKiemSinhVien(dsLop, maSV) !=NULL) {
		return ERR_SV_DUPLICATE;
	}
	return SUCCESS;
}

int themSinhVien(Lop* lop, SinhVien sv) {
	if (lop == NULL) return ERR_LOP_NOT_FOUND;
	SinhVien* newNode = new SinhVien;
	
	*newNode = sv;
	
    newNode->headDiem = saoChepDanhSachDiem(sv.headDiem);
	newNode->next = lop->headSV;
	lop->headSV = newNode;
	
	return SUCCESS;
}

int suaSinhVien(Lop* lop, const char* maSV, SinhVien svMoi) {
    if (lop == NULL) return ERR_LOP_NOT_FOUND;
    
    SinhVien* p = lop->headSV;
    while (p != NULL && strcmp(p->maSV, maSV) != 0) {
        p = p->next;
    }
    
    if (p == NULL) return ERR_SV_NOT_FOUND;

    if (strlen(svMoi.ho) > 0) {
        strcpy(p->ho, svMoi.ho);
    }
    
    if (strlen(svMoi.ten) > 0) {
        strcpy(p->ten, svMoi.ten);
    }
    
    p->phai = svMoi.phai;
    
    return SUCCESS;
}

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

int& getNextSTT(const char* maMH) {
    struct DemMon { char ma[20]; int dem; };
    static DemMon ds[100];
    static int soLuong = 0;

    for (int i = 0; i < soLuong; i++)
        if (strcmp(ds[i].ma, maMH) == 0)
            return ds[i].dem;

    strcpy(ds[soLuong].ma, maMH);
    ds[soLuong].dem = 1;
    return ds[soLuong++].dem;
}

void TaoIdCauHoi(char* bufOut, const char* maMH, int soThuTu) {
    snprintf(bufOut, 25, "%s_%04d", maMH, soThuTu);
}

void InOrderCauHoi(PTR_CAUHOI root, int &y, int x) {
    if (root == NULL) return;

    InOrderCauHoi(root->left, y, x);

    gotoxy(x, y);
    SetColor(0, 11);
    cout << "[" << root->id << "] ";
    SetColor(0, 7);

    int usedLines = InChuoiGioiHan(root->noiDung, x + 16, y, 65);

    y += usedLines; 

    gotoxy(x + 2, y++);
    cout << "A. " << root->a;

    gotoxy(x + 2, y++);
    cout << "B. " << root->b;

    gotoxy(x + 2, y++);
    cout << "C. " << root->c;

    gotoxy(x + 2, y++);
    cout << "D. " << root->d;

    // 👉 Đáp án đúng
    SetColor(0, 10);
    gotoxy(x + 2, y++);
    cout << "=> Dap an: " << root->dapAn;
    SetColor(0, 7);

    y += 1; // cách dòng giữa các câu

    InOrderCauHoi(root->right, y, x);
}

void CapNhatSTT(PTR_CAUHOI root) {
    if (root == NULL) return;
    
    // Tìm phần số trong ID (ví dụ "IT_0015" lấy ra 15)
    string idStr = root->id;
    size_t pos = idStr.find_last_of('_');
    if (pos != string::npos) {
        int soHienTai = atoi(idStr.substr(pos + 1).c_str());
        if (soHienTai >= getNextSTT(root->maMH)) {
            getNextSTT(root->maMH) = soHienTai + 1; // Số tiếp theo sẽ lớn hơn số lớn nhất hiện tại
        }
    }
    CapNhatSTT(root->left);
    CapNhatSTT(root->right);
}

PTR_CAUHOI ThemCauHoi(PTR_CAUHOI root, CauHoi* node) {
    if (root == NULL) return node;
    if (strcmp(node->id, root->id) < 0)
        root->left = ThemCauHoi(root->left, node);
    else
        root->right = ThemCauHoi(root->right, node);
    return root;
}

CauHoi* TimCauHoi(PTR_CAUHOI root, const char* id) {
    if (root == NULL) return NULL;
    int cmp = strcmp(id, root->id);
    if (cmp == 0) return root;
    if (cmp < 0)  return TimCauHoi(root->left, id);
    return TimCauHoi(root->right, id);
}

CauHoi* LayNhoNhat(CauHoi* node) {
    while (node->left != NULL)
        node = node->left;
    return node;
}

PTR_CAUHOI XoaCauHoi(PTR_CAUHOI root, const char* id) {
    if (root == NULL) return NULL;
    int cmp = strcmp(id, root->id);
    if (cmp < 0) {
        root->left = XoaCauHoi(root->left, id);
    } 
    else if (cmp > 0) {
        root->right = XoaCauHoi(root->right, id);
    } 
    else {
        // Trường hợp 1: Node lá (không có con nào)
        if (root->left == NULL && root->right == NULL) {
            delete root;
            return NULL;
        }
        
        // Trường hợp 2: Node chỉ có con phải
        if (root->left == NULL) {
            CauHoi* temp = root->right;
            delete root;
            return temp;
        }
        
        // Trường hợp 3: Node chỉ có con trái
        if (root->right == NULL) {
            CauHoi* temp = root->left;
            delete root;
            return temp;
        }
 
        // Trường hợp 4: Node có 2 con
        // Tìm node thế mạng (nhỏ nhất bên phải)
        CauHoi* minNode = LayNhoNhat(root->right);
 
        // Copy toàn bộ dữ liệu từ minNode sang root
        strcpy(root->id, minNode->id);
        strcpy(root->maMH, minNode->maMH);
        strcpy(root->noiDung, minNode->noiDung);
        strcpy(root->a, minNode->a);
        strcpy(root->b, minNode->b);
        strcpy(root->c, minNode->c);
        strcpy(root->d, minNode->d);
        root->dapAn = minNode->dapAn;
 
        // Xóa node thế mạng ở cây con bên phải
        root->right = XoaCauHoi(root->right, minNode->id);
    }
    return root;
}

DiemThi* saoChepDanhSachDiem(DiemThi* src) {
    if (src == NULL) return NULL;

    DiemThi* head = NULL;
    DiemThi* tail = NULL;

    while (src != NULL) {
        DiemThi* newDiem = new DiemThi;
        strcpy(newDiem->maMH, src->maMH);
        newDiem->diem   = src->diem;
        newDiem->lanThi = src->lanThi;
        newDiem->next   = NULL;

        if (head == NULL) head = newDiem;  
        else              tail->next = newDiem;
        tail = newDiem;

        src = src->next;
    }
    return head;
}

#endif