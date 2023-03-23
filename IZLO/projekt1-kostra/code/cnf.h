#ifndef __CNF_H
#define __CNF_H

#include <stdbool.h>
#include <assert.h>

typedef struct Literal Literal;

typedef struct Clause Clause;

typedef struct CNF CNF;

// Struktura vyjadrujici, ze predmet earlier_subject je prerekvizitou predmetu later_subject
typedef struct {
    unsigned earlier_subject;
    unsigned later_subject;
} Prerequisity;

// Vytvoreni nove klauzule, jejiz literaly budou kodovany pomoci num_of_subjects a num_of_semesters
Clause* create_new_clause(unsigned num_of_subjects, unsigned num_of_semesters);
// Prida (pripadne negovanou) promennou do klauzule reprezentujici, ze si student zapise predmet v danem semestru
void add_literal_to_clause(Clause *clause, bool is_positive, unsigned subject, unsigned semester);
// Pridani klauzule do formule
void add_clause_to_formula(Clause *clause, CNF *formula);

void each_subject_enrolled_at_least_once(CNF *formula, unsigned num_of_subjects, unsigned num_of_semesters);
void each_subject_enrolled_at_most_once(CNF *formula, unsigned num_of_subjects, unsigned num_of_semesters);
void add_prerequisities_to_formula(CNF *formula, Prerequisity* prerequisities, unsigned num_of_prerequisities, unsigned num_of_subjects, unsigned num_of_semesters);

#endif