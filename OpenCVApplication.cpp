// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <time.h>

struct Point1 {
	double x, y;     // coordinates
	int cluster;     // one way to represent that a point belongs to a specific cluster
	double minDist;  // for assigning the point to the nearest cluster. 
};

typedef struct weigh
{
	float w1, w2, w3, w4, w5;
	
}WEIGH;

//our code

//============pick k random pixels(points in 2d space), the initial centroids ======================
std::vector<Point1> pick_k_random_points(std::vector<Point1>& points, const int& k) {// pointer because we don't have to copy the vector of points(waste of time and memory)
	//reset distances
	for (auto& p : points)
	{
		p.minDist = INFINITY;
	}

	std::vector<Point1> centroids;
	srand(time(0));  // need to set the random seed
	for (int i = 0; i < k; ++i) {
		centroids.push_back(points.at(rand() % points.size()));
	}

	return centroids;
}

std::vector<Point1> computeCentroids(std::vector<Point1>& points, const int& k);


bool sameCentroids(const std::vector<Point1>& centroids, const std::vector<Point1>& prviouscentroids) {
	bool same = true;
	for (int i = 0; i < centroids.size(); i++) {
		if (centroids.at(i).x != prviouscentroids.at(i).x && centroids.at(i).y != prviouscentroids.at(i).y) {
			same = false;
		}
	}
	return same;
}

double euclidianDistance(Point1 p1, Point1 p2, const WEIGH& weigh) {
	//ponderi la tarsaturile din puncte => distante ponderate
	//vectorul de ponderi constant, trimis ca parametru la euclidianDistance
	return sqrt( weigh.w1*((p1.x - p2.x) * (p1.x - p2.x)) + weigh.w2*((p1.y - p2.y) * (p1.y - p2.y)));
}

//alegem noi centroizii sa fie unul langa altul ca sa se observe ca se deplaseaza 
//k = 2
void kMeansClustering(std::vector<Point1>& points, const WEIGH& weighs, const int& nrRepetitions, const int& k) {//the larger nrRepetitions, the better the solution. k-nr of clusters
	std::vector<Point1> centroids;
	std::vector<Point1> prviouscentroids;
	bool first = true;
	do {
		if (first) {
			centroids = pick_k_random_points(points, k); // resets distances
			first = false;
		}
		else {
			centroids = computeCentroids(points, k); // resets distances
		}
		int clusterId = 0;

		prviouscentroids = centroids;

		for (auto& centroid : centroids) //iterate over the clusters
		{ 		
			for (auto& point : points)//iterate over the points to assign them to the nearest cluster
			{				
				double dist = euclidianDistance(centroid, point, weighs); //by euclidian distance
				if (dist < point.minDist) {		//if the distance between a point and curent cluster
											//is smaller than distance between this point and previous 
											//cluster, update the point to be part of the 
											//current cluster and the new distance also.
					point.minDist = dist;
					point.cluster = clusterId;
				}
			}
			clusterId++;
		}

		///afisare pixeli la fiecare iteratie dupa actualizarea centroidului
		for (const auto& p : points)
		{
			std::cout << "P(" << p.x << "," << p.y << ") CLUSTER:" << p.cluster << std::endl;
		}
		
	} while (!sameCentroids(centroids, prviouscentroids));
	//after the first iteretion the points will not be equal distributed to each cluster
	//there has to be a second part where the new centroids are computed, done by 
	//calling computeCentroids method
}

std::vector<Point1> computeCentroids(std::vector<Point1>& points, const int& k) {
	std::vector<int> nPoints; //keep track of the number of points in each cluster 
	std::vector<double> sumX, sumY;//keep track of the sum of coordinates (then the 
									//average is just the latter divided by the former)
	std::vector<Point1> centroids;

	// Initialise with zeroes
	for (int j = 0; j < k; ++j) {
		nPoints.push_back(0);
		sumX.push_back(0.0);
		sumY.push_back(0.0);
	}

	// Iterate over points to append data to centroids
	for (int i = 0; i < points.size(); i++) {
		int clusterId = points.at(i).cluster;
		nPoints[clusterId] += 1;
		sumX[clusterId] += points.at(i).x;
		sumY[clusterId] += points.at(i).y;

		points.at(i).minDist = INFINITY;  // reset distance
	}

	// Compute the new centroids
	for (int i = 0; i < k; i++) {
		centroids.at(i).x = sumX[i] / nPoints[i];
		centroids.at(i).y = sumY[i] / nPoints[i];
	}
	return centroids;
}



Mat_<Vec3b> generateImage(std::vector<Point1> points) {
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
	std::vector<Point1> points;
	points.push_back(Point1{ 2.0, 3.0, 0, 0.0 });
	points.push_back(Point1{ 10.0, 11.0, 0, 0.0 });
	points.push_back(Point1{ 3.0, 2.0, 0, 0.0 });
	points.push_back(Point1{ 11.0, 12.0, 0, 0.0 });
	points.push_back(Point1{ 3.0, 4.0, 0, 0.0 });
	points.push_back(Point1{ 9.0, 10.0, 0, 0.0 });

	const WEIGH weighs{ 0.5f, 0.5f, 0.0f, 0.0f, 0.0f };

	kMeansClustering(points, weighs, 2, 2);

	return 0;
}
