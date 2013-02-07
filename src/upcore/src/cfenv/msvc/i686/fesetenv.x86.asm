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
;; int fesetenv(fenv_t const* envp);
;;
;; Sets the floating-point environment to the one supplied by the
;; caller, handling special environments FE_DFL_ENV and FE_NOMASK_ENV.
;; This function preserves the x87 FPU precision mode for all given
;; environments. The default and no-mask environments also preserve
;; the SSE flush-to-zero and denormals-as-zeros flags, but a user
;; environment is allowed to override them. This should ensure
;; maximum compatability with the MSVC CRT and user-code.
;;

.CODE
ALIGN 4
PUBLIC _fesetenv
_fesetenv PROC

    ; check input argument
    mov         ecx, DWORD PTR [esp + 4]
    test        ecx, ecx
    jz          _invalid_envp

    ; save registers
    push        ebp
    push        ebx
    push        edi
    push        esi

    ; fetch current floating point environment
    fnstenv     [esp - SIZEOF fenv]
    stmxcsr     [esp - SIZEOF fenv].fenv.mxcsr

    ; check for special environments
    xor         eax, eax
    cmp         ecx, FE_NOMASK_ENV
    jb          _user_environment

    ; clear out bits we don't want to set
    mov         [esp - SIZEOF fenv].fenv.ip_offset, eax
    mov         [esp - SIZEOF fenv].fenv.ip_selector, ax
    mov         [esp - SIZEOF fenv].fenv.opcode, ax
    mov         [esp - SIZEOF fenv].fenv.data_offset, eax
    mov         [esp - SIZEOF fenv].fenv.data_selector, ax

    ; apply default environment if specified by caller
    je          _nomask_environment
    mov         cx, [esp - SIZEOF fenv].fenv.control_word
    mov         edx, [esp - SIZEOF fenv].fenv.mxcsr
    or          cx, FE_CTRL_MASK_INIT                                       ; exceptions are disabled
    and         edx, MXSCR_NOROUND_UNMASK                                   ; round-to-nearest & exception status cleared
    and         cx, FE_CTRL_NOROUND                                         ; round-to-nearest
    or          edx, MXSCR_ALL_EXCEPT                                       ; exceptions are disabled
    mov         [esp - SIZEOF fenv].fenv.control_word, cx
    and         [esp - SIZEOF fenv].fenv.status_word, FE_STAT_UNMASK        ; exception status cleared
    mov         [esp - SIZEOF fenv].fenv.mxcsr, edx        
    jmp         _done

    ; apply no-mask environment if specified by caller
_nomask_environment:
    and         [esp - SIZEOF fenv].fenv.control_word, FE_CTRL_NOMASK_INIT  ; exceptions enabled & round-to-nearest
    and         [esp - SIZEOF fenv].fenv.status_word, FE_STAT_UNMASK        ; exception status cleared
    and         [esp - SIZEOF fenv].fenv.mxcsr, MXSCR_NOMASK_INIT           ; round-to-nearest, exceptions enabled, exception status cleared
    jmp         _done

    ; set user supplied environment
_user_environment:
    mov         bp, [ecx].fenv.control_word
    mov         bx, [ecx].fenv.status_word
    mov         di, [esp - SIZEOF fenv].fenv.control_word
    mov         si, [esp - SIZEOF fenv].fenv.status_word
    and         bp, FE_CTRL_USER_INIT                                      ; envp->control_word & (FE_ALL_EXCEPT | FE_TOWARDZERO)
    and         bx, FE_ALL_EXCEPT                                          ; envp->status_word & FE_ALL_EXCEPT
    and         di, FE_CTRL_NOMASK_INIT                                    ; control_word &= ~(FE_ALL_EXCEPT | FE_TOWARDZERO)
    and         si, FE_STAT_UNMASK                                         ; status_word &= ~FE_ALL_EXCEPT
    or          di, bp
    or          si, bx
    mov         [esp - SIZEOF fenv].fenv.control_word, di
    mov         [esp - SIZEOF fenv].fenv.status_word, si
    mov         ebx, [ecx].fenv.ip_offset
    mov         bp, [ecx].fenv.ip_selector
    mov         dx, [ecx].fenv.opcode
    mov         edi, [ecx].fenv.data_offset
    mov         si, [ecx].fenv.data_selector
    mov         [esp - SIZEOF fenv].fenv.ip_offset, ebx
    mov         [esp - SIZEOF fenv].fenv.ip_selector, bp
    mov         [esp - SIZEOF fenv].fenv.opcode, dx
    mov         ebx, [ecx].fenv.mxcsr
    mov         [esp - SIZEOF fenv].fenv.data_offset, edi
    mov         [esp - SIZEOF fenv].fenv.data_selector, si
    mov         [esp - SIZEOF fenv].fenv.mxcsr, ebx

    ; finish by setting the x87 FPU environment and SSE control-status register
_done:
    fldenv      [esp - SIZEOF fenv]
    ldmxcsr     [esp - SIZEOF fenv].fenv.mxcsr
    pop         esi
    pop         edi
    pop         ebx
    pop         ebp
    ret

    ; return error code
_invalid_envp:
    mov         eax, -1
    ret

_fesetenv ENDP

END
