#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>
#include <filesystem>

using namespace cv;
using namespace std;


vector<vector<pair<uchar, int>>> compressImage(Mat input) {
	int cntr = 1, cntg = 1, cntb = 1;
	uchar r,g,b;
	// 拆分通道
	std::vector<cv::Mat> channels;
	cv::split(input, channels);

	// 紅色通道&拉平為一維陣列
	cv::Mat redChannel = channels[2];
	Mat flattenedRedChannel = redChannel.reshape(1, 1).clone();
	// 綠色通道&拉平為一維陣列
	cv::Mat greenChannel = channels[1];
	Mat flattenedGreenChannel = greenChannel.reshape(1, 1).clone();
	// 藍色通道&拉平為一維陣列
	cv::Mat blueChannel = channels[0];
	Mat flattenedBlueChannel = blueChannel.reshape(1, 1).clone();
	
	
	// 獲取陣列大小
	int size = flattenedRedChannel.cols;

	vector<pair<uchar, int>> rcompress;
	vector<pair<uchar, int>> gcompress;
	vector<pair<uchar, int>> bcompress;

	r = flattenedRedChannel.at<uchar>(0, 0);
	g = flattenedGreenChannel.at<uchar>(0, 0);
	b = flattenedBlueChannel.at<uchar>(0, 0);
	for (int i = 1; i < size; i++) {
		//cout << int(r) << " " << int(g) << " " << int(b) << endl;
		
		if (flattenedRedChannel.at<uchar>(0, i) == r)	cntr++;
		else {
			rcompress.push_back({ r, cntr });
			cntr = 1;
			r = flattenedRedChannel.at<uchar>(0, i);
		}
		
		if (flattenedGreenChannel.at<uchar>(0, i) == g)	cntg++;
		else {
			gcompress.push_back({ g, cntg });
			cntg = 1;
			g = flattenedGreenChannel.at<uchar>(0, i);
		}

		if (flattenedBlueChannel.at<uchar>(0, i) == b)	cntb++;
		else {
			bcompress.push_back({ b, cntb });
			cntb = 1;
			b = flattenedBlueChannel.at<uchar>(0, i);
		}
	}
	vector<vector<pair<uchar, int>>> output;
	output.push_back(rcompress);
	output.push_back(gcompress);
	output.push_back(bcompress);

	return output;
}

string generateCompressedDataText(const vector<vector<pair<uchar, int>>>& compressedData, int width, int height) {
	string text;

	// 檔案大小
	text += to_string(width) + "*" + to_string(height)+"\n";
	text += "R G B\n";

	// 壓縮資料
	for (int i = 0; i < 3; i++) {
		vector < pair<uchar, int>> channel = compressedData[i];
		for (const auto& data : channel) {
			text += std::to_string(static_cast<int>(data.first)) + " " + std::to_string(data.second) + "\n";
			
		}
		
	}
	

	return text;
}

// 將文本內容寫入.bat檔案
void writeTextToBatFile(const std::string& text, const std::string& filename) {
	std::ofstream outFile(filename, std::ios::binary);

	if (!outFile) {
		std::cerr << "無法建立輸出檔案" << std::endl;
		return;
	}

	outFile << text;
	outFile.close();
}

int fileSize(string filename) {
	// 1. 開檔
	ifstream fin(filename, ifstream::in | ifstream::binary);
	// 2. Seek 檔案頭到尾
	fin.seekg(0, ios::end);
	// 3. tell 位置 
	long long length = fin.tellg();
	// 4. 關檔
	fin.close();
	
	return length;
}

int main() {
	int width, height, originalSize, compressedSize,filecnt = 0;
	vector<vector<pair<uchar, int>>> output;
	string tx;
	string inputfilename[3] = { "img1.bmp" ,"img2.bmp" ,"img3.bmp" };
	double compressrate = 0;
	for (auto &i : inputfilename) {
		filecnt++;
		Mat im1 = imread(i);
		if (im1.empty())
		{
			cout << "Failed to open input image file." << endl;
			return 0;
		}
		
		originalSize = fileSize(i);
		cout << "檔案: "<<i << endl << "原始大小: " << originalSize << " bytes." << endl;
		width = im1.cols;
		height = im1.rows;
		for (int j = 0; j <= 3; j++)		i.pop_back();
		cout << "壓縮中..." << endl;
		output = compressImage(im1);
		tx = generateCompressedDataText(output, width, height);
		writeTextToBatFile(tx, "compressed_"+i+".dat");
		cout << "壓縮完成" << endl;
		compressedSize = fileSize("compressed_" + i + ".dat");
		cout << "壓縮後大小: " << compressedSize << " bytes." << endl;
		cout << "壓縮率: " << (double(originalSize) / double(compressedSize)) << endl;
		compressrate += (double(originalSize) / double(compressedSize));
		cout << "==============================================================" << endl;
	}

	cout << "平均壓縮率: " << compressrate / filecnt<< endl;
	return 0;
}