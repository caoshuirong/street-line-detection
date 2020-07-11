#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "bmpFile.h"
//���ڲ�ɫͼ��ֻҪ����ͨ����ͬ�Ҷ�ֵ������һ�𣬾ͱ�֤���ٷ����ʱ������һ���൱�������Ҷ�ֱ��ͼ�ϲ���һ����ֻ�Ǹ߶ȱ���ˣ�������ϵ����һ���ġ�
void CsrHistogramEqualize(BYTE*pGryImg, int width, int height, int channel = 1);
void CsrMeanStdDev(BYTE*pGryImg, int width, int height, double bright, double contrast);
void CsrRgb2GrayLUT(BYTE*pGryImg, int width, int height,BYTE *pResImg);
void CsrGrayExtend(BYTE*pGryImg, int width, int height);
int RmwRead14BitImgFile(short int * p14Img, int width, int height, const char * fileName);
void Csr14BitChangeTo8Bit(short int*pGryImg, int width, int height, BYTE *pResImg);
