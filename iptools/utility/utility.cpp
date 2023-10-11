#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

using namespace cv;
using namespace std;
image temp1, temp2;

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

bool check_roi(int i, int j, int roi_i, int roi_j, int roi_i_size, int roi_j_size)
{
	if(i >= roi_i && i <= roi_i+roi_i_size-1)
		if(j >= roi_j && j <= roi_j+roi_j_size-1) 
			return 1;
	return 0;
}


void utility::roi(image &src, image &tgt, int roi_i, int roi_j, int roi_i_size, int roi_j_size)
{
	tgt.resize(roi_i_size, roi_j_size);
	int rows = src.getNumberOfRows();
	int cols = src.getNumberOfColumns();

	for (int i=0; i<roi_i_size; i++)
	{
		for (int j=0; j<roi_j_size; j++)
		{
			if((i+roi_i) >= rows || (j+roi_j) >= cols)
			{
				for(int channel = 0; channel<3; channel++)
					tgt.setPixel(i, j, channel, MINRGB);
			}
			else
			{
				for(int channel = 0; channel<3; channel++)
					tgt.setPixel(i, j, channel, src.getPixel(i+roi_i, j+roi_j, channel));
			}
		}
	}

}


void utility::mergeRoi(image &src, image &roi,image &tgt, int roi_i, int roi_j, 
int roi_i_size, int roi_j_size)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			
			if(check_roi(i, j, roi_i, roi_j, roi_i_size, roi_j_size))
			{
				for(int channel = 0; channel<3; channel++)
					tgt.setPixel(i, j, channel, roi.getPixel(i-roi_i, j-roi_j, channel) ); 
			}
			else
			{
				for(int channel = 0; channel<3; channel++)
					tgt.setPixel(i, j, channel, src.getPixel(i,j, channel) );
			}
				
		}
	}
}


void utility::binarize(image &src, image &tgt, int threshold)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if (src.getPixel(i,j) < threshold)
			{
				for(int channel = 0; channel<3; channel++)
					tgt.setPixel(i, j, channel, MINRGB);
			}
			else
			{
				for(int channel = 0; channel<3; channel++)
					tgt.setPixel(i, j, channel, MAXRGB);
			}
		}
	}
}


void utility::addGrey(image &src, image &tgt, int value)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			for(int channel = 0; channel<3; channel++)
				tgt.setPixel(i, j, channel, checkValue(src.getPixel(i, j, channel)+value));
		}
}


void utility::scale(image &src, image &tgt, float ratio)
{
	if(ratio < 10) ratio = 10;
	if(ratio > 20) ratio = 20;
	ratio/=10;
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			/* Map the pixel of new image back to original image */
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			// Directly copy the value 
			for(int channel = 0; channel<3; channel++)
				tgt.setPixel(i, j, channel, checkValue(src.getPixel(i2, j2, channel)));
			}
	}
}


void utility::rotate(image &src, image &tgt, int angle)
{
	int rows = src.getNumberOfRows();
	int cols = src.getNumberOfColumns();


	tgt.resize(rows, cols);
	if(angle == 90 || angle == 270)
		tgt.resize(cols, rows);
		
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{
			int new_i, new_j;
			if(angle == 90)
			{
				new_i = j;
				new_j = rows - 1 - i;
			}
			else if(angle == 180)
			{
				new_i = rows - 1 - i;
				new_j = cols - 1 - j;	
			}
			else if(angle == 270)
			{
				new_i = cols - 1 - j;
				new_j = i;	
			}
			
			for(int channel = 0; channel<3; channel++)
				tgt.setPixel(new_i, new_j, channel, src.getPixel(i, j, channel));
		}
	}
}

void utility::addColor(image &src, image &tgt, double value)
{
	if(value < 10) value = 10;
	if(value > 20) value = 20;

	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			for(int channel = 0; channel<3; channel++)
				tgt.setPixel(i, j, channel, checkValue( src.getPixel(i, j, channel)*(value/10) ));
		}
}

void utility::addColorBrightness(image &src, image &tgt, int value)
{
	if(value < -50) value = -50;
	if(value > 50) value = 50;

	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			for(int channel = 0; channel<3; channel++)
				tgt.setPixel(i, j, channel, checkValue(src.getPixel(i, j, channel) + value));
		}
}


void utility::binarizeWrapper(WRAPPER_PARAMS)
{
	roi(src, temp1, roi_i, roi_j, roi_i_size, roi_j_size);
	binarize(temp1, temp2, fnc_input);
	mergeRoi(src, temp2, tgt, roi_i, roi_j, roi_i_size, roi_j_size);
}


void utility::addGreyWrapper(WRAPPER_PARAMS)
{
	roi(src, temp1, roi_i, roi_j, roi_i_size, roi_j_size);
	addGrey(temp1, temp2, fnc_input);
	mergeRoi(src, temp2, tgt, roi_i, roi_j, roi_i_size, roi_j_size);
}


void utility::scaleWrapper(WRAPPER_PARAMS)
{
	roi(src, temp1, roi_i, roi_j, roi_i_size, roi_j_size);
	scale(temp1, tgt, fnc_input);
}


void utility::rotateWrapper(WRAPPER_PARAMS)
{
	roi(src, temp1, roi_i, roi_j, roi_i_size, roi_j_size);
	rotate(temp1, temp2, fnc_input);
	mergeRoi(src, temp2, tgt, roi_i, roi_j, roi_i_size, roi_j_size);
}


void utility::addColorWrapper(WRAPPER_PARAMS)
{
	roi(src, temp1, roi_i, roi_j, roi_i_size, roi_j_size);
	addColor(temp1, temp2, fnc_input);
	mergeRoi(src, temp2, tgt, roi_i, roi_j, roi_i_size, roi_j_size);

}


void utility::addColorBrightnessWrapper(WRAPPER_PARAMS)
{
	roi(src, temp1, roi_i, roi_j, roi_i_size, roi_j_size);
	addColorBrightness(temp1, temp2, fnc_input);
	mergeRoi(src, temp2, tgt, roi_i, roi_j, roi_i_size, roi_j_size);
}

// ONLY ROI FUNCTIONS

void utility::binarizeROI(WRAPPER_PARAMS)
{
	roi(src, temp1, roi_i, roi_j, roi_i_size, roi_j_size);
	binarize(temp1, tgt, fnc_input);
}


void utility::addGreyROI(WRAPPER_PARAMS)
{
	
	roi(src, temp1, roi_i, roi_j, roi_i_size, roi_j_size);
	addGrey(temp1, tgt, fnc_input);
}


void utility::scaleROI(WRAPPER_PARAMS)
{
	scale(temp1, tgt, fnc_input);
}


void utility::rotateROI(WRAPPER_PARAMS)
{
	roi(src, temp1, roi_i, roi_j, roi_i_size, roi_j_size);
	rotate(temp1, tgt, fnc_input);
}


void utility::addColorROI(WRAPPER_PARAMS)
{
	roi(src, temp1, roi_i, roi_j, roi_i_size, roi_j_size);
	addColor(temp1, tgt, fnc_input);

}


void utility::addColorBrightnessROI(WRAPPER_PARAMS)
{
	roi(src, temp1, roi_i, roi_j, roi_i_size, roi_j_size);
	addColorBrightness(temp1, tgt, fnc_input);
}

// Project 2 functions
void utility::histogramStretching(image &src, image &tgt, int A, int B)
{
	int min_pixel = 255;
	int max_pixel = 0;

	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			int pixel_val = src.getPixel(i, j);
			if(pixel_val > max_pixel) max_pixel = pixel_val;
			if(pixel_val < min_pixel) min_pixel = pixel_val;
		}
	}
			
	int C = min_pixel*1.05;
	int D = max_pixel*0.95;

	unordered_map<int, int> new_pixel_vals;
	
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			int old_val = src.getPixel(i, j);
			if(new_pixel_vals.find(old_val) == new_pixel_vals.end())
			{
				double new_val = (old_val-C) * (B-A) / (D-C) + A;
				// double new_val = ((old_val - C) / (D - C)) * (B - A) + A; 
				new_pixel_vals[old_val] = checkValue(new_val);
				tgt.setPixel(i, j, new_pixel_vals[old_val]);	
			}
			else
				tgt.setPixel(i, j, new_pixel_vals[old_val]);
		}
	}

}


void utility::histogramStretchingROI(image &src, image &tgt, int A, int B, 
	int roi_i, int roi_j, int roi_i_size, int roi_j_size)
{
	roi(src, temp1, roi_i, roi_j, roi_i_size, roi_j_size);
	histogramStretching(temp1, tgt, A, B);
}


void utility::equalizeGrey(string src, string tgt)
{
	Mat src_image = imread(src, IMREAD_GRAYSCALE);
    // if (src_image.empty()) return;
    // Apply histogram equalization
    Mat equalized_image;
    equalizeHist(src_image, equalized_image);
    // save img to new file
    imwrite(tgt, equalized_image);
}


void utility::equalizeGreyROI(image &src, string tgtfile
	,int roi_i, int roi_j, int roi_i_size, int roi_j_size)
{
	string temp_file = "temp_file.pgm";
	roi(src, temp1, roi_i, roi_j, roi_i_size, roi_j_size);
	temp1.save(temp_file.c_str());
	equalizeGrey(temp_file, tgtfile);
}
