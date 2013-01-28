#include "showslice.h"

ShowSlice::ShowSlice(int Idx)
{
    Index = Idx;
    slice.clear();
}

ShowSlice::~ShowSlice()
{
    qDeleteAll(slice);
    slice.clear();
}

void ShowSlice::clear()
{
    qDeleteAll(slice);
    slice.clear();
}
