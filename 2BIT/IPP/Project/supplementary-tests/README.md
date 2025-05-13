# Doplňkové testy k projektu do IPP 2023/2024
* Autor: Zbyněk Křivka, krivka@fit.vut.cz (Najdete-li v testech chybu, napište!)
* Verze: 2024-02-09
* Dostupné na gitea: https://git.fit.vutbr.cz/IPP/supplementary-tests

## Formát testů

Každý test je tvořen až 4 soubory stejného jména s příponami "src", "in", "out" a "rc" (ve stejném adresáři). Hlavní soubor testu s příponou "src" obsahuje zdrojový kód (příp. jeho XML reprezentaci). Soubory s příponami "in", "out" a "rc" obsahují vstup a očekávaný/referenční výstup a očekávaný první chybový návratový kód nebo bezchybový návratový kód 0. Pokud soubor s příponou "in" nebo "out" chybí, je-li třeba dogeneruje se prázdný soubor. Při nenulovém návratovém kódu je provedení testu považováno za úspěšné, pokud je získán správný návratový kód v odpovídajícím skriptu (výstupy se neporovnávají). V případě nulového kódu je třeba provést i porovnání výstupu skriptu s referenčním výstupem. Výstupy parse.py jsou porovnávány pomocí nástroje A7Soft JExamXML, jinak je použit unixový nástroj diff.

Při nalezení chyby prosím kontaktujte autora. V případě, že budete odevzdávat testy (nejlépe v podadresáři `tests` v archivu vámi vypracované úlohy), které jste nevypracovávali sami, tak prosím uveďte též jejich zdroj či autora například ve speciálním README souboru uloženém u testů. Nicméně raději odevzdávejte jen své vlastní testy.
Některé testovací soubory/data byly po vygenerování ručně pozměněny, aby lépe otestovaly schopnost vašich skriptů a upozornily na alternativní možnosti ve výstupech (např. zkrácený zápis prázdných značek XML). Použitá adresářová struktura je pouze ilustrační a v hodnotících testech bude zcela odlišná.

## Obsažené adresáře

 * `parse` - testy pro skript parse.py (výstup ve formátu XML je třeba porovnávat nástrojem *JExamXML* a ne nástrojem `diff`), vstupem jsou soubory s příponou `src`, očekávaný výstup najdete v souborech s příponou `out` a očekávané návratové kódy v souborech s příponou `rc`
 * `interpret` - testy pro interpret.php využívající rámec ipp-core (výstup se porovnává nástrojem `diff`), vstup `*.src` a `*.in`, očekávaný výstup `*.out` a očekávaný návratový kód `*.rc`
  
## Spuštění testů pro `parse.py`

```bash
cd parse
./test.sh [parse.py]
```
K dispozici jsou proměnné prostředí:
- `PARSE` - to same jako první parametr skriptu `test.sh`
- `NO_EXIT` - testy pokračují i po chybě
  ```bash
  NO_EXIT=1 ./test.sh [parse.py]
  ```
- `CLEANUP` - smazání všech souborů s příponami `*.your_out`, `*.your_rc`, `*_diff.xml` a `*.out.log` po úspěšném skončení skriptu `test.sh`
  ```bash
  CLEANUP=1 ./test.sh [parse.py]
  ```
Zda je skript spuštěn na serveru Merlin je detekovano automaticky, v případě lokálního spuštění potřebujete doložit `jexamxml.jar` a `options` do složky `supplementary-tests/parse/jexamxml` (viz níže).

## Příklady použití testů na serveru Merlin

Ručně a jednotlivě pro testy skriptu `parse.py` (předpokládejme jméno testu například `read_test`): 
```bash
python3.10 parse.py < read_test.src > read_test.your_out
echo $? > read_test.your_rc 
```
Následně musíte zjistit, zda jsou obsahy rovné těm referenčním v `read_test.rc` a `read_test.out`. Rovnost `read_test.out` a `read_test.your_out` je třeba ověřovat nástrojem *JExamXML* (viz níže).

Ručně a jednotlivě pro testy skriptu `interpret.php` (předpokládejme jméno testu například `write_test`): 
```bash
php8.3 interpret.php --source=write_test.src < write_test.in > write_test.your_out
echo $? > write_test.your_rc 
```

Alternativně, pokud bude vstup v souboru `read_test.in` pro test jménem `read_test`:
```bash
php8.3 interpret.php --input=read_test.in < read_test.src > read_test.your_out
echo $? > read_test.your_rc 
```


Pomocí nástroje `diff` zbývá zjistit, zda referenční výstupy odpovídají těm získaným v souborech s příponou `your_out` a `your_rc`.

Porovnání XML souborů např. `test.out` a `test.your_out` (s využitím *JExamXML* umístěného ve sdíleném adresáři na serveru Merlin):
```bash
java -jar /pub/courses/ipp/jexamxml/jexamxml.jar test.out test.your_out diffs.xml  /D /pub/courses/ipp/jexamxml/options
NAVRATOVA_HODNOTA="$?"
```
V případě, že bude nástrojem *JExamXML* detekována odlišnost zadaných XML souborů, tak bude návratová hodnota (viz proměnná `$NAVRATOVA_HODNOTA`) 1 a zadaný soubor `diffs.xml` bude obsahovat popis rozdílů. V případě, že jsou si oba soubory rovné vzhledem k nastavení porovnávání souborem `options`, tak bude návratová hodnota 0. POZOR, že pokud nástroji JExamXML nezadáte všechny povinné parametry, tak můžete dostat podivnou chybovou hlášku o nedostatku paměti.