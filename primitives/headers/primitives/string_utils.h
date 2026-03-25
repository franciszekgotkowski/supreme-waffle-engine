#pragma once

#include <stdbool.h>
#include <primitives/typedefs.h>
#include <primitives/errors.h>

mstr stickToWord(mstr s);

mstr nextWord(mstr s);

mstr newLine(mstr s);

mstr searchFor(mstr s, char c);

u32 wordLength(mstr s);

bool wordsMatch(str a, str b);

u8 charToHex(char c, Error* err);