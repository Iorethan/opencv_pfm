#include "src/opencv_pfm.hpp"

int main(int argc, char const *argv[])
{
	cv::Mat image = opencv_pfm::imread_pfm("test.pfm");
	cv:imwrite("test.png", image);
	return 0;
}