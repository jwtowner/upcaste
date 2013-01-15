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

.686P
.XMM
.MODEL FLAT
INCLUDE fenv.inc
EXTRN _fesetenv:PROC
EXTRN _feraiseexcept:PROC

;;
;; int feupdateenv(fenv_t const* envp);
;;
;; Saves the current exception status, applies the given environment, and
;; then raises the previously saved exceptions.
;;

.CODE
ALIGN 4
PUBLIC _feupdateenv
_feupdateenv PROC

    mov         ecx, DWORD PTR [esp + 4]
    test        ecx, ecx
    jz          _invalid_envp

    xor         eax, eax
    fnstsw      ax
    stmxcsr     DWORD PTR [esp - 4]
    or          eax, DWORD PTR [esp - 4]
    and         eax, FE_ALL_EXCEPT
    push        eax
    push        ecx
    call        _fesetenv
    add         esp, 4h
    call        _feraiseexcept
    add         esp, 4h
    ret

_invalid_envp:
    mov         eax, -1
    ret

_feupdateenv ENDP

END
