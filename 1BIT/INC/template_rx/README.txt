===============================================================================
N�VOD NA POU�IT�:
===============================================================================

1) Zkop�rujte V� .zip archiv pripraven� na odevzd�n� do WIS do t�to slo�ky.

2) Hodnotu sv�ho loginu vlo�te do promenn� LOGIN ve skriptu test.sh.

3) Spustte test projektu vol�n�m skriptu test.sh.

===============================================================================
V�STUPY:
===============================================================================

standardn� v�stup - Strucn� prehled prubehu a v�sledku testu.

work/ - Slo�ka obsahuj�c� pracovn� soubory vytvoren� v prubehu testu.

work/log/ - Slo�ka obsahuj�c� podrobnosti o prubehu jednotliv�ch kroku testu.

POZN1: Test nekontroluje obsah dokumentace, ale pouze jej� existenci.
POZN2: V�sledky testu jsou pouze orientacn� a mohou se li�it od konecn�ho
       hodnocen� projektu. V z�sade ale, pokud test objev� nekter� nedostatek, 
       je vysoce pravdepodobn�, �e se tento nedostatek objev� i pri hodnocen�.


===============================================================================
CO TEST.SH DEL�:
===============================================================================

1) Nejprve si rozbal� vstupn� .zip archiv a zkontroluje, zda obsahuje v�echny
   po�adovan� soubory ve spr�vn�ch slo�k�ch.

2) Spust� synt�zu a simulaci projektu v prostred� GHDL.

3) Provede ohodnocen� dosa�en�ch v�sledku. Porovn� odpovedi v log souborech 
   z�skan�ch v kroku 2) se vzorov�mi odpovedmi a zkontroluje �spech synt�zy.
