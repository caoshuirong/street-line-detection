#include"CsrALG_C3.h"
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void RmwAvrFilter2(BYTE *pGrylmg, //ԭʼ�Ҷ�ͼ��
	int *pSumImg, //����õ��Ļ���ͼ
	int width, int height,//ͼ��Ŀ�Ⱥ͸߶�
	int M, int N, //�˲�����M��N��
	BYTE *pResImg //���ͼ��
)
{
	int *pYl, *pY2;
	BYTE *pRes;
	int halfx, halfy;
	int x, y, x1, x2;
	int sum, c;
	// step.1------------��ʼ��---------------------------//
	M = M/2 * 2 + 1; //������
	N = N/2 * 2 + 1; //������
	halfx = M/2; //�˲����İ뾶x
	halfy = N/2; //�˲����İ뾶y
	c = (1 << 23) / (M*N);//�˷�����
	// step.2------------�˲�----------------------------//
	
	y = halfy + 1 ,pRes = pResImg + y * width, pYl = pSumImg, pY2 = pSumImg + N * width;
	for (;y < height - halfy;	y++, pYl += width, pY2 += width)
	{
		pRes += halfx + 1; //�������
		for (x = halfx + 1, x1 = 0, x2 = M; x < width - halfx; x++, x1++, x2++)
			//for(x1 = 0, x2 = M; x2<width; xl++, x2++) //��һ�п��Լ���ˣ�����̫���׶�
		{
			sum = *(pY2 + x2) - *(pY2 + x1) - *(pYl + x2) + *(pYl + x1); 
			
			*(pRes++) = (sum*c) >> 23; //�������˷�����λ�������
		}
		pRes += halfx;//�����Ҳ�

	}
	return;
}
void CsrAvgRowFilter(BYTE*pGryImg, BYTE *pSumImg, int width, int height,int N,int M , BYTE *pResImg)
{
	int halfx, halfy,y,x,c,sum;
	int x1, x2;
	BYTE *pRes = pResImg;
	N = N / 2 * 2 + 1;
	M = M / 2 * 2 + 1;
	halfx = M / 2 ;
	c = (1 << 23) / (M*N);
	memcpy(pResImg, pGryImg, width*height);
	for (y = 0; y < height; y++, pSumImg += width)
	{
		pRes += halfx+1;//������ࣻ
		for (x = halfx + 1,x1 = 0,x2 = M; x < width - halfx; x++,x1++,x2++)
		{
			sum = *(pSumImg + x2) - *(pSumImg + x1);

			*(pRes++) = ((sum*c) >> 23);
		}
		pRes += halfx;//�����Ҳ�

	}


}


//�����к͵�һά��ֵ�˲�
void CsrSumColAvgFilter(BYTE*pGryImg, int width, int height, int M, BYTE *pResImg)
{
	int x, y, halfx;
	int sum;
	M = M / 2 * 2 + 1;
	halfx = M / 2;
	int C = (1 << 23) / M;
	BYTE *pCur = pGryImg, *pRes = pResImg;
	for (y = 0; y < height; y++)
	{
		//----------step1.��ʼ�������ܺ�----------//
		sum = 0;
		for (x = 0; x < M; x++)
		{
			sum += *(pCur + x);
		}
		//----------step2.���ֵ-----------------//
		pRes += halfx;//������಻��������
		for (x = halfx; x < width - halfx ; x++)
		{
			*(pRes++) = (sum*C) >> 23;
			sum -= *(pCur + x - halfx);//��ȥ��ߵ��к�
			sum += *(pCur + x + halfx + 1);//�����ұߵ��к�
		}
		pRes += halfx;//�����Ҳ಻��������
		pCur += width;//����һ��
	}

}

//�л���ͼ
void CsrGetRowSumImg(BYTE *pGryImg, int width, int height, int *pResImg)
{
	int x, y;
	BYTE*pCur = pGryImg;
	int *pRes = pResImg;//  int*
	int sumCol = 0;
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			sumCol = *(pCur++);//�����к�
			if (!x)
			{
				*(pRes++) = sumCol;
			}
			else
			{
				*(pRes++) = sumCol + *(pRes - 1);//����
			}
		}

	}

}
//�о�ֵ�˲�
void CsrSumImgRowAvgFilter( int *pSumImg, int width, int height,int M, BYTE *pResImg)
{
	int x, y, halfx,sum,c;
	int x1, x2;
	BYTE *pRes = pResImg;
	int*pY = pSumImg;
	M = M / 2 * 2 + 1;
	halfx = M / 2;
	c = (1 << 23) / M;//�˷�����
	for (y = 0; y <height ; y++)
	{
		pRes += halfx+1;//������಻��������
		for (x = halfx+1,x1 = 0,x2 = M; x <width - halfx ; x++,x1++,x2++)
		{
			sum = *(pY + x2) - *(pY + x1);
			*(pRes++) = (sum*c) >> 23;

		}
		pRes += halfx;//�����Ҳ಻��������
		pY += width;//����ͼָ�뻻��
	}
}

//ͨ�û���ͼ
void CsrGetSumImg(BYTE *pGryImg, int width, int height, int *pResImg)
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


void CsrRgb2Gray_Factor(BYTE*pRgbImg, int width, int height, BYTE *pResImg)
{
	BYTE* pCur, *pEnd,pRes;
	memset(pResImg, 0, sizeof(BYTE)*width * height);
	// step.1----------���ɳ˷�����--------------------------//
	int cB = 0.114 * (1<<10);
	int cG = 0.587 * (1<<10);
	int cR = 0.299 * (1<<10);

	// step.2----------RGBתGray---------------------------//
	pEnd = pRgbImg + width * height * 3;
	for (pCur = pRgbImg; pCur < pEnd;)
	{
		//��λ���ȼ��ȼӷ����ͣ���������ţ�
		*(pResImg++) = (*(pCur) * cB + *(pCur+1) * cG + *(pCur+2) * cR)>>10;
		pCur += 3;
	}

}
void CsrRgb2Gray_Low(BYTE*pRgbImg, int width, int height, BYTE *pResImg)
{
	BYTE* pCur, *pEnd, pRes;
	memset(pResImg, 0, sizeof(BYTE)*width * height);
	// step.1----------RGBתGray---------------------------//
	pEnd = pRgbImg + width * height * 3;
	for (pCur = pRgbImg; pCur < pEnd;)
	{
		*(pResImg++) = (*(pCur)* 0.114) + (*(pCur+1)* 0.587) + (*(pCur+2)* 0.299);
		pCur += 3;
	}

}

void CsrInvertPhaseSSE(BYTE*pGryImg, int width, int height)
{
	/*_declspec(align(16)) BYTE sumCol[16];//һ����˵��_declspec(align(16)) �Ա�������ʮ���ֽڶ���*/
	if ((unsigned int)pGryImg % 16 != 0)
	{
		printf("\nͼƬ�׵�ַ��������16�ı�����\n");
		return;
	}
	if (width*height % 16 != 0)
	{
		printf("\nͼƬ��������������16�ı�����\n");
		return;
	}
	__m128i subFactor,A;
	memset(&subFactor, -1, sizeof(__m128i));
	BYTE *pCur = pGryImg, *pEnd=pCur+width*height;
	//-------------һ��16�����ط���-----------------//
	for (; pCur < pEnd; pCur += 16)
	{
					  A = _mm_loadu_si128((__m128i*)pCur);
		*(__m128i*)pCur = _mm_subs_epu8(subFactor, A);

	}
}

double CsrMedianFilter(BYTE*pGryImg, int width, int height, int N, int M, BYTE *pResImg)
{
	BYTE *pCur, *pRes;
	int i, j, g, x, y, y1, y2;
	int histogram[256];
	int wSize, left, right;
	int num = 0, med, v, halfx, halfy;
	int dbgCmpTimes = 0;

	//����������
	N = N / 2 * 2 + 1;
	M = M / 2 * 2 + 1;
	halfx = M / 2, halfy = N / 2;
	//�����������ܸ���
	wSize = M * N;
	//---------------step1.��ʼ��ֱ��ͼ
	for (y = halfy, pRes = pResImg + y * width; y < height - halfy; y++)
	{
		y1 = y - halfy;
		y2 = y + halfy;
		memset(histogram, 0, sizeof(int) * 256);
		for (i = y1, pCur = pGryImg + i * width; i <= y2; i++, pCur += width)
			for (j = 0; j < M; j++)
				histogram[*(pCur + j)]++;

		//step2,��ʼ����ֵ
		num = 0;
		for (g = 0; g < 256; g++)
		{
			num += histogram[g];
			if (2 * num > wSize)
			{
				med = g;
				break;
			}
		}
		//�˲�
		pRes += halfx;
		for (x = halfx; x < width - halfx; x++)
		{
			//��������ֵ
			*(pRes++) = med;
			//step3.ֱ��ͼ����
			left = x - halfx;
			right = x + halfx + 1;
			for (i = y1, pCur = pGryImg + i * width; i <= y2; i++, pCur += width)
			{
				g = *(pCur + left);
				histogram[g]--;
				if (g <= med)
					num--;
				g = *(pCur + right);
				histogram[g]++;
				if (g <= med)
					num++;

			}
			if (2 * num < wSize)
			{
				//numС�ˣ�ԭ����med�϶�����,med�����ơ�
				//����num�������٣�numֻ��˵�ӻҶ�ֵ0���ӵ�med�������ظ���.
				for (med = med + 1; med < 256; med++)
				{
					num += histogram[med];
					dbgCmpTimes += 2;
					if (2 * num > wSize)
					{
						break;
					}
				}
				dbgCmpTimes++;
			}
			else
			{
				while (2 * (num - histogram[med]) > wSize)
				{
					dbgCmpTimes++;
					num -= histogram[med];
					med--;
				}
				dbgCmpTimes += 2;
			}
		}

		pRes += halfx;

	}
	return dbgCmpTimes * 1.0 / ((width - halfx * 2)*(height - halfy * 2));

}

double CsrMinFilter(BYTE*pGryImg, int width, int height, int N, int M, BYTE *pResImg)
{
	BYTE *pCur, *pRes;
	int i, j, g, x, y, y1, y2;
	int histogram[256];
	int wSize, left, right;
	int num = 0, min, v, halfx, halfy;
	int dbgCmpTimes = 0;

	//����������
	N = N / 2 * 2 + 1;
	M = M / 2 * 2 + 1;
	halfx = M / 2, halfy = N / 2;
	//�����������ܸ���
	wSize = M * N;
	//---------------step1.��ʼ��ֱ��ͼ
	for (y = halfy, pRes = pResImg + y * width; y < height - halfy; y++)
	{
		y1 = y - halfy;
		y2 = y + halfy;
		memset(histogram, 0, sizeof(int) * 256);
		for (i = y1, pCur = pGryImg + i * width; i <= y2; i++, pCur += width)
			for (j = 0; j < M; j++)
				histogram[*(pCur + j)]++;

		//step2,��ʼ����Сֵ
		num = 0;
		for (g = 0; g < 256; g++)
		{
			num += histogram[g];
			if (num)
			{
				min = g;
				break;
			}
		}
		//�˲�
		pRes += halfx;
		for (x = halfx; x < width - halfx; x++)
		{
			//��������ֵ
			*(pRes++) = min;
			//step3.ֱ��ͼ����
			left = x - halfx;
			right = x + halfx + 1;
			for (i = y1, pCur = pGryImg + i * width; i <= y2; i++, pCur += width)
			{
				g = *(pCur + left);
				histogram[g]--;
				if (g == min)//��ȥ��min��ģ�min���ܻ���
					num--;
				g = *(pCur + right);
				histogram[g]++;
				if (g <= min)//���ϱ�minС�ģ�������
					num++;

			}
			if (num <= 0 )//��num����С��
			{
				//numС�ˣ�ԭ����med�϶�����,med�����ơ�
				//����num�������٣�numֻ��˵�ӻҶ�ֵ0���ӵ�med�������ظ���.
				for (min = min + 1; min < 256; min++)
				{
					num += histogram[min];
					dbgCmpTimes += 2;
					if ( num > 0)
					{
						break;
					}
				}
				dbgCmpTimes++;
			}
			else
			{
				while ( num - histogram[min] > 0)
				{
					dbgCmpTimes++;
					num -= histogram[min];
					min--;
				}
				dbgCmpTimes += 2;
			}
		}

		pRes += halfx;

	}
	return dbgCmpTimes * 1.0 / ((width - halfx * 2)*(height - halfy * 2));

}



double CsrMinFilter_Low(BYTE*pGryImg, int width, int height, int N, int M, BYTE *pResImg)
{
	BYTE *pCur, *pRes;
	int i, j, g, x, y, y1, y2;
	int histogram[256];
	int wSize, left, right;
	int num = 0, min, halfx, halfy;
	unsigned int count = 0;
	//������ʼ��
	N = N / 2 * 2 + 1; M = M / 2 * 2 + 1;
	halfx = M / 2; halfy = N / 2;
	pRes = pResImg + halfy * width;

	for (y = halfy; y < height - halfy; y++)
	{
		//---------------step1.��ʼ��ֱ��ͼ------------------//
		y1 = y - halfy;
		y2 = y + halfy;
		memset(histogram, 0, sizeof(int) * 256);
		for (i = y1, pCur = pGryImg + i * width; i <= y2; i++, pCur += width)
			for (j = 0; j < M; j++)
				histogram[*(pCur + j)]++;

		//---------------step2.��ʼ����Сֵ------------------//
		for (g = 0; g < 256; g++)
		{
			if (histogram[g])
			{
				min = g;
				break;
			}
		}
		//---------------step3.��Сֵ�˲�-------------------//
		pRes += halfx;
		for (x = halfx; x < width - halfx; x++)
		{
			*(pRes++) = min;//��������ֵ
			left = x - halfx;
			right = x + halfx + 1;
			for (i = y1, pCur = pGryImg + i * width; i <= y2; i++)
			{
				histogram[*(pCur + left)]--;//��ȥ���һ��
				histogram[*(pCur + right)]++;//�����ұ�һ��
				pCur += width;
			}
			for (g = 0; g < 256; g++)//��������Сֵ
			{
				count++;//��¼�Ƚϴ���
				if (histogram[g])
				{
					min = g;
					break;
				}
			}
		}
		pRes += halfx;
	}
	return count*1.0/((width-M)*(height - N));
}

double CsrMinFilterALG(BYTE*pGryImg, int width, int height, int N, int M, BYTE *pResImg)
{
	BYTE *pCur, *pRes;
	int i, j, g, x, y, y1, y2;
	int histogram[256];
	int left, right;
	int min,halfx, halfy;
	unsigned int count = 0;//��¼�Ƚϴ���
	//����������
	N = N / 2 * 2 + 1;	M = M / 2 * 2 + 1;
	halfx = M / 2;	halfy = N / 2;
	pRes = pResImg + halfy * width;
	for (y = halfy; y < height - halfy; y++)
	{
		//---------------step1.��ʼ��ֱ��ͼ------------------//
		y1 = y - halfy;
		y2 = y + halfy;
		memset(histogram, 0, sizeof(int) * 256);
		for (i = y1, pCur = pGryImg + i * width; i <= y2; i++, pCur += width)
			for (j = 0; j < M; j++)
				histogram[*(pCur + j)]++;
		//---------------step2.��ʼ����Сֵ------------------//
		for (g = 0; g < 256; g++)
		{
			if (histogram[g])
			{
				min = g;
				break;
			}
		}
		//---------------step3.��Сֵ�˲�-------------------//
		pRes += halfx;
		for (x = halfx; x < width - halfx; x++)
		{
			*(pRes++) = min;//��������ֵ
			left = x - halfx;
			right = x + halfx + 1;
			//-----------ֱ��ͼ����----------//
			for (i = y1, pCur = pGryImg + i * width; i <= y2; i++,pCur += width)
			{
				g = *(pCur + left);
				histogram[g]--;
				while (histogram[min] < 1)
				{
					count++;
					min++;
				}
				g = *(pCur + right);
				histogram[g]++;
				if (g < min) min = g;
				count += 2;
			}
		}
		pRes += halfx;
	}
	//---------------step4.���رȽϴ���-------------------//
	return count * 1.0 / ((width - M)*(height - N));
}




//Mat pSrc = imread("..\\0304Gry.bmp");

void Gaussian()
{
	int g, LUT1[256], LUT2[256],LUT3[256],LUT4[256];
	int T = (0.0111 * 2 + 0.1353 * 2 + 0.6065 * 2 + 1.0);
	int C1 = 0.0111 / T, C2 = 0.1353 / T, C3 = 0.6065 / T, C4 = 1.0 / T;
	for (g = 0 ;g < 256;g++)
	{
		LUT1[g] = C1 * g;
		LUT2[g] = C2 * g;
		LUT3[g] = C3 * g;
		LUT4[g] = C4 * g;
	}
	

}