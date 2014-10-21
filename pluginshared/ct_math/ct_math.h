#ifndef CT_MATH_H
#define CT_MATH_H

#include <math.h>
#include <algorithm>

#include "pluginShared_global.h"

class PLUGINSHAREDSHARED_EXPORT CT_Math
{
public:

    template<class ForwardIt, class Compare>
    static std::pair<ForwardIt, ForwardIt> minmax_element(ForwardIt first, ForwardIt last, Compare comp)
    {
        std::pair<ForwardIt, ForwardIt> result(first, first);

        if (first == last) return result;
        if (++first == last) return result;

        if (comp(*first, *result.first)) {
            result.second = result.first;
            result.first = first;
        } else {
            result.second = first;
        }
        while (++first != last) {
            ForwardIt i = first;
            if (++first == last) {
                if (comp(*i, *result.first)) result.first = i;
                else if (!(comp(*i, *result.second))) result.second = i;
                break;
            } else {
                if (comp(*first, *i)) {
                    if (comp(*first, *result.first)) result.first = first;
                    if (!(comp(*i, *result.second))) result.second = i;
                } else {
                    if (comp(*i, *result.first)) result.first = i;
                    if (!(comp(*first, *result.second))) result.second = first;
                }
            }
        }

        return result;
    }
};

#endif // CT_MATH_H
