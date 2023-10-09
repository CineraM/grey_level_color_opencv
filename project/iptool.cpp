/************************************************************
 *															*
 * This sample project include three functions:				*
 * 1. Add intensity for gray-level image.					*
 *    Input: source image, output image name, value			*
 *															*
 * 2. Image thresholding: pixels will become black if the	*
 *    intensity is below the threshold, and white if above	*
 *    or equal the threhold.								*
 *    Input: source image, output image name, threshold		*
 *															*
 * 3. Image scaling: reduction/expansion of 2 for 			*
 *    the width and length. This project uses averaging 	*
 *    technique for reduction and pixel replication			*
 *    technique for expansion.								*
 *    Input: source image, output image name, scale factor	*
 *															*
 ************************************************************/

#include "../iptools/core.h"
#include <string.h>
#include <vector>

using namespace std;

// :^) Please let us use python, trying to modify this code base is hell! 
#define ROI_INPUTS	pch = strtok(NULL, " ");int roi_i = atoi(pch); pch = strtok(NULL, " ");int roi_j = atoi(pch);pch = strtok(NULL, " ");int roi_i_size = atoi(pch);pch = strtok(NULL, " ");int roi_j_size = atoi(pch);pch = strtok(NULL, " ");
#define XD pch = strtok(NULL, " ");	// :^))))))))
#define MAXLEN 256


int main (int argc, char** argv)
{
	image src, tgt, temp;
	
	FILE *fp;
	char str[MAXLEN];
	char outfile[MAXLEN];
	char *pch;
	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	while(fgets(str,MAXLEN,fp) != NULL) 
	{
		pch = strtok(str, " ");
		src.read(pch);
		temp.copyImage(src);
		
		XD
		strcpy(outfile, pch);

		XD // XDDDDD

		bool flag = false;
		if (strcmp(pch,"binarize")==0)
		{
			XD
			flag = true;
			utility::binarize(src, tgt, atoi(pch));
		}
		else if (strcmp(pch,"add")==0)
		{
			XD
			flag = true;
			utility::addGrey(src, tgt, atoi(pch));
		}
		else if (strcmp(pch,"scale")==0)
		{
			XD
			flag = true;
			utility::scale(src, tgt, atoi(pch));
		}
		else if (strcmp(pch,"rotate")==0)
		{
			XD
			flag = true;
			utility::rotate(src, tgt, atoi(pch));
		}
		else if (strcmp(pch,"addColor")==0)
		{
			XD
			flag = true;
			utility::addColor(src, tgt, atoi(pch));
		}
		else if (strcmp(pch,"addBrightness")==0)
		{
			XD
			flag = true;
			utility::addColorBrightness(src, tgt, atoi(pch));
		}

		if(flag)
		{
			tgt.save(outfile);
			continue;
		}

		int num_of_rois = atoi(pch);

		for(int i = 0; i<num_of_rois; i++)
		{
			ROI_INPUTS
			if (strcmp(pch,"binarize")==0)
			{
				XD
				utility::binarizeWrapper(temp, tgt, atoi(pch), roi_i, roi_j, roi_i_size, roi_j_size);
			}
			else if (strcmp(pch,"add")==0)
			{
				XD
				utility::addGreyWrapper(temp, tgt, atoi(pch), roi_i, roi_j, roi_i_size, roi_j_size);
			}
			else if (strcmp(pch,"scale")==0)
			{
				XD
				utility::scaleWrapper(temp, tgt, atoi(pch), roi_i, roi_j, roi_i_size, roi_j_size);
			}
			else if (strcmp(pch,"rotate")==0)
			{
				XD
				utility::rotateWrapper(temp, tgt, atoi(pch), roi_i, roi_j, roi_i_size, roi_j_size);
			}
			else if (strcmp(pch,"addColor")==0)
			{	
				XD
				utility::addColorWrapper(temp, tgt, atoi(pch), roi_i, roi_j, roi_i_size, roi_j_size);
			}
			else if (strcmp(pch,"addBrightness")==0)
			{	
				XD
				utility::addColorBrightnessWrapper(temp, tgt, atoi(pch), roi_i, roi_j, roi_i_size, roi_j_size);
			}
			// Just ROI Fncs
			else if (strcmp(pch,"binarizeROI")==0)
			{
				XD
				utility::binarizeROI(temp, tgt, atoi(pch), roi_i, roi_j, roi_i_size, roi_j_size);
			}
			else if (strcmp(pch,"addROI")==0)
			{
				XD
				utility::addGreyROI(temp, tgt, atoi(pch), roi_i, roi_j, roi_i_size, roi_j_size);
			}
			else if (strcmp(pch,"scaleROI")==0)
			{
				XD
				utility::scaleROI(temp, tgt, atoi(pch), roi_i, roi_j, roi_i_size, roi_j_size);
			}
			else if (strcmp(pch,"rotateROI")==0)
			{
				XD
				utility::rotateROI(temp, tgt, atoi(pch), roi_i, roi_j, roi_i_size, roi_j_size);
			}
			else if (strcmp(pch,"addColorROI")==0)
			{	
				XD
				utility::addColorROI(temp, tgt, atoi(pch), roi_i, roi_j, roi_i_size, roi_j_size);
			}
			else if (strcmp(pch,"addBrightnessROI")==0)
			{	
				XD
				utility::addColorBrightnessROI(temp, tgt, atoi(pch), roi_i, roi_j, roi_i_size, roi_j_size);
			}

			temp.copyImage(tgt);
		}

		tgt.save(outfile);
	}
	fclose(fp);
	return 0;
}
