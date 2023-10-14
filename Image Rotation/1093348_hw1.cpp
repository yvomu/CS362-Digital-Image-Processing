#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat img = imread("yzu.bmp"); //原影像
int angle = 0;				 //旋轉整張圖片用的角度
int circleAngle = 0;         //旋轉圓形圖片的角度
Mat cirImage;                //裁切下來的圓形圖片
Mat mask = Mat::zeros(img.size(), CV_8UC1);   //圓形mask

void rotateImage(int, void*) {
	Mat rotated;				//旋轉矩陣
	Point2f center(img.cols / 2.0, img.rows / 2.0);		//找圖中心
	Mat M = getRotationMatrix2D(center, angle, 1.0);	//計算出旋轉矩陣
	warpAffine(img, rotated, M, { img.size().width,img.size().height });	//旋轉

	imshow("Image Rotation", rotated);
}

void rotateCirImage(int, void*) {

	Mat rotatedcir;				//旋轉矩陣
	Point2f circenter(cirImage.cols / 2.0, cirImage.rows / 2.0);		//找圖中心
	Mat cirM = getRotationMatrix2D(circenter, circleAngle, 1.0);	//計算出旋轉矩陣
	warpAffine(cirImage, rotatedcir, cirM, { cirImage.size().width,cirImage.size().height });//旋轉

	Mat rotatecirimg = img.clone();		//複製一份原圖(b)用
	rotatedcir.copyTo(rotatecirimg, mask);	//把選轉後的內切原圖切下中心圓形部分(有圖的部分)貼放到複製圖中間


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

	createTrackbar("Angle", "Image Rotation", &angle, 360, rotateImage);   //拉桿
	
	//製作圓形mask>圓形圖
	Point center(img.cols / 2, img.rows / 2);	//圖心(圓心所在地)
	int radius = min(img.cols, img.rows) / 2;   //半徑(圖的最小邊)
	circle(mask, center, radius, Scalar(255), -1);  //在mask上畫出一個白色圓
	img.copyTo(cirImage, mask);					//印下圖片中間圓型，做成圓形圖

	imshow("Circle Image Rotation", img);
	
	createTrackbar("Angle", "Circle Image Rotation", &circleAngle, 360, rotateCirImage);  //(b)部分用拉桿
	
	waitKey(0);
	return 0;
}