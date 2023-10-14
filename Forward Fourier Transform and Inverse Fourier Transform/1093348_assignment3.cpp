#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main() {
	Mat img = imread("image3.png", IMREAD_GRAYSCALE);
	//1.�X�i�Ϲ��x�}�A��2�A3�A5�����ƮɹB��t�ק�
	int m = getOptimalDFTSize(img.rows);
	int n = getOptimalDFTSize(img.cols);
	copyMakeBorder(img, img, 0, m - img.rows, 0, n - img.cols, BORDER_CONSTANT, Scalar::all(0));
	
	//2.�Ыؤ@�����q�D�x�}planes�A�Ψ��x�s�Ƽƪ��곡�P�곡
	Mat planes[2] = { Mat_<float>(img), Mat::zeros(img.size(), CV_32F) };
	
	//3.�q�h�ӳ�q�D�Ʋդ��Ыؤ@�Ӧh�q�D�Ʋ�:transform_image�C���Merge�N�X�ӼƲզX�֬��@�Ӧh�q�D�}�C�A�Y��X�Ʋժ��C�Ӥ����N�O��J�Ʋդ��������p
	Mat transform_image;
	merge(planes, 2, transform_image);
	
	//4.�i��ť߸��ܴ�
	dft(transform_image, transform_image);
	
	//5.�p��Ƽƪ��T�ȡA�O�s�boutput_image�]�W�йϡ^
	split(transform_image, planes);// �N���q�D������ӳ�q�D�A�@�Ӫ�ܹ곡�A�@�Ӫ�ܵ곡
	Mat magnitude_image, phase_img;
	magnitude(planes[0], planes[1], magnitude_image);//�p��Ƽƪ��T��
	phase(planes[0], planes[1], phase_img);
	
	
	//6.�e���o�쪺�W�йϼƯŹL�j�A���n��ܡA�]���ഫ
	magnitude_image += Scalar(1);
	log(magnitude_image, magnitude_image);
	normalize(magnitude_image, magnitude_image, 0, 1, NORM_MINMAX);
	/*phase_img += Scalar(1);
	log(phase_img, phase_img);*/
	normalize(phase_img, phase_img, 0, 1, NORM_MINMAX);
	
	//7.�Ť��M�����G�T�׹Ϲ���
	magnitude_image = magnitude_image(Rect(0, 0, magnitude_image.cols & -2, magnitude_image.rows & -2));
	phase_img = phase_img(Rect(0, 0, phase_img.cols & -2, phase_img.rows & -2));
	// ���s�ƦC�Ũ����Ϲ������H���A�ϭ��I���Ϲ�����
	/**/int cx = magnitude_image.cols / 2;
	int cy = magnitude_image.rows / 2;
	Mat q0(magnitude_image, Rect(0, 0, cx, cy));
	Mat q1(magnitude_image, Rect(cx, 0, cx, cy));
	Mat q2(magnitude_image, Rect(0, cy, cx, cy));
	Mat q3(magnitude_image, Rect(cx, cy, cx, cy));
	Mat q0p(phase_img, Rect(0, 0, cx, cy));
	Mat q1p(phase_img, Rect(cx, 0, cx, cy));
	Mat q2p(phase_img, Rect(0, cy, cx, cy));
	Mat q3p(phase_img, Rect(cx, cy, cx, cy));
	//�洫�H�����ߤ�
	Mat tmp;
	q0.copyTo(tmp); 
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
	Mat tmpp;
	q0p.copyTo(tmpp);
	q3p.copyTo(q0p);
	tmpp.copyTo(q3p);
	q1p.copyTo(tmpp);
	q2p.copyTo(q1p);
	tmpp.copyTo(q2p);
	
	imshow("magnitude_image", magnitude_image);
	imshow("phase", phase_img);

	//���W��Ϲ�����f�Ũ����ܴ��A�N�Ϲ��ഫ�^�Ű�Ϲ��C
	//�ϥ� magnitude() ��ƭp��f�ܴ��᪺�ƼƯx�}���T�ȡA�H����٭��Ϲ����j�׫H���C
	Mat restoredImg;
	idft(transform_image, restoredImg);
	split(restoredImg, planes); 
	magnitude(planes[0], planes[1], restoredImg);
	normalize(restoredImg, restoredImg, 0, 1, NORM_MINMAX);
	imshow("restoredImg", restoredImg);
	waitKey(0);
	
	return 0;
}