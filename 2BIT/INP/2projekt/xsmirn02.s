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
    daddi   $a0, r0, login
    jal bubble_sort

    daddi   r4, r0, login   ; vozrovy vypis: adresa login: do r4
    jal     print_string    ; vypis pomoci print_string - viz nize

    syscall 0   ; halt

bubble_sort:
    daddi   $t0, r0, 0          ;bool swapped 
    dadd    $t3, r0, $a0        ;shift              t3

for_loop:
    lb      $t1, 0($t3)         ;current symbol     t1
    lb      $t2, 1($t3)         ;second  symbol     t2
    bnez    $t2, check          ;if *arr+1 != "\0" do

    bnez    $t0, bubble_sort    ;while (swapped);
    jr      r31

check:
    dsub    $t4, $t1, $t2       ;if *arr > *arr+1
    beqz    $t4, end_of_for
    bgez    $t4, swap           ;  swap
    b       end_of_for          ;else jump to end

swap:
    sb      $t1, 1($t3)
    sb      $t2, 0($t3)
    daddi   $t0, r0, 1

end_of_for:
    daddi   $t3, $t3, 1
    b		for_loop
    


print_string:   ; adresa retezce se ocekava v r4
                sw      r4, params_sys5(r0)
                daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
                syscall 5   ; systemova procedura - vypis retezce na terminal
                jr      r31 ; return - r31 je urcen na return address
