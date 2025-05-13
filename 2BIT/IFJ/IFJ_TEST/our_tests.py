from test_runtime import ErrorType, test, print_exit

def test_all():
    # comment_test
    test("""
        //some comment
        /*biiiiiiiiiiiiiiiiiiiiiiiiiiiiiii
         iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii
         g comment */
         """, "")
    
    # test  read
    test("""
         let a : Int? = readInt()
         if let a {
            write(a)
         } else {
            write("proebali")
         }
         """, "5", program_stdin="5")
    
    # test bad read
    test("""
         let a : Int? = readInt()
         if let a {
            write(a)
         } else {
            write("proebali")
         }
         """, "proebali")
    
    test("""
// Program 1: Vypocet faktorialu (iterativne)
/* Hlavni telo programu */
write("Zadejte cislo pro vypocet faktorialu\n")
let a : Int? = readInt()
if let a {
    if (a < 0) {write("Faktorial nelze spocitat\n")
    } else {
        var a = Int2Double(a)
        var vysl: Double = 1
        while (a > 0) {
            vysl = vysl * a
            a = a - 1
        }
        write("Vysledek je: ", vysl, "\n")
    }
} else {
    write("Chyba pri nacitani celeho cisla!\n")
}

         """, f"Vysledek je: 120\n", program_stdin="5")
    
    # test("""
    # // Program 1: Vypocet faktorialu (iterativne)
    # /* Hlavni telo programu */
    # write("Zadejte cislo pro vypocet faktorialu\\n")
    # let a : Int? = readInt()
    # if let a {
    #     if (a < 0) {write("Faktorial nelze spocitat\\n")
    #     } else {
    #         var a = Int2Double(a)
    #         var vysl : Double = 1
    #         while (a > 0) {
    #             vysl = vysl * a
    #             a = a - 1
    #         }
    #         write("Vysledek je: ", vysl, "\\n")
    # } else {
    #     write("Chyba pri nacitani celeho cisla!\\n")
    # }
    # """, f"Zadejte cislo pro vypocet faktorialu\nVysledek je: {hexfloat(120.0)}\n",
    #      program_stdin="5")