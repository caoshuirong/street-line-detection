// CsrLocatePos.h : header file
//
#ifndef CSR_GET_POS_H
#define CSR_GET_POS_H
#include"bmpFile.h"
#include<opencv2/opencv.hpp>
using namespace cv;
class CsrLocatePos
{
public:
	//����/����
	CsrLocatePos();
	~CsrLocatePos();
    //��ʼ��
    bool Initialize(bool isRGB,int width,int height,int nFast0123);
	//ִ��
	bool DoNext(BYTE *pImgData);
    //���
    BYTE *GetResImg();
	int GetResWidth();
    int GetResHeight();
	void CsrSobel(BYTE*pGryImg, int width, int height, BYTE *pResImg);
	void CsrGetSumImg(BYTE *pGryImg, int width, int height, int *pResImg);
	void CsrReduceImgSize(BYTE*pGryImg, int width, int height, int nFast_0123, BYTE *pResImg);
	//������
	void CsrSearchMaxRectA(int* pSumImg, int width, int height, int w, int h, int delLR, int*xL, int*xR, int*yL, int*yR);
	//����
	void CsrSearchMaxRectLeftUp(int* pSumImg, int width, int height, int w, int h, int delLR, int*xL, int*xR, int*yL, int*yR);
	void CsrDrawRect(BYTE*pGryImg, int width, int height, int& xL, int& xR, int& yL, int& yR, BYTE*pResImg);
	bool CsrSearchRectTarget(BYTE*pGryImg, int width, int height, int targetW, int targetH, int delLR, BYTE *pResImg);
private:
	// �ڴ��ͷ�
	void Dump();
	// ����
	void Debug();
	void Debug(BYTE*pGryImg, int width, int height, const char * name);
private:
	//��ʼ���ɹ�
	bool m_isInitOK;
	//ͼ������
	bool m_isRGB;
	int m_width;
	int m_height;
	int nFast;
	int m_nShrink;
	int shrinkWidth;
	int shrinkHeight;
	//�ڴ�
	BYTE *m_pRGBImg;
	BYTE *m_pGryImg;
	BYTE *m_pGrdImg;
	BYTE *m_pTmpImg;
	BYTE *m_pResImg;
	int * m_pSumImg;
	int m_memSize;
	//�ڲ�������
	int m_nFrameID;
};
#endif
