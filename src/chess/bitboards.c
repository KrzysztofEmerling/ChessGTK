#include "bitboards.h"

#include <math.h>
#include <assert.h>


uint64_t get_index_mask(int index)
{
    assert(index >= 0 && index < 64);
    return IndexBitMasks[index];
}

void get_pieces_indices(uint64_t pieces, int *indices, size_t *count)
{
    int index = 63;
    for(count = 0; pieces > 0; count++)
    {
        while(2 << index > pieces) { --index; }
        pieces -= 2 << index;

        *indices++ = index;
    }
}

int get_piece_type(const GameData * gameData, int index)
{
    int i = 11;
    while(i > -1 && gameData->chessPieces.pieces[i] & get_index_mask(index) > 0) { --i; }

    return i;
}

bool is_white_piece(const GameData * gameData, int index)
{
    uint64_t mask = 0;
    for(int i = 0; i < 6; i++) { mask |= gameData->chessPieces.pieces[i]; }
    return mask & get_index_mask(index) > 0;
}

bool is_black_piece(const GameData * gameData, int index)
{
    uint64_t mask = 0;
    for(int i = 6; i < 12; i++) { mask |= gameData->chessPieces.pieces[i]; }
    return mask & get_index_mask(index) > 0;
}