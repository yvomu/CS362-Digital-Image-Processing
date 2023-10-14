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
	// ����q�D
	std::vector<cv::Mat> channels;
	cv::split(input, channels);

	// ����q�D&�ԥ����@���}�C
	cv::Mat redChannel = channels[2];
	Mat flattenedRedChannel = redChannel.reshape(1, 1).clone();
	// ���q�D&�ԥ����@���}�C
	cv::Mat greenChannel = channels[1];
	Mat flattenedGreenChannel = greenChannel.reshape(1, 1).clone();
	// �Ŧ�q�D&�ԥ����@���}�C
	cv::Mat blueChannel = channels[0];
	Mat flattenedBlueChannel = blueChannel.reshape(1, 1).clone();
	
	
	// ����}�C�j�p
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

	// �ɮפj�p
	text += to_string(width) + "*" + to_string(height)+"\n";
	text += "R G B\n";

	// ���Y���
	for (int i = 0; i < 3; i++) {
		vector < pair<uchar, int>> channel = compressedData[i];
		for (const auto& data : channel) {
			text += std::to_string(static_cast<int>(data.first)) + " " + std::to_string(data.second) + "\n";
			
		}
		
	}
	

	return text;
}

// �N�奻���e�g�J.bat�ɮ�
void writeTextToBatFile(const std::string& text, const std::string& filename) {
	std::ofstream outFile(filename, std::ios::binary);

	if (!outFile) {
		std::cerr << "�L�k�إ߿�X�ɮ�" << std::endl;
		return;
	}

	outFile << text;
	outFile.close();
}

int fileSize(string filename) {
	// 1. �}��
	ifstream fin(filename, ifstream::in | ifstream::binary);
	// 2. Seek �ɮ��Y���
	fin.seekg(0, ios::end);
	// 3. tell ��m 
	long long length = fin.tellg();
	// 4. ����
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
		cout << "�ɮ�: "<<i << endl << "��l�j�p: " << originalSize << " bytes." << endl;
		width = im1.cols;
		height = im1.rows;
		for (int j = 0; j <= 3; j++)		i.pop_back();
		cout << "���Y��..." << endl;
		output = compressImage(im1);
		tx = generateCompressedDataText(output, width, height);
		writeTextToBatFile(tx, "compressed_"+i+".dat");
		cout << "���Y����" << endl;
		compressedSize = fileSize("compressed_" + i + ".dat");
		cout << "���Y��j�p: " << compressedSize << " bytes." << endl;
		cout << "���Y�v: " << (double(originalSize) / double(compressedSize)) << endl;
		compressrate += (double(originalSize) / double(compressedSize));
		cout << "==============================================================" << endl;
	}

	cout << "�������Y�v: " << compressrate / filecnt<< endl;
	return 0;
}