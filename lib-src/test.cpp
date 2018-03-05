#include <iostream>
#include <algorithm>
#include <math.h>
#include "AABB.h"

using namespace std;
using namespace aabb;


int main()

{
	Tree treeObjectPointernew =  Tree(2, 0.1, 100);

	vector<double> lowerBound({10,10});
	vector<double> upperBound({15,15});
	treeObjectPointernew.insertParticle(0, lowerBound, upperBound);
	treeObjectPointernew.getHeight();
	treeObjectPointernew.getNodeCount();
	cout << "Test";
	printf("Hello World!\n");
	cout << treeObjectPointernew.getNodeCount();

return 0;

}