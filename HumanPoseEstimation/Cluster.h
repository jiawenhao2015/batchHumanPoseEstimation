# ifndef _CLUSTER_H
# define _CLUSTER_H

#include "Data.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
class Cluster
{
public:
	int hand[2];
	int foot[2];
	void Classify(float** Dist, int PointNum, vector<Data>src);
	void DisClassify(float** Dist, int PointNum, vector<Data>src, int v0);
	void Chek(int Vertex[6][2]);
	bool isHand(int t);
	bool isFoot(int t);

	void savePixels(ofstream& outPart, int x, int y);
	vector<vector<int>>pixelBelongToSeedX;
	vector<vector<int>>pixelBelongToSeedY;
	int* label;
	string outputPath;
	~Cluster();
};

# endif