#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
Mat img = imread("fig.jpg", IMREAD_GRAYSCALE);

int main() {

	Mat sobelx,sobely, blurred, result, edges,absx,absy;
	//imshow("img", img);
	
	// ���u��
	//��sobel���O��Ϲ���x��V�My��V�i��p��
	Sobel(img, sobelx, CV_16S, 1, 0);
	Sobel(img, sobely, CV_16S, 0, 1);
	//�N�Ϲ��ഫ��8��L�Ÿ��㫬�Ϲ��A�_�h�L�k���
	convertScaleAbs(sobelx, absx);
	convertScaleAbs(sobely, absy);
	//�N��Ӥ�V�����G�i��[�v�X��
	addWeighted(absx, 0.5, absy, 0.5, 10, edges);
	imshow("edges", edges);

	//���y��
	Mat blurbitwise;
	//�����ҽk
	GaussianBlur(edges, blurred, Size(3,3), 0);
	//�G�ȤơATHRESH_TOZERO:�s�H�������A�����Ȥj��ε����H�ȮɡA�O�����ܡA�_�h�N��]�m��0
	threshold(blurred, result, 85, 255, THRESH_TOZERO);
	//�¥����
	bitwise_not(result, result);
	imshow("result", result);
	waitKey(0);
	return 0;
}