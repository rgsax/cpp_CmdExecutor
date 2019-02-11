#ifndef CMDEXECUTOR_H
#define CMDEXECUTOR_H

/*
 * Compile using the following flags: -std=c++17 -lstdc++fs
 */

#include <experimental/filesystem> //C++17 and newer versions
#include <cstdlib>
#include <fstream>
#include <streambuf>
#include <stdexcept>

using std::experimental::filesystem::temp_directory_path;
using std::ifstream;
using std::string;
using std::system;
using std::istreambuf_iterator;
using std::runtime_error;

class CmdExecutor {
public:
	static CmdExecutor* getInstance() {
		if(instance == nullptr)
			instance = new CmdExecutor();
		
		return instance;
	}
	
	int execute(string command) {
		command += " > " + output_path; //The output will be redirected to the temporary file
		int status = system(command.c_str());
		
		collectOutput();
		
		return status;
	}
	
	string getOutput() {
		if(output == nullptr)
			throw runtime_error("Missing execution before output request");
			
		return *output;
	}
	
	~CmdExecutor() {
		delete output;
	}
	
private:
	static CmdExecutor *instance;
	static const string OUTPUT_FILENAME;
	
	string *output = nullptr;
	string output_path;
	
	
	void collectOutput() {
		delete output; //Prevent memory leak
		ifstream out_file(output_path); //Open the temporary file containing the output of the command
		output = new string((istreambuf_iterator<char>(out_file)), istreambuf_iterator<char>()); // Put the output into a string
	}
	
	CmdExecutor() { 
		output_path = temp_directory_path().string() + "/" + OUTPUT_FILENAME; //The temporary file for the output
	}
	
	CmdExecutor(const CmdExecutor &c) {	}

};

CmdExecutor* CmdExecutor::instance = nullptr;
const string CmdExecutor::OUTPUT_FILENAME = "cmd_executor_output";

#endif