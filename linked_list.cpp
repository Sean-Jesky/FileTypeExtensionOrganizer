#include "linked_list.h"
#include <cstring>
#include <filesystem>
#include <cstdlib>
#include <cmath>
size_t running_total = 0;
std::string curr_bar = "";
int current_count = 0;
//#define PERCENT_COMPLETE (running_total, curr_vector_size)  ((running_total / curr_vector_size) * 100) ?

/// <summary>
/// create node and link, and add elements of data to the next node
/// </summary>
/// <param name="el"></param>
/// <param name="vector_of_elements"></param>
/// <returns></returns>
linked_list& linked_list::add_to_tail_node(element_t& el, std::vector<element_t>& vector_of_elements) 
{
	size_t total_node_cnt = 0;

	if (head != nullptr) {
		tail ->pNext_linked_list = new node(el, vector_of_elements);	//creates a node, and initializes an element_t element and a vector of elements_t
		tail = tail->pNext_linked_list;					//then initializes the next link in the link as the tail.
	}
	else {
		head = tail = new node(el, vector_of_elements);			//else make the first elem. the head the tail
	}
	total_node_cnt += 1;

	// NOTE TO SELF:
	//    this section below adds ~22 seconds to complete this function & to keep trak of data in use
	// function w/ this section added takes  >=44 seconds:
	// is it worth it - can I rationalize it, Yes? ok, it stays. sig. S.M.J
	// 
	//to add to the linked list data structure for data otherwise not directly related to nodes but "metadata" of all nodes

	//do act. calc. of perf.
	size_t total_file_count = 0;
	if (vector_of_elements.size() > 0)
	{
		for (auto& fs_iter : vector_of_elements)
		{
			//this slows the code and uses 12% of the function time to calculate and keep a total size
			lldata.total_file_sizeMb += (double(fs::file_size(fs_iter)) / 1024.0000);
			total_file_count += 1;
		}
	}
	lldata.total_nodes			+=	total_node_cnt;
	lldata.total_vector_files	+=	total_file_count;
	lldata.total_vector_files	+=	vector_of_elements.size();
	return *this;
}
/// <summary>
/// get the length of the linked list structure. eg. n = 6
/// </summary>
/// <returns></returns>
int linked_list::linked_list_size() const 
{
	int node_cnt = 0;
	for (node* current = head; current; current = current->pNext_linked_list)
	{
		node_cnt += 1;
	}
	return node_cnt;
}

/// <summary>
/// print the data within the node with the correct matching elem. data type
/// </summary>
/// <param name="elems"></param>
void linked_list::print_linked_list_node(const element_t& elems)
{
	for (node* current = head; current; current = current->pNext_linked_list)
	{
		if (current->file_extension_type.string() == elems.string())
		{
			std::cout << "\n\n[+] Current Node:\t\t\t" << current->file_extension_type << '\n';
			std::cout << "[+] Total # of Files In This Node:\t" << current->files_of_type.size() << "\n";
			std::cout << "[+] Total Accum. File Sizes (Mb):\t" << get_node_file_size(current->file_extension_type) << "\n\n";

		}	
	}
}
/// <summary>
/// print each node and its elem name.
/// </summary>
void linked_list::print_all_linked_list_node_data() const
{
	for (node* current = head; current; current = current->pNext_linked_list)
	{
		for (auto& fs_iter : current->files_of_type)
		{
			std::cout << fs_iter << '\n';
		}
	}
}
/// <summary>
/// print the linked list node w/ correct elem. and data within it.
/// </summary>
/// <param name="elems"></param>
void linked_list::print_all_linked_node_data(const element_t& elems) const
{
	for (node* current = head; current; current = current->pNext_linked_list)
	{
		//check !empty()
		if (current->file_extension_type.string() == elems.string())
		{
			for (auto& fs_iter : current->files_of_type)
			{
				std::cout << fs_iter << '\n';
			}
		}
	}
}
/// <summary>
/// print each node, the elem type, the size of each vector of elem within the node, and size of all the data sizes accum. together
/// </summary>
void linked_list::print_all_linked_list_nodes()
{
	for (node* current = head; current; current = current->pNext_linked_list) 
	{
		std::cout << "Extension Type of Node:\t\t" << current->file_extension_type << '\n';
		std::cout << "Size Of Vector In Node:\t\t" << current->files_of_type.size() << '\n';
		std::cout << "Total Accumulated Size of This node's file data: (Mb):\t" << get_node_file_size(current->file_extension_type) << "\n\n";
	}
}
/// <summary>
/// get the file sizes of a specific node within the linked list.
/// </summary>
/// <param name="el"></param>
/// <returns></returns>
const double linked_list::get_node_file_size(const element_t& elem)
{
	double ll_nfs = 0.0000;
	for (node* current = head; current; current = current->pNext_linked_list)
	{
		if (current->file_extension_type.string() == elem.string()) {
			if (current->files_of_type.size() > 0) {
				for (const auto& cfto : current->files_of_type)
				{
					ll_nfs += (double(fs::file_size(cfto)) / 1024.0000 / 1024.0000);
				}
			}
			else {
				return 0;
			}
		}
	}
	return ll_nfs;
}

/// <summary>
/// collect, sort, and organize files (and their file extensions),
/// 		   collected, then organize all the data into the linked list nodes of their type
/// </summary>
/// <param name="collected_file_data"></param>
/// <param name="llist"></param>
/// <param name="previous_exten"></param>
/// <returns></returns>
linked_list& linked_list::orgainze_nodes(const std::vector<element_t>* collected_file_data, element_t& previous_exten)
{
	//from here to 
	std::vector<std::string>* p_types = new std::vector<std::string>;
	p_types->reserve(collected_file_data->size());

	for (const auto& current_filename : (*(collected_file_data)))
	{
		if (current_filename.has_extension())
		{
			std::string path_str = current_filename.extension().string();
			p_types->push_back(path_str);
		}
	}
	std::sort(p_types->begin(), p_types->end());
	//here if placed into its own function, it goes from 1.5 to 2.1 seconds to run.
	// increadible - intuition wins again!!! i figured that because the overhead of calling a non-member function
	// but i had no clue how much of a difference it would have made until i made it into its own function!
	// is there really anything better then C++ and optimization, Thanks MIT!
	
	
	//then organize all the data into the linked list nodes of their type
	int cnt = 0;
	std::vector<element_t> data;
	unsigned int counter = 0;

	for (auto iter = p_types->cbegin(); iter != p_types->cend(); ++iter) // go through a vector of extensions one by one
	{
		while (previous_exten != (*p_types)[counter])
		{
			previous_exten = (*p_types)[counter];
			//make this for loop supercharged. how?
			//#pragma omp parallel for
			//#pragma omp barrier 
			for (const auto& cfto : *collected_file_data)
			{
				//#pragma omp flush(cfto)
				if ((*p_types)[counter] == cfto.extension().string())
				{
					data.push_back(cfto);
				}
			}
			// hidden functionality of add_to_tail_node():
			// when adding to node; aux. data memb.s of an associated class holds 
			// all the accumulated global data in the linked list nodes - relevent to the data 
			add_to_tail_node(previous_exten, data);
		}
		data.clear();
		counter += 1;
	}
	delete p_types;
	return *this;
}
//for_testing_only
int kill_switch(element_t kill_condition){
	if(kill_condition == ".idx" || kill_condition == ".IDX")
		throw std::logic_error("reached testing kill point");
	else{
		;
	}
	return 0;
}

/// <summary>
/// copy files from 'destination' after organization of data within the nodes.
/// </summary>
/// <param name="dest"></param>
/// <param name="choice"></param>
/// <param name="filesNotCPY"></param>
void linked_list::copy_relevent_files(const element_t dest, const int choice, std::vector<element_t>&filesNotCPY)
{
	#define KEEP_COPIES 1
	#define KEEP_NO_COPIES 2

	int total_ext = 0;
	int i = 1;
	double curr_percent = 0.0;
	std::vector<std::string> curr_bar = { "" };
	int prev_per = 0;
	size_t current_total_completed = 0;
	short step = 50;
	auto prev = 0;

	for (node* current = head; current; current = current->pNext_linked_list)
	{
		//kill_switch(current->file_extension_type);
		create_directory_from_extension_type(dest, current->file_extension_type);
		print_linked_list_node(current->file_extension_type);

		std::cout << "[!] Copying  >>>> " << current->file_extension_type.string().erase(0, 1) << " <<<< files..." << '\n';
		std::cout << "\n";
		int _incr = 0;
		auto t1_start = std::chrono::high_resolution_clock::now();
		for (const auto& file_iter : ((current->files_of_type)))
		{
#if __linux__
			const fs::path core_path = dest.string() + current->file_extension_type.string().erase(0, 1);//this part of the
			const fs::path fname_loc = core_path.string() + '/' + file_iter.filename().string();//i hope i never
			const fs::path fname_ = core_path.string() + '/' + file_iter.filename().string();//have to rewrite
#elif _WIN64
			const fs::path core_path = dest.string() + current->file_extension_type.string().erase(0, 1);//this part of the
			const fs::path fname_loc = core_path.string() + '\\' + file_iter.filename().string();//i hope i never
			const fs::path fname_ = core_path.string() + '\\' + file_iter.filename().string();//have to rewrite
#endif
                        auto tmp = 0;
                        if(current->files_of_type.size() > 10){
                               tmp =  std::log(current->files_of_type.size());
                                curr_percent  = i / double(current->files_of_type.size()) * 100.0;
                                
                        }else{
			curr_percent = i / double(current->files_of_type.size()) * 100.0;	
                        }
                        if (choice == KEEP_COPIES)
			{
				if (fs::exists(fname_loc)) 
					fs::rename(fname_loc.string(), fname_.string() + "( Copy #" + std::to_string(i) + ')' + current->file_extension_type.string());
					i += 1;
#if _WIN64
				CopyFileEx(file_iter.string().c_str(), (fname_.string().c_str()), NULL, NULL, FALSE,  COPY_FILE_NO_BUFFERING | COPY_FILE_FAIL_IF_EXISTS);
				total_ext+=1;	
#elif __linux__
				fs::copy(file_iter.string().c_str(), (fname_loc.string().c_str()), fs::copy_options::update_existing);
				total_ext += 1;
#endif
			}else if (choice == KEEP_NO_COPIES) 
			{
				if (fs::exists(file_iter))
				{
					if (file_iter.extension() ==  current->file_extension_type){
#if _WIN64
						if (CopyFileEx(file_iter.string().c_str(), (fname_.string().c_str()), NULL, NULL, FALSE, COPY_FILE_NO_BUFFERING | COPY_FILE_FAIL_IF_EXISTS))
							total_ext += 1;
						else
							filesNotCPY.push_back(fname_.c_str());
#elif _WIN32 
						if (CopyFileEx(file_iter.c_str(), (fname_.c_str()), NULL, NULL, FALSE, COPY_FILE_FAIL_IF_EXISTS))
							total_ext += 1;
						else 
							filesNotCPY.push_back(fname_.c_str());
#elif _APPLE		
						//TODO: kernal level for apple
#endif
					}

				}
				else ;
				//total_ext += 1;
			}
			std::cout << curr_percent << "%|";
		}
		//system("cls");
		std::cout << "\n\n";
		auto t2_end = std::chrono::high_resolution_clock::now();
		auto time_to_copy_delta =  std::chrono::duration_cast<std::chrono::milliseconds> (t2_end - t1_start).count();
		//std::cout << "[*] Total Completed duplicate Copies of:\t" << current->file_extension_type << " Node\t--\t" << i << '\n';
		//std::cout << "[*] Total Completed Copies of:\t\t\t" << current->file_extension_type << " Node\t--\t" << current_total_completed << "\n";
		//std::cout << "\n[*] Total Time To Copy " << current->file_extension_type << ":\t\t" << time_to_copy_delta << " ms\n\n";

	
		
		//#pragma omp flush()
		curr_percent = 0;
		total_ext = 0;
		step = 0;
		i = 1;
		current_total_completed += 1;
	}
	std::cout << "\n[!]Finished Copying all files..." << '\n';
}
//void remove_all_duplicate_filenames_from_node(const element_t& el);


/// <summary>
/// Nonmemb. funct. that simply beeps when called.
/// </summary>
/// <param name="times"></param>
void end_program_signal(const unsigned short times) {
	for (int i = 1; i <= times;) {

		//Beep(180, 100);//750, 900 old
		++i;
	}
	system("pause");
}


