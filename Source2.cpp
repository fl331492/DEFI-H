#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <iostream>
#define RED 2
#define GREEN 1
#define BLUE 0
#define BRIGHTNESS_TRESHOLD 75

using namespace cv;
using namespace std;

void displayImage(String displayname, Mat img) {
	namedWindow(displayname, WINDOW_NORMAL);
	imshow(displayname, img);
}

double getBrightnessByRVB(double r, double v, double b) {
	if (r >= v && r >= b) {
		return r * 100 / 255;
	}
	else if (v >= r && v >= b) {
		return v * 100 / 255;
	}
	else if (b >= r && b >= v) {
		return b * 100 / 255;
	}
	return -1;
}

/*Code pour aller de RVB vers TLS*/

int getMaxValue(int red, int green, int blue) {
	if (red >= green && red >= blue) {
		return red;
	}
	else if (green >= red && green >= blue) {
		return green;
	}
	else if (blue >= green && blue >= red) {
		return blue;
	}
	else {
		return -1;
	}
}

char* getRVBMax(int red, int green, int blue) {
	if (red >= green && red >= blue) {
		return "Red";
	}
	else if (green >= red && green >= blue) {
		return "Green";
	}
	else if (blue >= green && blue >= red) {
		return "Blue";
	}
	else {
		return "N/A";
	}
}

int getMinValue(int red, int green, int blue) {
	if (red <= green && red <= blue) {
		return red;
	}
	else if (green <= red && green <= blue) {
		return green;
	}
	else if (blue <= green && blue <= red) {
		return blue;
	}
	else {
		return -1;
	}
}

double getChroma(int max, int min) {
	return (double)max - min;
}

double getMod(double value, int mod) {
	double result = value / mod;
	return value - (int)result;
}

double getHue(int red, int green, int blue) {
	if (getRVBMax(red, green, blue) == "Red") {
		return (60 * getMod(((green - blue) / getChroma(getMaxValue(red, green, blue), getMinValue(red, green, blue)) + 6), 6));
	}
	else if (getRVBMax(red, green, blue) == "Green") {
		return (double)(60 * ((blue - red) / getChroma(getMaxValue(red, green, blue), getMinValue(red, green, blue)) + 2));
	}
	else if (getRVBMax(red, green, blue) == "Blue") {
		return (double)(60 * ((red - green) / getChroma(getMaxValue(red, green, blue), getMinValue(red, green, blue)) + 4));
	}
	else if (getChroma(getMaxValue(red, green, blue), getMinValue(red, green, blue) == 0)) {
		return -1;
	}
}

int getBrightness(int red, int green, int blue) {
	return getMaxValue(red, green, blue) * 100 / 255;
}

float getSaturation(int max, int chroma) {
	if (max == 0) {
		return 0;
	}
	else if (max>0) {
		return (float)chroma / (float)max * 100;
	}
}

void printTLS(int red, int green, int blue) {
	int max = getMaxValue(red, green, blue);
	printf("Max : %d \n", max);
	int min = getMinValue(red, green, blue);
	printf("Min : %d \n", min);
	double chroma = getChroma(max, min);
	int brightness = getBrightness(red, green, blue);
	float saturation = getSaturation(max, chroma);
	double hue = getHue(red, green, blue);
	printf("Chroma : %f \n", chroma);
	printf("Luminosite : %d \n", brightness);
	printf("Saturation : %f \n", saturation);
	printf("Teinte : %f \n", hue);
	int d;
	scanf("d", &d);
}

/*Fin du code pour aller de RVB vers TLS*/

/*Code pour aller de TLS vers RVB*/

int getChromaPercentage(int brightness, float saturation) {
	return brightness*saturation / 100;
}

double getHuePrime(double hue) {
	return hue / 60;
}

int getColorX(int chroma, double hueprime) {
	return chroma*(1 - abs(getMod(hueprime, 2) - 1));
}

double* getRVB(double hueprime, int brightness, int chroma, int colorX) {
	double rvb[3];
	double m = (double)brightness - (double)chroma;
	if (0.0 <= hueprime && hueprime< 1.0) {
		rvb[0] = (chroma + m) * 255 / 100;
		rvb[1] = (colorX + m) * 255 / 100;
		rvb[2] = (m) * 255 / 100;
	}
	else if (1.0 <= hueprime && hueprime < 2.0) {
		rvb[0] = (colorX + m) * 255 / 100;
		rvb[1] = (chroma + m) * 255 / 100;
		rvb[2] = (m) * 255 / 100;
	}
	else if (2.0 <= hueprime && hueprime< 3.0) {
		rvb[0] = (m) * 255 / 100;
		rvb[1] = (chroma + m) * 255 / 100;
		rvb[2] = (colorX + m) * 255 / 100;
	}
	else if (3.0 <= hueprime && hueprime< 4.0) {
		rvb[0] = (m) * 255 / 100;
		rvb[1] = (colorX + m) * 255 / 100;
		rvb[2] = (chroma + m) * 255 / 100;
	}
	else if (4.0 <= hueprime && hueprime< 5.0) {
		rvb[0] = (colorX + m) * 255 / 100;
		rvb[1] = (m) * 255 / 100;
		rvb[2] = (chroma + m) * 255 / 100;
	}
	else if (5.0 <= hueprime && hueprime< 6.0) {
		rvb[0] = (chroma + m) * 255 / 100;
		rvb[1] = (m) * 255 / 100;
		rvb[2] = (colorX + m) * 255 / 100;
	}
	else {
		rvb[0] = (m) * 255 / 100;
		rvb[1] = (m) * 255 / 100;
		rvb[2] = (m) * 255 / 100;
	}
	return rvb;
}

void printRVB() {
	int i = 0;
	char* letter = { 0 };
	int brightness = 79;
	float saturation = 82;
	double hue = 82;
	int chromaPercentage = getChromaPercentage(brightness, saturation);
	double  hueprime = getHuePrime(hue);
	int colorX = getColorX(chromaPercentage, hueprime);
	printf("Chroma : %d \n", chromaPercentage);
	printf("Hue' : %f \n", hueprime);
	printf("ColorX : %d \n", colorX);
	for (i; i < 3; i++) {
		if (i == 0) {
			letter = "Red";
		}
		else if (i == 1) {
			letter = "Green";
		}
		else if (i == 2) {
			letter = "Blue";
		}
		printf("%s : %d \n", letter, getRVB(hueprime, brightness, chromaPercentage, colorX)[i]);
	}
	int d;
	scanf("d", &d);
}

/*Fin du code pour aller de TLS vers RVB*/


int main(int argc, char** argv)
{
	IplImage* imagen = cvLoadImage("C:\\Users\\Balzrog\\Pictures\\image-joyeuse.jpg");
	Mat image = cv::cvarrToMat(imagen, false);
	displayImage("Base window", image);
	int* rvb;
	int red, green, blue;
	int max;
	int min;
	double chroma;
	float saturation;
	double hue;
	int chromaPercentage;
	double hueprime;
	int colorX;

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			CvScalar pixel = cvGet2D(imagen, i, j);
			double brightness = getBrightnessByRVB(pixel.val[BLUE], pixel.val[GREEN], pixel.val[RED]);

			int brightness2 = getBrightness(pixel.val[BLUE], pixel.val[GREEN], pixel.val[RED]);
			red = pixel.val[RED];
			green = pixel.val[GREEN];
			blue = pixel.val[BLUE];
			max = getMaxValue(red, green, blue);
			min = getMinValue(red, green, blue);
			chroma = getChroma(max, min);
			saturation = getSaturation(max, chroma);
			hue = getHue(red, green, blue);

			chromaPercentage = getChromaPercentage(70, saturation);
			hueprime = getHuePrime(hue);
			colorX = getColorX(chromaPercentage, hueprime);
			



			if (brightness2 >= BRIGHTNESS_TRESHOLD) {
				int difference = brightness - BRIGHTNESS_TRESHOLD;
				float ratio = ((float)BRIGHTNESS_TRESHOLD / 100.0) + (1 - (float)BRIGHTNESS_TRESHOLD / 100.0) - (1 - (float)BRIGHTNESS_TRESHOLD / 100.0)*difference / (100 - BRIGHTNESS_TRESHOLD);
				pixel.val[BLUE] *= ratio;
				pixel.val[GREEN] *= ratio;
				pixel.val[RED] *= ratio;
				
				/*pixel.val[BLUE] -= difference;
				pixel.val[GREEN] -= difference;
				pixel.val[RED] -= difference;*/
				//pixel.val[RED] = getRVB(hueprime, 70, chromaPercentage, colorX)[0];
				//pixel.val[GREEN] = getRVB(hueprime, 70, chromaPercentage, colorX)[1];
				//pixel.val[BLUE] = getRVB(hueprime, 70, chromaPercentage, colorX)[2];
			}
			cvSet2D(imagen, i, j, pixel);
			//printf("i:%d j:%d Bleu : %f ; Vert : %f ; Rouge : %f \n ", i, j, pixel.val[BLUE], pixel.val[GREEN], pixel.val[RED]);
		}
	}

	displayImage("Destination window", cv::cvarrToMat(imagen, false));
	waitKey(0); // Wait for a keystroke in the window
	return 0;
}