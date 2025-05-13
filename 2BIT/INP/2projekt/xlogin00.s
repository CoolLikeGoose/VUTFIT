; Autor reseni: Nikita Smirnov xsmirn02
; Pocet cyklu k serazeni puvodniho retezce:         3792
; Pocet cyklu razeni sestupne serazeneho retezce:   4733
; Pocet cyklu razeni vzestupne serazeneho retezce:  288
; Pocet cyklu razeni retezce s vasim loginem:       926
; Implementovany radici algoritmus:                 Bubble sort
; ------------------------------------------------

; DATA SEGMENT
                .data
;login:          .asciiz "vitejte-v-inp-2023"    ; puvodni uvitaci retezec
;login:          .asciiz "vvttpnjiiee3220---"  ; sestupne serazeny retezec
;login:          .asciiz "---0223eeiijnpttvv"  ; vzestupne serazeny retezec
login:          .asciiz "xsmirn02"            ; SEM DOPLNTE VLASTNI LOGIN
                                                ; A POUZE S TIMTO ODEVZDEJTE

params_sys5:    .space  8   ; misto pro ulozeni adresy pocatku
                            ; retezce pro vypis pomoci syscall 5
                            ; (viz nize - "funkce" print_string)

new_line:       .asciiz "\n"

; CODE SEGMENT
                .text
 main:
     ; SEM DOPLNTE VASE RESENI
    daddi   $t0, r0, login
    daddi   $t1, r0, 0      ; s0 - array size
loop_size:
    lb      $t2, 0($t0)
    beqz    $t2, end_loop_size
    addi    $t1, $t1, 1
    addi    $t0, $t0, 1
    j       loop_size
    
end_loop_size:

    daddi   $a0, r0, login
    dadd    $a1, r0, $t1

    jal quick_sort

    daddi   r4, r0, login   ; vozrovy vypis: adresa login: do r4
    jal     print_string    ; vypis pomoci print_string - viz nize

    syscall 0   ; halt

QuickSort:
    dadd
    bgez $a1, QS_done

    addi $sp, $sp, -8
    sw $a0, 0($sp)
    sw $a1, 4($sp)

    srl $t3, $a1, 1
    add $t3, $a0, $t3
    lw $t4, 0($t3)

    move $t0, $a0  
    move $t1, $a1  
    subi $t1, $t1, 1

QS_partition:
    lw $t5, 0($t0)
    bge $t5, $t4, QS_right
    addi $t0, $t0, 1
    j QS_partition

QS_right:
    lw $t6, 0($t1)
    ble $t6, $t4, QS_swap
    subi $t1, $t1, 1
    j QS_right

QS_swap:
    sw $t6, 0($t0)
    sw $t5, 0($t1)
    addi $t0, $t0, 1
    subi $t1, $t1, 1
    j QS_partition

QS_done:
    lw $a0, 0($sp)
    lw $a1, 4($sp)
    addi $sp, $sp, 8

    jal QuickSort

    la $a0, 0($sp)
    jr $ra

print_string:   ; adresa retezce se ocekava v r4
                sw      r4, params_sys5(r0)
                daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
                syscall 5   ; systemova procedura - vypis retezce na terminal
                jr      r31 ; return - r31 je urcen na return address
