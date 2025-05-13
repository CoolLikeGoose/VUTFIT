#ifndef IFJ2023_SWIFT_INTER_CODE_H
#define IFJ2023_SWIFT_INTER_CODE_H
#include "../../model/grammar/rule/grammar_rule.h"
#include "../../model/grammar/token/grammar_token.h"
#include "ctype.h"
#include "stdbool.h"
#include <stdio.h>
#include "string.h"

void InterCodeInit();
void InterCodeEnd();
void InitPrebuildFunc();
void StoreString(GrammarToken *grammarToken, AddressTable *addressTable);
void StoreInt(GrammarToken *grammarToken, AddressTable *addressTable);
void StoreDouble(GrammarToken *grammarToken, AddressTable *addressTable);
void StoreBrackets(GrammarToken *grammarToken, AddressTable *addressTable);
void ReadDouble();
void ReadString();
void ReadInt();
void StoreBool(GrammarToken *grammarToken, AddressTable *addressTable);
void SumInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void SubInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void MulInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void DivInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void EqualInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void NotEqualInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void GreaterInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void LessInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void GreaterEqualInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void LessEqualInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void SoftUnwrapInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void HardUnwrapInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void OrInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void AndInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void NotInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void HardUnwrap();
void Int2Double();
void Double2Int();
void StrLength();
void SubStringIntercode();
void Ord();
void Chr();
void Write();
void SoftUnwrap();
void WhileInitInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void CondInterCode (GrammarToken *grammarToken, AddressTable *addressTable);
void BlockWhileInterCode (GrammarToken *grammarToken, AddressTable *addressTable);
void GetF(GrammarToken *grammarToken, AddressTable *addressTable);
void PushArg_ARGS(GrammarToken *grammarToken, AddressTable *addressTable);
void PushArg_TMP(GrammarToken *grammarToken, AddressTable *addressTable);
void ADDArg(GrammarToken *grammarToken, AddressTable *addressTable);
void ADDArgLabeled(GrammarToken *grammarToken, AddressTable *addressTable);
void FuncInitialize(GrammarToken *grammarToken, AddressTable *addressTable);
void FuncInitParams(GrammarToken *grammarToken, AddressTable *addressTable);
void FuncDivideScope(GrammarToken *grammarToken, AddressTable *addressTable);
void FuncInitializeEscape(GrammarToken *grammarToken, AddressTable *addressTable);
void FuncCall(GrammarToken *grammarToken, AddressTable *addressTable);
void FuncCallPreOrder(GrammarToken *grammarToken, AddressTable *addressTable);
void FuncArgAdd(GrammarToken *grammarToken, AddressTable *addressTable);
void PreOrderForIf (GrammarToken *grammarToken, AddressTable *addressTable);
void PostOrderForIf (GrammarToken *grammarToken, AddressTable *addressTable);
void PreOrderForIfEslse (GrammarToken *grammarToken, AddressTable *addressTable);
void UnwrapCond(GrammarToken *grammarToken, AddressTable *addressTable);
void ReturnInterCode (GrammarToken *grammarToken, AddressTable *addressTable);
void ContinueInterCode (GrammarToken *grammarToken, AddressTable *addressTable);
void BreakInterCode (GrammarToken *grammarToken, AddressTable *addressTable);
void VarIdInit(GrammarToken *grammarToken, AddressTable *addressTable);
void VarTypedIdInit(GrammarToken *grammarToken, AddressTable *addressTable);
void InitProcess(GrammarToken *grammarToken, AddressTable *addressTable);
void StoreNil(GrammarToken *grammarToken, AddressTable *addressTable);
void IdAssignInterCode(GrammarToken *grammarToken, AddressTable *addressTable);
void IdInitAssignPropagate(GrammarToken *grammarToken, AddressTable *addressTable);
void Interpolation(GrammarToken *grammarToken, AddressTable *addressTable);
void InterpolationInterCode(GrammarToken *grammarToken, AddressTable *addressTable,int reg,char *leftSide,char *rightSide);
void $Int2Double();

#endif // IFJ2023_SWIFT_INTER_CODE_H
