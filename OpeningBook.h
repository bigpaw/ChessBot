#ifndef OPENINGBOOK_H_INCLUDED
#define OPENINGBOOK_H_INCLUDED

extern int numOutOfOpeningBook;
extern HashKey** openingZobristKeys;
extern HashKey* openingChangeSideKey;
extern HashKey* openingWhiteShortCastleKey;
extern HashKey* openingWhiteLongCastleKey;
extern HashKey* openingBlackShortCastleKey;
extern HashKey* openingBlackLongCastleKey;
extern HashKey* openingEnPassantKeys;
extern BookReference* bookReferenceArray;
extern int* ReferenceArraySize;
extern HashKey openingBookKey;
extern bool bookVariablesDeleted;

void LoadOpeningBookReference();
Move SearchOpeningMove();
int SearchBookReference();
void UpdateOpeningBookKey(const Move& move);
void InitializeBookKey(string fenString);

const string OpeningBookFile = "D:\\Users\\Wing\\My Documents\\Visual Studio 2012\\Projects\\Chess\\Book.opb";
const string OpeningBookReference = "D:\\Users\\Wing\\My Documents\\Visual Studio 2012\\Projects\\Chess\\BookReference.opb";
const string OpeningBookMoveSeparator = ";";
const string OpeningBookDataSeparator = " ";

#endif