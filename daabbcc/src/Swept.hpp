//
//  Swept.hpp
//  DAABBCC
//
//  Created by selimanac on 27.02.2018.
//  Copyright © 2018 Selim Anac. All rights reserved.
//

#ifndef Swept_hpp
#define Swept_hpp

#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <aabb/AABB.h>

using namespace std;
using namespace aabb;

class Swept{
public:
    float _normalx;
    float _normaly;
    float SweptAABB(AABB box1, AABB box2, vector<float> vel1, float normalx,float normaly);
protected:
private:
};

#endif /* Swept_hpp */
