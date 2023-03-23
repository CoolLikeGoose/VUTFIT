#include <stddef.h>
#include "cnf.h"

//
// LOGIN: xsmirn02
//

// Tato funkce je prikladem pouziti funkci create_new_clause, add_literal_to_clause a add_clause_to_formula
// Vysvetleni, co dela, naleznete v zadani
void example_condition(CNF *formula, unsigned num_of_subjects, unsigned num_of_semesters) {
    assert(formula != NULL);
    assert(num_of_subjects > 0);
    assert(num_of_semesters > 0);

    for (unsigned subject_i = 0; subject_i < num_of_subjects; ++subject_i) {
        for (unsigned semester_j = 0; semester_j < num_of_semesters; ++semester_j) {
            // vytvori novou klauzuli
            Clause *example_clause = create_new_clause(num_of_subjects, num_of_semesters);
            // vlozi do klauzule literal x_{i,j}    
            add_literal_to_clause(example_clause, true, subject_i, semester_j);
            // vlozi do klauzule literal ~x_{i,j}
            add_literal_to_clause(example_clause, false, subject_i, semester_j);
            // prida klauzuli do formule
            add_clause_to_formula(example_clause, formula);
        }
    }
}

// Tato funkce by mela do formule pridat klauzule predstavujici podminku a)
// Predmety jsou reprezentovany cisly 0, 1, ..., num_of_subjects-1
// Semestry jsou reprezentovany cisly 0, 1, ..., num_of_semesters-1
void each_subject_enrolled_at_least_once(CNF *formula, unsigned num_of_subjects, unsigned num_of_semesters) {
    assert(formula != NULL);
    assert(num_of_subjects > 0);
    assert(num_of_semesters > 0);

    for (int i = 0; i < num_of_subjects; i++)
    {
        Clause *nclause = create_new_clause(1, num_of_semesters);
        for (int j = 0; j < num_of_semesters; j++)
        {
            add_literal_to_clause(nclause, true, i, j);
        }
        add_clause_to_formula(nclause, formula);
    }
}

// Tato funkce by mela do formule pridat klauzule predstavujici podminku b)
// Predmety jsou reprezentovany cisly 0, 1, ..., num_of_subjects-1
// Semestry jsou reprezentovany cisly 0, 1, ..., num_of_semesters-1
void each_subject_enrolled_at_most_once(CNF *formula, unsigned num_of_subjects, unsigned num_of_semesters) {
    assert(formula != NULL);
    assert(num_of_subjects > 0);
    assert(num_of_semesters > 0);

    for (int i = 0; i < num_of_subjects; i++)
    {
        for (int j = 0; j < num_of_semesters; j++)
        {
            for (int k = j+1; k < num_of_semesters; k++)
            {
                Clause *nclause = create_new_clause(1, num_of_semesters);
                add_literal_to_clause(nclause, false, i, j);   
                add_literal_to_clause(nclause, false, i, k);
                add_clause_to_formula(nclause, formula);   
            }
        }
    }
}


// Tato funkce by mela do formule pridat klauzule predstavujici podminku c)
// Promenna prerequisities obsahuje pole s poctem prvku rovnym num_of_prerequisities
// Predmety jsou reprezentovany cisly 0, 1, ..., num_of_subjects-1
// Semestry jsou reprezentovany cisly 0, 1, ..., num_of_semesters-1
void add_prerequisities_to_formula(CNF *formula, Prerequisity* prerequisities, unsigned num_of_prerequisities, unsigned num_of_subjects, unsigned num_of_semesters) {
    assert(formula != NULL);
    assert(prerequisities != NULL);
    assert(num_of_subjects > 0);
    assert(num_of_semesters > 0);

    for (unsigned i = 0; i < num_of_prerequisities; i++) {
        // prerequisities[i].earlier_subject je predmet, ktery by si mel student zapsat v nekterem semestru pred predmetem prerequisities[i].later_subject
        for (int k = 0; k < num_of_semesters; k++)
        {
            for (int j = 0; j <= k; j++)
            {
                Clause *nclause = create_new_clause(2, num_of_semesters);
                add_literal_to_clause(nclause, false, prerequisities[i].earlier_subject, k);
                add_literal_to_clause(nclause, false, prerequisities[i].later_subject, j);
                add_clause_to_formula(nclause, formula);
            }
        }
        

        // for (int l = 0; l < num_of_semesters; l++) //late
        // {
        //     if (l != 0) {
        //         Clause *nclause = create_new_clause(l+1, l+1);
        //         for (int j = 0; j < l; j++) {  
        //             add_literal_to_clause(nclause, true, prerequisities[i].earlier_subject, j);
        //         }
        //         add_literal_to_clause(nclause, false, prerequisities[i].later_subject, l);
        //         add_clause_to_formula(nclause, formula);
        //     }

        //     Clause *nclause = create_new_clause(2, 1);
        //     add_literal_to_clause(nclause, false, prerequisities[i].earlier_subject, l);   
        //     add_literal_to_clause(nclause, false, prerequisities[i].later_subject, l);
        //     add_clause_to_formula(nclause, formula); 
        // }
    }
}
