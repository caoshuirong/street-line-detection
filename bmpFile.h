#ifndef BMP_FILE_H
#define BMP_FILE_H
//����ͨ����λͼ
BYTE *Rmw_Read8BitBmpFile2Img(const char * filename,int *width,int *height);
//д8λ��ͨ��ͼ
bool Rmw_Write8BitImg2BmpFile(BYTE *pImg,int width,int height,const char * filename);
//����ɫ��
bool Rmw_Write8BitImg2BmpFileMark(BYTE *pImg,int width,int height,const char * filename);
//��24λ��ͨ��
BYTE *Rmw_Read24BitBmpFile2Img(const char * filename,int *width,int *height);
//д24λ��ͨ��ͼ
bool Rmw_Write24BitImg2BmpFile(BYTE *pImg,int width,int height,const char * filename);
//����ֱ��ͼ
void RmwDrawHistogram2Img(int *histogram, int histSize,
	double maxScale,
	BYTE *pGryImg, int width, int height
);
#endif
