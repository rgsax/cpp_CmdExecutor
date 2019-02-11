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
using std::istream;

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

	istream* getOutputStream() {
        if(output_stream == nullptr)
            throw runtime_error("Missing execution before output stream request");

        return output_stream;
	}

	~CmdExecutor() {
		delete output_stream;
	}

private:
	static CmdExecutor *instance;
	static const string OUTPUT_FILENAME;

    istream *output_stream = nullptr;

	string output_path;


	void collectOutput() {
		delete output_stream; //Prevent memory leak
		output_stream = new ifstream(output_path); //Open the temporary file containing the output of the command using a stream
	}

	CmdExecutor() {
		output_path = temp_directory_path().string() + "/" + OUTPUT_FILENAME; //The temporary file for the output
	}

	CmdExecutor(const CmdExecutor &c) {	}

};

CmdExecutor* CmdExecutor::instance = nullptr;
const string CmdExecutor::OUTPUT_FILENAME = "cmd_executor_output";

#endif
