// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <time.h>

//TODO x,y sa fie vector
struct Point1 {
	double x, y;     // coordinates
	int cluster;     // one way to represent that a point belongs to a specific cluster
};

//TODO si weigh trebuie tot un vector sa fie
typedef struct weigh
{
	float w1, w2, w3, w4, w5;
	
}WEIGH;

//our code

//============pick k random pixels(points in 2d space), the initial centroids ======================
std::vector<Point1> pick_k_random_points(std::vector<Point1>& points, const int& k) {// pointer because we don't have to copy the vector of points(waste of time and memory)

	std::vector<Point1> centroids;
	srand(time(0));  // need to set the random seed
	for (int i = 0; i < k; ++i) {
		centroids.push_back(points.at(rand() % points.size()));
	}

	return centroids;
}

void computeCentroids(std::vector<Point1>& points, const int& k, std::vector<Point1>& centroids) {
	//TODO use all points from vectors, not just x,y..
	
	std::vector<int> nPoints; //keep track of the number of points in each cluster 
	std::vector<double> sumX, sumY;//keep track of the sum of coordinates (then the 
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
		nPoints[clusterId] += 1;
		sumX[clusterId] += points.at(i).x;
		sumY[clusterId] += points.at(i).y;
	}

	// Compute the new centroids
	//TODO division by 0
	for (int i = 0; i < k; i++) {
		centroids.at(i).x = sumX[i] / nPoints[i];
		centroids.at(i).y = sumY[i] / nPoints[i];
	}
}


bool sameCentroids(const std::vector<Point1>& centroids, const std::vector<Point1>& prviouscentroids, const double& error) {
	//TODO trebuie si weighs, also use all from vectos not just x,y
	int count = 0;
	for (int i = 0; i < centroids.size(); i++)
	{
		double dist = sqrt((centroids.at(i).x - prviouscentroids.at(i).x) * (centroids.at(i).x - prviouscentroids.at(i).x) 
						+ (centroids.at(i).y - prviouscentroids.at(i).y) * (centroids.at(i).y - prviouscentroids.at(i).y));

		if  (dist <= error)	
		{
			count++;
		}
	}
	return count == centroids.size();
}

double euclidianDistance(Point1 p1, Point1 p2, const WEIGH& weigh) {
	//ponderi la tarsaturile din puncte => distante ponderate
	//vectorul de ponderi constant, trimis ca parametru la euclidianDistance
	return sqrt( weigh.w1*((p1.x - p2.x) * (p1.x - p2.x)) + weigh.w2*((p1.y - p2.y) * (p1.y - p2.y)));
}

//alegem noi centroizii sa fie unul langa altul ca sa se observe ca se deplaseaza 
//k = 2
void kMeansClustering(std::vector<Point1>& points, const int& k, const int& nrRepetitions,  const WEIGH& weighs,  const double& error) {//the larger nrRepetitions, the better the solution. k-nr of clusters
	std::vector<Point1> centroids;
	std::vector<Point1> prviouscentroids;
	int reps = 0;
//TODO pick k random points from point vector
	centroids.push_back(Point1{ 2.0, 3.0, 0 }); //= pick_k_random_points(points, k); // resets distances
	centroids.push_back(Point1{ 3.0, 2.0, 1 });

	do {
		
		prviouscentroids = centroids;

		for (auto& point : points)
		{
			double shortest = INFINITY;
			

			for (auto& centroid : centroids)
			{	
				

				double dist = euclidianDistance(centroid, point, weighs); //by euclidian distance
				if (dist <= shortest) {		//if the distance between a point and curent cluster
											//is smaller than distance between this point and previous 
											//cluster, update the point to be part of the 
											//current cluster and the new distance also.
					shortest = dist;
					point.cluster = centroid.cluster;
				}

			
			}
			
		}

		computeCentroids(points, k, centroids);  


		std::cout << "Iteration " << reps << std::endl;
		for (const auto& p : points)
		{
			std::cout << "P(" << p.x << "," << p.y << ") CLUSTER:" << p.cluster << std::endl;
			
		}
		std::cout << std::endl;

		std::cout << "Centroids "<< std::endl;
		for (const auto& c : centroids)
		{
			std::cout << "C(" << c.x << "," << c.y << ")" << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
		
		reps++;

	} while (reps < nrRepetitions || !sameCentroids(centroids, prviouscentroids, error));
	//after the first iteretion the points will not be equal distributed to each cluster
	//there has to be a second part where the new centroids are computed, done by 
	//calling computeCentroids method
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
	points.push_back(Point1{ 2.0, 3.0, 0 });
	points.push_back(Point1{ 10.0, 11.0, 0 });
	points.push_back(Point1{ 3.0, 2.0, 0 });
	points.push_back(Point1{ 11.0, 12.0, 0 });
	points.push_back(Point1{ 3.0, 4.0, 0 });
	points.push_back(Point1{ 9.0, 10.0, 0});

	const WEIGH weighs{ 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };
	int numberOfRepetitions = 2;
	int K = 2;
	double error = 0.001;

	kMeansClustering(points, K, numberOfRepetitions, weighs, error);

	return 0;
}
