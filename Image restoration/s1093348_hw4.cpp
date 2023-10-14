#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
Mat transform_image, magnitude_image, phase_img;
void on_mouse(int EVENT, int x, int y, int flags, void* ustc);
bool click = 0;

int main() {
	Mat img = imread("image4.png", IMREAD_GRAYSCALE);
	//�X�i�Ϲ��x�}�A��2�A3�A5�����ƮɹB��t�ק�
	imshow("img", img);
	int m = getOptimalDFTSize(img.rows);
	int n = getOptimalDFTSize(img.cols);
	copyMakeBorder(img, img, 0, m - img.rows, 0, n - img.cols, BORDER_CONSTANT, Scalar::all(0));

	//�Ыؤ@�����q�D�x�}planes�A�Ψ��x�s�Ƽƪ��곡�P�곡
	Mat planes[2] = { Mat_<float>(img), Mat::zeros(img.size(), CV_32F) };

	//�q�h�ӳ�q�D�Ʋդ��Ыؤ@�Ӧh�q�D�Ʋ�:transform_image�C���Merge�N�X�ӼƲզX�֬��@�Ӧh�q�D�}�C�A�Y��X�Ʋժ��C�Ӥ����N�O��J�Ʋդ��������p
	//Mat transform_image;
	merge(planes, 2, transform_image);

	//�i��ť߸��ܴ�
	dft(transform_image, transform_image);
	
	//�p��Ƽƪ��T�ȡA�O�s�boutput_image�]�W�йϡ^
	split(transform_image, planes);// �N���q�D������ӳ�q�D�A�@�Ӫ�ܹ곡�A�@�Ӫ�ܵ곡
	//Mat magnitude_image, phase_img;
	magnitude(planes[0], planes[1], magnitude_image);//�p��Ƽƪ��T��


	//�e���o�쪺�W�йϼƯŹL�j�A���n��ܡA�]���ഫ
	magnitude_image += Scalar(1);
	log(magnitude_image, magnitude_image);
	normalize(magnitude_image, magnitude_image, 0, 1, NORM_MINMAX);
	
	//�Ť��M�����G�T�׹Ϲ���
	magnitude_image = magnitude_image(Rect(0, 0, magnitude_image.cols & -2, magnitude_image.rows & -2));
	
	// ���s�ƦC�Ũ����Ϲ������H���A�ϭ��I���Ϲ�����
	/**/int cx = magnitude_image.cols / 2;
	int cy = magnitude_image.rows / 2;
	Mat q0(magnitude_image, Rect(0, 0, cx, cy));
	Mat q1(magnitude_image, Rect(cx, 0, cx, cy));
	Mat q2(magnitude_image, Rect(0, cy, cx, cy));
	Mat q3(magnitude_image, Rect(cx, cy, cx, cy));
	//�洫�H�����ߤ�
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	// ���s�ƦCtransform_image���H���A�ϭ��I���Ϲ�����
	Mat q0t(transform_image, Rect(0, 0, cx, cy));
	Mat q1t(transform_image, Rect(cx, 0, cx, cy));
	Mat q2t(transform_image, Rect(0, cy, cx, cy));
	Mat q3t(transform_image, Rect(cx, cy, cx, cy));
	//ransform_image�洫�H�����ߤ�
	q0t.copyTo(tmp);
	q3t.copyTo(q0t);
	tmp.copyTo(q3t);
	q1t.copyTo(tmp);
	q2t.copyTo(q1t);
	tmp.copyTo(q2t);
	
	//Mat transform_image_copy = transform_image.clone();
	//circle(transform_image_copy, Point(796, 384), 10, (0, 0, 0), -1);
	//circle(magnitude_image, Point(796, 384), 10, (0, 0, 0), -1);

	//circle(transform_image_copy, Point(807, 418), 10, (0, 0, 0), -1);
	//circle(magnitude_image, Point(807, 418), 10, (0, 0, 0), -1);

	//circle(transform_image_copy, Point(790, 448), 10, (0, 0, 0), -1);
	//circle(magnitude_image, Point(790, 448), 10, (0, 0, 0), -1);

	//circle(transform_image_copy, Point(magnitude_image.cols - 796, magnitude_image.rows - 384), 10, (0, 0, 0), -1);
	//circle(magnitude_image, Point(magnitude_image.cols - 796, magnitude_image.rows-384), 10, (0, 0, 0), -1);

	//circle(transform_image_copy, Point(magnitude_image.cols - 807, magnitude_image.rows - 418), 10, (0, 0, 0), -1);
	//circle(magnitude_image, Point(magnitude_image.cols - 807, magnitude_image.rows-418), 10, (0, 0, 0), -1);

	//circle(transform_image_copy, Point(magnitude_image.cols - 790, magnitude_image.rows - 448), 10, (0, 0, 0), -1);
	//circle(magnitude_image, Point(magnitude_image.cols - 790, magnitude_image.rows-448), 10, (0, 0, 0), -1);/**/
	//
	////imshow("transform_image_copy", transform_image_copy);
	
		imshow("magnitude_image", magnitude_image);
		setMouseCallback("magnitude_image", on_mouse);
		waitKey(0);
		q0t.copyTo(tmp);
		q3t.copyTo(q0t);
		tmp.copyTo(q3t);
		q1t.copyTo(tmp);
		q2t.copyTo(q1t);
		tmp.copyTo(q2t);
		//���W��Ϲ�����f�Ũ����ܴ��A�N�Ϲ��ഫ�^�Ű�Ϲ��C
		//�ϥ� magnitude() ��ƭp��f�ܴ��᪺�ƼƯx�}���T�ȡA�H����٭��Ϲ����j�׫H���C
		Mat restoredImg;
		idft(transform_image, restoredImg);
		split(restoredImg, planes);
		magnitude(planes[0], planes[1], restoredImg);
		normalize(restoredImg, restoredImg, 0, 1, NORM_MINMAX);
		imshow("restoredImg", restoredImg);
		waitKey(0);
	//while (1) {
	//	if (waitKey(10) == 'q') {
	//		break;
	//	}
	//	/*if (waitKey(10) == 'b') {
	//		continue;
	//	}*/
	//}
	

	return 0;
}

void on_mouse(int EVENT, int x, int y, int flags, void* ustc) {

	if (EVENT == 1) {
		click = 1;
		circle(transform_image, Point(x, y), 3, (0, 0, 0), -1);
		circle(magnitude_image, Point(x, y), 3, (0, 0, 0), -1);
		imshow("magnitude_image", magnitude_image);
	}
	if (EVENT == 0 && click == 1) {
		circle(transform_image, Point(x, y), 3, (0,0, 0), -1);
		circle(magnitude_image, Point(x, y), 3, (0, 0, 0), -1);
		imshow("magnitude_image", magnitude_image);
	}
	if (EVENT == 4) {
		click = 0;
	}
}