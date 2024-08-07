#pragma once

#include <iostream>
#include <vector>
#include <filesystem>
namespace fs =  std::filesystem;
typedef fs::path element_t;
#include <string>
using std::string;
#include <iostream>
#include <algorithm>

///     TODO:
/// add Linux and Apple API librarys, & write sections for Linux, Windows, & Apple products 
/// for everything else, the standard - Windows Specific API only; currenty, TBW (To Be Written): 1/18/2024
#if _WIN64
#include <Windows.h>
//#include "/usr/share/mingw-w64/include/windows.h"
#define WIN64_LEAN_AND_MEAN
#elif _WIN32
#include <Windows.h>
//#include "/usr/share/mingw-w64/include/windows.h"
#define WIN32_LEAN_AND_MEAN
#endif

#include "fsHelper.h" //filesystem helper functions used within the program and linked list

//struct __declspec(align(8))
struct LINKED_LIST_DATA
{
	size_t total_vector_files = 0;		//how many files are within all the vectors within the nodes within the linked list
	size_t total_nodes = 0;			//holds the size of the node network
	double total_file_sizeMb = 0.0000;	// holds the size of all files if added together in Megabytes
	//size_t time_to_copy_delta;
};
 
//__declspec(align(16))
 class node
{
  public:
	node() {
		pNext_linked_list = nullptr;
	}

	/// <summary>
	/// constructor for node class
	/// </summary>
	/// <param name="fep"> - file_extension_type </param>
	/// <param name="fot">- files_of_type</param>
	/// <param name="next">- pNext_linked_list</param>
	node(const element_t& fep, std::vector<element_t>& fot, node* next = nullptr)
		:
		
		file_extension_type	{ fep },	// get the current type of extension found within this linked list
		files_of_type		{ fot },	// store all files of type here
		pNext_linked_list	{ next }	// pointer to next node.
	
	{}
	const element_t		file_extension_type;	// each node contains the type of extension its carrying
	std::vector<element_t>  files_of_type;		//each node holds a vector of element_t's (in this case, fs::path's) for analysis 
	node*			pNext_linked_list;	//each node has a pointer to the next node.
};

//__declspec(align(8))
class linked_list
{ 
  public:
	LINKED_LIST_DATA lldata; //data structure which holds extra data related to nodes [while adding a new node].
	  
	linked_list() 
	{
		head = tail = nullptr;
	}

	/// <summary>
	/// check if the linked list is empty, return 1 | 0 - if head == nullptr
	/// </summary>
	/// <returns></returns>
	int is_Empty() 
	{
		return head == nullptr;
	}
	
	linked_list& add_to_tail_node (element_t& element, std::vector<element_t>& vector_of_files);

	void  print_linked_list_node (const element_t& element);
	
	void  print_all_linked_list_nodes () ;
	
	void  print_all_linked_list_node_data () const;

	void print_all_linked_node_data(const element_t& elems) const;

	int  linked_list_size () const;
		
	const double get_node_file_size	(const element_t& elem);

	linked_list& orgainze_nodes (const std::vector<element_t>* collected_file_data, element_t& previous_exten);

	void copy_relevent_files (const element_t dest, int choice, std::vector<element_t>& filesNotCPY);

	//linked_list& remove_all_duplicate_filenames_from_node ();
	// 
	//linked_list& remove_all_duplicate_filenames_from_nodes ();

  private:
	node* head, *tail;
}; 


/// <summary>
/// linked list of vectors to directory entries, the data is then examined, 
/// and then a directory with the same name as the extension of the entry;
/// the file is then copied to that location.
/// </summary>
///
///
/// [DIRECTORY(s) TO EXAMINE] #------------------------------------>----> ... -----* 
/// 	|		      |					   |			
///	|		      |					 __#_______	 		
///	|		      #					| AUX. Data| <---------[auxillery structure of data variables; for other (related) linked list data per node]>
///	|--------->   ---------				 --------- Struct. |  
///		      |	      |				 |	| |	   |
///	              |	      |				 |	| |	   |
///		      |	      |        --Links-->	 |	 ----------
//  HEAD  -- ... --*  | .TIF  |>---...  ... ... ...----* | .TIFF  |>----- ... -----* TAIL
///		      |       |				 |        |    
///		      |       |				 |        |    
///		      |       |				 |        |    
///		      ---------				 ----------
///		       <|>				   <|>
///		       <|>				_____________
///		     <--|>				|    DATA   |
///	  	     <|>-----------------------		|   SET n+1 |
///		        | Dynamic set of data |		-------------
///		        | as a vector, etc.   |
///			| DATA SET n          |    <-----------[each Node has a vector that (.jpeg, .tif, .xml, etc.) Contains the filenames file-paths w/ that Extension Type]>
///		        -----------------------
///				|
///				|
///				eg. [+] Total Size of all files sizes (disk space Size of All files in the Vectors, accumulated in MB or GB of data).
///				    [+] Total Number Of accum. Nodes. (each unique extension type produces its own node w/ a vector to use for file paths that have that extension type).
///			            [+] Total Number Of accum. Files within all the nodes data vectors.
//run batch file before trying to compile on Windows vcvars64.bat

//[STEP #1]: cd "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build//"
//[STEP #2]: ./vcvars64.bat
//[STEP #3]: cd C:\Users\Lynx\source\repos_linux\UpdatedWinTest\fto\FileTypeExtensionOrganize
//[Compile]: cl main.cpp /std:c++17 /O2