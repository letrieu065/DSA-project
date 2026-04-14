#ifndef HAMCAUHOI_H
#define HAMCAUHOI_H

#include "khaibao.h"
#include "dohoa.h"
#include "hamtienich.h"
#include "ErrorCodes.h"

// ============================================================
// BIEN TOAN CUC - ID tu tang
// ============================================================
int nextCauHoiId = 1;

// ============================================================
// ThemCauHoi - Insert vao BST, khoa la id tu tang
// ============================================================
PTR_CAUHOI ThemCauHoi(PTR_CAUHOI root, CauHoi* node) {
    if (root == NULL) return node;
    if (node->id < root->id)
        root->left = ThemCauHoi(root->left, node);
    else
        root->right = ThemCauHoi(root->right, node);
    return root;
}

// ============================================================
// TimCauHoi - Tim node theo ID (ho tro HieuChinhCauHoi)
// ============================================================
CauHoi* TimCauHoi(PTR_CAUHOI root, int id) {
    if (root == NULL) return NULL;
    if (id == root->id) return root;
    if (id < root->id)  return TimCauHoi(root->left, id);
    return TimCauHoi(root->right, id);
}

// ============================================================
// HieuChinhCauHoi - Tim theo ID roi sua noi dung + dap an
// ============================================================
bool HieuChinhCauHoi(PTR_CAUHOI root, int id,
                     const char* noiDung,
                     const char* a, const char* b,
                     const char* c, const char* d,
                     char dapAn) {
    CauHoi* node = TimCauHoi(root, id);
    if (node == NULL) return false;

    strcpy(node->noiDung, noiDung);
    strcpy(node->a, a);
    strcpy(node->b, b);
    strcpy(node->c, c);
    strcpy(node->d, d);
    node->dapAn = dapAn;
    return true;
}

// ============================================================
// LayNhoNhat - Ham phu cho XoaCauHoi (TH 2 cay con)
// ============================================================
CauHoi* LayNhoNhat(CauHoi* node) {
    while (node->left != NULL)
        node = node->left;
    return node;
}

// ============================================================
// XoaCauHoi - Xu ly du 3 truong hop xoa node BST
// ============================================================
PTR_CAUHOI XoaCauHoi(PTR_CAUHOI root, int id) {
    if (root == NULL) return NULL;

    if (id < root->id) {
        root->left = XoaCauHoi(root->left, id);
    } else if (id > root->id) {
        root->right = XoaCauHoi(root->right, id);
    } else {
        // Truong hop 1: Node la
        if (root->left == NULL && root->right == NULL) {
            delete root;
            return NULL;
        }
        // Truong hop 2: Chi co 1 cay con
        if (root->left == NULL) {
            CauHoi* temp = root->right;
            delete root;
            return temp;
        }
        if (root->right == NULL) {
            CauHoi* temp = root->left;
            delete root;
            return temp;
        }
        // Truong hop 3: Co 2 cay con
        // Lay node trai nhat cua cay con phai de the mang
        CauHoi* minNode = LayNhoNhat(root->right);
        root->id = minNode->id;
        strcpy(root->maMH,    minNode->maMH);
        strcpy(root->noiDung, minNode->noiDung);
        strcpy(root->a, minNode->a);
        strcpy(root->b, minNode->b);
        strcpy(root->c, minNode->c);
        strcpy(root->d, minNode->d);
        root->dapAn = minNode->dapAn;
        root->right = XoaCauHoi(root->right, minNode->id);
    }
    return root;
}

// ============================================================
// InOrder - Duyet in danh sach theo thu tu ID tang dan
// ============================================================
void InOrderCauHoi(PTR_CAUHOI root, int &soThuTu, int x, int y) {
    if (root == NULL) return;
    InOrderCauHoi(root->left, soThuTu, x, y);

    gotoxy(x, y + soThuTu * 3);
    SetColor(0, 11);
    cout << "[ID:" << root->id << "] [" << root->maMH << "] " << root->noiDung;
    SetColor(0, 7);
    gotoxy(x + 2, y + soThuTu * 3 + 1);
    cout << "A." << root->a << "  B." << root->b
         << "  C." << root->c << "  D." << root->d
         << "  => Dap an: " << root->dapAn;

    soThuTu++;
    InOrderCauHoi(root->right, soThuTu, x, y);
}

// ============================================================
// flowQuanLyCauHoi - Giao dien quan ly cau hoi (rap vao flowGiaoVien)
// ============================================================
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

#endif