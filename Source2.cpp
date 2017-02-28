#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <iostream>
#include <math.h>

#define RED 2
#define GREEN 1
#define BLUE 0
#define BRIGHTNESS_TRESHOLD 80

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

double getMaxValue(double red, double green, double blue) {
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

char* getRVBMax(double red, double green, double blue) {
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

double getMinValue(double red, double green, double blue) {
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

double getChroma(double max, double min) {
	return (double)max - min;
}


double getHue(double red, double green, double blue) {
	if (getRVBMax(red, green, blue) == "Red") {
		return (60 * fmod(((green - blue) / getChroma(red, getMinValue(red, green, blue)) + 6), 6));
	}
	else if (getRVBMax(red, green, blue) == "Green") {
		return (60 * ((blue - red) / getChroma(green, getMinValue(red, green, blue)) + 2));
	}
	else if (getRVBMax(red, green, blue) == "Blue") {
		return (60 * ((red - green) / getChroma(blue, getMinValue(red, green, blue)) + 4));
	}
	else if (getChroma(getMaxValue(red, green, blue), getMinValue(red, green, blue) == 0)) {
		return -1;
	}
}

double getBrightness(double red, double green, double blue) {
	return getMaxValue(red, green, blue) * 100 / 255;
}

double getSaturation(double max, double chroma) {
	if (max == 0) {
		return 0;
	}
	else if (max>0) {
		return chroma / max * 100;
	}
}

void printTLS(double red, double green, double blue) {
	double max = getMaxValue(red, green, blue);
	printf("Max : %f \n", max);
	double min = getMinValue(red, green, blue);
	printf("Min : %f \n", min);
	double chroma = getChroma(max, min);
	double brightness = getBrightness(red, green, blue);
	double saturation = getSaturation(max, chroma);
	double hue = getHue(red, green, blue);
	printf("Chroma : %f \n", chroma);
	printf("Luminosite : %f \n", brightness);
	printf("Saturation : %f \n", saturation);
	printf("Teinte : %f \n", hue);
	int d;
	scanf("d", &d);
}

/*Fin du code pour aller de RVB vers TLS*/

/*Code pour aller de TLS vers RVB*/

double getChromaPercentage(double brightness, double saturation) {
	return (double)brightness*saturation / 100;
}

double getHuePrime(double hue) {
	return hue / 60;
}

double getColorX(double chroma, double hueprime) {
	return chroma*(1 - abs(fmod(hueprime, 2) - 1));
}

double* getRVB(double hueprime, double brightness, double chroma, double colorX) {
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

void printRVB(double _brightness, double _saturation, double _hue) {
	int i = 0;
	char* letter = { 0 };
	double brightness = _brightness;
	double saturation = _saturation;
	double hue = _hue;
	double chromaPercentage = getChromaPercentage(brightness, saturation);
	double hueprime = getHuePrime(hue);
	double colorX = getColorX(chromaPercentage, hueprime);
	printf("Chroma : %f \n", chromaPercentage);
	printf("Hue' : %f \n", hueprime);
	printf("ColorX : %f \n", colorX);
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
		printf("%s : %f \n", letter, getRVB(hueprime, brightness, chromaPercentage, colorX)[i]);
	}
}


int main(int argc, char** argv)
{
	IplImage* imagen = cvLoadImage("C:\\Users\\Balzrog\\Pictures\\Images DEFI H\\Descargar.jpg");
	Mat image = cv::cvarrToMat(imagen, false);
	displayImage("Base window", image);
	double* rvb;
	double red, green, blue;
	double max;
	double min;
	double chroma;
	double saturation;
	double hue;
	double chromaPercentage;
	double hueprime;
	double colorX;

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			CvScalar pixel = cvGet2D(imagen, i, j);
			double brightness = getBrightnessByRVB(pixel.val[BLUE], pixel.val[GREEN], pixel.val[RED]);
			red = pixel.val[RED];
			green = pixel.val[GREEN];
			blue = pixel.val[BLUE];
			max = getMaxValue(red, green, blue);
			min = getMinValue(red, green, blue);
			chroma = getChroma(max, min);

			saturation = getSaturation(max, chroma);
			hue = getHue(red, green, blue);

			chromaPercentage = getChromaPercentage(brightness, saturation);
			hueprime = getHuePrime(hue);
			colorX = getColorX(chromaPercentage, hueprime);


			if (brightness >= BRIGHTNESS_TRESHOLD) {

				int difference = brightness - BRIGHTNESS_TRESHOLD;
				float ratio = ((float)BRIGHTNESS_TRESHOLD / 100.0) + (1 - (float)BRIGHTNESS_TRESHOLD / 100.0) - (1 - (float)BRIGHTNESS_TRESHOLD / 100.0)*difference / (100 - BRIGHTNESS_TRESHOLD);
				pixel.val[BLUE] *= ratio;
				pixel.val[GREEN] *= ratio;
				pixel.val[RED] *= ratio;
				
				//pixel.val[BLUE] -= difference;
				//pixel.val[GREEN] -= difference;
				//pixel.val[RED] -= difference;

				//pixel.val[RED] = getRVB(hueprime, BRIGHTNESS_TRESHOLD, chromaPercentage, colorX)[0];
				//pixel.val[GREEN] = getRVB(hueprime, BRIGHTNESS_TRESHOLD, chromaPercentage, colorX)[1];
				//pixel.val[BLUE] = getRVB(hueprime, BRIGHTNESS_TRESHOLD, chromaPercentage, colorX)[2];
			}
			cvSet2D(imagen, i, j, pixel);
			//printf("i:%d j:%d Bleu : %f ; Vert : %f ; Rouge : %f \n ", i, j, pixel.val[BLUE], pixel.val[GREEN], pixel.val[RED]);
		}
	}

	displayImage("Destination window", cv::cvarrToMat(imagen, false));
	waitKey(0); // Wait for a keystroke in the window
	return 0;
}
