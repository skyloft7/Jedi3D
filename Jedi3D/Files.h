#pragma once
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Files {

public: 
	static const string ReadFile(string path) {
        ifstream input(path);

        string output = "";

        while (!input.eof()) {
            string line;
            getline(input, line);

            output.append(line).append("\n");
        }

        return output;
	}


};