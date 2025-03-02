#pragma once

class enums
{
public:
    enum class EGemType {
        ruby = 0,
        amethyst = 1,
        topaz = 2,
        emerald = 3,
        sapphire = 4,
        diamond = 5,
    };
    
    enum EGemSize
    {
        small,
        large,
        splendid,
        flawless
    };

    enum ESocketType
    {
        NoSocket = 0,
        NoAdd,
        Percentage25,
        Percentage66,
    };
    
};
