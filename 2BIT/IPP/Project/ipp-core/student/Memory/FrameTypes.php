<?php

namespace IPP\Student\Memory;

abstract class FrameTypes
{
    const int TF = 1;
    const int LF = 2;
    const int GF = 3;
    const int ERROR_FRAME = 99;

    public static function getFrameType(string $frame): int
    {
        switch ($frame)
        {
            case "LF":
                return self::LF;
            case "TF":
                return self::TF;
            case "GF":
                return self::GF;
            default:
                return self::ERROR_FRAME;
        }
    }
}