#include "src/opencv_pfm.hpp"

int main(int argc, char const *argv[])
{
	cv::Mat image = opencv_pfm::imread_pfm("test.pfm");
	opencv_pfm::imwrite_pfm("out.pfm", image);
	return 0;
}