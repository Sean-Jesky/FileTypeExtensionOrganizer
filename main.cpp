//by. Sean Jesky i hope this helps someone out there. Please use in a responsible way.. 
/*
 * FILE('s): main.cpp
 * DATE&TIME: feb 15th 2023
 * CREATED_BY: Sean Jesky ()
 * CREATED_FOR: the dream
 * WHAT_IT_DOES:  This data is saved in users requested location. Segregation and organization of file extension type.
 * WINDOWS WARNING: DOES NOT WORK IN DEBUG MODE

 vcvarsall.bat x64
 vcvars64.bat
 cl...
 cl main.cpp /std:c++17 /EHsc /GS /Od /GA /Gs /Gu /homeparams /Qpar /Qsafe_fp_loads /RTCs /RTCu /RTCc
 
 cl main.cpp /std:c++17 /EHsc /GS /Od /GA /Fe winFTO.exe /P preprocess.asm
	
	cl main.cpp /std:c++17 /EHsc /GS /Od /GA /Gs /Gu /homeparams /Qpar /Fa /Fe:FTO(noOpt).exe

	TODO: keep track of # of duplicates, and sort files in node in order by files name.
vector of filenames, cmp with file path + filename of source location, if == add one to copy #, else copy# = 0

*/

#include "linked_list.h"
#include "linked_list.cpp"
#include "fsHelper.h"
//,,,
#include <filesystem>
namespace fs = std::filesystem;
#include <exception>
#include <vector>
using std::vector;
//...
#include <iostream>
//#include  <fstream>
//using std::ofstream;
//...
#include <chrono>
//#include <ctime>
#include <iomanip>
using std::setfill; using std::setw;
//...
//END OF WINDOWS INCLUDES, AND OTHER SPECIFICS NEEDED/WANTED FOR PROPER FUNCTIONING OF WINDOWS API's.
#define __AUTHOR__		"Sean Mikhial Jesky"
#define __EXE_VERSION__		"3.00"
#define __EXECUTABLE_NAME__ "FTO.exe"
#define __YEAR_WRITTEN__	"2023"

const std::string Written_On	= __YEAR_WRITTEN__;
const std::string Compiled_Time	= __TIME__;
const std::string Compiled_Date	= __DATE__;
const std::string WrittenBy	= __AUTHOR__;
const std::string ExeName	= __EXECUTABLE_NAME__;
const std::string Version	= __EXE_VERSION__;

//size_t curr_percent = 0;
//size_t running_total = 0;
// cl main.cpp /std:c++17 /EHsc /GS /Od /GA /Gs /Gu /homeparams /Qpar /Fa /Fe:FTO(noOpt).exe
//#define PERCENT_COMPLETE (running_total, curr_vector_size)  ((running_total / curr_vector_size) * 100) 
//for a loading bar... [.|.|.|.|.|] (1 loading segment = N == 5% to 20% / 100%) in other words: 10 bars - 1 per 10% - total of 100
/// @brief (FTeO Version 3.00) 
///			This program goes through a filesystem, gets all the file extensions,
///			creates directories from those extensions, and copies files to the corresponding directory extension name.
/// @param int argv 
/// @param char *argc 
/// @return 
int main(int argv, char* argc[])
{
	system("color F8");
	if (argv <= 2) {
		std::cout << "\n\t|Written & Disgned By. " << __AUTHOR__ << "|\n";
		std::cerr << "invalid input, proper use:\n\nFileTypeOrganizer.exe <source> <destination> <MODE Choices: <1|2>)>\n";
		std::cerr << "\t[!!]MODE - 1: Keep Duplicate Files\n\t[!!]MODE - 2: Don't Keep Duplicates.\n";
		std::cerr << "\nPlease restart program with valid input.";
		return 1;
	}
	if (argv >= 2) {
		if ((!fs::is_directory(argc[1]) || !fs::is_directory(argc[2])))
		{
			std::cout << "\n\t|Written & Disgned By. " << __AUTHOR__ << "|\n";
			std::cerr << "invalid data input....\n";
			std::cerr << "please enter a valid directory(s)";
			return -2;
		}
	}
	
	const std::string copy_from = argc[1];
	auto time = std::chrono::system_clock::now();
	const std::time_t t =  std::chrono::system_clock::to_time_t(time);

	std::cout << "\n[_] Starting Program:\t\t\t\t" << __EXECUTABLE_NAME__;
	std::cout << "\n[_] Started Running @:\t\t\t" << std::ctime(&t)<< '\n';
	std::cout << setw(45) << setfill('-') << '\n';

	//fs::path loc = L"/home/sean/";
	const fs::path path_to_copy_from = copy_from;

	const std::string save_location = argc[2];
	const fs::path base_save_location = save_location;

	auto t1_start = std::chrono::high_resolution_clock::now();
	linked_list* llist = new linked_list();
	vector<fs::path>* p_files = new vector<fs::path> { path_to_copy_from };
	std::cout << "[!] ...Collecting relevant files from source location provided...\n";
	*p_files = collect_relivant_files(*p_files);
	
	fs::path p_path = "";
	std::cout << "[!] ...Starting Organization Procedure on the data collected...\n";

	llist->orgainze_nodes(p_files, p_path);
	delete p_files;

	std::cout << "\n[*] Total # of files:\t\t\t\t"			<< llist->lldata.total_vector_files		<< '\n';
	std::cout << "[*] Total # of File Extension's:\t\t"		<< llist->lldata.total_nodes			<< '\n';
	std::cout << "[*] Total physical data of files (GB):\t\t"	<< llist->lldata.total_file_sizeMb / 1024.0000 / 1024.00<< "\n";
	
	std::vector<fs::path>fno;
	auto t1_end = std::chrono::high_resolution_clock::now();
	auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(t1_end - t1_start).count();

	if (total_time <= 1000)
		std::cout << "[+] Total Time to complete 'startp Functions':\t" << total_time << " Miliseconds...\n";
	else 
		std::cout << "[+] Total Time to complete 'startp Functions':\t" << total_time * 0.001 << " seconds...\n";
	//...
	std::cout << setw(45) << setfill('|') << '\n';

	auto t1_copy_start = std::chrono::high_resolution_clock::now();

	std::cout << "\n[!] Starting Copying Relevent Files to their Relevent locations....\n\n";
	
	//copy files, w/ keep duplicates(1) or dont keep(2) choice.
	int choice = 1;
	llist->copy_relevent_files(base_save_location, choice, fno);
	
	auto t1_copy_end = std::chrono::high_resolution_clock::now();
	auto total_copy_time = std::chrono::duration_cast<std::chrono::minutes> (t1_copy_end - t1_copy_start).count();


	std::cout << setw(45) << setfill('^') << '\n';

	if (total_copy_time <= 60)
		std::cout << "\n[#] Total Time to complete Copying of relevent data:\t" << total_copy_time << " minutes\n";
	else 
		std::cout << "[#] Total Time to complete Copying of relevent data:\t" << total_copy_time*60 << " hours\n";

	//---------------------------------------------------------------------------------------------------
	std::cout << "\n[?] # of Files Not Copied For Reasions Unknown:\t" << fno.size() << "\n\n";
	std::cout << "\n\nCompiled On:\t\t"	<< Compiled_Date	<< "  @  "	<< Compiled_Time	<< '\n';
	std::cout << "Written By:\t\t"		<< WrittenBy		<< '\n';
	std::cout << "Written For Free For:\t"		<< "[Everyone]" << '\n';
	std::cout << "FileName:\t\t"		<< ExeName		<< "\t|  Ver: "	<< Version		<< '\n';
	std::cout << "Date Written:\t\t"	<< Written_On		<< '\n';
	std::cout << setw(45) << setfill('~') << '\n';


	//const fs::path elem = ".xml";
	//llist->print_all_linked_node_data(elem);
	end_program_signal(4);
	system("color 02");
	delete llist;
	//10:02 : 18        Nov 22 2023
}


/*
	USER OPTIONS TO ADD FUNCTIONALITY FOR:
	----------------------------------------------------------------------------------------------------------------------
	SET A:) 1:]	Remove duplicates	-	Remove duplicates from a singular node.
		2:]	Remove all duplicates	-	Remove duplicates from all nodes.

	SET B.)	1:]	Specific Extension	-	copy/move/delete specific node selected.
		2:]	Specific Extensions	-	copy/move/delete specific nodes selected.
		3:]	All Extensions		-	copy/move all files in all nodes.
		
	SET C.)	1:]	Single Threaded		-	Run the program a single thread (for older systems without MT capability).
		2:}	Multi-Threaded		=	Run the program on multipul threads for blazingly fast speeds.
		3:]	Low and Slow Mode	=	Run the program using slower methods that use less RAM, and on a single thread.

	SET D.)	1:]	copy			-	[default] to copy programs data to location requested.
		2:]	move			-	Move files to the requested location
		3:]	remove			-	Remove files from a specific node(s) specified by text file of entires - (Not blanket removals)

* Require password to remove > 1 all files from the system
* 
*/

