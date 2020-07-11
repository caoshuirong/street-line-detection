////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CsrLocatePos.cpp : implementation file
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "CsrLocatePos.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����/��������
//
////////////////////////////////////////////////////////////////////////////////////////////////////
CsrLocatePos::CsrLocatePos()
{  	//<�ڹ��캯���а����еĳ�Ա��������ֵ>
	//�Ƿ��ʼ���ɹ�
	m_isInitOK=false;
	//ͼ������
    m_isRGB=false;
	m_width=0;
	m_height=0;
    //�ڴ�ռ�
	m_pRGBImg=NULL;
	m_pGryImg=NULL;
	m_pGrdImg=NULL;
	m_pTmpImg=NULL;
	m_pResImg=NULL;
	m_pSumImg=NULL;
	m_memSize=0; //��������ڴ��ֽ���
	//�ڲ�������
	m_nFrameID=0;
}
	
CsrLocatePos::~CsrLocatePos()
{	//<�������������ͷ����е��ڴ�>
	Dump();
}

void CsrLocatePos::Dump()
{   //<дһ��ר�ŵ������ͷ��ڴ�ĺ���,��Ϊ�ڴ���ж�ε��������ͷ�>
	if (m_pRGBImg) { delete m_pRGBImg; m_pRGBImg=NULL; }
	if (m_pGryImg) { delete m_pGryImg; m_pGryImg=NULL; }
	if (m_pGrdImg) { delete m_pGrdImg; m_pGrdImg=NULL; }
	if (m_pTmpImg) { delete m_pTmpImg; m_pTmpImg=NULL; }
	if (m_pResImg) { delete m_pResImg; m_pResImg=NULL; }
	if (m_pSumImg) { delete m_pSumImg; m_pSumImg=NULL; }
	m_memSize=0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ʼ��
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CsrLocatePos::Initialize(bool isRGB,int width,int height,int nFast0123)
{   //<�ڳ�ʼ�������ж����еĳ�Ա��������ֵ>
	//<���ڳ�ʼ�������������ڴ�,��Ϊ��ʼ���������ᱻƵ������>
	//<��ʼ���������Ա���ε���>
	//<�ڴ�����ʱ,���Ѿ�������ڴ���ڱ�����Ҫ���ڴ�,���ٴ�����>

	//step.1------ͼ������--------------------------//
	m_isRGB=isRGB;
	m_width=width;
	m_height=height;
	nFast = min(3, max(0, nFast0123));
	m_nShrink = (int)(pow(2, nFast) + 0.5);
	//��С��ĳߴ�
	m_width = m_width / m_nShrink;
	m_height = m_height / m_nShrink;
	shrinkWidth = m_width;
	shrinkHeight = m_height;
	//step.2------�ڴ�����--------------------------//
	if (m_width*m_height>m_memSize)
	{
		//���ͷ�
		Dump();
		//������
		m_pRGBImg=new BYTE [m_width*m_height*3]; //��ɫͼ��
		m_pGryImg=new BYTE [m_width*m_height];  //�Ҷ�ͼ��
		m_pGrdImg=new BYTE [m_width*m_height]; //�ݶ�ͼ��
		m_pTmpImg=new BYTE [m_width*m_height]; //��ʱͼ��
		m_pResImg=new BYTE [m_width*m_height]; //���ͼ��
		m_pSumImg=new int[m_width*m_height];   //����ͼ
        //��¼����Ĵ�С
		m_memSize=m_width*m_height;
	}
	//step.3------��ʼ���ɹ���־--------------------//
	m_isInitOK=(m_pRGBImg && m_pGryImg && m_pGrdImg && m_pTmpImg && m_pResImg && m_pSumImg);
	return m_isInitOK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ִ��
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CsrLocatePos::DoNext(BYTE *pImgData)
{ 	
	if (!m_isInitOK) return false;
    //...
	//...
	//...
	//ĳ���ض�֡�ĵ���,����10
	if (m_nFrameID==10)
	{
		//Debug();
	}
	//�ڲ��������ۼ�
	m_nFrameID++;
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���
//
////////////////////////////////////////////////////////////////////////////////////////////////////
BYTE *CsrLocatePos::GetResImg()
{   //���ؽ��ͼ��
	return m_pResImg;
}
int CsrLocatePos::GetResWidth()
{   //���ؽ��ͼ��Ŀ��
	return m_width;
}
int CsrLocatePos::GetResHeight()
{   //���ؽ��ͼ��ĸ߶�
	return m_height;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CsrLocatePos::Debug(BYTE*pGryImg, int width, int height, const char * name)
{   //<������ڲ�����: ��Ļ������ļ������>
	//Rmw_Write8BitImg2BmpFileMark(pSbImg, width, height, "..\\0402Gry_RiceSobel.bmp");

	char str[256];
	sprintf_s(str, "%s", name);
	namedWindow(str, WINDOW_NORMAL);
	Mat pMat = Mat(height, width, CV_8UC1, pGryImg);
	imshow(str, pMat);
	waitKey(0);
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\DectectGry_Sobel.bmp");
	return;
}


void CsrLocatePos::CsrSobel(BYTE*pGryImg, int width, int height, BYTE *pResImg)
{
	int x, y;
	int dx, dy;
	BYTE *pCur = pGryImg + width;
	BYTE *pRes = pResImg + width;
	//���в���
	memset(pResImg, 0, sizeof(BYTE)*width);
	for (y = 1; y < height - 1; y++)
	{
		//���в���
		*(pRes++) = 0;
		pCur++;
		for (x = 1; x < width - 1; x++)
		{
			//1.��dx
			dx = *(pCur - 1 - width) + (*(pCur - 1) << 1) + *(pCur - 1 + width)
				- *(pCur + 1 - width) - (*(pCur + 1) << 1) - *(pCur + 1 + width);
			//2.��dy
			dy = *(pCur - 1 - width) + (*(pCur - width) << 1) + *(pCur + 1 - width)
				- *(pCur - 1 + width) - (*(pCur + width) << 1) - *(pCur + 1 + width);
			//3.���Եǿ��
			*(pRes++) = min(255, abs(dx) + abs(dy));
			*pCur++;
		}
		pCur++;
		//β�в���
		*(pRes++) = 0;
	}
	//β�в���
	memset(pRes, 0, sizeof(BYTE)*width);
}
//����,nFast_0123,������С2^n��
void CsrLocatePos::CsrReduceImgSize(BYTE*pGryImg, int width, int height, int nFast_0123, BYTE *pResImg)
{
	int x, y, scale, num;
	BYTE *pCur = pGryImg, *pRes = pResImg;
	nFast_0123 = max(0, min(3, nFast_0123));
	scale = (int)(pow(2, nFast_0123) + 0.5);
	//ÿ������ȡ��һ��
	for (y = 0; y < height; y += scale)
	{
		for (x = 0; x < width; x += scale)
		{
			*(pRes++) = *(pCur + x);
		}

		pCur += scale * width;

	}

}

void CsrLocatePos::CsrSearchMaxRectA(int* pSumImg, int width, int height, int w, int h, int delLR, int*xL, int*xR, int*yL, int*yR)
{
	int x, y, x1, x2;
	int *pY1, *pCur, *pY2, delH;
	int  sumC, sumL, sumR, sumU, maxVal, diff;
	maxVal = 0, y = 1;
	delH = delLR * width;//sumU����ĸ߶�
	pY1 = pSumImg + (y + delLR)*width;
	pY2 = pSumImg + (y + h + delLR)*width;
	for (y = 1; y < height - h - delLR; y++, pY1 += width, pY2 += width)
	{
		for (x = 1, x1 = x + delLR, x2 = x + w + delLR; x < width - w - delLR * 2; x++, x1++, x2++)
		{
			//-------step1.��������ݶ�ǿ��--------//
			sumC = *(pY2 + x2) - *(pY1 + x2) - *(pY2 + x1) + *(pY1 + x1);//Center���ݶ�ǿ��
			sumL = *(pY2 + x1) - *(pY2 + x1 - delLR) - *(pY1 + x1) + *(pY1 + x1 - delLR);
			sumR = *(pY2 + x2 + delLR) - *(pY2 + x2) - *(pY1 + x2 + delLR) + *(pY1 + x2);
			sumU = *(pY1 + x2) - *(pY1 + x1) - *(pY1 - delH + x2) + *(pY1 - delH + x1);
			diff = sumC - sumU - sumL - sumR;//�����ֵ
			//-------step2.��������--------------//
			if (diff > maxVal)
			{
				maxVal = diff;
				*xL = x1;//ֻ���¼���Ͻǵ�����
				*yL = y + delLR;
			}
		}
	}
	//�������Ͻ�����
	*xR = *xL + w;  *yR = *yL + h;
	return;
}


void CsrLocatePos::CsrSearchMaxRectLeftUp(int* pSumImg, int width, int height, int w, int h, int delLR, int*xL, int*xR, int*yL, int*yR)
{
	int x, y, x1, x2;
	int *pY1, *pCur, *pY2, delH;
	int  sumC, sumL, sumR, sumU, maxVal, diff;
	maxVal = 0, y = 1;
	delH = delLR * width;//sumU����ĸ߶�
	pY1 = pSumImg + (y + delLR)*width;
	pY2 = pSumImg + (y + h + delLR)*width;
	for (y = 1; y < height - h - delLR; y++, pY1 += width, pY2 += width)
	{
		for (x = 1, x1 = x + delLR, x2 = x + w + delLR; x < width - w - delLR * 2; x++, x1++, x2++)
		{
			//-------step1.��������ݶ�ǿ��--------//
			sumC = *(pY2 + x2) - *(pY1 + x2) - *(pY2 + x1) + *(pY1 + x1);//Center���ݶ�ǿ��
			sumL = *(pY2 + x1) - *(pY2 + x1 - delLR) - *(pY1 + x1) + *(pY1 + x1 - delLR);
			sumR = *(pY2 + x2 + delLR) - *(pY2 + x2) - *(pY1 + x2 + delLR) + *(pY1 + x2);
			sumU = *(pY1 + x2) - *(pY1 + x1) - *(pY1 - delH + x2) + *(pY1 - delH + x1);
			diff = sumC - sumU - sumL - sumR;//�����ֵ
											 //-------step2.��������--------------//
			if (diff > maxVal)
			{
				maxVal = diff;
				*xL = x1;//ֻ���¼���Ͻǵ�����
				*yL = y + delLR;
			}
		}
	}
	//�������Ͻ�����
	*xR = *xL + w;  *yR = *yL + h;
	return;
}



void CsrLocatePos::CsrDrawRect(BYTE*pGryImg, int width, int height, int& xL, int& xR, int& yL, int& yR, BYTE*pResImg)
{	
	BYTE *pGry;
	int x, y,gryVal = 255;
	//�ϱ߽�
	pGry = pResImg + yL * width;
	for (x = xL; x <= xR; x++) *(pGry + x) = gryVal;
	//�±߽�
	pGry += (yR-yL) * width;
	for (x = xL; x <= xR; x++) *(pGry + x) = gryVal;
	//��߽�
	pGry = pResImg + yL * width + xL;
	for (y = yL; y <= yR; y++, pGry += width) *(pGry) = gryVal;
	//�ұ߽�
	pGry = pResImg + yL * width + xR;
	for (y = yL; y <= yR; y++, pGry += width) *(pGry) = gryVal;

}

void CsrLocatePos::CsrGetSumImg(BYTE *pGryImg, int width, int height, int *pResImg)
{
	int x, y;
	BYTE*pCur = pGryImg;
	int *pRes = pResImg;
	int sumCol[4096];
	//��ʼ��
	memset(sumCol, 0, sizeof(int) * 4096);
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			sumCol[x] += *(pCur++);
			//����ͼ����
			if (!x)
			{
				*(pRes++) = sumCol[x];
			}
			else
			{
				*(pRes++) = sumCol[x] + *(pRes - 1);
			}
		}

	}

}

bool CsrLocatePos::CsrSearchRectTarget(BYTE*pGryImg, int width,int height, int targetW,int targetH,int delLR,BYTE *pResImg)
{
	int xL = width, xR = width, yL = height, yR = height;//��¼λ��

	//----------------step1.���ͷֱ���-------------------------------//
	CsrReduceImgSize(pGryImg, width, height, nFast, m_pResImg);

	//----------------step2.sobel���Եǿ��--------------------------//
	CsrSobel(m_pResImg, shrinkWidth, shrinkHeight, m_pGrdImg);
	Debug(m_pGrdImg, shrinkWidth, shrinkHeight, "m_pGrdImg");
	//----------------step3.��Եǿ�ȵĻ���ͼ-------------------------//
	CsrGetSumImg(m_pGrdImg, shrinkWidth, shrinkHeight, m_pSumImg);
	
	//----------------step4.�ɻ���ͼ��λ��������������λ-----------//
	CsrSearchMaxRectLeftUp(m_pSumImg, shrinkWidth, shrinkHeight, targetW,targetH,delLR, &xL, &xR, &yL, &yR);

	//----------------step5.��ԭͼ�ϻ�������-------------------------//
	xL = xL << nFast; xR = xR << nFast; 
	yL = yL << nFast; yR = yR << nFast;//ԭλ��
	if (xL < width&& xR < width && yL < height && yR < height)
	{
		CsrDrawRect(pGryImg, width, height, xL, xR, yL, yR, pResImg);
		return true;
	}
	else
		return false;

}
