#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int h_min = 4, h_max = 19, s_min = 43, s_max = 231, v_min = 35, v_max = 255;

// trackbar 的回調函數
//void onTrackbarChange(int value, void* userdata)
//{
//     在這裡實現 trackbar 值變化時的處理邏輯
//    cout << "Trackbar value: " << value << endl;
//}
int main()
{
    // 讀取輸入圖片
    Mat image1 = imread("img1.jpg");
    Mat image2 = imread("img2.jpg");
    Mat image3 = imread("img3.jpg");
    resize(image1, image1, Size(image1.cols / 5, image1.rows/5));
    resize(image2, image2, Size(image2.cols / 5, image2.rows / 5));
    resize(image3, image3, Size(image3.cols / 5, image3.rows / 5));
    imshow("Original Image1", image1);
    imshow("Original image2", image2);
    imshow("Original image3", image3);
    
    // 將圖片轉換為 HSV 色彩空間
    Mat hsvImage1, hsvImage2, hsvImage3;
    cvtColor(image1, hsvImage1, COLOR_BGR2HSV);
    cvtColor(image2, hsvImage2, COLOR_BGR2HSV);
    cvtColor(image3, hsvImage3, COLOR_BGR2HSV);
 
    // 創建一個 trackbar視窗
   /* namedWindow("Trackbar Window");
    createTrackbar("Hue Min", "Trackbar Window", &h_min, 179, onTrackbarChange);
    createTrackbar("Hue Max", "Trackbar Window", &h_max, 179, onTrackbarChange);
    createTrackbar("Sat Min", "Trackbar Window", &s_min, 255, onTrackbarChange);
    createTrackbar("Sat Max", "Trackbar Window", &s_max, 255, onTrackbarChange);
    createTrackbar("Val Min", "Trackbar Window", &v_min, 255, onTrackbarChange);
    createTrackbar("Val Max", "Trackbar Window", &v_max, 255, onTrackbarChange);*/

    Scalar lowerBound, upperBound;
    Mat skinMask1, skinMask2, skinMask3;
    Mat result1, result2, result3;
    //複製原圖
    image1.copyTo(result1);
    image2.copyTo(result2);
    image3.copyTo(result3);
    //while (true)
    //{
    //    // 確定膚色範圍（在 HSV 色彩空間中）
    //    lowerBound = Scalar(h_min, s_min, v_min);
    //    upperBound = Scalar(h_max, s_max, v_max);
    //    
    //    // 進行膚色偵測
    //    inRange(hsvImage1, lowerBound, upperBound, skinMask1);
    //    inRange(hsvImage2, lowerBound, upperBound, skinMask2);
    //    inRange(hsvImage3, lowerBound, upperBound, skinMask3);
    //    imshow("skinMask1", skinMask1);
    //    imshow("skinMask2", skinMask2);
    //    imshow("skinMask3", skinMask3);
    //    // 將膚色區域轉換為紅色
    //    
    //    result1.setTo(Scalar(0, 0, 255), skinMask1);
    //    result2.setTo(Scalar(0, 0, 255), skinMask2);
    //    result3.setTo(Scalar(0, 0, 255), skinMask3);
    //    imshow("Skin Detection1", result1);
    //    imshow("Skin Detection2", result2);
    //    imshow("Skin Detection3", result3);
    //    // 按下 'q' 鍵退出迴圈
    //    if (waitKey(1) == 'q')
    //        break;
    //}
    
    //hsv上下限
    lowerBound = Scalar(h_min, s_min, v_min);
    upperBound = Scalar(h_max, s_max, v_max);
    //造上面範圍製作mask
    inRange(hsvImage1, lowerBound, upperBound, skinMask1);
    inRange(hsvImage2, lowerBound, upperBound, skinMask2);
    inRange(hsvImage3, lowerBound, upperBound, skinMask3);
    /*imshow("skinMask1", skinMask1);
    imshow("skinMask2", skinMask2);
    imshow("skinMask3", skinMask3);*/
    //圖中mask範圍塗上紅色
    result1.setTo(Scalar(0, 0, 255), skinMask1);
    result2.setTo(Scalar(0, 0, 255), skinMask2);
    result3.setTo(Scalar(0, 0, 255), skinMask3);
    // 顯示結果
    imshow("Skin Detection1", result1);
    imshow("Skin Detection2", result2);
    imshow("Skin Detection3", result3);
    waitKey(0);

    return 0;
}