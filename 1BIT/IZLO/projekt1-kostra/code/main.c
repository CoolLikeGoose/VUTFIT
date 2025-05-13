#include <stdlib.h>
#include <stdio.h>

#include "cnf.h"

struct Literal {
    int var;
    struct Literal *next_literal;
};

struct Clause {
    Literal *first_literal;
    Literal *last_literal;

    struct Clause *next_clause;

    unsigned num_of_subjects;
    unsigned num_of_semesters;
};

struct CNF {
    Clause *first_clause;
    Clause *last_clause;

    unsigned num_of_clauses;

    unsigned num_of_subjects;
    unsigned num_of_semesters;
};

Clause* create_new_clause(unsigned num_of_subjects, unsigned num_of_semesters) {
    assert(num_of_subjects > 0);
    assert(num_of_semesters > 0);

    Clause *new_clause = malloc(sizeof(Clause));
    new_clause->first_literal = NULL;
    new_clause->last_literal = NULL;
    new_clause->next_clause = NULL;
    new_clause->num_of_subjects = num_of_subjects;
    new_clause->num_of_semesters = num_of_semesters;
    return new_clause;
}

void add_literal_to_clause(Clause *clause, bool is_positive, unsigned subject, unsigned semester) {
    assert(clause != NULL);

    Literal *new_literal = malloc(sizeof(Literal));

    int lit_num = subject*clause->num_of_semesters + semester + 1;
    if (!is_positive) {
        lit_num = -lit_num;
    }
    new_literal->var = lit_num;
    new_literal->next_literal = NULL;

    if (clause->last_literal == NULL) {
        assert(clause->first_literal == NULL);
        clause->first_literal = new_literal;
    } else {
        clause->last_literal->next_literal = new_literal;
    }
    clause->last_literal = new_literal;
}

void add_clause_to_formula(Clause *clause, CNF *formula) {
    assert(clause != NULL);
    assert(formula != NULL);

    if (formula->last_clause == NULL) {
        assert(formula->first_clause == NULL);
        formula->first_clause = clause;
    } else {
        formula->last_clause->next_clause = clause;
    }
    formula->last_clause = clause;

    ++formula->num_of_clauses;
}

unsigned get_num_of_variables(CNF *formula) {
    assert(formula != NULL);
    return formula->num_of_semesters * formula->num_of_subjects;
}

unsigned get_num_of_clauses(CNF *formula) {
    assert(formula != NULL);
    return formula->num_of_clauses;
}

void clear_clause(Clause *cl) {
    assert(cl != NULL);
    while(cl->first_literal != NULL) {
        Literal *this_lit = cl->first_literal;
        cl->first_literal = cl->first_literal->next_literal;
        free(this_lit);
    }
    cl->last_literal = NULL;
}

void clear_cnf(CNF *formula) {
    assert(formula != NULL);
    while(formula->first_clause != 0) {
        Clause *this_cl = formula->first_clause;
        formula->first_clause = formula->first_clause->next_clause;
        clear_clause(this_cl);
        free(this_cl);
    }
    formula->last_clause = NULL;
    formula->num_of_clauses = 0;
}

void error(char *error_msg) {
    fprintf(stderr, "%s\n", error_msg);
    exit(-1);
}

void print_formula(CNF *formula) {
    assert(formula != NULL);
    
    printf("p cnf %u %u\n", get_num_of_variables(formula), get_num_of_clauses(formula));
    Clause *next_cl = formula->first_clause;
    while (next_cl != 0) {
        Literal *next_lit = next_cl->first_literal;
        while (next_lit != 0) {
            printf("%d ", next_lit->var);
            next_lit = next_lit->next_literal;
        }
        next_cl = next_cl->next_clause;
        printf("0\n");
    }
}

int main (int argc, char *argv[]) {
    if (argc != 2) {
        error("Program ocekava presne jeden argument: soubor obsahujici prerekvizity predmetu");
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        error("Vstupni soubor nelze otevrit");
    }

    printf("c Vstupni soubor:\n");
    
    unsigned num_of_subjects, num_of_semesters, num_of_prerequisities;
    if (fscanf(input_file, "%u %u %u", &num_of_subjects, &num_of_semesters, &num_of_prerequisities) != 3) {
        fclose(input_file);
        error("Hlavicka souboru s prerekvizitami by mela obsahovat tri cisla: pocet predmetu, pocet semestru a pocet prerekvizit v souboru");
    }
    printf("c %u %u %u\n", num_of_subjects, num_of_semesters, num_of_prerequisities);
    if (num_of_subjects == 0 || num_of_semesters == 0) {
        fclose(input_file);
        error("Je treba stanovit kladny pocet predmetu a semestru");
    }

    Prerequisity *prerequisities = malloc(num_of_prerequisities * sizeof(Prerequisity));
    for (unsigned i = 0; i < num_of_prerequisities; ++i) {
        Prerequisity p;
        if (fscanf(input_file, "%u %u", &p.earlier_subject, &p.later_subject) != 2) {
            free(prerequisities);
            fclose(input_file);
            error("Ocekavani dalsich predmetovych prerekvizit");
        }
        printf("c %u %u\n", p.earlier_subject, p.later_subject);
        if (p.earlier_subject >= num_of_subjects || p.later_subject >= num_of_subjects) {
            free(prerequisities);
            fclose(input_file);
            error("Nektery predmet v predmetech je vetsi nez dane maximum");
        }
        prerequisities[i] = p;
    }
    fclose(input_file);

    printf("c\nc Mapovani promennych na predmety a semestry:\n");
    for (unsigned var=1; var <= num_of_subjects * num_of_semesters; ++var) {
        printf("c %u - predmet %u, semester %u\n", var, (var-1) / num_of_semesters, (var-1) % num_of_semesters);
    }

    printf("c\nc\n");

    CNF f = { .first_clause = NULL, .last_clause = NULL, .num_of_clauses = 0, .num_of_subjects = num_of_subjects, .num_of_semesters = num_of_semesters };
    each_subject_enrolled_at_least_once(&f, num_of_subjects, num_of_semesters);
    each_subject_enrolled_at_most_once(&f, num_of_subjects, num_of_semesters);
    add_prerequisities_to_formula(&f, prerequisities, num_of_prerequisities, num_of_subjects, num_of_semesters);
    free(prerequisities);

    Clause* cl = create_new_clause(num_of_subjects, num_of_semesters);

    printf("c Formula:\n");
    print_formula(&f);

    clear_cnf(&f);

    return 0;
} 
