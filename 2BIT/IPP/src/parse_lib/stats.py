from .debug import ExitCodes

class Statistics():
    def __init__(self, args, debugger):
        self.debug = debugger

        #quantity stats
        self.q_opcode = 0
        self.q_comments = 0
        self.q_labels = 0
        self.q_jumps = 0
        self.q_fwjumps = 0
        self.q_backjumps = 0
        self.q_badjumps = 0
        self.q_freq = []

        self.dict_opcode = {}   # Opcode : Count
        self.dict_label = {}    # Label  : Line
        self.arr_jumps = []     # [Label, Line]
        
        #[[FileName, [Option, ...]], ...]
        self.save_stats = []
        #{Option: lambda}
        self.options = {
            "--loc": self.q_opcode,
            "--comments": self.q_comments,
            "--labels": self.q_labels,
            "--jumps": self.q_jumps,
            "--fwjumps": self.q_fwjumps,
            "--backjumps": self.q_backjumps,
            "--frequent": self.q_freq,
            "--print": "",
            "--eol": '\n'
        }

        self.__configure_stats(args)
    
    def add_comment(self):
        self.q_comments += 1

    def __configure_stats(self, args):
        skip_arg = False
        for i in range(len(args)):
            if (args[i] == "-v") or (skip_arg):
                skip_arg = False
                continue

            if (args[i].startswith("--stats")):
                if args[i] == "--stats":
                    self.save_stats.append([args[i+1], []])
                    skip_arg = True
                    continue
                self.save_stats.append([args[i].split("="), []])
            if (args[i] not in self.options) or (len(self.save_stats) == 0):
                exit(ExitCodes.wrongArguments.value)
            self.save_stats[-1][1].append(args[i])


    def process_opcode(self, instrucion):
        opcode = instrucion[0].upper()
        self.q_opcode += 1

        #--freq
        if (opcode in self.dict_opcode):
            self.dict_opcode[opcode] += 1
        else:
            self.dict_opcode[opcode] = 1

        #jumps
        if (opcode == "LABEL"):
            self.dict_label[instrucion[1]] = self.q_opcode
            self.q_labels += 1
        elif (opcode in ["CALL", "JUMP", "JUMPIFEQ", "JUMPIFNEQ"]):
            self.arr_jumps.append([instrucion[1], self.q_opcode])
            self.q_jumps += 1
        elif (opcode == "RETURN"):
            self.q_jumps += 1
    
    def count_statistics(self):
        self.debug.log("Counting stats")
        #Count jumps stats
        for jmp in self.arr_jumps:
            if (jmp[0] not in self.dict_label):
                self.q_badjumps += 1
                continue

            if (jmp[1] > self.dict_label[jmp[0]]):
                self.q_fwjumps += 1
            else:
                self.q_backjumps += 1

        #Count freq stats
        max_freq = max(self.dict_opcode.values())
        print("HUI\n")
        print(self.dict_opcode)
        self.q_freq = sorted([key for key, value in self.dict_opcode if value == max_freq])
        
        self.print_stats()

    def print_to_file(self, file, options):
        for option in options:
            file.write(f"{self.options[option]}\n")

    def print_stats(self):
        self.debug.log("Printing stats to files")
        for stat in self.save_stats:
            with open(stat[0], 'w') as file:
                self.print_to_file(file, stat[1])