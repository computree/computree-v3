#ifndef CT_COUNTER_H
#define CT_COUNTER_H

#include "stddef.h"

class CT_Counter
{
public:
    CT_Counter(const size_t &nTurns);

    inline size_t getNTurns() const {return _nTurns;}
    inline size_t getCurrentTurn() const {return _currentTurn;}
    inline bool hasNextTurn() {return _currentTurn < _nTurns;}
    inline bool isFinished() {return _currentTurn > _nTurns;}

    inline void beginNextTurn() {++_currentTurn;}

private:
    size_t       _currentTurn;
    size_t       _nTurns;

};

#endif // CT_COUNTER_H
