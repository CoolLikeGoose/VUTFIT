-- IDS projekt 2024 Cast 2
-- Autor: Nikita Smirnov (xsmirn02), Bogdan Shaposhnik(xshapo04)

--Drop

ALTER TABLE "pracovnici" DROP CONSTRAINT "check_rc"
ALTER TABLE "pracovnici" DROP CONSTRAINT "check_telefonni_cislo"
ALTER TABLE "prac_kuchyne" DROP CONSTRAINT "fk_prac_kuchyne_pracovnici"
ALTER TABLE "prac_salu" DROP CONSTRAINT "fk_prac_salu_pracovnici"
ALTER TABLE "objednavky" DROP CONSTRAINT "fk_stul_cislo"
ALTER TABLE "rezervace" DROP CONSTRAINT "fk_sal_cislo"
ALTER TABLE "rezervace" DROP CONSTRAINT "fk_stul_cislo"
ALTER TABLE "rezervace" DROP CONSTRAINT "chk_stul_or_sal"
ALTER TABLE "stoly" DROP CONSTRAINT "fk_cislo_salu"
ALTER TABLE "sal_stoly" DROP CONSTRAINT "fk_sal"
ALTER TABLE "sal_stoly" DROP CONSTRAINT "fk_stoly"
ALTER TABLE "sal_stoly" DROP CONSTRAINT "uc_stul_stoly"

DROP TABLE "pracovnici";
DROP TABLE "prac_kuchyne";
DROP TABLE "prac_salu";
DROP TABLE "objednavky";
DROP TABLE "rezervace";
DROP TABLE "saly";
DROP TABLE "stoly"; 
DROP TABLE "sal_stoly";


--Create
-- Here we have main table for our workers with basic information
CREATE TABLE "pracovnici" (
    "rc" INT NOT NULL PRIMARY KEY,
    "jmeno" VARCHAR(20) NOT NULL,
    "prijmeni" VARCHAR(20) NOT NULL,
    "adresa" VARCHAR(100) NOT NULL,
    "telefon" VARCHAR(9),
    "profese" VARCHAR(13)
        CHECK ("profese" = 'cisnik' OR
               "profese" = 'kuchar' OR
               "profese" = 'administrator'),

    --length rc = 10 and % 11 == 0 didnt work
    CONSTRAINT "check_rc" CHECK (
        "rc" NOT LIKE '%[^0-9]%' AND
        LENGTH("rc") >= 9 AND
        LENGTH("rc") <= 10
    ),

    CONSTRAINT "check_telefonni_cislo" CHECK (
        "telefon" NOT LIKE '%[^0-9+-()]%' AND
        LENGTH("telefon") >= 9 AND
        LENGTH("telefon") <= 15
    )
);

--After previous table we can specify area of activity with unique properties
--All workers separeted to kitchen worker and hall workers
CREATE TABLE "prac_kuchyne" (
    "rc" INT NOT NULL PRIMARY KEY,
    "pracovni_pozice" VARCHAR(15),
    "tipy" INT,
    CONSTRAINT "fk_prac_kuchyne_pracovnici" 
        FOREIGN KEY ("rc")
        REFERENCES "pracovnici" ("rc")
        ON DELETE CASCADE
);

CREATE TABLE "prac_salu" (
    "rc" INT NOT NULL PRIMARY KEY,
    "tipy" INT,
    CONSTRAINT "fk_prac_salu_pracovnici" 
        FOREIGN KEY ("rc")
        REFERENCES "pracovnici" ("rc")
        ON DELETE CASCADE
);

CREATE TABLE "objednavky" (
    "cislo" INT NOT NULL PRIMARY KEY,
    "polozky" VARCHAR(100) NOT NULL,
    "suma" INT NOT NULL
        CHECK ("suma" >= 0),
    "cas" DATETIME NOT NULL,

    "stul_cislo" VARCHAR(4) NOT NULL,
    CONSTRAINT "fk_stul_cislo" 
        FOREIGN KEY ("stul_cislo")
        REFERENCES "stoly" ("stul_cislo")
        ON DELETE SET NULL
);

CREATE TABLE "rezervace" (
    "id" VARCHAR(10) NOT NULL,
    "datum" DATETIME NOT NULL,
    "jmeno_prijmeni" VARCHAR(50) NOT NULL,
    "pocet_lidi" INT NOT NULL,
    "typ_akce" VARCHAR(10),

    "stul_cislo" VARCHAR(4),
    "sal_cislo" INT,

    CONSTRAINT "fk_stul_cislo" 
        FOREIGN KEY ("stul_cislo")
        REFERENCES "stoly" ("stul_cislo")
        ON DELETE SET NULL,
        
    CONSTRAINT "fk_sal_cislo" 
        FOREIGN KEY ("sal_cislo")
        REFERENCES "saly" ("cislo")
        ON DELETE SET NULL,
    
    CONSTRAINT "chk_stul_or_sal" 
        CHECK (("stul_cislo" IS NOT NULL AND "sal_cislo" IS NULL) OR 
               ("stul_cislo" IS NULL AND "sal_cislo" IS NOT NULL))
);

CREATE TABLE "saly" (
    "cislo" INT NOT NULL PRIMARY KEY
        CHECK ("cislo" >= 0 AND "cislo" < 10),
    "pocet_stolu" INT NOT NULL
        CHECK ("pocet_stolu" >= 0 AND "pocet_stolu" < 100)
);

CREATE TABLE "stoly" (
    "stul_cislo" VARCHAR(4) NOT NULL PRIMARY KEY, -- 1-23 [sal-stul] max 9 salu & max 99 stolu
    "pocet_zidle" INT NOT NULL
);

CREATE TABLE "sal_stoly" (
    "sal_cislo" INT NOT NULL
        CHECK ("sal_cislo" >= 0 AND "sal_cislo" < 10),
    "stul_cislo" VARCHAR(4) NOT NULL PRIMARY KEY,
    CONSTRAINT "fk_sal" 
        FOREIGN KEY ("sal_cislo")
        REFERENCES "saly" ("cislo")
        ON DELETE CASCADE,
    CONSTRAINT "fk_stoly" 
        FOREIGN KEY ("stul_cislo")
        REFERENCES "stoly" ("stul_cislo")
        ON DELETE CASCADE,
    CONSTRAINT "uc_stul_stoly"
        UNIQUE ("stul_cislo")
);

--Create Objects

--stoly
INSERT INTO "stoly" ("stul_cislo", "pocet_zidle")
    VALUES ('1-01', 4);
INSERT INTO "stoly" ("stul_cislo", "pocet_zidle")
    VALUES ('1-02', 3);
INSERT INTO "stoly" ("stul_cislo", "pocet_zidle")
    VALUES ('1-03', 2);
INSERT INTO "stoly" ("stul_cislo", "pocet_zidle")
    VALUES ('1-04', 2);
INSERT INTO "stoly" ("stul_cislo", "pocet_zidle")
    VALUES ('2-01', 3);
INSERT INTO "stoly" ("stul_cislo", "pocet_zidle")
    VALUES ('2-02', 4);
INSERT INTO "stoly" ("stul_cislo", "pocet_zidle")
    VALUES ('2-03', 2);
INSERT INTO "stoly" ("stul_cislo", "pocet_zidle")
    VALUES ('3-01', 2);
INSERT INTO "stoly" ("stul_cislo", "pocet_zidle")
    VALUES ('3-02', 3);
INSERT INTO "stoly" ("stul_cislo", "pocet_zidle")
    VALUES ('3-03', 4);

--saly
INSERT INTO "saly" ("cislo", "pocet_stolu")
    VALUES (1, 4);
INSERT INTO "saly" ("cislo", "pocet_stolu")
    VALUES (2, 3);
INSERT INTO "saly" ("cislo", "pocet_stolu")
    VALUES (3, 3);

--sal_stoly
INSERT INTO "sal_stoly" ("sal_cislo", "stul_cislo")
    VALUES (1, '1-01');
INSERT INTO "sal_stoly" ("sal_cislo", "stul_cislo")
    VALUES (1, '1-02');
INSERT INTO "sal_stoly" ("sal_cislo", "stul_cislo")
    VALUES (1, '1-03');
INSERT INTO "sal_stoly" ("sal_cislo", "stul_cislo")
    VALUES (1, '1-04');
INSERT INTO "sal_stoly" ("sal_cislo", "stul_cislo")
    VALUES (2, '2-01');
INSERT INTO "sal_stoly" ("sal_cislo", "stul_cislo")
    VALUES (2, '2-02');
INSERT INTO "sal_stoly" ("sal_cislo", "stul_cislo")
    VALUES (2, '2-03');
INSERT INTO "sal_stoly" ("sal_cislo", "stul_cislo")
    VALUES (3, '3-01');
INSERT INTO "sal_stoly" ("sal_cislo", "stul_cislo")
    VALUES (3, '3-02');
INSERT INTO "sal_stoly" ("sal_cislo", "stul_cislo")
    VALUES (3, '3-03');

--rezervace
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1059383', '2024-03-19 15:30:00', 'Jiri Kamo',70,'svatba',NULL,"1");
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1059384', '2024-03-19 15:30:00', 'Jiri Kamo',70,'svatba',NULL,"2");
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1032186', '2024-06-08 10:45:00', 'Pavel Volya',4,NULL,'2-02',NULL);
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1012944', '2024-06-08 12:00:00', 'Ondrej Kral',2,NULL,'2-03',NULL);
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1014213', '2024-03-08 16:00:00', 'Ondrej Volya',4,NULL,'3-03',NULL);
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1014212', '2024-03-05 17:00:00', 'Ondrej Volya',4,NULL,'3-03',NULL);
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1014232', '2023-07-21 12:00:00', 'Valya Karmilita',8,NULL,'3-03',NULL);
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1567213', '2023-01-01 12:30:00', 'Nikita Smirnov',2,'Den Narozeni','1-04',NULL);
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1014212', '2024-04-21 12:00:00', 'Jiri Tomato',2,NULL,'1-03',NULL);

--objednavky
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (4124, 'spagetti,pizza-pepperoni,salata', 946,'2024-06-08 12:05:14','2-03');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (1137, 'polevka-basilikova', 159,'2024-06-08 17:39:43','3-01');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (11447, 'omeleta,polevka-tomatova,pizza-qutroformaggi', 749,'2024-09-27 13:28:44','1-02');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (6731, 'carbonara,pizza-carbonara', 749,'2024-09-27 13:28:44','1-02');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (6732, 'omeleta,pizza-carbonara', 359,'2024-09-27 13:40:00','2-02');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (6790, 'pizza-pepperoni', 359,'2024-09-27 15:45:23','2-02');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (4421, 'pizza-pepperoni,polevka-tomatova', 638,'2024-03-08 16:00:00','3-03');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (4112, 'omeleta,pizza-pepperoni,polevka-tomatova', 1400,'2024-03-05 17:00:00','3-03');

--pracovnici
INSERT INTO "pracovnici" ("rc", "jmeno", "prijmeni", "adresa", "telefon", "profese")
    VALUES (0203216651, 'Jiri','Tomato','Puskina 216/34F','934231946','kuchar');
INSERT INTO "pracovnici" ("rc", "jmeno", "prijmeni", "adresa", "telefon", "profese")
    VALUES (7262143449, 'Jirka','Tomato','Puskina 216/34F','524756245','kuchar');
INSERT INTO "pracovnici" ("rc", "jmeno", "prijmeni", "adresa", "telefon", "profese")
    VALUES (6807120859, 'Ales','Smrcka','Skacelova 24/7','245435815','administrator');
INSERT INTO "pracovnici" ("rc", "jmeno", "prijmeni", "adresa", "telefon", "profese")
    VALUES (9104054432, 'Ales','Volkov','Vaclavska 9/11','624502345','cisnik');
INSERT INTO "pracovnici" ("rc", "jmeno", "prijmeni", "adresa", "telefon", "profese")
    VALUES (8054025694, 'Holka','Lidova','Namesti Miru 91/1','697125485','cisnik');

--prac_kuchyn
INSERT INTO "prac_kuchyne" ("rc", "pracovni_pozice", "tipy")
    VALUES (0203216651, "sefkuchar", NULL);
INSERT INTO "prac_kuchyne" ("rc", "pracovni_pozice", "tipy")
    VALUES (7262143449, "sous-chef", NULL);
INSERT INTO "prac_kuchyne" ("rc", "pracovni_pozice", "tipy")
    VALUES (9104054432, NULL, 300);
INSERT INTO "prac_kuchyne" ("rc", "pracovni_pozice", "tipy")
    VALUES (8054025694, NULL, 15000);

--prac_salu
INSERT INTO "prac_salu" ("rc", "tipy")
    VALUES (6807120859, NULL);
INSERT INTO "prac_salu" ("rc", "tipy")
    VALUES (9104054432, 300);
INSERT INTO "prac_salu" ("rc", "tipy")
    VALUES (8054025694, 15000);


-- dva dotazy využívající spojení dvou tabulek:
-- Searching for sefkuchar home adress
SELECT adresa
FROM Pracovnici NATURAL JOIN Prac_kuchyne 	
WHERE pracovni_pozice = "sefkuchar"

--find all orders from 1 hall
SELECT * 
FROM Objednavky NATURAL JOIN Sal_stoly
WHERE sal_cislo = 1


-- jeden využívající spojení tří tabulek:
-- Selecting all tables in a particular hall(here in first) that do not have a reservation
SELECT stoly.stul_cislo
FROM stoly
JOIN sal_stoly ON stoly.stul_cislo = sal_stoly.stul_cislo
LEFT JOIN rezervace ON stoly.stul_cislo = rezervace.stul_cislo
WHERE rezervace.stul_cislo IS NULL
AND sal_stoly.sal_cislo = 1;

-- dva dotazy s klauzulí GROUP BY a agregační funkcí):
-- Count overall sum of all orders per hall
SELECT sal_cislo, SUM(suma) AS total_sum
FROM Objednavky NATURAL JOIN Sal_stoly
GROUP BY sal_cislo

-- Count all employee sorted by profesion
SELECT profese, COUNT(*) AS employee_count
FROM Pracovnici
GROUP BY profese


-- jeden dotaz obsahující predikát EXISTS:
-- Select all hall workers 
SELECT *
FROM Pracovnici
WHERE EXISTS(SELECT 1
            FROM Prac_salu
            WHERE Prac_salu.rc = Pracovnici.rc)


-- jeden dotaz s predikátem IN s vnořeným selectem (nikoliv IN s množinou konstantních dat),
-- Selecting all tables in a particular hall that do not have a reservation
SELECT stul_cislo
FROM stoly
WHERE stul_cislo NOT IN (
    SELECT DISTINCT stul_cislo
    FROM rezervace
    WHERE stul_cislo IS NOT NULL
);