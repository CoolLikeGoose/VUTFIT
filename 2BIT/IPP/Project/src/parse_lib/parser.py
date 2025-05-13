import argparse
import sys
import re

from .debug import *
from .instructions import Instruction
from .instructions import Types
from .xml_builder import XML_builder

class Parser:
     def __init__(self):

          #Pre-compiled paterns for optimization(indices relative to Types Enum)
          self.regexPatterns = [
               #label
               #(?!int$|bool$|string$|nil$) add if label cannot be a name of type
               re.compile(r"^(([A-Za-z_\-$&%*!?])[A-Za-z0-9_\-$&%*!?]*)$"),     

               #add (?!int$|bool$|string$|nil$) before @ if you cannot use name of type
               #var
               re.compile(r"^(GF|LF|TF)@(([A-Za-z_\-$&%*!?])[A-Za-z0-9_\-$&%*!?]*)$"),  
               #symb   
               re.compile(r"(^(GF|LF|TF)@(([A-Za-z_\-$&%*!?])[A-Za-z0-9_\-$&%*!?]*)$)|(^(int@((\-?\+?0x[0-9A-Fa-f]+)|(\-?\+?0o[0-7]+)|(\-?\+?[0-9]+)))|(string@.*)|(bool@(true|false))|(nil@nil))"),    
               #type 
               re.compile(r"^(int|string|bool|nil)$")      
          ]
          #Regex for detecting code & comments in the line
          self.regexLineSep = re.compile(r"(.*?)#(.*)")
          self.regexVarCheck = re.compile(r"^(GF|LF|TF)@.*")
          self.stringComp = re.compile(r"\\[0-9]{3}")

          self.__argument_parse()
          self.debug = Debug(self.args.verbose)
          self.xml_builder = XML_builder()

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


     def __detect_data_types(self, arg, type):
          if (type != Types.symb):
               return [type.name, arg]
          if (re.fullmatch(self.regexVarCheck, arg) != None):
               return ["var", arg]
          
          #check for valid string
          arg_symb = arg.split("@", 1)
          if (arg_symb[0].lower() == "string"):
               if (arg_symb[1].count("\\") != len(re.findall(self.stringComp, arg_symb[1]))):
                    exit(ExitCodes.sourceCodeError.value)
          return arg_symb
               

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

          self.xml_builder.add_instruction(instruction[0], args)


     def __line_parse(self, line):
          if (len(line) == 0):
               return
          
          splitted = re.split(self.regexLineSep, line)
          if (len(splitted) == 1):                #OpCode without comment
               line = line.split()       
          elif (len(splitted) == 4):              
               if (len(splitted[1]) == 0):        #Just Comment - skip
                    return
               else:                              #OpCode with comment
                    line = splitted[1].split()
          
          self.__check_opcode_types(line)


     def __find_header(self):
          for line in sys.stdin:
               #matches header | header with comment | commnet
               splitted = re.split(r"(^\s*$)|(^\s*#.*$)|(^\s*(\.ippcode24)\s*(#.*)*$)", line.lower())
               if (len(splitted) < 7):
                    self.debug.log(f"Instead of header found: {line}")
                    exit(ExitCodes.noHeaderError.value)
               if (splitted[4] != None):
                    break
          else:
               self.debug.log("Header not found")
               exit(ExitCodes.noHeaderError.value)
          

     def process_code(self):
          self.debug.log("Processing code")

          self.__find_header()

          #process file line by line
          for line in sys.stdin:
               self.debug.log(f"Processing line:\t {line}")
               self.__line_parse(line.strip())
          
          #create XML output 
          self.xml_builder.print_xml_output()
               