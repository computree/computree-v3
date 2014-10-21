#include "ct_counter.h"

CT_Counter::CT_Counter(const size_t &nTurns)
{
    _nTurns = nTurns;
    _currentTurn = 1;
}
