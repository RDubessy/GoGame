#include "stone.h"
Stone::Stone() {
    _colour='.';
    _group=0;
}
void Stone::setCoordinates(int i, int j) {
    _i=i;
    _j=j;
    return;
}
bool Stone::nextTo(const Stone &stone) const {
    int i=_i-stone._i;
    int j=_j-stone._j;
    int d=i*i+j*j;
    return (d==1);
}
int Stone::distance(const Stone &stone) const {
    int i=_i-stone._i;
    int j=_j-stone._j;
    return i*i+j*j;
}
/* stone.ccp */