//#include "fsHelper.h"
#include <iostream>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;
typedef fs::path element_t;
#include <cmath>

/// @brief collect all filenames and their paths from the Source location provided by the user to use within the rest of the program.
/// @param const fs::path directory location to search for filenames, and the data associated with their use within the program
// @return const inline vector<fs::path>
// multi-variable [directory with files to copy] function overload
const std::vector<element_t> collect_relivant_files(const std::vector<element_t>& directory)
{
	std::vector<element_t> p_vfiles;
	p_vfiles.reserve(10'000);
	try {
		for (auto dir_iter : directory) {
			for (const fs::directory_entry& dir : fs::recursive_directory_iterator{ dir_iter , fs::directory_options::skip_permission_denied })
			{
				const element_t& file_path = dir;
				const element_t* file_path_p = &file_path;

				if (!fs::is_directory(*file_path_p))
				{
					if (file_path_p->has_extension())
					{
						p_vfiles.push_back(*file_path_p);
					}
				}
			}
		}
	}
	catch (std::filesystem::filesystem_error& err) {
		std::cout << "Filesys error" << "\n" << err.what() << std::endl;
	}
	return p_vfiles;
}

//single variable [directory with files to copy] function overload
const std::vector<element_t> collect_relivant_files(const element_t& directory) {

	std::vector<element_t> p_vfiles;// = new std::vector<element_t>;
	p_vfiles.reserve(50'000);
	try {
		for (const fs::directory_entry& dir : fs::recursive_directory_iterator{ directory, fs::directory_options::skip_permission_denied })
		{
			const element_t& file_path = dir;
			const element_t* file_path_p = &file_path;

			if (!fs::is_directory(*file_path_p))
			{
				if (file_path_p->has_extension() && fs::exists(*file_path_p))
				{
					p_vfiles.push_back(*file_path_p);
				}
			}
		}
	}
	catch (std::filesystem::filesystem_error& err) {
		std::cout << "Filesys error" << "\n" << err.what() << std::endl;
	}
	return p_vfiles;
}

/// @brief create a directorys from the extensions of the files collected from earlier.
/// @param const fs::path base_save_location -  destination location; where to save folders, and files.
/// @param const vector<fs::path>* extensions - a vector of pointers to a vector of extension types collected from the files
void create_directory_from_extension_type(const element_t base_save_location, const element_t curr_extension)
{
	fs::create_directory((base_save_location.string()) + curr_extension.string().erase(0, 1));
}

/// <summary>
/// 
/// </summary>
/// <param name="list_of_files"></param>
inline std::vector<element_t> remove_duplicates(std::vector<element_t>& list_of_files)
{
	element_t curr_file;
	size_t iter_pos = 0;
	std::vector<element_t> rd;
	for (auto iter = 0; iter != list_of_files.size(); ++iter, iter_pos = +1)
	{
		fs::path lof = list_of_files.at(iter_pos);

		if (list_of_files[iter].filename().string() == list_of_files[iter - 1].filename().string()) {
			//make sorted by filenames ^
			rd.push_back(list_of_files[iter]);
		}
	}
	return rd;
}
/*
* TODO::	Tomorrow - for each Node within the list, create a thread for N, number of threads
*		to be used for copying files to their destination locations.
*/

/// <summary>
/// easy calculation of a loading bar... buggy but, its a feature.
/// </summary>
/// <param name="curr_percent"></param>
/// <param name="curr_cnt"></param>
/// <param name="prev_per"></param>
/// <param name="curr_bar"></param>
void loading_bar(double curr_percent, int curr_cnt, int& prev_per, std::vector<std::string>& curr_bar)
{
	auto curr_per = std::ceil(curr_percent);
	if (curr_per < 100)
	{
		if (curr_per > prev_per)
		{
			if ((int(curr_per) % 10) == 0)
			{
				curr_bar.push_back("]");
				for (auto& i : curr_bar)
				{
					std::cout << i << ' ';
				}
			}
		}
	}
	else if (curr_percent >= 100.00)
	{
		curr_bar.clear();
		std::cout << "\n\n";
	}
	prev_per = int(curr_per);
}