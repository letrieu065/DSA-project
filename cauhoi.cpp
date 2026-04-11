#include <iostream>
#include <string>
using namespace std;

// ============================================================
// CẤU TRÚC NODE
// ============================================================
struct CauHoi {
    int id;
    string noiDung;
    string A, B, C, D;
    char dapAn;
    CauHoi* trai;
    CauHoi* phai;
};

int nextId = 1;

CauHoi* taoNode(string nd, string a, string b, string c, string d, char dap) {
    CauHoi* node = new CauHoi();
    node->id      = nextId++;
    node->noiDung = nd;
    node->A = a; node->B = b; node->C = c; node->D = d;
    node->dapAn   = dap;
    node->trai    = nullptr;
    node->phai    = nullptr;
    return node;
}

// ============================================================
// ThemCauHoi — Insert vào BST, khóa là id tự tăng
// ============================================================
CauHoi* ThemCauHoi(CauHoi* root, CauHoi* node) {
    if (root == nullptr) return node;
    if (node->id < root->id)
        root->trai = ThemCauHoi(root->trai, node);
    else
        root->phai = ThemCauHoi(root->phai, node);
    return root;
}

// ============================================================
// TimNode — Tìm node theo ID (hỗ trợ HieuChinhCauHoi)
// ============================================================
CauHoi* TimNode(CauHoi* root, int id) {
    if (root == nullptr) return nullptr;
    if (id == root->id)  return root;
    if (id < root->id)   return TimNode(root->trai, id);
    return TimNode(root->phai, id);
}

// ============================================================
// HieuChinhCauHoi — Tìm theo ID rồi sửa nội dung + đáp án
// ============================================================
bool HieuChinhCauHoi(CauHoi* root, int id,
                     string nd, string a, string b,
                     string c, string d, char dap) {
    CauHoi* node = TimNode(root, id);
    if (node == nullptr) return false;
    node->noiDung = nd;
    node->A = a; node->B = b;
    node->C = c; node->D = d;
    node->dapAn = dap;
    return true;
}

// ============================================================
// XoaCauHoi — Xử lý 3 trường hợp xóa node BST
// ============================================================
CauHoi* layNhoNhat(CauHoi* node) {
    while (node->trai != nullptr)
        node = node->trai;
    return node;
}

CauHoi* XoaCauHoi(CauHoi* root, int id) {
    if (root == nullptr) return nullptr;

    if (id < root->id) {
        root->trai = XoaCauHoi(root->trai, id);
    } else if (id > root->id) {
        root->phai = XoaCauHoi(root->phai, id);
    } else {
        // Trường hợp 1: Node lá
        if (root->trai == nullptr && root->phai == nullptr) {
            delete root;
            return nullptr;
        }
        // Trường hợp 2: Node có 1 cây con
        if (root->trai == nullptr) {
            CauHoi* temp = root->phai;
            delete root;
            return temp;
        }
        if (root->phai == nullptr) {
            CauHoi* temp = root->trai;
            delete root;
            return temp;
        }
        // Trường hợp 3: Node có 2 cây con
        // Lấy node trái nhất của cây con phải để thế mạng
        CauHoi* min = layNhoNhat(root->phai);
        root->id      = min->id;
        root->noiDung = min->noiDung;
        root->A = min->A; root->B = min->B;
        root->C = min->C; root->D = min->D;
        root->dapAn   = min->dapAn;
        root->phai = XoaCauHoi(root->phai, min->id);
    }
    return root;
}

// ============================================================
// InOrder — Duyệt in danh sách theo thứ tự ID tăng dần
// ============================================================
void InOrder(CauHoi* root) {
    if (root == nullptr) return;
    InOrder(root->trai);
    cout << "ID: " << root->id << endl;
    cout << "  Cau hoi : " << root->noiDung << endl;
    cout << "  A. " << root->A << "  B. " << root->B << endl;
    cout << "  C. " << root->C << "  D. " << root->D << endl;
    cout << "  Dap an  : " << root->dapAn << endl;
    cout << "----------------------------" << endl;
    InOrder(root->phai);
}

// ============================================================
// GIAO DIEN — flowQuanLyCauHoi
// ============================================================
void nhapCauHoi(CauHoi*& root) {
    string nd, a, b, c, d;
    char dap;
    cin.ignore();
    cout << "Noi dung: "; getline(cin, nd);
    cout << "A: ";        getline(cin, a);
    cout << "B: ";        getline(cin, b);
    cout << "C: ";        getline(cin, c);
    cout << "D: ";        getline(cin, d);
    cout << "Dap an dung (A/B/C/D): "; cin >> dap;
    dap = toupper(dap);
    CauHoi* node = taoNode(nd, a, b, c, d, dap);
    root = ThemCauHoi(root, node);
    cout << ">> Da them cau hoi voi ID = " << node->id << endl;
}

void suaCauHoi(CauHoi* root) {
    int id;
    cout << "Nhap ID can sua: "; cin >> id;
    string nd, a, b, c, d;
    char dap;
    cin.ignore();
    cout << "Noi dung moi: "; getline(cin, nd);
    cout << "A: ";             getline(cin, a);
    cout << "B: ";             getline(cin, b);
    cout << "C: ";             getline(cin, c);
    cout << "D: ";             getline(cin, d);
    cout << "Dap an dung (A/B/C/D): "; cin >> dap;
    dap = toupper(dap);
    if (HieuChinhCauHoi(root, id, nd, a, b, c, d, dap))
        cout << ">> Da cap nhat cau hoi ID = " << id << endl;
    else
        cout << ">> Khong tim thay ID = " << id << endl;
}

void xoaCauHoiMenu(CauHoi*& root) {
    int id;
    cout << "Nhap ID can xoa: "; cin >> id;
    if (TimNode(root, id) == nullptr) {
        cout << ">> Khong tim thay ID = " << id << endl;
        return;
    }
    root = XoaCauHoi(root, id);
    cout << ">> Da xoa cau hoi ID = " << id << endl;
}

void flowQuanLyCauHoi() {
    CauHoi* root = nullptr;
    int chon;
    do {
        cout << "\n===== QUAN LY CAU HOI (BST) =====" << endl;
        cout << "1. Them cau hoi" << endl;
        cout << "2. Hieu chinh cau hoi" << endl;
        cout << "3. Xoa cau hoi" << endl;
        cout << "4. Hien thi danh sach (In-order)" << endl;
        cout << "0. Thoat" << endl;
        cout << "Chon: "; cin >> chon;
        switch (chon) {
            case 1: nhapCauHoi(root);    break;
            case 2: suaCauHoi(root);     break;
            case 3: xoaCauHoiMenu(root); break;
            case 4:
                cout << "\n--- DANH SACH CAU HOI (tang dan theo ID) ---" << endl;
                if (root == nullptr) cout << "Chua co cau hoi nao." << endl;
                else InOrder(root);
                break;
            case 0: cout << "Thoat." << endl; break;
            default: cout << "Lua chon khong hop le." << endl;
        }
    } while (chon != 0);
}

int main() {
    flowQuanLyCauHoi();
    return 0;
}