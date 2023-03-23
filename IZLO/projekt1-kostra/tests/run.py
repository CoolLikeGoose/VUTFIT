import sys

from model import ModelError
from run_tests import execute, smoke_test, print_err, SolverError, GeneratorError


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: ./run.sh input")
        exit(1)

    smoke_test()

    try:
        model = execute(sys.argv[1])
    except GeneratorError as e:
        print_err("Chyba generátoru:")
        print(e)
        exit(1)
    except SolverError as e:
        print_err("Chyba SAT solveru:")
        print(e)
        exit(1)

    model.print()

    try:
        model.check()
    except ModelError as e:
        print_err(f"\n{e}")
        exit(1)
