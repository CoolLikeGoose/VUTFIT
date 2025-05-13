#include "vector.h"
#include "../../model/token/token.h"
#include "../../parser/precedence_parser/precedence_parser.h"
#include "../../structures/hashmap/hashmap.h"

vector_impl(Token, Token, token_);
vector_impl(PParserItem, PParserItem, pparser_item_);
vector_impl(int, Int, int_);
vector_impl(double, Double, double_);
vector_impl(String, String, string_);
vector_impl(char, Char, char_);
vector_impl(StringPointer, StringPointer, string_pointer_);
vector_impl(HashMapPointer, HashMapPointer, hashmap_);
vector_impl(HashMapPointer2D, HashMapPointer2D, hasmap2d_);