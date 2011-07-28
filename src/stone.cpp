#include "stone.h"
void Stone::setCoordinates(int i, int j) {
    _i=i;
    _j=j;
    return;
}
bool Stone::nextTo(Stone &stone) {
    int i=_i-stone._i;
    int j=_j-stone._j;
    int d=i*i+j*j;
    return (d==1);
}
/* stone.ccp */