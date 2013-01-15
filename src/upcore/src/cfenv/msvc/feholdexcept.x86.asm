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

;;
;; int feholdexcept(fenv_t* envp);
;;
;; Saves the current floating-point environment, clears the exception
;; status bits, and sets all exceptions to non-stop.
;;

.CODE
ALIGN 4
PUBLIC _feholdexcept
_feholdexcept PROC

    ; check input arguments
    mov         ecx, DWORD PTR [esp + 4]
    test        ecx, ecx
    jz          _invalid_envp

    ; set up to copy environment to user envp
    push        edi
    push        esi
    lea         esi, [esp - SIZEOF fenv]
    mov         edi, ecx

    ; fetch current environment
    fnstenv     [esi]
    stmxcsr     [esi].fenv.mxcsr

    ; copy it to caller's envp object
    mov         ecx, 8
    rep         movsd

    ; clear all exception status bits and set exception mask to non-stop mode
    mov         eax, [esp - SIZEOF fenv].fenv.mxcsr
    or          [esp - SIZEOF fenv].fenv.control_word, FE_ALL_EXCEPT
    or          eax, MXSCR_ALL_EXCEPT
    and         [esp - SIZEOF fenv].fenv.status_word, FE_STAT_UNMASK
    and         eax, MXSCR_STAT_UNMASK

    ; apply the updated environment
    fldenv      [esp - SIZEOF fenv]
    mov         [esp - SIZEOF fenv].fenv.mxcsr, eax
    ldmxcsr     [esp - SIZEOF fenv].fenv.mxcsr

    ; cleanup
    xor         eax, eax
    push        esi
    pop         edi
    ret

_invalid_envp:
    mov         eax, -1
    ret

_feholdexcept ENDP

END
