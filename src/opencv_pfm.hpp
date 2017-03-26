#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>

#define bytes_per_float 4

namespace opencv_pfm
{
	enum endianness
	{
		little_endian,
		big_endian
	};

	struct header
	{
		std::string type;
		int rows;
		int cols;
		float endianness;
	};

	/**
	 @brief Loads .pfm image in specified endianness.

	 @param pfm_path path to the image.
	 @param pfm_path endianness of returned image (use either LITTLE_ENDIAN or BIG_ENDIAN enum value)
	 @return loaded image.
	*/
	cv::Mat imread_pfm(std::string pfm_path, int endianness = opencv_pfm::little_endian);

	header read_pfm_header(std::ifstream &pfm_file);

	cv::Mat read_pfm_data(std::ifstream &pfm_file, header &pfm_header, int endianness);

	float read_pixel(std::ifstream &pfm_file, bool endianness_matching);

	int adjust_endianness(int byte_position, bool endianness_matching);

	bool check_endianness_match(float pfm_endianness, int endianness);

	bool check_proper_eof(std::ifstream &pfm_file);
}
