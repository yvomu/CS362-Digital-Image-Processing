#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat img = imread("yzu.bmp"); //��v��
int angle = 0;				 //�����i�Ϥ��Ϊ�����
int circleAngle = 0;         //�����ιϤ�������
Mat cirImage;                //�����U�Ӫ���ιϤ�
Mat mask = Mat::zeros(img.size(), CV_8UC1);   //���mask

void rotateImage(int, void*) {
	Mat rotated;				//����x�}
	Point2f center(img.cols / 2.0, img.rows / 2.0);		//��Ϥ���
	Mat M = getRotationMatrix2D(center, angle, 1.0);	//�p��X����x�}
	warpAffine(img, rotated, M, { img.size().width,img.size().height });	//����

	imshow("Image Rotation", rotated);
}

void rotateCirImage(int, void*) {

	Mat rotatedcir;				//����x�}
	Point2f circenter(cirImage.cols / 2.0, cirImage.rows / 2.0);		//��Ϥ���
	Mat cirM = getRotationMatrix2D(circenter, circleAngle, 1.0);	//�p��X����x�}
	warpAffine(cirImage, rotatedcir, cirM, { cirImage.size().width,cirImage.size().height });//����

	Mat rotatecirimg = img.clone();		//�ƻs�@�����(b)��
	rotatedcir.copyTo(rotatecirimg, mask);	//�����᪺������Ϥ��U���߶�γ���(���Ϫ�����)�K���ƻs�Ϥ���


	imshow("Circle Image Rotation", rotatecirimg);

}

int main() {
	cout << "opencv version:"<<(CV_VERSION)<<endl;
	if (!img.data)
	{
		cout << "Can't read the img" << endl;
		return 0;
	}

	namedWindow("Image Rotation");
	imshow("Image Rotation", img);

	createTrackbar("Angle", "Image Rotation", &angle, 360, rotateImage);   //�Ա�
	
	//�s�@���mask>��ι�
	Point center(img.cols / 2, img.rows / 2);	//�Ϥ�(��ߩҦb�a)
	int radius = min(img.cols, img.rows) / 2;   //�b�|(�Ϫ��̤p��)
	circle(mask, center, radius, Scalar(255), -1);  //�bmask�W�e�X�@�ӥզ��
	img.copyTo(cirImage, mask);					//�L�U�Ϥ������ꫬ�A������ι�

	imshow("Circle Image Rotation", img);
	
	createTrackbar("Angle", "Circle Image Rotation", &circleAngle, 360, rotateCirImage);  //(b)�����ΩԱ�
	
	waitKey(0);
	return 0;
}