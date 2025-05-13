<?php

namespace IPP\Student\Instructions;

//Implements enum of TYPES and some functions for converting from/to enum
abstract class Types 
{
    const int VAR_type = 1;
    const int SYMB_type = 2;
    const int INT_type = 3;
    const int STRING_type = 4;
    const int BOOL_type = 5;
    const int NIL_type = 6;
    const int TYPE_type = 7; //(╯°□°)╯︵ ┻━┻
    const int ANY_type = 99;

    public static function getSymbType(string $type): int
    {
        switch ($type)
        {
            case "var":
                return self::VAR_type;
            case "string":
                return self::STRING_type;
            case "int":
                return self::INT_type;
            case "bool":
                return self::BOOL_type;
            case "nil":
                return self::NIL_type;
            case "type":
                return self::TYPE_type;
            default:
                return self::ANY_type;
        }
    }

    public static function getTypeName(int $type): string
    {
        switch ($type)
        {
            case self::STRING_type:
                return "string";
            case self::INT_type:
                return "int";
            case self::BOOL_type:
                return "bool";
            case self::NIL_type:
                return "nil";
            case self::TYPE_type:
                return "type";
            default:
                return "?";
        }
    }

    public static function getVarType(int|string|bool|null $var): int 
    {
        if (is_int($var)) return self::INT_type;
        if (is_string($var)) return self::STRING_type;
        if (is_bool($var)) return self::BOOL_type;
        if (is_null($var)) return self::NIL_type;
    }
}