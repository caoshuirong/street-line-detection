///////////////////////////////////////////////////////////////////////////////////
//
//
//��ֱ��ͼ
//
//
///////////////////////////////////////////////////////////////////////////////////
void RmwDrawHistogram2Img( int *histogram, int histSize, 
	                       double maxScale,
	                       BYTE *pGryImg, int width, int height
                         )
{   //ֱ��ͼ���ݻ�Ϊ��ɫ
	int x1, x2, y1;
	int maxV, i;
	int x, y;

	// step.1-------------��ʼ��----------------------------//
	memset(pGryImg, 225, width*height);
	// step.2-------------Ѱ�����ֵ------------------------//
	maxV = 0;
	for (i = 0; i<histSize; i++)
	{
		maxV = max(maxV, histogram[i]);
	}
	maxV = (int)(maxV*maxScale);
	// step.3-------------�������--------------------------//
	for (i = 0; i<histSize; i++)
	{
		y1 = (height-1)-histogram[i]*(height-1)/(maxV+1);
		y1 = max(0, y1);
		x1 = i*width/histSize;
		x2 = (i+1)*width/histSize;
		for (x = x1; x<x2; x++)
		{
			for (y = height-1; y>y1; y--) *(pGryImg+y*width+x) = 0;
		}
	}
	// step.4-------------����------------------------------//
	return;
}
