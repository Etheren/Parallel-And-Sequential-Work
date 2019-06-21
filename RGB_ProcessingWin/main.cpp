#include <iostream>#include <vector>
//Thread building blocks library
#include <tbb/task_scheduler_init.h>//Free Image library
#include <FreeImagePlus.h>
#include <cmath>
#include <functional>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range2d.h>
#include <stdlib.h>
#include <time.h>
#include <mutex>
#include <chrono>

using namespace std;
using namespace tbb;int main() {
	int nt = task_scheduler_init::default_num_threads();
	task_scheduler_init T(nt);	// Setup Input image arrays	fipImage inputImage1;
	fipImage inputImage2;
	inputImage1.load("../Images/35k1.png");
	inputImage2.load("../Images/35k2.png");

	int width = inputImage1.getWidth(); //Gets width and height of the first image, to be used as the output image
	int height = inputImage1.getHeight();
	int redPixelWidth = rand() % width; //random used for the red pixel placement.
	int redPixelHeight = rand() % height;
	int redPixelHeightLoc = 0, redPixelWidthLoc = 0;
	float whiteCount = 0, pixelCount = 0;
	float whitePercent = 0;

	// Setup Output image array
	fipImage outputImage;
	fipImage ImageOne;
	fipImage ImageTwo;
	outputImage = fipImage(FIT_BITMAP, width, height, 24);
	ImageOne = fipImage(FIT_BITMAP, width, height, 24);
	ImageTwo = fipImage(FIT_BITMAP, width, height, 24);

	//2D Vectors to hold the RGB colour data of an image
	vector<vector<RGBQUAD>> rgbValues1;
	vector<vector<RGBQUAD>> rgbValues2;
	vector<vector<RGBQUAD>> outputRGB;
	rgbValues1.resize(height, vector<RGBQUAD>(width));
	rgbValues2.resize(height, vector<RGBQUAD>(width));
	outputRGB.resize(height, vector<RGBQUAD>(width));
	RGBQUAD rgb; //FreeImage structure to hold RGB values of a single pixel
	// SEQUENTIAL METHOD
	std::cout << "Sequential Method Start" << endl;
	auto st5 = std::chrono::high_resolution_clock::now();

	std::cout << "Before Extract 1" << endl;

	//Extract colour data from image and store it as individual RGBQUAD elements for every pixel
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			inputImage1.getPixelColor(x, y, &rgb); //Extract pixel(x,y) colour data and place it in rgb			rgbValues1[y][x].rgbRed = rgb.rgbRed;
			rgbValues1[y][x].rgbGreen = rgb.rgbGreen;
			rgbValues1[y][x].rgbBlue = rgb.rgbBlue;
		}
	}

	//Same for the 2nd image
	std::cout << "Before Extract 2" << endl;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			inputImage2.getPixelColor(x, y, &rgb); //Extract pixel(x,y)colour data and place it in rgb
			rgbValues2[y][x].rgbRed = rgb.rgbRed;
			rgbValues2[y][x].rgbGreen = rgb.rgbGreen;
			rgbValues2[y][x].rgbBlue = rgb.rgbBlue;
		}
	}

	std::cout << "Before Sub" << endl;
	//Computing the absolute differences between the two inputted images
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			outputRGB[y][x].rgbRed = rgbValues1[y][x].rgbRed - rgbValues2[y][x].rgbRed;
			outputRGB[y][x].rgbGreen = rgbValues1[y][x].rgbGreen - rgbValues2[y][x].rgbGreen;
			outputRGB[y][x].rgbBlue = rgbValues1[y][x].rgbBlue - rgbValues2[y][x].rgbBlue;
		}
	}

	std::cout << "before binary" << endl;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (outputRGB[y][x].rgbRed > 20 || outputRGB[y][x].rgbGreen > 20 || outputRGB[y][x].rgbBlue > 20)
			{
				outputRGB[y][x].rgbRed = 255;
				outputRGB[y][x].rgbGreen = 255;
				outputRGB[y][x].rgbBlue = 255;
			}		}	}	std::cout << "before save" << endl;	for (int y = 0; y < height; y++)	{		for (int x = 0; x < width; x++)		{			outputImage.setPixelColor(x, y, &outputRGB[y][x]);		}	}	std::cout << "before export" << endl;	outputImage.save("RGB_processed.png");	//Count all the white pixels on the saved image	std::cout << "before white count" << endl;	for (int y = 0; y < height; y++)	{		for (int x = 0; x < width; x++)		{			if (outputRGB[y][x].rgbRed == 255 && outputRGB[y][x].rgbGreen == 255 && outputRGB[y][x].rgbBlue == 255)			{				whiteCount += 1;			}		}	}	std::cout << "before total count" << endl;	pixelCount = width * height;	whitePercent = (whiteCount / pixelCount) * 100;	// Calculate the % of white pixels.	std::cout << fixed;	std::cout << "There are a total of : " << whiteCount << " whitepixels out of a possible " << pixelCount << " in the finalimage." << endl;	std::cout << "This means that " << whitePercent << "% of the pixelson the screen are white." << endl;	outputRGB[redPixelHeight][redPixelWidth].rgbRed = 255;	outputRGB[redPixelHeight][redPixelWidth].rgbGreen = 0;	outputRGB[redPixelHeight][redPixelWidth].rgbBlue = 0;	std::cout << "Red Pixel placed. Searching..." << endl;	//Search for the random red pixel on the screen.	for (int y = 0; y < height; y++)	{		for (int x = 0; x < width; x++)
		{
			if (outputRGB[y][x].rgbRed == 255 && outputRGB[y][x].rgbGreen == 0 && outputRGB[y][x].rgbBlue == 0)
			{
				redPixelWidthLoc = x + 1;
				redPixelHeightLoc = y + 1;
			}
		}
	}

	std::cout << "The red pixel has been located at: " << "Width - " << redPixelWidthLoc << ", Height - " << redPixelHeightLoc << "." << endl;
	auto st6 = std::chrono::high_resolution_clock::now();
	pixelCount = 0;
	whiteCount = 0;
	whitePercent = 0;

	//PARALLEL METHOD
	std::cout << "Parallel Method Start" << endl;
	auto st7 = std::chrono::high_resolution_clock::now();

	std::cout << "Before Extract 1" << endl;
	tbb::parallel_for(tbb::blocked_range2d<int, int>(0, height, 0, width), [&](const tbb::blocked_range2d<int, int>& r)
	{
		for (int y = r.rows().begin(); y < r.rows().end(); ++y)
		{
			for (int x = r.cols().begin(); x < r.cols().end(); ++x)
			{
				inputImage1.getPixelColor(x, y, &rgb);

				//Extractpixel(x,y) colour data and place it in rgb

				rgbValues1[y][x].rgbRed = rgb.rgbRed;
				rgbValues1[y][x].rgbGreen = rgb.rgbGreen;
				rgbValues1[y][x].rgbBlue = rgb.rgbBlue;
			}
		}
	});

	std::cout << "Before Extract 2" << endl;
	tbb::parallel_for(tbb::blocked_range2d<int, int>(0, height, 0, width), [&](const tbb::blocked_range2d<int, int>& r)
	{
		for (int y = r.rows().begin(); y < r.rows().end(); ++y)
		{
			for (int x = r.cols().begin(); x < r.cols().end(); ++x)
			{
				inputImage2.getPixelColor(x, y, &rgb); //Extractpixel(x, y) colour data and place it in rgb

				rgbValues2[y][x].rgbRed = rgb.rgbRed;
				rgbValues2[y][x].rgbGreen = rgb.rgbGreen;
				rgbValues2[y][x].rgbBlue = rgb.rgbBlue;
			}
		}
	});

	std::cout << "Before Sub" << endl;
	tbb::parallel_for(tbb::blocked_range2d<int, int>(0, height, 0, width), [&](const tbb::blocked_range2d<int, int>& r)
	{
		for (int y = r.rows().begin(); y < r.rows().end(); ++y)
		{
			for (int x = r.cols().begin(); x < r.cols().end(); ++x)
			{
				outputRGB[y][x].rgbRed = rgbValues1[y][x].rgbRed - rgbValues2[y][x].rgbRed;
				outputRGB[y][x].rgbGreen = rgbValues1[y][x].rgbGreen - rgbValues2[y][x].rgbGreen;
				outputRGB[y][x].rgbBlue = rgbValues1[y][x].rgbBlue - rgbValues2[y][x].rgbBlue;
			}
		}
	});

	std::cout << "Before Binary" << endl;
	tbb::parallel_for(tbb::blocked_range2d<int, int>(0, height, 0, width), [&](tbb::blocked_range2d<int, int>& r)
	{
		for (int y = r.rows().begin(); y < r.rows().end(); y++)
		{
			for (int x = r.cols().begin(); x < r.cols().end(); x++)
			{
				if (outputRGB[y][x].rgbRed > 20 || outputRGB[y][x].rgbGreen > 20 || outputRGB[y][x].rgbBlue > 20)
				{
					outputRGB[y][x].rgbRed = 255;
					outputRGB[y][x].rgbGreen = 255;
					outputRGB[y][x].rgbBlue = 255;
				}
			}
		}
	});

	std::cout << "Before Save" << endl;
	tbb::parallel_for(tbb::blocked_range2d<int, int>(0, height, 0, width), [&](tbb::blocked_range2d<int, int> r)
	{
		for (int y = r.rows().begin(); y < r.rows().end(); y++)
		{
			for (int x = r.cols().begin(); x < r.cols().end(); x++)
			{
				outputImage.setPixelColor(x, y, &outputRGB[y][x]);
			}
		}
	});

	std::cout << "Before Export" << endl;
	//Save the processed image
	outputImage.save("RGB_processed.png");

	std::cout << "Before White Count" << endl;
	tbb::parallel_for(tbb::blocked_range<int>(0, height), [&](tbb::blocked_range<int> r)
	{
		tbb::parallel_for(tbb::blocked_range<int>(0, width), [&](tbb::blocked_range<int> s)
		{
			for (int y = r.begin(); y < r.end(); y++)
			{
				for (int x = s.begin(); x < s.end(); x++)
				{
					if (outputRGB[y][x].rgbRed == 255 && outputRGB[y][x].rgbGreen == 255 && outputRGB[y][x].rgbBlue == 255)
					{
						whiteCount += 1;
					}
					pixelCount += 1;
				}
			}
		});
	});

	whiteCount = tbb::parallel_reduce(tbb::blocked_range2d<int, int>(0, height, 0, width), 0.0, [&](tbb::blocked_range2d<int, int>& r, double running_total)
	{
		for (int y = r.rows().begin(); y < r.rows().end(); y++)
		{
			for (int x = r.cols().begin(); x < r.cols().end(); x++)
			{
				if (outputRGB[y][x].rgbRed == 255 && outputRGB[y][x].rgbGreen == 255 && outputRGB[y][x].rgbBlue == 255)
				{
					running_total += 1;
				}
			}
		}
		return running_total;
	}, std::plus<float>());

	std::cout << "before total count" << endl;
	pixelCount = tbb::parallel_reduce(tbb::blocked_range<int>(0, (width*height)), 0.0, [&](tbb::blocked_range<int> r, double running_total)
	{
		for (int y = r.begin(); y < r.end(); y++)
		{
			running_total += 1;
		}
		return running_total;
	}, std::plus<double>());

	pixelCount = width * height;

	whitePercent = (whiteCount / pixelCount) * 100; // Calculate the % of white pixels.

	std::cout << fixed;
	std::cout << "There are a total of : " << whiteCount << " whitepixels out of a possible " << pixelCount << " in the final image." << endl;
	std::cout << "This means that " << whitePercent << "% of the pixels on the screen are white." << endl;


	//random red pixel placed into the image
	outputRGB[redPixelHeight][redPixelWidth].rgbRed = 255;
	outputRGB[redPixelHeight][redPixelWidth].rgbGreen = 0;
	outputRGB[redPixelHeight][redPixelWidth].rgbBlue = 0;

	std::cout << "Red Pixel placed. Searching..." << endl;

	tbb::parallel_for(tbb::blocked_range<int>(0, height), [&](tbb::blocked_range<int> r)
	{
		tbb::parallel_for(tbb::blocked_range<int>(0, width), [&](tbb::blocked_range<int> s)
		{
			for (int y = r.begin(); y < r.end(); y++)
			{
				for (int x = s.begin(); x < s.end(); x++)
				{
					if (outputRGB[y][x].rgbRed == 255 && outputRGB[y][x].rgbGreen == 0 && outputRGB[y][x].rgbBlue == 0)
					{
						redPixelWidthLoc = x + 1;
						redPixelHeightLoc = y + 1;
					}
				}
			}
		});
	});

	std::cout << "The red pixel has been located at: " << "Width - " << redPixelWidthLoc << ", Height - " << redPixelHeightLoc << "." << endl;

	auto st8 = std::chrono::high_resolution_clock::now();
	auto st_sqdur2 =
		std::chrono::duration_cast<std::chrono::microseconds>(st6 - st5);
	auto st_pldur2 = std::chrono::duration_cast<std::chrono::microseconds>(st8 - st7);
	std::cout << "Sequential Operations took " << st_sqdur2.count() << " microseconds." << endl;
	std::cout << "Parallel Operations took " << st_pldur2.count() << " microseconds." << endl;

	double speedup = double(st_sqdur2.count()) /
		double(st_pldur2.count());
	std::cout << "Test speedup = " << speedup << endl;

	return 0;
}