                section         .text

                global          _start


print_number:
                push            rax
; Start printing number
; Malloc to R8
                lea             r8, [buf_num+18]
                mov             rbx, 10      
iter:
; take mod 10, store in rdx
                xor             rdx, rdx
                div             rbx
                ; get the symbol here
                add             rdx, 48
                ; store in reverse order
                mov             [r8], dl
                dec             r8
                ; check condition
                cmp             rax, 0
                jne iter
                ; exit
                mov             rdx, 19
                mov             rax, 1
                mov             rdi, 1
                mov             rsi, r8
                syscall
                pop             rax
                ret



_start:
; File opening
                pop             rax
                cmp             rax, 2
                jne             missing_args
                pop             rax
                pop             rdi
                mov             rax, 2
                xor             rsi, rsi
                xor             rdx, rdx
                syscall
                cmp             rax, 0
                jl              open_failed
; Initialize counter in R9 - the number of words
                xor             r9, r9
                mov             rdi, rax
                xor             r8, r8
; read one char from stdin
; result stored in rdx
; if error occured - throws ud2
.iterread:
                xor             rax, rax
                mov             rsi, buf
                mov             rdx, 1
                syscall

                cmp             rax, 0
                jl              read_fail
                je              exit
                mov             rdx, [buf]

; reading completed, now checking for the end of word
; r8 - boolean value, 1 if currently in word, otherwise 0, initial state - not in word
                call            check_whitespace
                cmp             rsi, 0
                jne             .next_char

.handle_whitespace:
                cmp             r8, 1
                jne             .next_char
                inc             r9

.next_char:
                mov             r8, rsi
                jmp             .iterread



read_fail:      ud2
missing_args:   ud2
open_failed:    ud2

exit:
; Check the last symbol, print number and exit.
                cmp             r8, 1
                jne             exit2
                inc             r9
exit2:          
                mov             rax, 3
                xor             rdi, rdi
                syscall
                mov             rax, r9
                call            print_number
                mov             rax, 60
                xor             rdi, rdi
                syscall


; checks the symbol for codes 9, 10, 11, 12, 13, 32
; argument - rdx
; return value - rsi
; rsi = 1 if not whitespace, otherwise rsi = 0
check_whitespace:
                push            rdx
                xor             rsi, rsi
                cmp             rdx, 9
                je              .exit
                cmp             rdx, 10
                je              .exit
                cmp             rdx, 11
                je              .exit
                cmp             rdx, 12
                je              .exit
                cmp             rdx, 13
                je              .exit
                cmp             rdx, 32
                je              .exit

                inc             rsi

.exit:
                pop             rdx
                ret

                section         .bss
buf:            resb 1
buf_num:        resb 19