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

	 @param pfm_path path to the image
	 @param endianness endianness of returned image (use either little_endian or big_endian enum value)
	 @return loaded image
	*/
	cv::Mat imread_pfm(std::string pfm_path, int endianness = opencv_pfm::little_endian);

	/**
	 @brief Saves .pfm image in specified endianness.

	 @param pfm_path path image.
	 @param image image to be saved.
	 @param pfm_endianness endianness of saved image (use either little_endian or big_endian enum value)
	 @param save_endianness endianness to be used in saved image file (use either little_endian or big_endian enum value)
	*/
	void imwrite_pfm(std::string pfm_path, cv::Mat &image, int pfm_endianness = opencv_pfm::little_endian, int save_endianness = opencv_pfm::little_endian);

	header read_pfm_header(std::ifstream &pfm_file);

	cv::Mat read_pfm_data(std::ifstream &pfm_file, header &pfm_header, int endianness);

	float read_pixel(std::ifstream &pfm_file, bool endianness_matching);

	void write_pfm_header(std::ofstream &pfm_file, cv::Mat &image, int save_endianness);

	void write_pfm_data(std::ofstream &pfm_file, cv::Mat &image, int pfm_endianness, int save_endianness);

	void write_pixel(std::ofstream &pfm_file, bool endianness_matching, float pixel);

	int adjust_endianness(int byte_position, bool endianness_matching);

	bool check_endianness_match(float pfm_endianness, int endianness);

	bool check_proper_eof(std::ifstream &pfm_file);

	void check_mat_type(cv::Mat &image);
}
