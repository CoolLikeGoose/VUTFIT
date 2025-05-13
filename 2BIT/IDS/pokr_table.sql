-- IDS projekt 2024 Cast 2
-- Autor: Nikita Smirnov (xsmirn02), Bogdan Shaposhnik(xshapo04)

--Drop

ALTER TABLE "pracovnici" DROP CONSTRAINT "check_rc";
ALTER TABLE "pracovnici" DROP CONSTRAINT "check_telefonni_cislo";
ALTER TABLE "prac_kuchyne" DROP CONSTRAINT "fk_prac_kuchyne_pracovnici";
ALTER TABLE "prac_salu" DROP CONSTRAINT "fk_prac_salu_pracovnici";
ALTER TABLE "sal_stoly" DROP CONSTRAINT "fk_sal";
ALTER TABLE "sal_stoly" DROP CONSTRAINT "fk_stoly";
ALTER TABLE "objednavky" DROP CONSTRAINT "fk_stul_cislo_o";
ALTER TABLE "rezervace" DROP CONSTRAINT "fk_sal_cislo";
ALTER TABLE "rezervace" DROP CONSTRAINT "fk_stul_cislo";
ALTER TABLE "rezervace" DROP CONSTRAINT "chk_stul_or_sal";

DROP TABLE "pracovnici";
DROP TABLE "prac_kuchyne";
DROP TABLE "prac_salu";
DROP TABLE "saly";
DROP TABLE "stoly"; 
DROP TABLE "sal_stoly";
DROP TABLE "objednavky";
DROP TABLE "rezervace";


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
        ON DELETE CASCADE
);

CREATE TABLE "objednavky" (
    "cislo" INT NOT NULL PRIMARY KEY,
    "polozky" VARCHAR(1000) NOT NULL,
    "suma" INT NOT NULL
        CHECK ("suma" >= 0),
    "cas" TIMESTAMP NOT NULL,

    "stul_cislo" VARCHAR(4) NOT NULL,
    CONSTRAINT "fk_stul_cislo_o" 
        FOREIGN KEY ("stul_cislo")
        REFERENCES "stoly" ("stul_cislo")
        ON DELETE SET NULL
);

CREATE TABLE "rezervace" (
    "id" VARCHAR(10) NOT NULL PRIMARY KEY,
    "datum" TIMESTAMP NOT NULL,
    "jmeno_prijmeni" VARCHAR(50) NOT NULL,
    "pocet_lidi" INT NOT NULL,
    "typ_akce" VARCHAR(100),

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

-- --saly
INSERT INTO "saly" ("cislo", "pocet_stolu")
    VALUES (1, 4);
INSERT INTO "saly" ("cislo", "pocet_stolu")
    VALUES (2, 3);
INSERT INTO "saly" ("cislo", "pocet_stolu")
    VALUES (3, 3);

-- --sal_stoly
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

-- --rezervace
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1059383', TIMESTAMP '2024-03-19 15:30:00', 'Jiri Kamo',70,'svatba',NULL,'1');
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1059384', TIMESTAMP '2024-03-19 15:30:00', 'Jiri Kamo',70,'svatba',NULL,'2');
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1032186', TIMESTAMP '2024-06-08 10:45:00', 'Pavel Volya',4,NULL,'2-02',NULL);
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1012944', TIMESTAMP '2024-06-08 12:00:00', 'Ondrej Kral',2,NULL,'2-03',NULL);
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1014213', TIMESTAMP '2024-03-08 16:00:00', 'Ondrej Volya',4,NULL,'3-03',NULL);
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1014212', TIMESTAMP '2024-03-05 17:00:00', 'Ondrej Volya',4,NULL,'3-03',NULL);
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1014232', TIMESTAMP '2023-07-21 12:00:00', 'Valya Karmilita',8,NULL,'3-03',NULL);
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1567213', TIMESTAMP '2023-01-01 12:30:00', 'Nikita Smirnov',2,'Den Narozeni','1-04',NULL);
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('5014512', TIMESTAMP '2024-04-21 12:00:00', 'Jiri Tomato',2,NULL,'1-03',NULL);

--objednavky
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (4124, 'spagetti,pizza-pepperoni,salata', 946, TIMESTAMP '2024-06-08 12:05:14','2-03');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (1137, 'polevka-basilikova', 159, TIMESTAMP '2024-06-08 17:39:43','3-01');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (11447, 'omeleta,polevka-tomatova,pizza-qutroformaggi', 749, TIMESTAMP '2024-09-27 13:28:44','1-02');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (6731, 'carbonara,pizza-carbonara', 749, TIMESTAMP '2024-09-27 13:28:44','1-02');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (6732, 'omeleta,pizza-carbonara', 359, TIMESTAMP '2024-09-27 13:40:00','2-02');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (6790, 'pizza-pepperoni', 359, TIMESTAMP '2024-09-27 15:45:23','2-02');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (4421, 'pizza-pepperoni,polevka-tomatova', 638, TIMESTAMP '2024-03-08 16:00:00','3-03');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (4112, 'omeleta,pizza-pepperoni,polevka-tomatova', 1400, TIMESTAMP '2024-03-05 17:00:00','3-03');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5001, 'hamburger, hranolky, cola', 350, TIMESTAMP '2024-03-05 12:00:00', '1-01');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5002, 'pizza salami, coca-cola', 450, TIMESTAMP '2024-03-05 18:30:00', '1-02');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5003, 'bramborový salát, pivo', 180, TIMESTAMP '2024-03-06 14:15:00', '1-03');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5004, 'sendvič s lososem, zelený čaj', 280, TIMESTAMP '2024-03-06 10:45:00', '1-04');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5005, 'spaghetti carbonara, limonáda', 380, TIMESTAMP '2024-03-07 13:30:00', '2-01');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5006, 'cheeseburger, hranolky, cola', 370, TIMESTAMP '2024-03-07 19:00:00', '2-02');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5007, 'řízek s bramborovou kaší, pivo', 280, TIMESTAMP '2024-03-08 12:45:00', '2-03');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5008, 'césar salát, ice tea', 210, TIMESTAMP '2024-03-08 17:20:00', '3-01');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5009, 'pasta alfredo, limonáda', 320, TIMESTAMP '2024-03-09 11:00:00', '3-02');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5010, 'sendvič s kuřecím masem, cappuccino', 240, TIMESTAMP '2024-03-09 16:40:00', '3-03');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5011, 'česnečka, chlebíček', 120, TIMESTAMP '2024-03-10 09:15:00', '2-01');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5012, 'steak s hranolkami, cola', 480, TIMESTAMP '2024-03-10 14:00:00', '2-02');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5013, 'sushi set, zelený čaj', 550, TIMESTAMP '2024-03-11 17:45:00', '2-03');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5014, 'salát Caprese, prosecco', 320, TIMESTAMP '2024-03-11 19:30:00', '3-01');
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas", "stul_cislo")
VALUES (5015, 'bageta s lososem, černý čaj', 260, TIMESTAMP '2024-03-12 10:30:00', '3-02');


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
    VALUES (0203216651, 'sefkuchar', NULL);
INSERT INTO "prac_kuchyne" ("rc", "pracovni_pozice", "tipy")
    VALUES (7262143449, 'sous-chef', NULL);
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


SET SERVEROUTPUT ON;
-- vytvoření alespoň dvou netriviálních databázových triggerů vč. jejich předvedení

-- Check if the new reservation does not overlap with other reservations for a given table/hall
-- (it is assumed that the standard reservation time for a table is 1 hour, for a hall - 1 day).
CREATE OR REPLACE TRIGGER Check_reservation_overlap
    BEFORE INSERT ON "rezervace"
    FOR EACH ROW
DECLARE 
    overlapping_reservations INT;
BEGIN 
    SELECT COUNT(*)
    INTO overlapping_reservations
    FROM "rezervace"
    WHERE (:NEW."id" <> "id" AND
            (:NEW."stul_cislo" IS NOT NULL AND "stul_cislo" = :NEW."stul_cislo" AND     -- Check by stul_cislo
           (:NEW."datum" >= "datum" AND :NEW."datum" < "datum" + NUMTODSINTERVAL(1, 'HOUR') OR   -- if the new reservation is a little later
            :NEW."datum" + NUMTODSINTERVAL(1, 'HOUR') > "datum" AND
            :NEW."datum" + NUMTODSINTERVAL(1, 'HOUR') <= "datum" + NUMTODSINTERVAL(1, 'HOUR'))) OR   -- if the new reservation is a little earlier
          (:NEW."sal_cislo" IS NOT NULL AND "sal_cislo" = :NEW."sal_cislo" AND        -- Check by stul_cislo
           (:NEW."datum" >= "datum" AND :NEW."datum" < "datum" + NUMTODSINTERVAL(1, 'DAY') OR
            :NEW."datum" + NUMTODSINTERVAL(1, 'DAY') > "datum" AND
            :NEW."datum" + NUMTODSINTERVAL(1, 'DAY') <= "datum" + NUMTODSINTERVAL(1, 'DAY'))));
    IF overlapping_reservations > 0 THEN
        RAISE_APPLICATION_ERROR(-20001, 'Overlapping reservations');
    END IF;
END;
/

-- Tests
--ok
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('1123212', TIMESTAMP '2025-01-11 18:00:00', 'Jiri Tomato',2,NULL,'1-03',NULL);
--nok
-- INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
--     VALUES ('10532212', TIMESTAMP '2025-01-11 18:00:00', 'Ondrej Volya',1,NULL,'1-03',NULL);

-- Checking if the table has enough seats for all guests
CREATE OR REPLACE TRIGGER check_guests_limit
BEFORE INSERT OR UPDATE ON "rezervace"
FOR EACH ROW
DECLARE
    max_guests INT;
BEGIN
    IF :NEW."stul_cislo" IS NOT NULL THEN
        SELECT "pocet_zidle" INTO max_guests
        FROM "stoly"
        WHERE "stul_cislo" = :NEW."stul_cislo";
    ELSE
        RETURN; -- max guests for a hall is undefined
    END IF;

    IF :NEW."pocet_lidi" > max_guests THEN
        RAISE_APPLICATION_ERROR(-20002, 'There is not enough seats for guests');
    END IF;
END;
/

-- Tests
--ok
INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
    VALUES ('11294212', TIMESTAMP '2024-08-11 18:00:00', 'Jiri Tomato',2,NULL,'1-03',NULL);
--nok
-- INSERT INTO "rezervace" ("id", "datum", "jmeno_prijmeni", "pocet_lidi","typ_akce","stul_cislo","sal_cislo")
--     VALUES ('10112312', TIMESTAMP '2024-11-11 21:00:00', 'Jiri Tomato',40,NULL,'1-03',NULL);



--vytvoření alespoň dvou netriviálních uložených procedur vč. jejich předvedení

--Adding an item to an order and updates the price by the cost of that item
CREATE OR REPLACE PROCEDURE AddItemToOrder(
    p_order_number "objednavky"."cislo"%TYPE,
    p_item VARCHAR2,
    p_item_cost "objednavky"."suma"%TYPE
)
IS
    v_existing_items "objednavky"."polozky"%TYPE;
    v_existing_sum "objednavky"."suma"%TYPE;
BEGIN
    SELECT "polozky", "suma" INTO v_existing_items, v_existing_sum
    FROM "objednavky"
    WHERE "cislo" = p_order_number;

    UPDATE "objednavky"
    SET "polozky" = v_existing_items || ', ' || p_item,
        "suma" = v_existing_sum + p_item_cost
    WHERE "cislo" = p_order_number;
EXCEPTION
    WHEN OTHERS THEN
        ROLLBACK;
        DBMS_OUTPUT.PUT_LINE('Error occured while updating order.');
END AddItemToOrder;
/

-- Tests
-- Add testing object
INSERT INTO "objednavky" ("cislo", "polozky", "suma", "cas","stul_cislo")
    VALUES (9223, 'omeleta, pizza-pepperoni', 1400, TIMESTAMP '2024-09-25 11:00:00','3-03');

-- test print 
SELECT * FROM "objednavky" WHERE "cislo" = 9223;

--update
BEGIN
    AddItemToOrder(9223, 'polevka-tomatova', 1400);
END;
/

-- test print 
SELECT * FROM "objednavky" WHERE "cislo" = 9223;


-- Write out information about employees
CREATE OR REPLACE PROCEDURE DisplayEmployeesInfo
IS
    CURSOR c_employees IS 
        SELECT p."jmeno", P."prijmeni", p."telefon", p."profese",
            CASE 
                WHEN p."profese" = 'cisnik' THEN TO_CHAR(s."tipy")
                WHEN p."profese" = 'kuchar' THEN k."pracovni_pozice"
                ELSE NULL
            END AS additional_info
        FROM "pracovnici" p
        LEFT JOIN "prac_salu" s ON p."rc" = s."rc"
        LEFT JOIN "prac_kuchyne" k ON p."rc" = k."rc";

    v_name "pracovnici"."jmeno"%TYPE;
    v_surname "pracovnici"."prijmeni"%TYPE;
    v_phone "pracovnici"."telefon"%TYPE;
    v_profession "pracovnici"."profese"%TYPE;
    v_additional_info VARCHAR2(100);
BEGIN
    OPEN c_employees;

    LOOP
        FETCH c_employees INTO v_name, v_surname, v_phone, v_profession, v_additional_info;
        EXIT WHEN c_employees%NOTFOUND;

        DBMS_OUTPUT.PUT_LINE('Name: ' || v_name || ', Surname: ' || v_surname || ', Phone: ' || v_phone || ', Profession: ' || v_profession);
        IF v_additional_info IS NOT NULL THEN
            DBMS_OUTPUT.PUT_LINE('Additional Info: ' || v_additional_info);
        END IF;
    END LOOP;

    CLOSE c_employees;
END DisplayEmployeesInfo;
/

BEGIN
    DisplayEmployeesInfo();
END;
/


-- explicitní vytvoření alespoň jednoho indexu
-- Test: calculate the average yield per table in each hall

-- DROP INDEX idx_orders_table;

EXPLAIN PLAN FOR
SELECT "sal_stoly"."sal_cislo", AVG("objednavky"."suma") As "Average income"
FROM "objednavky"
JOIN "sal_stoly" ON "objednavky"."stul_cislo" = "sal_stoly"."stul_cislo"
GROUP BY "sal_stoly"."sal_cislo";

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);


CREATE INDEX idx_orders_table ON "objednavky"("stul_cislo");
EXPLAIN PLAN FOR
SELECT "sal_stoly"."sal_cislo", AVG("objednavky"."suma") As "Average income"
FROM "objednavky"
JOIN "sal_stoly" ON "objednavky"."stul_cislo" = "sal_stoly"."stul_cislo"
GROUP BY "sal_stoly"."sal_cislo";

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);
-- as we can see CPU load decreased and execution speed increased due to JOIN acceleration



-- vytvoření alespoň jednoho materializovaného pohledu
-- display of all future reservations
DROP MATERIALIZED VIEW mv_future_reservations;

CREATE MATERIALIZED VIEW mv_future_reservations
REFRESH ON DEMAND
AS 
SELECT "id", "datum", "jmeno_prijmeni"
FROM "rezervace"
WHERE "datum" > SYSDATE;

-- show MATERIALIZED VIEW
SELECT * FROM mv_future_reservations;

-- update rezervace
UPDATE "rezervace" SET "jmeno_prijmeni" = 'Jiri Orange' WHERE "id" = '1123212';

-- show MATERIALIZED VIEW again
SELECT * FROM mv_future_reservations;

-- refresh MATERIALIZED VIEW
BEGIN 
    DBMS_MVIEW.REFRESH('mv_future_reservations');
END;
/
SELECT * FROM mv_future_reservations;



-- vytvoření jednoho komplexního dotazu SELECT
-- profit check in every hall
WITH revenue_per_table AS (
    SELECT "sal_stoly"."sal_cislo", SUM("objednavky"."suma") AS total_revenue
    FROM "sal_stoly"
    JOIN "objednavky" ON "sal_stoly"."stul_cislo" = "objednavky"."stul_cislo"
    GROUP BY "sal_stoly"."sal_cislo"
)
SELECT
    revenue_per_table."sal_cislo",
    CASE
        WHEN revenue_per_table.total_revenue > 5000 THEN 'High Revenue'
        WHEN revenue_per_table.total_revenue > 2000 THEN 'Medium Revenue'
        ELSE 'Low Revenue'
    END AS revenue_category
FROM revenue_per_table;


-- definici přístupových práv k databázovým objektům pro druhého člena týmu
GRANT ALL ON "saly" TO XSHAPO04;
GRANT ALL ON "stoly" TO XSHAPO04;
GRANT ALL ON "sal_stoly" TO XSHAPO04;
GRANT ALL ON "pracovnici" TO XSHAPO04;
GRANT ALL ON "prac_kuchyne" TO XSHAPO04;
GRANT ALL ON "prac_salu" TO XSHAPO04;
GRANT ALL ON "objednavky" TO XSHAPO04;
GRANT ALL ON "rezervace" TO XSHAPO04;

GRANT EXECUTE ON AddItemToOrder TO XSHAPO04;
GRANT EXECUTE ON DisplayEmployeesInfo TO XSHAPO04;

GRANT ALL ON mv_future_reservations TO XSHAPO04;