;;
;; Upcaste Performance Libraries
;; Copyright (C) 2012-2013 Jesse W. Towner
;;
;; Permission is hereby granted, free of charge, to any person obtaining
;; a copy of this software and associated documentation files (the
;; "Software"), to deal in the Software without restriction, including
;; without limitation the rights to use, copy, modify, merge, publish,
;; distribute, sublicense, and/or sell copies of the Software, and to
;; permit persons to whom the Software is furnished to do so, subject to
;; the following conditions:
;;
;; The above copyright notice and this permission notice shall be
;; included in all copies or substantial portions of the Software.
;;
;; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
;; EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
;; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
;; IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
;; CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
;; TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
;; SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;;

FE_INVALID              EQU     00001h
FE_DENORMAL             EQU     00002h
FE_DIVBYZERO            EQU     00004h
FE_OVERFLOW             EQU     00008h
FE_UNDERFLOW            EQU     00010h
FE_INEXACT              EQU     00020h
FE_ALL_EXCEPT           EQU     0003Fh

FE_TONEAREST            EQU     00000h
FE_DOWNWARD             EQU     00400h
FE_UPWARD               EQU     00800h
FE_TOWARDZERO           EQU     00C00h

FE_CTRL_MASK_INIT       EQU     0003Fh
FE_CTRL_NOMASK_INIT     EQU     0F3C0h
FE_CTRL_USER_INIT       EQU     00C3Fh
FE_CTRL_NOEXCEPT        EQU     0FFC0h
FE_CTRL_NOROUND         EQU     0F3FFh
FE_STAT_UNMASK          EQU     0FFC0h

MXSCR_ALL_EXCEPT        EQU     000001F80h
MXSCR_NOMASK_INIT       EQU     0FFFF8040h
MXSCR_NOROUND_UNMASK    EQU     0FFFF9FC0h
MXSCR_NOROUND           EQU     0FFFF9FFFh
MXSCR_STAT_UNMASK       EQU     0FFFFFFC0h
MXSCR_EXCEPT_SHIFT      EQU     07h
MXSCR_ROUND_SHIFT       EQU     03h

fenv                STRUC
    control_word    WORD    ?
    unused0         WORD    ?
    status_word     WORD    ?
    unused1         WORD    ?
    tag_word        WORD    ?
    unused2         WORD    ?
    ip_offset       DWORD   ?
    ip_selector     WORD    ?
    opcode          WORD    ?
    data_offset     DWORD   ?
    data_selector   WORD    ?
    unused3         WORD    ?
    mxcsr           DWORD   ?
fenv                ENDS

IFDEF X64
FE_DFL_ENV          EQU     0FFFFFFFFFFFFFFFFh
FE_NOMASK_ENV       EQU     0FFFFFFFFFFFFFFFEh
ELSE
FE_DFL_ENV          EQU     0FFFFFFFFh
FE_NOMASK_ENV       EQU     0FFFFFFFEh
ENDIF
