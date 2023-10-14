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
	
	// 取線條
	//做sobel分別對圖像的x方向和y方向進行計算
	Sobel(img, sobelx, CV_16S, 1, 0);
	Sobel(img, sobely, CV_16S, 0, 1);
	//將圖像轉換為8位無符號整型圖像，否則無法顯示
	convertScaleAbs(sobelx, absx);
	convertScaleAbs(sobely, absy);
	//將兩個方向的結果進行加權合併
	addWeighted(absx, 0.5, absy, 0.5, 10, edges);
	imshow("edges", edges);

	//素描風
	Mat blurbitwise;
	//高斯模糊
	GaussianBlur(edges, blurred, Size(3,3), 0);
	//二值化，THRESH_TOZERO:零閾值類型，當像素值大於或等於閾值時，保持不變，否則將其設置為0
	threshold(blurred, result, 85, 255, THRESH_TOZERO);
	//黑白轉化
	bitwise_not(result, result);
	imshow("result", result);
	waitKey(0);
	return 0;
}