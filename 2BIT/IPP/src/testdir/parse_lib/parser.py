import argparse
import sys
import re
import xml.etree.ElementTree as ET

from .debug import *
from .stats import Statistics
from .instructions import Instruction
from .instructions import Types
#statp ext
    #--stats=file   ------ location for stats
    #--loc          ------ instructions number(without empty and comments)
    #--comments     ------ Number of lines with comments
    #--labels       ------ Number of defined labels
    #--jumps        ------ Number of jumps(calls )
    #--fwjumps
    #--backjumps
    #--badjumps
    #--frequent     ------ jména operačních kódů (velkými písmeny, čárkou oddělené, bez mezer, abecedně vzestupně)
    #--print=string
    #--eol
    #
class Parser:
     def __init__(self):
          self.order = 1

          #Pre-compiled paterns for optimization(indices relative to Types Enum)
          self.regexPatterns = [
               #label
               #(?!int$|bool$|string$|nil$) add if label cannot be a name of type
               re.compile(r"^(([A-Za-z_\-$&%*!?]*)+[A-Za-z0-9_\-$&%*!?]*)$"),     
               #var
               re.compile(r"^(GF|LF|TF)@(?!int$|bool$|string$|nil$)(([A-Za-z_\-$&%*!?]*)+[A-Za-z0-9_\-$&%*!?]*)$"),  
               #symb   
               re.compile(r"(^(GF|LF|TF)@(?!int$|bool$|string$|nil$)(([A-Za-z_\-$&%*!?]*)+[A-Za-z0-9_\-$&%*!?]*)$)|(^(int@((\-?0x[0-9A-Fa-f]+)|(\-?0o[0-7]+)|(\-?[0-9]+)))|(string@.*)|(bool@(true|false))|(nil@nil))"),    
               #type 
               re.compile(r"^(int|string|bool|nil)$")      
          ]
          #Regex for detecting code & comments in the line
          self.regexLineSep = re.compile(r"(.*?)#(.*)")
          self.regexVarCheck = re.compile(r"^(GF|LF|TF)@.*")

          self.__argument_parse()
          self.debug = Debug(self.args.verbose)
          self.stats = Statistics(sys.argv[1:])
          self.etRoot = ET.Element("program")
          self.etRoot.set("language", "IPPcode24")

          self.debug.log("Parser initialization completed")
     
     def __argument_parse(self):
          parser = argparse.ArgumentParser(description="IPPcode24 to XML parser", add_help=False)
          parser.add_argument('-v', '--verbose', help="Show debug infromation", action="store_true")
          parser.add_argument('-h', '--help', help="show this help message and exit", action="store_true")

          parser.add_argument('--stats', help="file to which the statistics will be saved(you must specify the following parameters)", metavar="FILE", type=str)
          parser.add_argument('--loc', help="Saves to statistic file count of instructions", action="store_true")
          parser.add_argument('--comments', help="Saves to statistic file count of comments", action="store_true")
          parser.add_argument('--labels', help="Saves to statistic file count of labels", action="store_true")
          parser.add_argument('--jumps', help="Saves to statistic file count of jumps", action="store_true")
          parser.add_argument('--fwjumps', help="Saves to statistic file count of forward jumps", action="store_true")
          parser.add_argument('--backjumps', help="Saves to statistic file count of back jumps", action="store_true")
          parser.add_argument('--badjumps', help="Saves to statistic file count of jumps to non-existing label", action="store_true")
          parser.add_argument('--frequent', help="Saves to statistic file most frequently used instructions", action="store_true")
          parser.add_argument('--print', help="Prints STRING to statistic file", metavar="STRING", type=str)
          parser.add_argument('--eol', help="Adds an EOL symbol to statistic file", action="store_true")

          self.args = parser.parse_args()
          if self.args.help:
               if len(sys.argv) == 2:
                    parser.print_help()
                    exit()
               exit(ExitCodes.wrongArguments.value)


     
     def __xml_builder(self, OpCode, args):
          instr = ET.SubElement(self.etRoot, "instruction")
          instr.set("order", str(self.order))
          instr.set("opcode", OpCode)
          self.order += 1

          argc = 1
          for arg in args:
               argElem = ET.SubElement(instr, f"arg{argc}")
               argElem.set("type", arg[0])
               argElem.text = arg[1]
               argc += 1

     def __detect_data_types(self, arg, type):
          if (type != Types.symb):
               return [type.name, arg]
          if (re.fullmatch(self.regexVarCheck, arg) != None):
               return ["var", arg]
          return arg.split("@", 1)
               
     def __check_opcode_types(self, instruction):
          types = Instruction.get_types_by_opcode(instruction[0].upper())
          if types is None:
               self.debug.log(f"Bad OpCode:\t {instruction}")
               exit(ExitCodes.badOpCode.value) 
          if len(instruction)-1 != len(types):
               self.debug.log(f"Wrong arg count:\t {instruction}")
               exit(ExitCodes.sourceCodeError.value)
          
          args = []
          for arg, type in zip(instruction[1:], types):
               if (re.fullmatch(self.regexPatterns[type.value], arg) == None):
                    self.debug.log(f"Bad arg types:\t {instruction} -> at {arg}, must be {type}")
                    exit(ExitCodes.sourceCodeError.value)

               args.append(self.__detect_data_types(arg, type))

          self.stats.process_opcode(instruction)
          self.__xml_builder(instruction[0], args)

     def __line_parse(self, line):
          if (len(line) == 0):
               return
          
          splitted = re.split(self.regexLineSep, line)
          if (len(splitted) == 1):                #OpCode without comment
               line = line.split()       
          elif (len(splitted) == 4):              #Just Comment
               if (len(splitted[1]) == 0):
                    self.stats.add_comment()
                    return
               else:                              #OpCode with comment
                    line = splitted[1].split()
                    self.stats.add_comment()
          
          self.__check_opcode_types(line)

          
     
     def process_code(self):
          self.debug.log("Processing code")
          
          #check for title .IPPcode24
          for line in sys.stdin:
               splitted = re.split(r"(^\s*$)|(^\s*#.*$)|(^\s*(\.ippcode24)\s*(#.*)*$)", line.lower())
               if (len(splitted) < 7):
                    self.debug.log(f"Instead of header found: {line}")
                    exit(ExitCodes.noHeaderError.value)
               if (splitted[4] != None):
                    break
               if (splitted[3] != None):
                    self.stats.add_comment()
          else:
               self.debug.log("Header not found")
               exit(ExitCodes.noHeaderError.value)

          #process file line by line
          for line in sys.stdin:
               self.debug.log(f"Processing line:\t {line}")
               self.__line_parse(line.strip())
          
          #create XML output 
          tree = ET.ElementTree(self.etRoot)
          tree.write(sys.stdout, encoding="unicode", xml_declaration=True)

          # self.stats.count_statistics()
               