// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"

struct Point1 {
	double x, y;     // coordinates
	int cluster;     // one way to represent that a point belongs to a specific cluster
	double minDist;  // for assigning the point to the nearest cluster. 
};

void testOpenImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image",src);
		waitKey();
	}
}

void testOpenImagesFld()
{
	char folderName[MAX_PATH];
	if (openFolderDlg(folderName)==0)
		return;
	char fname[MAX_PATH];
	FileGetter fg(folderName,"bmp");
	while(fg.getNextAbsFile(fname))
	{
		Mat src;
		src = imread(fname);
		imshow(fg.getFoundFileName(),src);
		if (waitKey()==27) //ESC pressed
			break;
	}
}

void testImageOpenAndSave()
{
	Mat src, dst;

	src = imread("Images/Lena_24bits.bmp", CV_LOAD_IMAGE_COLOR);	// Read the image

	if (!src.data)	// Check for invalid input
	{
		printf("Could not open or find the image\n");
		return;
	}

	// Get the image resolution
	Size src_size = Size(src.cols, src.rows);

	// Display window
	const char* WIN_SRC = "Src"; //window for the source image
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Dst"; //window for the destination (processed) image
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, src_size.width + 10, 0);

	cvtColor(src, dst, CV_BGR2GRAY); //converts the source image to a grayscale one

	imwrite("Images/Lena_24bits_gray.bmp", dst); //writes the destination to file

	imshow(WIN_SRC, src);
	imshow(WIN_DST, dst);

	printf("Press any key to continue ...\n");
	waitKey(0);
}

void testNegativeImage()
{
	char fname[MAX_PATH];
	int MAX_GREY_VALUE = 256;
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname,CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat_<uchar> dst = Mat_<uchar>(height,width);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		for (int i=0; i<height; i++)
		{
			for (int j=0; j<width; j++)
			{
				uchar val = src(i,j);
				uchar neg = MAX_GREY_VALUE - val;
				dst(i,j) = neg;
			}
		}
		imshow("Input image",src);
		imshow("Negative image",dst);
		waitKey();
	}
}

void new_function() {
	printf("Bau Bau!");
	printf("Bau! Bau! Ca trebuie!");
}

//our code

//============pick k random pixels(points in 2d space), the initial centroids ======================
std::vector<Point1> pick_k_random_points(std::vector<Point1>* centroids, int k) {// pointer because we don't have to copy the vector of points(waste of time and memory)
	//std::vector<Point1> centroids;
	srand(time(0));  // need to set the random seed
	for (int i = 0; i < k; ++i) {
		(*centroids).push_back(centroids->at(rand() % centroids->size));
	}
}



void kMeansClustering(std::vector<Point1>* points, int nrRepetitions, int k) {//the larger nrRepetitions, the better the solution. k-nr of clusters
	std::vector<Point1> centroids;
	std::vector<Point1> prviouscentroids;
	bool first = true;
	while (!sameCentroids(centroids, prviouscentroids)) {
		if (first) {
			std::vector<Point1> centroids = pick_k_random_points(points, k);
			first = false;
		}
		else {
			std::vector<Point1> centroids = computeCentroids(*points, k);
		}
		int clusterId = 0;
		for (int i = 0; i < centroids.size(); i++) { //iterate over the clusters
			Point1 c = centroids.at(i);
			for (int j = 0; j < points->size(); j++) {//iterate over the points to assign them to the nearest cluster
				Point1 p = points->at(j);
				double dist = euclidianDistance(c, p); //by euclidian distance
				if (dist < p.minDist) {		//if the distance between a point and curent cluster
											//is smaller than distance between this point and previous 
											//cluster, update the point to be part of the 
											//current cluster and the new distance also.
					p.minDist = dist;
					p.cluster = clusterId;
				}
			}
			clusterId++;
		}
	}
	//after the first iteretion the points will not be equal distributed to each cluster
	//there has to be a second part where the new centroids are computed, done by 
	//calling computeCentroids method
}





Mat_<Vec3b> generateImage(std::vector<Point1> points) {
	Mat_<Vec3b> img(100, 100);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			for (int k = 0; k < points.size; i++) {

			}

			img(i, j) = 255;
		}
	}
}

int main()
{
	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Open image\n");
		printf(" 2 - Open BMP images from folder\n");
		printf(" 3 - Negative image\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
				testOpenImage();
				break;
			case 2:
				testOpenImagesFld();
				break;
			case 3:
				testNegativeImage();
				break;

		}
	}
	while (op!=0);
	return 0;
}
