#ifndef ERROR_CODES_H
#define ERROR_CODES_H

const int SUCCESS = 0;

// Nhom loi 100: Loi lop
const int ERR_LOP_FULL       = 101;
const int ERR_LOP_DUPLICATE  = 102;
const int ERR_LOP_NOT_FOUND  = -1;
const int ERR_LOP_EMPTY = 104;
const int ERR_LOP_HAS_SV = 105;

// Nhom loi 200: loi sinh vien
const int ERR_SV_DUPLICATE   = 201;
const int ERR_SV_NOT_FOUND   = 202;
const int ERR_SV_FULL = 203;

// Nhom 300: loi dang nhap - he thong
const int ERR_LOGIN_USER     = 301; 
const int ERR_LOGIN_PASS     = 302; 
const int ERR_LOGIN_EMPTY    = 303; 

#endif