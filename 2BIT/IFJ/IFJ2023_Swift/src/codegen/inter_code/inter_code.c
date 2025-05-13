#include "inter_code.h"
#include"../codegen.h"



char *registerPrefixGen(bool isGlobal) { return isGlobal ? "GF" : "LF"; }

void convertRegisterIntToDouble(int reg, bool isGlobal) {
  printf("PUSHS %s@r%d\n",
         registerPrefixGen(isGlobal),
         reg);
  printf("CALL $Int2Double\n");
  printf("MOVE %s@r%d GF@FuncReturn\n",
         registerPrefixGen(isGlobal),
         reg);
}

void convertTokenIntToDouble(Token* a) {
  convertRegisterIntToDouble(a->data.grammarToken->reg, a->data.grammarToken->isGlobal);
}

void fitTheType(Token *a) {
  if (a->data.grammarToken->returnType == DOUBLE_TYPE)
    convertTokenIntToDouble(a);
}

void InterCodeInit() {
  printf(".IFJcode23\n");

  printf("DEFVAR GF@FuncReturn\n");
  printf("MOVE GF@FuncReturn nil@nil\n");
  printf("DEFVAR GF@__ArgCount__\n");
  printf("MOVE GF@__ArgCount__ nil@nil\n");
  printf("DEFVAR GF@__Arg__\n");
  printf("MOVE GF@__Arg__ nil@nil\n");
  printf("DEFVAR GF@__Trash__\n");
  printf("MOVE GF@__Trash__ nil@nil\n");
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");

  InitPrebuildFunc();
  // InsertPrebuildFUNCS
}

void InitPrebuildFunc() {
  SubStringIntercode();
  Chr();
  Ord();
  ReadString();
  ReadInt();
  ReadDouble();
  Int2Double();
  Double2Int();
  StrLength();
  Write();
  HardUnwrap();
  SoftUnwrap();
  $Int2Double();
}

void InterCodeEnd() {
  printf("LABEL ProgramEnd\n");
  printf("POPFRAME\n");
}
// ARG, NULL, (TokenType[]){E, ARG_TMP

void GetF(GrammarToken *grammarToken, AddressTable *addressTable) {
  fitTheType(grammarToken->tokensHolder[0]);
  grammarToken->reg = grammarToken->tokensHolder[0]->data.grammarToken->reg;
  grammarToken->isGlobal =
      grammarToken->tokensHolder[0]->data.grammarToken->isGlobal;
}

void HardUnwrap() {
  printf("JUMP $HardUnwrapEnd\n");
  printf("LABEL $HardUnwrap\n");//jump by operator name
  printf("PUSHS nil@nil\n");
  printf("JUMPIFEQS $HardUnwrapBadExit\n");
  printf("RETURN\n");
  printf("LABEL $HardUnwrapBadExit\n");
  printf("EXIT int@57\n");
  printf("LABEL $HardUnwrapEnd\n");
}

// string - arg0
// od - arg1
// do - arg2
void SubStringIntercode() {
  printf("JUMP substringIntercodeEnd\n");
  printf("LABEL substring\n");
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");
  printf("DEFVAR LF@trash\n");
  printf("POPS LF@trash\n");
  printf("DEFVAR LF@TempBool\n");
  printf("DEFVAR LF@arg0\n");
  printf("POPS LF@arg0\n");
  printf("DEFVAR LF@arg1\n");
  printf("POPS LF@arg1\n");
  printf("DEFVAR LF@arg2\n");
  printf("POPS LF@arg2\n");
  // i<0
  printf("LT LF@TempBool LF@arg1 int@0\n");
  printf("JUMPIFEQ ReturnNil LF@TempBool bool@true\n");
  // j<0
  printf("LT LF@TempBool LF@arg2 int@0\n");
  printf("JUMPIFEQ ReturnNil LF@TempBool bool@true\n");
  // i<j
  printf("GT LF@TempBool LF@arg1 LF@arg2\n");
  printf("JUMPIFEQ ReturnNil LF@TempBool bool@true\n");
  // strlen(s)
  printf("DEFVAR LF@TempInt\n");
  printf("STRLEN LF@TempInt LF@arg0\n");
  // i>=strlen(s)
  printf("DEFVAR LF@TempBool1\n");
  printf("DEFVAR LF@TempBool2\n");
  printf("GT LF@TempBool1 LF@arg1 LF@TempInt\n");
  printf("EQ LF@TempBool2 LF@arg1 LF@TempInt\n");
  printf("OR LF@TempBool LF@TempBool1 LF@TempBool2\n");
  printf("JUMPIFEQ ReturnNil LF@TempBool bool@true\n");
  // j>strlen(s)
  printf("GT LF@TempBool LF@arg2 LF@TempInt\n");
  printf("JUMPIFEQ ReturnNil LF@TempBool bool@true\n");
  //*-----*-----*-----*-----*-----*-----*-----*-----*-----*-----*-----*-----*//
  // temps creations
  printf("DEFVAR LF@TempBoolCycle\n");
  // (1*) int inc = i
  printf("DEFVAR LF@Increment\n");
  printf("MOVE LF@Increment LF@arg1\n");
  // substring and curchar def
  printf("DEFVAR LF@SubString\n");
  printf("MOVE LF@SubString string@\n");
  printf("DEFVAR LF@CurChar\n");
  // for( (1*) ; inc<=j; (2*) )
  printf("LABEL For_Head\n");
  printf("LT LF@TempBoolCycle LF@Increment LF@arg2\n");
  printf("JUMPIFEQ For LF@TempBoolCycle bool@true\n");
  //*-----*-----*-----*-----*-----*-----*-----*-----*-----*//
  // after For
  printf("MOVE GF@FuncReturn LF@SubString\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  //*-----*-----*-----*-----*-----*-----*-----*-----*-----*//
  printf("LABEL For\n");
  // code zone {
  // char c = arg0[inc];
  printf("GETCHAR LF@CurChar LF@arg0 LF@Increment\n");
  // string += c;
  printf("CONCAT LF@SubString LF@SubString LF@CurChar\n");
  // (2*)inc++
  printf("ADD LF@Increment LF@Increment int@1\n");
  // end of code zone }
  printf("JUMP For_Head\n");
  //*-----*-----*-----*-----*-----*-----*-----*-----*-----*-----*-----*-----*//
  printf("LABEL ReturnNil\n");
  printf("MOVE GF@FuncReturn nil@nil\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  printf("LABEL substringIntercodeEnd\n");
}

// void InsertPrebuildFUNCS
// create readString
// arg1

// func ord(_ 𝑐 : String) -> Int – Vrátí ordinální hodnotu (ASCII) prvního znaku
// v řetězci 𝑐. Je-li řetězec prázdný, vrací funkce 0.
void Ord() {
  printf("JUMP ordEnd\n");
  printf("LABEL ord\n");
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");

  printf("DEFVAR LF@trash\n");
  printf("POPS LF@trash\n");

  printf("DEFVAR LF@str\n");
  printf("DEFVAR LF@strlen\n");
  printf("POPS LF@str\n");
  printf("STRLEN LF@strlen LF@str\n");
  printf("JUMPIFEQ IfLinezero LF@strlen int@0\n");
  printf("STRI2INT LF@strlen LF@str int@0\n");
  printf("MOVE GF@FuncReturn LF@strlen\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  printf("LABEL IfLinezero\n");
  //*----*----*----*----*----*----*----*----*//
  printf("MOVE GF@FuncReturn int@0\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  //*----*----*----*----*----*----*----*----*//
  printf("LABEL ordEnd\n");
}

// func chr(_ 𝑖 : Int) -> String – Vrátí jednoznakový řetězec se znakem, jehož
// ASCII kód je zadán parametrem 𝑖. Hodnotu 𝑖 mimo interval[0; 255]řeší
// odpovídající instrukce IFJcode23.
void Chr() {

  printf("JUMP chrend\n");
  printf("LABEL chr\n");
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");

  printf("DEFVAR LF@trash\n");
  printf("POPS LF@trash\n");

  printf("DEFVAR LF@number\n");
  printf("DEFVAR LF@strlen\n");
  printf("MOVE LF@strlen string@a\n");
  printf("POPS LF@number\n");
  printf("INT2CHAR LF@strlen LF@number\n");
  printf("MOVE GF@FuncReturn LF@strlen\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  printf("LABEL chrend\n");
}

// func readString() -> String?
void ReadString() {
  printf("JUMP readStringEnd\n");
  printf("LABEL readString\n");
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");

  printf("DEFVAR LF@trash\n");
  printf("POPS LF@trash\n");

  printf("DEFVAR LF@ReadString\n");
  printf("READ LF@ReadString string\n");
  printf("MOVE GF@FuncReturn LF@ReadString\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  printf("LABEL readStringEnd\n");
}

// func readInt() -> Int?
void ReadInt() {
  printf("JUMP ReadIntEnd\n");
  printf("LABEL readInt\n");
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");

  printf("DEFVAR LF@trash\n");
  printf("POPS LF@trash\n");

  printf("DEFVAR LF@ReadInt\n");
  printf("READ LF@ReadInt int\n");
  printf("MOVE GF@FuncReturn LF@ReadInt\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  printf("LABEL ReadIntEnd\n");
}

// call ReadDouble
// func readDouble() -> Double?
void ReadDouble() {
  printf("JUMP readDoubleEnd\n");
  printf("LABEL readDouble\n");
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");

  printf("DEFVAR LF@trash\n");
  printf("POPS LF@trash\n");

  printf("DEFVAR LF@ReadFloat\n");
  printf("READ LF@ReadFloat float\n");
  printf("MOVE GF@FuncReturn LF@ReadFloat\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  printf("LABEL readDoubleEnd\n");
}

// func Int2Double(_ term ∶ Int) -> Double – Vrátí hodnotu celočíselného termu
// term konvertovanou na desetinné číslo. Pro konverzi z celého čísla využijte
// odpovídající instrukci z IFJcode23.
void Int2Double() {
  printf("JUMP Int2DoubleEnd\n");
  printf("LABEL Int2Double\n");
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");

  printf("DEFVAR LF@trash\n");
  printf("POPS LF@trash\n");

  printf("DEFVAR LF@IntToFloat\n");

  printf("DEFVAR LF@tempInt\n");
  printf("POPS LF@tempInt\n");
  printf("INT2FLOAT LF@IntToFloat LF@tempInt\n");
  printf("MOVE GF@FuncReturn LF@IntToFloat\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  printf("LABEL Int2DoubleEnd\n");
}

void $Int2Double() {
  printf("JUMP $Int2DoubleEnd\n");
  printf("LABEL $Int2Double\n");
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");

  printf("DEFVAR LF@IntToFloat\n");

  printf("DEFVAR LF@tempInt\n");
  printf("POPS LF@tempInt\n");
  printf("DEFVAR LF@tempString\n");
  printf("TYPE LF@tempString LF@tempInt\n");
  printf("JUMPIFEQ $Double2Double LF@tempString string@float\n");


  printf("INT2FLOAT LF@IntToFloat LF@tempInt\n");
  printf("MOVE GF@FuncReturn LF@IntToFloat\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  //if we have double already
  printf("LABEL $Double2Double\n");
  printf("MOVE GF@FuncReturn LF@tempInt\n");
  printf("POPFRAME\n");
  printf("RETURN\n");

  printf("LABEL $Int2DoubleEnd\n");
}

// func Double2Int(_ term ∶ Double) -> Int – Vrátí hodnotu desetinného termu
// term konvertovanou na celé číslo, a to oříznutím desetinné části. Pro
// konverzi z desetinného čísla využijte odpovídající instrukci z IFJcode23.
void Double2Int() {
  printf("JUMP Double2IntEnd\n");
  printf("LABEL Double2Int\n");
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");

  printf("DEFVAR LF@trash\n");
  printf("POPS LF@trash\n");

  printf("DEFVAR LF@FloatToInt\n");

  printf("DEFVAR LF@tempDouble\n");
  printf("POPS LF@tempDouble\n");
  printf("FLOAT2INT LF@FloatToInt LF@tempDouble\n");
  printf("MOVE GF@FuncReturn LF@FloatToInt\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  printf("LABEL Double2IntEnd\n");
}

// func length(_ 𝑠 : String) -> Int – Vrátí délku (počet znaků) řetězce 𝑠. Např.
// length("x\nz") vrací 3.
void StrLength() {
  printf("JUMP lengthEnd\n");
  printf("LABEL length\n");
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");

  printf("DEFVAR LF@trash\n");
  printf("POPS LF@trash\n");

  printf("DEFVAR LF@tempString\n");
  printf("DEFVAR LF@StringLegnth\n");
  printf("POPS LF@tempString\n");
  printf("STRLEN LF@StringLegnth LF@tempString\n");
  printf("MOVE GF@FuncReturn  LF@StringLegnth\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  printf("LABEL lengthEnd\n");
}

// op1 = int to store
// res = register to store
void StoreInt(GrammarToken *grammarToken, AddressTable *addressTable) {
  // printf("DEFVAR LF@r%d\n", get_reg_new(addressTable));
  printf("MOVE %s@r%d int@%lld\n", registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg, grammarToken->tokensHolder[0]->data.integer_value);
  //  grammarToken->reg = get_reg_cur(addressTable);
  //  grammarToken->isGlobal = false;
}

void StoreDouble(GrammarToken *grammarToken, AddressTable *addressTable) {
  // printf("DEFVAR LF@r%d\n", get_reg_new(addressTable));
  printf("MOVE %s@r%d float@%a\n", registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg, grammarToken->tokensHolder[0]->data.double_value);
}

void StoreBrackets(GrammarToken *grammarToken, AddressTable *addressTable) {
  //  grammarToken->reg = grammarToken->tokensHolder[1]->data.grammarToken->reg;
  //  grammarToken->isGlobal =
  //      grammarToken->tokensHolder[1]->data.grammarToken->isGlobal;
  fitTheType(grammarToken->tokensHolder[1]);
  printf("MOVE %s@r%d %s@r%d\n", registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[1]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[1]->data.grammarToken->reg);
}

void StoreBool(GrammarToken *grammarToken, AddressTable *addressTable) {
  // printf("DEFVAR LF@r%d\n", get_reg_new(addressTable));
  if (grammarToken->tokensHolder[0]->data.integer_value) {
    printf("MOVE %s@r%d bool@true\n", registerPrefixGen(grammarToken->isGlobal),
           grammarToken->reg);
  } else {
    printf("MOVE %s@r%d bool@false\n",
           registerPrefixGen(grammarToken->isGlobal), grammarToken->reg);
  }
}

void StoreNil(GrammarToken *grammarToken, AddressTable *addressTable) {
  // printf("DEFVAR LF@r%d\n", get_reg_new(addressTable));
  printf("MOVE %s@r%d nil@nil\n", registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg);
}

// typedef struct string {
//   size_t length;
//   char *data;
// } String;
void StoreString(GrammarToken *grammarToken, AddressTable *addressTable) {
  // printf("DEFVAR LF@r%d\n", get_reg_new(addressTable));
  //   if (grammarToken->tokensHolder[0]->data.string.length == 0) {
  //     printf("MOVE LF@r%d nil@nil\n", get_reg_cur(addressTable));//TODO wtf
  //     empty string
  //   } else {
  //     printf("MOVE LF@r%d string@\n",get_reg_cur(addressTable));
  //     printf("DEFVAR TF@temp\n");
  //     printf("DEFVAR TF@tempSymb\n");
  //     for (int i = 0; i < grammarToken->tokensHolder[0]->data.string.length;
  //     i++) {
  //       if (isalpha(grammarToken->tokensHolder[0]->data.string.data[i])) {
  //         printf("MOVE TF@temp string@%c\n",
  //         grammarToken->tokensHolder[0]->data.string.data[i]); printf("CONCAT
  //         LF@r%d LF@r%d
  //         TF@temp\n",get_reg_cur(addressTable),get_reg_cur(addressTable));
  //       } else {
  //         printf("MOVE TF@temp string@%c\n",
  //         grammarToken->tokensHolder[0]->data.string.data[i]);
  //         printf("STRI2INT TF@tempSymb TF@temp int@0\n");
  //         printf("CONCAT LF@r%d LF@r%d
  //         string@\\\n",get_reg_cur(addressTable),get_reg_cur(addressTable));
  //         printf("CONCAT LF@r%d LF@r%d
  //         TF@tempSymb\n",get_reg_cur(addressTable),get_reg_cur(addressTable));
  //       }
  //     }
  //   } GUYS WTF what this all means????
  printf("MOVE %s@r%d string@", registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg);
  char *str = grammarToken->tokensHolder[0]->data.string.data;
  int sz = strlen(str);
  for (int i = 0; i < sz; i++) {
    printf("\\%03d", (int)str[i]);
  }
  printf("\n");
  // grammarToken->reg = get_reg_cur(addressTable);
}

void OrInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  // printf("DEFVAR LF@r%d\n", res);
  printf("OR %s@r%d %s@r%d %s@r%d\n", registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
}

void AndInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  // int res = get_reg_new(addressTable);
  // printf("DEFVAR LF@r%d\n", res);
  printf("AND %s@r%d %s@r%d %s@r%d\n",
         registerPrefixGen(grammarToken->isGlobal), grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
  // grammarToken->reg = get_reg_cur(addressTable);
}

void NotInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  // int res = get_reg_new(addressTable);
  // printf("DEFVAR LF@r%d\n", res);
  printf("NOT %s@r%d %s@r%d\n", registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg);
  // grammarToken->reg = get_reg_cur(addressTable);
}

//{E, PLUS, E}
// E -> {F}
// GrammarToken -> {TOKEN, TOKEN, TOKEN, ...}
// TOKEN -> GrammarToken
void SumInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  // int res = get_reg_new(addressTable);
  if (grammarToken->returnType ==
      STRING) { // TODO: ID(contains string) + string doesnt work
    // printf("DEFVAR LF@r%d\n", res);
    printf("CONCAT %s@r%d %s@r%d %s@r%d\n",
           registerPrefixGen(grammarToken->isGlobal), grammarToken->reg,
           registerPrefixGen(
               grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
           grammarToken->tokensHolder[0]->data.grammarToken->reg,
           registerPrefixGen(
               grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
           grammarToken->tokensHolder[2]->data.grammarToken->reg);
  } else {
    fitTheType(grammarToken->tokensHolder[0]);
    fitTheType(grammarToken->tokensHolder[2]);
    printf("ADD %s@r%d %s@r%d %s@r%d\n",
           registerPrefixGen(grammarToken->isGlobal), grammarToken->reg,
           registerPrefixGen(
               grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
           grammarToken->tokensHolder[0]->data.grammarToken->reg,
           registerPrefixGen(
               grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
           grammarToken->tokensHolder[2]->data.grammarToken->reg);
  }
  //  grammarToken->reg = get_reg_cur(addressTable);
  //  grammarToken->isGlobal = false;
}

void SubInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  fitTheType(grammarToken->tokensHolder[0]);
  fitTheType(grammarToken->tokensHolder[2]);
  printf("SUB %s@r%d %s@r%d %s@r%d\n",
         registerPrefixGen(grammarToken->isGlobal), grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
  // grammarToken->reg = get_reg_cur(addressTable);
}

void MulInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  fitTheType(grammarToken->tokensHolder[0]);
  fitTheType(grammarToken->tokensHolder[2]);
  printf("MUL %s@r%d %s@r%d %s@r%d\n",
         registerPrefixGen(grammarToken->isGlobal), grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
  // grammarToken->reg = get_reg_cur(addressTable);
}

void DivInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
//  printf("JUMPIFNEQ CheckDivByZero %s@r%d int@0\n", registerPrefixGen(grammarToken->isGlobal), grammarToken->tokensHolder[2]->data.grammarToken->reg);
//  printf("EXIT int@57\n");
//  printf("LABEL CheckDivByZero\n");
  fitTheType(grammarToken->tokensHolder[0]);
  fitTheType(grammarToken->tokensHolder[2]);
  printf("DEFVAR LF@tempString1\n");
  printf("TYPE LF@tempString1 %s@r%d\n", registerPrefixGen(grammarToken->isGlobal), grammarToken->tokensHolder[0]->data.grammarToken->reg);
  printf("DEFVAR LF@tempString2\n");
  printf("TYPE LF@tempString2 %s@r%d\n", registerPrefixGen(grammarToken->isGlobal), grammarToken->tokensHolder[2]->data.grammarToken->reg);

  printf("JUMPIFEQ Div1IsInt LF@tempString1 string@int\n");
  printf("JUMPIFEQ IDiv1IsFloat LF@tempString1 string@float\n");

  printf("JUMPIFEQ ENDDIV LF@tempString1 string@string\n");


  printf("LABEL Div1IsInt\n");
  printf("JUMPIFEQ DivIsInt LF@tempString2 string@int\n");
  printf("EXIT int@4\n");

  printf("LABEL IDiv1IsFloat\n");
  printf("JUMPIFEQ IDivIsFloat LF@tempString2 string@float\n");
  printf("EXIT int@4\n");


  printf("LABEL IDivIsFloat\n");
  printf("DIV %s@r%d %s@r%d %s@r%d\n",
         registerPrefixGen(grammarToken->isGlobal), grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
  printf("JUMP ENDDIV\n");

  printf("LABEL DivIsInt\n");
  printf("IDIV %s@r%d %s@r%d %s@r%d\n",
         registerPrefixGen(grammarToken->isGlobal), grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
  printf("JUMP ENDDIV\n");

  printf("LABEL ENDDIV\n");
}

void EqualInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  fitTheType(grammarToken->tokensHolder[0]);
  fitTheType(grammarToken->tokensHolder[2]);
  printf("EQ %s@r%d %s@r%d %s@r%d\n", registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
}

void NotEqualInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  fitTheType(grammarToken->tokensHolder[0]);
  fitTheType(grammarToken->tokensHolder[2]);
  printf("JUMPIFNEQ NotEqual %s@r%d %s@r%d\n",
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
  printf("MOVE %s@r%d bool@false\n", registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg);
  //  printf("POPFRAME\n");
  printf("JUMP NotEqualInterCodeEnd\n");

  printf("LABEL NotEqual\n");
  printf("MOVE %s@r%d bool@true\n", registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg);
  //  printf("POPFRAME\n");
  printf("LABEL NotEqualInterCodeEnd\n");
}

void GreaterInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  fitTheType(grammarToken->tokensHolder[0]);
  fitTheType(grammarToken->tokensHolder[2]);
  printf("GT %s@r%d %s@r%d %s@r%d\n", registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
}

void LessInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  fitTheType(grammarToken->tokensHolder[0]);
  fitTheType(grammarToken->tokensHolder[2]);
  printf("LT %s@r%d %s@r%d %s@r%d\n", registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
}

void GreaterEqualInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  fitTheType(grammarToken->tokensHolder[0]);
  fitTheType(grammarToken->tokensHolder[2]);
  printf("PUSHS %s@r%d\n",
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg);

  printf("PUSHS %s@r%d\n",
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
  printf("GTS\n");
  printf("PUSHS %s@r%d\n",
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg);
  printf("PUSHS %s@r%d\n",
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
  printf("EQS\n");
  printf("ORS\n");

  printf("POPS %s@r%d\n",registerPrefixGen(grammarToken->isGlobal),grammarToken->reg);
}

void LessEqualInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  fitTheType(grammarToken->tokensHolder[0]);
  fitTheType(grammarToken->tokensHolder[2]);
  printf("PUSHS %s@r%d\n",
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg);
  printf("PUSHS %s@r%d\n",
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
  printf("LTS\n");
  printf("PUSHS %s@r%d\n",
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg);
  printf("PUSHS %s@r%d\n",
         registerPrefixGen(
             grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);
  printf("EQS\n");
  printf("ORS\n");

  printf("POPS %s@r%d\n",registerPrefixGen(grammarToken->isGlobal),grammarToken->reg);
}

void SoftUnwrap() {
    printf("JUMP $SoftUnwrapEnd\n");
    printf("LABEL $??\n");//jump by operator name
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@trashcan\n");


    printf("PUSHS nil@nil\n");
    printf("JUMPIFEQS $SoftUnwrapBadExit\n");
    printf("POPS GF@FuncReturn\n");
    printf("POPS LF@trashcan\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $SoftUnwrapBadExit\n");
    printf("POPS LF@trashcan\n");
    printf("POPS GF@FuncReturn\n");


    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $SoftUnwrapEnd\n");
}
void SoftUnwrapInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
    fitTheType(grammarToken->tokensHolder[0]);
    fitTheType(grammarToken->tokensHolder[2]);
    printf("PUSHS %s@r%d\n",registerPrefixGen(
                               grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
                               grammarToken->tokensHolder[2]->data.grammarToken->reg);
    printf("PUSHS %s@r%d\n",registerPrefixGen(
                               grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
                               grammarToken->tokensHolder[0]->data.grammarToken->reg);
    printf("PUSHS %s@r%d\n",registerPrefixGen(
                                 grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
                                 grammarToken->tokensHolder[0]->data.grammarToken->reg);
    printf("CALL $??\n");
    printf("MOVE %s@r%d GF@FuncReturn\n",
           registerPrefixGen(grammarToken->isGlobal),
           grammarToken->reg);
}

void Write() {
  printf("JUMP EscapeWrite\n");
  printf("LABEL write\n");
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");
  printf("POPS GF@__ArgCount__\n");
  printf("JUMPIFEQ ReturnWrite GF@__ArgCount__ int@0\n"); // if argCount == 0
                                                          // jump label return
  printf("LABEL ForWrite\n");
  printf("SUB GF@__ArgCount__ GF@__ArgCount__ int@1\n");
  printf("POPS GF@__Arg__\n");
  printf("WRITE GF@__Arg__\n"); // write pop
  printf("JUMPIFNEQ ForWrite GF@__ArgCount__ int@0\n");
  printf("POPFRAME\n");

  // label return
  printf("LABEL ReturnWrite\n");
  printf("RETURN\n");
  printf("LABEL EscapeWrite\n");
}

void FuncInitialize(GrammarToken *grammarToken, AddressTable *addressTable) {
  init_function(addressTable, &grammarToken->tokensHolder[1]->data.string);
  printf("JUMP Escape%s\n", grammarToken->tokensHolder[1]->data.string.data);
  printf("LABEL %s\n", grammarToken->tokensHolder[1]->data.string.data);
  printf("POPS GF@__Trash__\n");
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");
  enumerate_grammar_tokens(grammarToken->tokensHolder[3]->
                           data.grammarToken, "LF", &addressTable->resRegisters);
  enumerate_grammar_tokens(grammarToken->tokensHolder[6]->
                           data.grammarToken->tokensHolder[1]->
                           data.grammarToken, "LF", &addressTable->resRegisters);
}

void FuncInitParams(GrammarToken *grammarToken, AddressTable *addressTable) {
  bool isGlobal = false;

  AT_create_withReg(addressTable,
                    &grammarToken->tokensHolder[1]->data.string,
                    grammarToken->reg,
                    &isGlobal);
  printf("POPS %s@r%d\n",
         registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg);
}

void FuncInitializeEscape(GrammarToken *grammarToken,
                          AddressTable *addressTable) {
  printf("POPFRAME\n");
  printf("RETURN\n");
  printf("LABEL Escape%s\n", grammarToken->tokensHolder[1]->data.string.data);
  end_function(addressTable);
}

void FuncDivideScope(GrammarToken *grammarToken, AddressTable *addressTable) {
  create_hashmap_inVector(addressTable);
}

// grammar_rule_create(F, NULL, NULL, NULL, (TokenType[]){ID, F_CALL}, 2),
void FuncCall(GrammarToken *grammarToken, AddressTable *addressTable) {
  if (grammarToken->tokensHolder[1]->data.grammarToken->tokensHolderSize == 3) {//we found exactly function
    printf("CALL %s\n", grammarToken->tokensHolder[0]->data.string.data);
    printf("MOVE %s@r%d GF@FuncReturn\n",
           registerPrefixGen(grammarToken->isGlobal),
           grammarToken->reg);
  } else {
    bool isGlobal = false;
    int reg = AT_get(addressTable, &grammarToken->tokensHolder[0]->data.string,
                     &isGlobal);
    if (reg == -1)
      exit(3);
//    isGlobal = addressTable->isGlobal[reg];

    if (grammarToken->tokensHolder[1]->data.grammarToken->tokensHolderSize == 0) //we found variable
    {
      printf("MOVE %s@r%d %s@r%d\n", registerPrefixGen(grammarToken->isGlobal),
             grammarToken->reg, registerPrefixGen(isGlobal), reg);
    }

//    if (grammarToken->tokensHolder[1]->data.grammarToken->tokensHolderSize == 1) //we found operator call
//    {
//      printf("PUSHS %s@r%d\n",
//             registerPrefixGen(isGlobal),
//             reg
//      );
//      printf("CALL $%s\n",
//             grammarToken->tokensHolder[1]
//                 ->data.grammarToken->tokensHolder[0]->data.string.data);
//      printf("MOVE %s@r%d %s@r%d\n",
//             registerPrefixGen(grammarToken->isGlobal),
//             grammarToken->reg,
//             registerPrefixGen(isGlobal),
//             reg);
//    }

  }
}

void PushArg_ARGS(GrammarToken *grammarToken, AddressTable *addressTable) {
  add_arg(addressTable);
  printf("PUSHS %s@r%d\n",
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg);
}

void PushArg_TMP(GrammarToken *grammarToken, AddressTable *addressTable) {
  add_arg(addressTable);
  printf("PUSHS %s@r%d\n",
         registerPrefixGen(
             grammarToken->tokensHolder[1]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[1]->data.grammarToken->reg);
}

void ADDArg(GrammarToken *grammarToken, AddressTable *addressTable) {
  grammarToken->reg = grammarToken->tokensHolder[0]->data.grammarToken->reg;
  grammarToken->isGlobal = grammarToken->tokensHolder[0]->data.grammarToken->isGlobal;
}

void ADDArgLabeled(GrammarToken *grammarToken, AddressTable *addressTable) {
  grammarToken->reg = grammarToken->tokensHolder[1]->data.grammarToken->reg;
  grammarToken->isGlobal = grammarToken->tokensHolder[1]->data.grammarToken->isGlobal;
}

void FuncArgAdd(GrammarToken *grammarToken, AddressTable *addressTable) {
  int argCount = get_args(addressTable);
//  printf("CREATEFRAME\n");
//  printf("PUSHFRAME\n");
//  for (int i = 0; i < argCount; i++) {
//    printf("DEFVAR LF@tempArg%d\n", i); // 5, 6
//    printf("POPS LF@tempArg%d\n", i);
//  }
//
//  for (int i = 0; i < argCount; i++) {
//    printf("PUSHS LF@tempArg%d\n", i);
//  }

  printf("MOVE GF@__ArgCount__ int@%d\n", argCount);
  printf("PUSHS GF@__ArgCount__\n");
//  printf("POPFRAME\n");
}

// void HardUnwrapInterCode(GrammarToken *grammarToken, AddressTable
// *addressTable) {
//   if (addressCode->op1 == -1) {
//     printf("DEFVAR LF@r%d\n",addressCode->result);
//     printf("MOVE LF@r%d LF@r%d\n",addressCode->result,addressCode->op2);
//   }
// }

void HardUnwrapInterCode(GrammarToken *grammarToken,
                         AddressTable *addressTable) {
  printf("PUSHS %s@r%d\n",
         registerPrefixGen(grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg
  );
  printf("CALL $HardUnwrap\n");
  printf("MOVE %s@r%d %s@r%d\n",
         registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg,
         registerPrefixGen(grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[0]->data.grammarToken->reg);

  // printf("DEFVAR LF@tempReg\n");
  // printf("MOVE LF@tempReg TF@r%d\n",
  // grammarToken->tokensHolder[0]->data.grammarToken->reg);
//  printf("JUMPIFEQ ExitIfZero %s@r%d nil@nil\n",
//         registerPrefixGen(
//             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
//         grammarToken->tokensHolder[0]->data.grammarToken->reg);
//  printf("JUMP EscapeHardUnwrap\n");
//  printf("LABEL ExitIfZero\n");
//  printf("EXIT int@57\n");
//  printf("LABEL EscapeHardUnwrap\n");
}

void WhileInitInterCode(GrammarToken *grammarToken,
                        AddressTable *addressTable) {
  int num = init_cycle(addressTable);
  printf("LABEL cycle%d\n", num);
  //    printf("CREATEFRAME\n"
  //           "PUSHFRAME\n");
}

void CondInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  // printf("POPFRAME\n");
  printf("JUMPIFEQ BLOCK%d %s@r%d bool@true\n", get_cur_cycle(addressTable),
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         //          "LF",
         grammarToken->tokensHolder[0]->data.grammarToken->reg);
  printf("JUMPIFEQ ESCAPE%d %s@r%d bool@false\n", get_cur_cycle(addressTable),
         registerPrefixGen(
             grammarToken->tokensHolder[0]->data.grammarToken->isGlobal),
         //          "LF",
         grammarToken->tokensHolder[0]->data.grammarToken->reg);
  printf("LABEL BLOCK%d\n", get_cur_cycle(addressTable));
}

void BlockWhileInterCode(GrammarToken *grammarToken,
                         AddressTable *addressTable) {
  printf("JUMP cycle%d\n", get_cur_cycle(addressTable));
  printf("LABEL ESCAPE%d\n", get_cur_cycle(addressTable));
  end_cycle(addressTable);
}

void BreakInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  printf("JUMP ESCAPE%d\n", get_cur_cycle(addressTable));
}

void ContinueInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  printf("JUMP cycle%d\n", get_cur_cycle(addressTable));
}

void ReturnInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  if (grammarToken->tokensHolder[1]->data.grammarToken->tokensHolderSize != 0)
    printf("MOVE GF@FuncReturn %s@r%d\n",
         registerPrefixGen(grammarToken->tokensHolder[1]->data.grammarToken->isGlobal),
           grammarToken->tokensHolder[1]->data.grammarToken->tokensHolder[0]->data.grammarToken->reg);
  printf("POPFRAME\n");
  printf("RETURN\n");
}

void PreOrderForIf(GrammarToken *grammarToken, AddressTable *addressTable) {
  init_cycle(addressTable);
}

void PreOrderForIfEslse(GrammarToken *grammarToken,
                        AddressTable *addressTable) {
  printf("JUMP EXITIF%d\n", get_cur_cycle(addressTable));
  printf("LABEL ESCAPE%d\n", get_cur_cycle(addressTable));
}

void PostOrderForIf(GrammarToken *grammarToken, AddressTable *addressTable) {
  printf("LABEL EXITIF%d\n", get_cur_cycle(addressTable));
  end_cycle(addressTable);
}

void UnwrapCond(GrammarToken *grammarToken, AddressTable *addressTable) {
  bool isGlobal = false;
  int reg = AT_get(addressTable, &grammarToken->tokensHolder[1]->data.string,
                   &isGlobal);
  if (reg == -1)
    exit(3);
//  isGlobal = addressTable->isGlobal[reg];

  int exitReg = get_temp_label(addressTable);
  int escReg = get_temp_label(addressTable);
  printf("JUMPIFEQ ExitIfZero%d %s@r%d nil@nil\n",
         exitReg,
         registerPrefixGen(isGlobal),
         reg);
  printf("JUMP EscapeHardUnwrap%d\n", escReg);
  printf("LABEL ExitIfZero%d\n", exitReg);
  printf("JUMP ESCAPE%d\n", get_cur_cycle(addressTable));
  printf("LABEL EscapeHardUnwrap%d\n", escReg);
}

void VarIdInit(GrammarToken *grammarToken, AddressTable *addressTable) {
  bool isGlobal = false;
  int reg = AT_create_withReg(addressTable,
                              &grammarToken->tokensHolder[0]->data.string,
                              grammarToken->reg, &isGlobal);

  if (reg == -1)
    exit(3);
//  addressTable->isGlobal[reg] = grammarToken->isGlobal;

  printf("MOVE %s@r%d %s@r%d\n", registerPrefixGen(grammarToken->isGlobal), reg,
         registerPrefixGen(
             grammarToken->tokensHolder[1]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[1]->data.grammarToken->reg);
  // grammarToken->reg = get_reg_cur(addressTable);
}

void VarTypedIdInit(GrammarToken *grammarToken, AddressTable *addressTable) {
  bool isGlobal = false;
  int reg = AT_create_withReg(addressTable,
                              &grammarToken->tokensHolder[0]->data.string,
                              grammarToken->reg, &isGlobal);
  if (reg == -1)
    exit(3);
//  addressTable->isGlobal[reg] = grammarToken->isGlobal;

  //for case var a: Int
  if (grammarToken->tokensHolder[2]->data.grammarToken->tokensHolderSize == 0) {
    printf("MOVE %s@r%d nil@nil\n",
           registerPrefixGen(grammarToken->isGlobal),
           reg);
    return;
  }

//  fitTheType();
  if (grammarToken->tokensHolder[1]->data.grammarToken->tokensHolderSize != 0 &&
      grammarToken->tokensHolder[1]->data.grammarToken->tokensHolder[0]->type == DOUBLE_TYPE ||
      grammarToken->tokensHolder[1]->data.grammarToken->tokensHolder[0]->type == DOUBLE_NULLABLE_TYPE) {
      printf("TYPE GF@__Trash__ %s@r%d\n",
             registerPrefixGen(grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
             grammarToken->tokensHolder[2]->data.grammarToken->reg);
      printf("JUMPIFEQ $ESCAPETYPE%d GF@__Trash__ string@float\n", get_temp_label(addressTable));
      printf("JUMPIFEQ $ESCAPETYPE%d GF@__Trash__ string@nil\n", get_temp_label_cur(addressTable));
      printf("INT2FLOAT %s@r%d %s@r%d\n",
           registerPrefixGen(grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
           grammarToken->tokensHolder[2]->data.grammarToken->reg,
               registerPrefixGen(grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
           grammarToken->tokensHolder[2]->data.grammarToken->reg);
      printf("LABEL $ESCAPETYPE%d\n", get_temp_label_cur(addressTable));
  }
  printf("MOVE %s@r%d %s@r%d\n",
         registerPrefixGen(grammarToken->isGlobal),
         reg,
         registerPrefixGen(grammarToken->tokensHolder[2]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[2]->data.grammarToken->reg);

//  if (grammarToken->tokensHolder[2]->data.grammarToken->grammarRule->resultTokenType) {
//    printf("INT2FLOAT %s@%d %s@%d")
//  }
  // grammarToken->reg = get_reg_cur(addressTable);
}

void IdInitAssignPropagate(GrammarToken *grammarToken,
                           AddressTable *addressTable) {
  printf("MOVE %s@r%d %s@r%d\n",
         registerPrefixGen(grammarToken->isGlobal),
         grammarToken->reg,
         registerPrefixGen(
             grammarToken->tokensHolder[1]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[1]->data.grammarToken->reg);
}

    void InitProcess(GrammarToken *grammarToken, AddressTable *addressTable) {
  grammarToken->reg = grammarToken->tokensHolder[1]->data.grammarToken->reg;
}

void IdAssignInterCode(GrammarToken *grammarToken, AddressTable *addressTable) {
  bool isGlobal = false;
  int reg = AT_get(addressTable, &(grammarToken->tokensHolder[0]->data.string),
                   &isGlobal);
  if (reg == -1)
    exit(3);
//  isGlobal = addressTable->isGlobal[reg];

  printf("MOVE %s@r%d %s@r%d\n", registerPrefixGen(isGlobal), reg,
         registerPrefixGen(
             grammarToken->tokensHolder[1]->data.grammarToken->isGlobal),
         grammarToken->tokensHolder[1]->data.grammarToken->reg);
}

void InterpolationInterCode(GrammarToken *grammarToken, AddressTable *addressTable,int reg,
                                                        char *leftSide,char *rightSide){

  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");
  printf("DEFVAR LF@str\n");

  printf("DEFVAR LF@strLeft\n");
  printf("MOVE LF@strLeft string@%s\n", leftSide);

  printf("DEFVAR LF@typeReg\n");
  printf("TYPE LF@typeReg LF@%d\n", reg);
  printf("JUMPIFEQ RegIsInt LF@typeReg int@int\n");
  printf("JUMPIFEQ RegIsFloat LF@typeReg float@float\n");
  printf("JUMPIFEQ RegIsString LF@type string@string\n");

  printf("LABEL RegIsInt\n");
  printf("DEFVAR LF@int\n");
  printf("MOVE LF@int LF@%d\n", reg);
  printf("DEFVAR LF@strMid\n");
  printf("INT2CHAR LF@strMid LF@int\n");
  printf("JUMP END\n");

  printf("LABEL RegIsFloat\n");
  printf("DEFVAR LF@int\n");
  printf("MOVE LF@int LF@%d\n", reg);
  printf("DEFVAR LF@floatToCharResult\n");
  printf("FLOAT2INT LF@floatToCharResult LF@int\n");
  printf("INT2CHAR LF@strMid LF@floatToCharResult\n");
  printf("JUMP END\n");

  printf("LABEL RegIsString\n");
  printf("MOVE LF@strMid LF@%d\n", reg);
  printf("JUMP END\n");

  printf("LABEL END\n");
  printf("DEFVAR LF@strRight\n");
  printf("MOVE LF@strRight string@%s\n", rightSide);

  printf("DEFVAR LF@result\n");
  printf("CONCAT LF@result LF@strLeft LF@strMid\n");
  printf("CONCAT LF@result LF@result LF@strRight\n");
  printf("MOVE GF@ReturnFunction LF@result\n");

  printf("POPFRAME\n");

}

void Interpolation(GrammarToken *grammarToken, AddressTable *addressTable){
  char *c = grammarToken->tokensHolder[0]->data.string.data;
  for (int i = 0; i < grammarToken->tokensHolder[0]->data.string.length; i++) {
    if (c[i] == '\\') {
      int FirstSymbPos = 0;
      int LastSymbPos = 0;
      for (int j = i; j < grammarToken->tokensHolder[0]->data.string.length; j++) {
        if (c[j] == '(') {
          FirstSymbPos = j+1;
        }
        if (c[j] == ')') {
          LastSymbPos = j-1;
        }
      }
      int ResLength = LastSymbPos - FirstSymbPos;
      char *result = malloc(ResLength * sizeof(char));
      strncpy(result,&c[FirstSymbPos],ResLength);
      bool isGlobal = false;
      String string =  string_create(result);
      int reg = AT_get(addressTable, &string, &isGlobal);
      char *leftSide = malloc((FirstSymbPos-2)*sizeof(char));
      strncpy(leftSide,&c[0],(FirstSymbPos-2)/*voprosik*/);
      char *rightSide = malloc((grammarToken->tokensHolder[0]->data.string.length - (LastSymbPos+1)) *sizeof(char));
      strncpy(rightSide,&c[LastSymbPos+1]/*voprosik*/,(grammarToken->tokensHolder[0]->data.string.length - (LastSymbPos+1))/*voprosik*/);

      InterpolationInterCode(grammarToken,addressTable,reg,leftSide,rightSide);
      free(leftSide);
      free(rightSide);
      free(result);
    } else
      return;
  }
}
void StsCreateFrame(GrammarToken *grammarToken, AddressTable *addressTable) {

  //  char str[100];
  //  int res = get_reg_new(addressTable);
  //  sprintf(str, "$WHILE%d$", res);
  //  String string = string_create(str);
  //  init_function(addressTable, &string);
}

void StsPopFrame(GrammarToken *grammarToken, AddressTable *addressTable) {
  // end_function(addressTable);
  // printf("POPFRAME\n");
}