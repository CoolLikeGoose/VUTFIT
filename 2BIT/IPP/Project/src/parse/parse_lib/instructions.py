from enum import Enum

class Types(Enum):
    label = 0
    var = 1
    symb = 2
    type = 3

class Instruction():
    __instructions = {
        #Práce s paměťovými rámci, volání funkcí
        "MOVE":         [Types.var, Types.symb],
        "CREATEFRAME":  [],
        "PUSHFRAME":    [],
        "POPFRAME":     [],
        "DEFVAR":       [Types.var],
        "CALL":         [Types.label],
        "RETURN":       [],

        #Práce s datovým zásobníkem
        "PUSHS":        [Types.symb],
        "POPS":         [Types.var],

        #Aritmetické, relační, booleovské a konverzní instrukce
        "ADD":          [Types.var, Types.symb, Types.symb],
        "SUB":          [Types.var, Types.symb, Types.symb],
        "MUL":          [Types.var, Types.symb, Types.symb],
        "IDIV":         [Types.var, Types.symb, Types.symb],
        "LT":           [Types.var, Types.symb, Types.symb],
        "GT":           [Types.var, Types.symb, Types.symb],
        "EQ":           [Types.var, Types.symb, Types.symb],
        "AND":          [Types.var, Types.symb, Types.symb],
        "OR":           [Types.var, Types.symb, Types.symb],
        "NOT":          [Types.var, Types.symb],
        "INT2CHAR":     [Types.var, Types.symb],
        "STRI2INT":     [Types.var, Types.symb, Types.symb],

        #Vstupně-výstupní instrukce
        "READ":         [Types.var, Types.type],
        "WRITE":        [Types.symb],

        #Práce s řetězci
        "CONCAT":       [Types.var, Types.symb, Types.symb],
        "STRLEN":       [Types.var, Types.type],
        "GETCHAR":      [Types.var, Types.symb, Types.symb],
        "SETCHAR":      [Types.var, Types.symb, Types.symb],

        #Práce s typy
        "TYPE":         [Types.var, Types.type],

        #Instrukce pro řízení toku programu
        "LABEL":        [Types.label],
        "JUMP":         [Types.label],
        "JUMPIFEQ":     [Types.label, Types.symb, Types.symb],
        "JUMPIFNEQ":    [Types.label, Types.symb, Types.symb],
        "EXIT":         [Types.symb],

        #Ladicí instrukce
        "EXIT":         [Types.symb],
        "BREAK":        []
    }

    @staticmethod
    def get_types_by_opcode(OpCode):
        #specifically for duplicate headers
        if (OpCode.lower() == ".ippcode24"):
            exit(23)

        if OpCode in Instruction.__instructions:
            return Instruction.__instructions[OpCode]
        return None