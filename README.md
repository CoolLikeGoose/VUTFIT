# Projekt 1 - Práce s textem
## Motivační příklad
Snad všichni známe telefonní seznam v chytrých telefonech. Vyhledávání tam většinou funguje i za pomocí numerické klávesnice. Uživatel zadává posloupnosti číslic, ale každým stiskem klávesy myslí jeden ze znaků, které daná čislice reprezentuje (například číslo 5 reprezentuje samotnou 5 či jedno z písmen j, k nebo l). Telefon pak filtruje telefonní seznam podle dané posloupnosti, přičemž se snaží v seznamu najít libovolný kontakt, jehož jméno nebo telefonní číslo obsahuje sekvenci (nepřerušenou nebo rozdělenou) zadaných znaků.
## Popis projektu
Cílem projektu je vytvořit program, který by emuloval zjednodušený algoritmus hledání kontaktů zadaných pomocí posloupnosti číslic. Vstupem programu je posloupnost číslic. Výstup programu bude obsahovat seznam kontaktů, které odpovídají zadanému číselnému filtru.
## Detailní specifikace
Program implementujte ve zdrojovém souboru ''t9search.c''. Vstupní data budou čtena ze standardního vstupu (stdin), výstup bude tisknut na standardní výstup (stdout).

# Implementační detaily
## Vstupní telefonní seznam
Telefonní seznam jsou ASCII textová data. Každý záznam o kontaktu obsahuje dva řádky: jméno osoby a jeho telefonní číslo. Každý řádek obsahuje maximálně 100 znaků, jinak se jedná o neplatná data. Seznam kontaktů je neuspořádaný. U všech dat nezáleží na velikosti písmen (tzv. case insensitive). Program musí podporovat alespoň 42 kontaktů.

## Kritérium nalezení kontaktu
Pro zjednodušení budeme v tomto projektu uvažovat hledání '''nepřerušené posloupnosti''' zadaných znaků/číslic v telefonním kontaktu. Nepřerušenou posloupností znaků se myslí takový podřetězec, u kterého se na každé pozici vyskytuje znak, který odpovídá jednomu ze znaků, které reprezentuje číslice na stejné pozici v zadaném řetězci. Každá číslice kromě sebe sama reprezentuje ještě následující znaky: 2 (abc), 3 (def), 4 (ghi), 5 (jkl), 6 (mno), 7 (pqrs), 8 (tuv), 9 (wxyz), 0 (+).

## Výstup programu
### Výstup programu může být dvojího druhu:
 - žádný kontakt nenalezen
 - nalezen jeden a více kontaktů.

### Kontakt nenalezen
 - Not found

Pokud v telefonním seznamu neexistuje kontakt, u něhož by nějaká část odpovídala hledanému řetězci CISLO, vytiskne program toto hlášení.
 

### Kontakt(y) nalezen(y)
Každý kontakt, který odpovídá hledanému kritériu, program vytiskne na jeden řádek ve formátu "JMENO, TELCISLO", kde JMENO je jméno kontaktu z telefonního seznamu a TELCISLO je záznam o telefonním čísle. Program tiskne všechny kontakty, které kritériu odpovídají. Na pořadí řádků nezáleží. Stejně tak nezáleží ani na velikosti písmen.

## Omezení v projektu
 - Je ***zakázané*** použít následující funkce:
volání z rodiny ***malloc*** a ***free*** - práce s dynamickou pamětí není v tomto projektu zapotřebí,
 - volání z rodiny ***fopen***, ***fclose***, ***fscanf***, ... - práce se soubory (dočasnými) není v tomto projektu žádoucí,
 - volání ***qsort***, ***lsearch***, ***bsearch*** a ***hsearch*** - cílem je zamyslet se nad algoritmizací a strukturou dat.

## Neočekávané chování
Na chyby za běhu programu reagujte obvyklým způsobem: Na neočekávaná vstupní data, formát vstupních dat nebo chyby při volání funkcí reagujte přerušením programu se stručným a výstižným chybovým hlášením na příslušný výstup a odpovídajícím návratovým kódem. Hlášení budou v kódování ASCII česky nebo anglicky.

# Hodnocení
Na výsledném hodnocení mají hlavní vliv následující faktory:
 - přeložitelnost zdrojového souboru,
 - formát zdrojového souboru (členění, zarovnání, komentáře, vhodně zvolené identifikátory),
 - dekompozice problému na podproblémy (vhodné funkce, vhodná délka funkcí a parametry funkcí),
 - správná volba datových typů, případně tvorba nových typů,
 - správná funkcionalita vyhledání kláves a
 - ošetření chybových stavů.

## Priority funkcionality
1. Vyhledání kontaktů, jejichž telefonní čísla začínají zadaným číslem.
2. Vyhledání kontaktů, jejichž jména začínají znaky, které zadané číslo reprezentuje.
3. (nepovinné, avšak může zlepšit hodnocení) Hledání přerušených posloupností znaků. Program najde i takové kontakty, u který mezi každými dvěma nalezenými znaky může být libovolná posloupnost jiných znaků. Důležité je, že nalezený řetězec (jména nebo telefonního čísla) musí obsahovat všechny znaky, které číslovky reprezentují, a ve stejném pořadí.
4. (prémiové) Hledání podobných kontaktů. Pokud zadal uživatel špatný vyhledávací řetězec (spletl se v čísle), program mu nabídne podobné výsledky. Uvažujte následující chyby uživatele: chybná číslice, chybějící číslice, číslice zadaná navíc. Program by měl být konfigurován argumentem programu -l L, kde L reprezentuje maximální počet uvažovaných chyb uživatele (pozn. symbol L je inspirován z tzv. Levenshteinovy vzdálenosti dvou řetězců).