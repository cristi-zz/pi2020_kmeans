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
	srand(time(0));
	for (int i = 0; i < k; i++) {
		centroids.push_back(points.at(rand() % (points.size() - 1)));
	}

	return centroids;
}

void computeCentroids(vector<Point1>& points, const int& k, vector<Point1>& centroids) {
	//TODO use all points from vectors, not just x,y..

	//vector<int> nPoints(k); //keep track of the number of points in each cluster 
	/*vector<double> sumX, sumY;//keep track of the sum of coordinates (then the
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
	}*/

	//std::vector<std::vector<double>> sum(k);
	int** sum = new int*[k];
	int size = points.at(0).point.size();

	int* nPoints = new int[k];

	for (int i = 0; i < k; ++i) {
		sum[i] = new int[size + 1];
	}

	for (int j = 0; j < k; ++j) {
		nPoints[j] = 0;
		for (int i = 0; i < size; ++i) {
			sum[j][i] = 0;
		}
	}

	/*for (int i = 0; i < k; ++i) {
		printf("\n\n --- %d --- \n\n", nPoints[i]);
		for (int j = 0; j < sizeof(sum[0]) / sizeof(sum[0][0]); ++j) {
			printf("%d, ", sum[i][j]);
		}
		printf("\n");
	}*/


	// Iterate over points to append data to centroids
	for (int i = 0; i < points.size(); i++) {
		int clusterId = points.at(i).cluster;
		nPoints[clusterId] += 1;
		for (int j = 0; j < size; ++j) {
			//printf("%d, %d\n", clusterId, j) ;
			sum[clusterId][j] += points.at(i).point.at(j);
		}
	}

	// Compute the new centroids
	for (int j = 0; j < size; j++) {
		for (int i = 0; i < k; i++) {
			centroids.at(i).point.at(j) = sum[j][i] / nPoints[points.at(i).cluster];
		}
	}
}


bool sameCentroids(const vector<Point1>& centroids, const vector<Point1>& prviouscentroids, const double& error, const WEIGHT& weighs) {
	//TODO trebuie si weighs, also use all from vectos not just x,y
	int count = 0;
	/*for (int i = 0; i < centroids.size(); i++)
	{
		double dist = sqrt(pow(centroids.at(i).point.at(0) - prviouscentroids.at(i).point.at(0), 2) + pow(centroids.at(i).point.at(1) - prviouscentroids.at(i).point.at(1), 2));

		if  (dist <= error)
		{
			count++;
		}
	}*/
	
	int size = centroids.at(0).point.size();

	for (int i = 0; i < centroids.size(); i++)
	{
		double dist = 0.0;
		for (int j = 0; j < size; j++) {
			dist += weighs.weights.at(i) * (centroids.at(i).point.at(j) - prviouscentroids.at(i).point.at(j)) * (centroids.at(i).point.at(j) - prviouscentroids.at(i).point.at(j));
		}
		if (sqrt(dist) <= error)
		{
			count++;
		}
	}
	return count == centroids.size();
}

double euclidianDistance(Point1 p1, Point1 p2, const WEIGHT& weight) {
	//ponderi la tarsaturile din puncte => distante ponderate
	//vectorul de ponderi constant, trimis ca parametru la euclidianDistance
	//double x1 = p1.point.at(0);
	//double x2 = p2.point.at(0);
	//double y1 = p1.point.at(1);
	//double y2 = p2.point.at(1);

	//return sqrt(weight.weights.at(0) * (pow(x1 - x2, 2)) + weight.weights.at(1) * (pow(y1 - y2, 2)));

	double result = 0.0;
	for (int i = 0; i < p1.point.size(); ++i) {
		result += (double) weight.weights.at(i) * (pow(p1.point.at(i) - p2.point.at(i), 2));
	}
	return sqrt(result);
}

//alegem noi centroizii sa fie unul langa altul ca sa se observe ca se deplaseaza 
//k = 2
void kMeansClustering(vector<Point1>& points, const int& k, const int& nrRepetitions, const WEIGHT& weights, const double& error) {//the larger nrRepetitions, the better the solution. k-nr of clusters
	//printf("hasfajfksdjfkjdfsfsf\n");
	vector<Point1> centroids;
	vector<Point1> prviouscentroids;

	int reps = 0;
	//TODO pick k random points from point vector
	vector<double> p1{ 2.0, 3.0 };
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
				//printf("%f\n\n\n", dist);
				if (dist <= shortest) {		//if the distance between a point and curent cluster
											//is smaller than distance between this point and previous 
											//cluster, update the point to be part of the 
											//current cluster and the new distance also.
					shortest = dist;
					points.at(i).cluster = centroids.at(j).cluster;
					//printf("%d : %d\n", i, points.at(i).cluster);
				}


			}

		}
		computeCentroids(points, k, centroids);


		/*std::cout << "Iteration " << reps << std::endl;
		for (const auto& p : points)
		{
			std::cout << "P(" << p.point.at(0) << "," << p.point.at(1) << ") CLUSTER:" << p.cluster << std::endl;

		}
		std::cout << std::endl;

		std::cout << "Centroids "<< std::endl;
		for (const auto& c : centroids)
		{
			std::cout << "C(" << c.point.at(0) << "," << c.point.at(1) << ")" << std::endl;
		}*/
		int size = points[0].point.size();
		std::cout << "Iteration " << reps << std::endl;
		for (const auto& p : points)
		{
			std::cout << "P(";
			for (int i = 0; i < size; ++i) {
				if (i != size - 1) {
					std::cout << p.point.at(i) << ", ";
				}
				else {
					std::cout << p.point.at(i);
				}
			}
			std::cout << ") CLUSTER:" << p.cluster << std::endl;
		}
		std::cout << std::endl;

		std::cout << "Centroids " << std::endl;
		int sizeC = centroids[0].point.size();
		for (const auto& c : centroids)
		{
			std::cout << "C(";
			for (int i = 0; i < sizeC; ++i) {
				if (i != sizeC - 1) {
					std::cout << c.point.at(i) << ", ";
				}
				else {
					std::cout << c.point.at(i);
				}
			}
			std::cout << ")" << std::endl;
		}

		std::cout << std::endl;
		std::cout << std::endl;

		reps++;

	} while (reps < nrRepetitions || !sameCentroids(centroids, prviouscentroids, error, weights));
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

vector<Point1> extractFeatures(Mat_<Vec3b> src)
{
	vector<Point1> features;
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if ((src(i, j)[0] != 76) && (src(i, j)[1] != 112) && (src(i, j)[2] != 71))
			{
				//x  y   R   G  B
				//------0 1 2
				//Vec3b(x,y,z)
				//------B G R
				vector<double> p{ (double)i ,  (double)j, (double)src(i,j)[2], (double)src(i,j)[1], (double)src(i,j)[0] };
				features.push_back(Point1{ p, 0 });
			}
		}
	}
	imshow("src", src);
	return features;

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

	const WEIGHT weights{ {1.0f, 1.0f, 1.0f, 1.0f, 1.0f} };
	int numberOfRepetitions = 3;
	int K = 2;
	double error = 0.001;

	kMeansClustering(points, K, numberOfRepetitions, weights, error);
	char fName[MAX_PATH];
	while (openFileDlg(fName))
	{
		Mat_<Vec3b> src = imread(fName, CV_LOAD_IMAGE_COLOR);
		vector<Point1> points = extractFeatures(src);
		std::cout << "P(" << points.at(0).point.at(0) << "," << points.at(0).point.at(1) << "," << points.at(0).point.at(2) << "," << points.at(0).point.at(3) << "," << points.at(0).point.at(4) << ")" << std::endl;
		waitKey(0);
	}

	return 0;
}
