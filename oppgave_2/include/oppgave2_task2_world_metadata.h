#ifndef PG3401_V24_38_TASK2_WORLD_METADATA_H
#define PG3401_V24_38_TASK2_WORLD_METADATA_H

#include <stdbool.h>

struct TASK2_WORD_METADATA {

    int iIndex;
    bool IsPalindrom;
    bool bIsHeterogram;
    bool bIsUppercase;
    bool bIsLowercase;
    bool bIsAnagram;
    bool bIsDisjoint;
    int iSize;
    char szWord[];
};

#endif //PG3401_V24_38_TASK2_WORLD_METADATA_H
