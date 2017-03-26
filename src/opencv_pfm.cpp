#include "opencv_pfm.hpp"

namespace opencv_pfm
{
	cv::Mat imread_pfm(std::string pfm_path, int endianness)
	{
		std::ifstream pfm_file(pfm_path.c_str());
		if (!pfm_file.is_open())
			return cv::Mat();
	
		header pfm_header = read_pfm_header(pfm_file);
		cv::Mat pfm_image = read_pfm_data(pfm_file, pfm_header, endianness);
		
		pfm_file.close();
		return pfm_image;
	}

	void imwrite_pfm(std::string pfm_path, cv::Mat &image, int pfm_endianness, int save_endianness)
	{
		check_mat_type(image);
		std::ofstream pfm_file(pfm_path.c_str());
		if (!pfm_file.is_open())
		{
			throw;
			return;
		}

		write_pfm_header(pfm_file, image, save_endianness);
		write_pfm_data(pfm_file, image, pfm_endianness, save_endianness);

		pfm_file.close();
		return;
	}

	header read_pfm_header(std::ifstream &pfm_file)
	{
		header pfm_header;
		pfm_file >> pfm_header.type
				 >> pfm_header.cols
				 >> pfm_header.rows
				 >> pfm_header.endianness;

		pfm_file.seekg(1, std::ios::cur);		// skip last whitepsace
		return pfm_header;
	}

	cv::Mat read_pfm_data(std::ifstream &pfm_file, header &pfm_header, int endianness)
	{
		cv::Mat pfm_image(pfm_header.rows, pfm_header.cols, CV_32FC1);	// prepare mat
		bool endianness_matching = check_endianness_match(pfm_header.endianness, endianness);

		for (int i = 0; i < pfm_header.rows; i++)	// load values
		{
			for (int j = 0; j < pfm_header.cols; j++)
			{
				pfm_image.at<float>(i, j) = read_pixel(pfm_file, endianness_matching);
			}
		}
		
		if (check_proper_eof(pfm_file) == false)
			return cv::Mat();

		return pfm_image;
	}

	float read_pixel(std::ifstream &pfm_file, bool endianness_matching)
	{
		float float_data;
		char raw_byte, raw_data[bytes_per_float];

		for (int bytes = 0; bytes < bytes_per_float; bytes++)		// assemble whole number
		{
			pfm_file.read(&raw_byte, 1);
			raw_data[adjust_endianness(bytes, endianness_matching)] = raw_byte;		// inser in order respecting endianness
		}
		memcpy(&float_data, raw_data, bytes_per_float);		// copy to float type
		return float_data;
	}

	void write_pfm_header(std::ofstream &pfm_file, cv::Mat &image, int save_endianness)
	{
		float endianness = (save_endianness == opencv_pfm::little_endian) ? (double)-1.0 : (double)1.0;
		pfm_file << "Pf\n"
				 << image.cols
				 << " "
				 << image.rows
				 << "\n"
				 << endianness
				 << "\n";
	}

	void write_pfm_data(std::ofstream &pfm_file, cv::Mat &image, int pfm_endianness, int save_endianness)
	{		
		bool endianness_matching = !check_endianness_match(pfm_endianness, save_endianness);

		for (int i = 0; i < image.rows; i++)	// load values
		{
			for (int j = 0; j < image.cols; j++)
			{
				write_pixel(pfm_file, endianness_matching, image.at<float>(i, j));
			}
		}
		pfm_file << "\n";
		return;
	}

	void write_pixel(std::ofstream &pfm_file, bool endianness_matching, float float_data)
	{
		char raw_data[bytes_per_float];
		memcpy(raw_data, &float_data, bytes_per_float);

		for (int bytes = 0; bytes < bytes_per_float; bytes++)		// disassemble whole number
		{
			pfm_file << raw_data[adjust_endianness(bytes, endianness_matching)];		// write in order respecting endianness
		}
		return;
	}

	int adjust_endianness(int byte_position, bool endianness_matching)
	{
		if (endianness_matching)
			return byte_position;
		else
			return bytes_per_float - 1 - byte_position;
	}

	bool check_endianness_match(float pfm_endianness, int endianness)
	{
		if (pfm_endianness < 0 && endianness == opencv_pfm::little_endian ||
			pfm_endianness >= 0 && endianness == opencv_pfm::big_endian)
			return true;
		else
			return false;
	}

	bool check_proper_eof(std::ifstream &pfm_file)
	{
		char data;		
		pfm_file.read(&data, 1);
		if (!pfm_file.eof() && data != 0x0a)
			return false;
		return true;
	}

	void check_mat_type(cv::Mat &image)
	{
		if (image.type() != CV_32FC1 || image.data == NULL)
			throw;
		return;
	}
}
