// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <time.h>

using namespace std;
//TODO x,y sa fie vector
struct Point1 {
	vector<double> point;     // coordinates(x, y), R, G, B, H, S, V
	int cluster;     // a way to represent that a point belongs to a specific cluster
};

//TODO si weigh trebuie tot un vector sa fie
typedef struct weight
{
	vector<float> weights; //
}WEIGHT;


//============pick k random pixels, the initial centroids ======================
vector<Point1> pick_k_random_points(vector<Point1>& points, const int& k) {

	vector<Point1> centroids;
	int x = 0, y = 0;
	srand(time(0));
	for (int i = 0; i < k; i++) {
		centroids.push_back(points.at(rand() % (points.size() - 1)));
	}

	return centroids;
}

void computeCentroids(vector<Point1>& points, const int& k, vector<Point1>& centroids) {
	//TODO use all points from vectors, not just x,y..
	
	vector<int> nPoints; //keep track of the number of points in each cluster 
	vector<double> sumX, sumY;//keep track of the sum of coordinates (then the 
									//average is just the latter divided by the former)


	// Initialise with zeroes
	for (int j = 0; j < k; j++) {
		nPoints.push_back(0);
		sumX.push_back(0.0);
		sumY.push_back(0.0);
	}

	// Iterate over points to append data to centroids
	for (int i = 0; i < points.size(); i++) {
		int clusterId = points.at(i).cluster;
		nPoints[clusterId]++;
		sumX[clusterId] += points.at(i).point.at(0);
		sumY[clusterId] += points.at(i).point.at(1);
	}

	// Compute the new centroids
	//TODO division by 0
	for (int i = 0; i < k; i++) {
		centroids.at(i).point.at(0) = sumX[i] / nPoints[i];
		centroids.at(i).point.at(1) = sumY[i] / nPoints[i];
	}
}


bool sameCentroids(const vector<Point1>& centroids, const vector<Point1>& prviouscentroids, const double& error) {
	//TODO trebuie si weighs, also use all from vectos not just x,y
	int count = 0;
	for (int i = 0; i < centroids.size(); i++)
	{
		double dist = sqrt(pow(centroids.at(i).point.at(0) - prviouscentroids.at(i).point.at(0), 2) + pow(centroids.at(i).point.at(1) - prviouscentroids.at(i).point.at(1), 2));

		if  (dist <= error)	
		{
			count++;
		}
	}
	return count == centroids.size();
}

double euclidianDistance(Point1 p1, Point1 p2, const WEIGHT& weight) {
	//ponderi la tarsaturile din puncte => distante ponderate
	//vectorul de ponderi constant, trimis ca parametru la euclidianDistance
	double x1 = p1.point.at(0);
	double x2 = p2.point.at(0);
	double y1 = p1.point.at(1);
	double y2 = p2.point.at(1);
	
	return sqrt(weight.weights.at(0) * (pow(x1 - x2, 2)) + weight.weights.at(1) * (pow(y1 - y2, 2)));
}

//alegem noi centroizii sa fie unul langa altul ca sa se observe ca se deplaseaza 
//k = 2
void kMeansClustering(vector<Point1>& points, const int& k, const int& nrRepetitions,  const WEIGHT& weights,  const double& error) {//the larger nrRepetitions, the better the solution. k-nr of clusters
	vector<Point1> centroids;
	vector<Point1> prviouscentroids;
	int reps = 0;
//TODO pick k random points from point vector
	vector<double> p1 { 2.0, 3.0 };
	vector<double> p2{ 3.0, 2.0 };
	centroids.push_back(Point1{ p1, 0 }); //= pick_k_random_points(points, k); // resets distances
	centroids.push_back(Point1{ p2, 1 });

	do {
		prviouscentroids = centroids;
		for (int i = 0; i < points.size(); i++)
		{
			double shortest = INFINITY;

			for (int j = 0; j < centroids.size(); j++)
			{	
				
				double dist = euclidianDistance(points.at(i), centroids.at(j), weights); //by euclidian distance
				if (dist <= shortest) {		//if the distance between a point and curent cluster
											//is smaller than distance between this point and previous 
											//cluster, update the point to be part of the 
											//current cluster and the new distance also.
					shortest = dist;
					points.at(i).cluster = centroids.at(j).cluster;
				}

			
			}
			
		}

		computeCentroids(points, k, centroids);  


		std::cout << "Iteration " << reps << std::endl;
		for (const auto& p : points)
		{
			std::cout << "P(" << p.point.at(0) << "," << p.point.at(1) << ") CLUSTER:" << p.cluster << std::endl;
			
		}
		std::cout << std::endl;

		std::cout << "Centroids "<< std::endl;
		for (const auto& c : centroids)
		{
			std::cout << "C(" << c.point.at(0) << "," << c.point.at(1) << ")" << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
		
		reps++;

	} while (reps < nrRepetitions || !sameCentroids(centroids, prviouscentroids, error));
	//after the first iteretion the points will not be equal distributed to each cluster
	//there has to be a second part where the new centroids are computed, done by 
	//calling computeCentroids method
}





Mat_<Vec3b> generateImage(vector<Point1> points) {
	Mat_<Vec3b> img(100, 100);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			for (int k = 0; k < points.size(); i++) {

			}

			img(i, j) = 255;
		}
	}

	return img;
}

int main()
{
	vector<Point1> points;
	points.push_back(Point1{ {2.0, 3.0}, 0 });
	points.push_back(Point1{ {10.0, 11.0}, 0 });
	points.push_back(Point1{ {3.0, 2.0}, 0 });
	points.push_back(Point1{ {11.0, 12.0}, 0 });
	points.push_back(Point1{ {3.0, 4.0}, 0 });
	points.push_back(Point1{ {9.0, 10.0}, 0 });

	const WEIGHT weights{ {1.0f, 1.0f, 0.0f, 0.0f, 0.0f} };
	int numberOfRepetitions = 4;
	int K = 2;
	double error = 0.001;

	kMeansClustering(points, K, numberOfRepetitions, weights, error);

	return 0;
}
