STATUS_SAT = "SAT"
STATUS_UNSAT = "UNSAT"


class ModelError(Exception):
    pass


class Input:
    def __init__(self, num_of_subjects, num_of_semesters, prerequisities):
        self.num_of_subjects = num_of_subjects
        self.num_of_semesters = num_of_semesters
        self.prerequisities = prerequisities

    @staticmethod
    def load(path):
        with open(path) as f:
            lines = f.read().split("\n")
            header = lines[0].split(" ")
            num_of_subjects = int(header[0])
            num_of_semesters = int(header[1])

            prerequisities = []
            for line in lines[1:]:
                if line not in ["", "\n"]:
                    prereq = line.split(" ")
                    first = int(prereq[0])
                    second = int(prereq[1])
                    prerequisities.append((first, second))

            return Input(num_of_subjects, num_of_semesters, prerequisities)

    def compute_var_index(self, semester, subject):
        return subject * self.num_of_semesters + semester + 1

    def subject_from_var(input, var):
        return int((var - 1) / input.num_of_semesters)


class Model:
    def __init__(self, status, literals, input):
        self.status = status
        self.literals = literals
        self.input = input

    @staticmethod
    def load(path, input):
        """
        The output of the minisat always has form:
            STATUS
            [MODEL 0]
        """
        with open(path, "r") as f:
            lines = f.read().split("\n")
            status = lines[0]

            if status == STATUS_UNSAT:
                return Model(status, None, input)
            else:
                model = lines[1].split(" ")[0:-1]  # Discard '0'

                if model == [""]:
                    return Model(status, [], input)

                model = list(map(lambda x: int(x), model))
                return Model(status, model, input)

    def __getitem__(self, key):
        semester, subject = key
        var = self.input.compute_var_index(*key)

        if var in self.literals:
            return True
        elif -var in self.literals:
            return False
        else:
            return True  # variable is undefined

    def subject_row(self, subject):
        acc = []
        for semester in range(self.input.num_of_semesters):
            acc.append(self[semester, subject])
        return acc

    def find_semester(self, subject):
        row = self.subject_row(subject)
        assert sum(row) == 1
        return row.index(True)

    def print(self):
        print(self.status)

        if self.status == STATUS_UNSAT:
            return

        print("Model:")
        for semester in range(self.input.num_of_semesters):
            print(f"Semestr {semester+1}:")
            for subject in range(self.input.num_of_subjects):
                if self[semester, subject]:
                    print(f"  předmět {subject}")

    def check_exactly_once(self):
        for subject in range(self.input.num_of_subjects):
            row = self.subject_row(subject)
            if sum(row) == 0:
                raise ModelError(f"Nesprávný model: předmět {subject} není zapsán")
            if sum(row) > 1:
                raise ModelError(
                    f"Nesprávný model: předmět {subject} je zapsán více než jednou"
                )

    def check_prerequisities(self):
        for (first, second) in self.input.prerequisities:
            first_index = self.find_semester(first)
            second_index = self.find_semester(second)
            if first_index >= second_index:
                raise ModelError(
                    f"Nesprávný model: prerekvizita {first} < {second} není splněna"
                )

    def check(self):
        if self.status == STATUS_SAT:
            self.check_exactly_once()
            self.check_prerequisities()
