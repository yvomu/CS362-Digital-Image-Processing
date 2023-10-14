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
	//擴展圖像矩陣，為2，3，5的倍數時運算速度快
	imshow("img", img);
	int m = getOptimalDFTSize(img.rows);
	int n = getOptimalDFTSize(img.cols);
	copyMakeBorder(img, img, 0, m - img.rows, 0, n - img.cols, BORDER_CONSTANT, Scalar::all(0));

	//創建一個雙通道矩陣planes，用來儲存複數的實部與虛部
	Mat planes[2] = { Mat_<float>(img), Mat::zeros(img.size(), CV_32F) };

	//從多個單通道數組中創建一個多通道數組:transform_image。函數Merge將幾個數組合併為一個多通道陣列，即輸出數組的每個元素將是輸入數組元素的級聯
	//Mat transform_image;
	merge(planes, 2, transform_image);

	//進行傅立葉變換
	dft(transform_image, transform_image);
	
	//計算複數的幅值，保存在output_image（頻譜圖）
	split(transform_image, planes);// 將雙通道分為兩個單通道，一個表示實部，一個表示虛部
	//Mat magnitude_image, phase_img;
	magnitude(planes[0], planes[1], magnitude_image);//計算複數的幅值


	//前面得到的頻譜圖數級過大，不好顯示，因此轉換
	magnitude_image += Scalar(1);
	log(magnitude_image, magnitude_image);
	normalize(magnitude_image, magnitude_image, 0, 1, NORM_MINMAX);
	
	//剪切和重分佈幅度圖像限
	magnitude_image = magnitude_image(Rect(0, 0, magnitude_image.cols & -2, magnitude_image.rows & -2));
	
	// 重新排列傅里葉圖像中的象限，使原點位於圖像中心
	/**/int cx = magnitude_image.cols / 2;
	int cy = magnitude_image.rows / 2;
	Mat q0(magnitude_image, Rect(0, 0, cx, cy));
	Mat q1(magnitude_image, Rect(cx, 0, cx, cy));
	Mat q2(magnitude_image, Rect(0, cy, cx, cy));
	Mat q3(magnitude_image, Rect(cx, cy, cx, cy));
	//交換象限中心化
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	// 重新排列transform_image的象限，使原點位於圖像中心
	Mat q0t(transform_image, Rect(0, 0, cx, cy));
	Mat q1t(transform_image, Rect(cx, 0, cx, cy));
	Mat q2t(transform_image, Rect(0, cy, cx, cy));
	Mat q3t(transform_image, Rect(cx, cy, cx, cy));
	//ransform_image交換象限中心化
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
		//對頻域圖像執行逆傅里葉變換，將圖像轉換回空域圖像。
		//使用 magnitude() 函數計算逆變換後的複數矩陣的幅值，以獲取還原後圖像的強度信息。
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