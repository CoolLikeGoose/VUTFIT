import xml.etree.ElementTree as ET
import sys

class XML_builder():
    def __init__(self):
        self.order = 1

        self.etRoot = ET.Element("program")
        self.etRoot.set("language", "IPPcode24")


    def add_instruction(self, OpCode, args):
        instr = ET.SubElement(self.etRoot, "instruction")
        instr.set("order", str(self.order))
        instr.set("opcode", OpCode.upper())
        self.order += 1

        argc = 1
        for arg in args:
             argElem = ET.SubElement(instr, f"arg{argc}")
             argElem.set("type", arg[0])
             argElem.text = arg[1]
             argc += 1
    

    def print_xml_output(self):
        tree = ET.ElementTree(self.etRoot)
        tree.write(sys.stdout, encoding="unicode", xml_declaration=True)