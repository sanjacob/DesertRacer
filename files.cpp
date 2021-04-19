/**
 * @file files.cpp
 * Helper class to simplify file operations
 * Mainly used to read scene setup table
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "files.h"

using namespace std;
using namespace desert;


// Retrieves contents from file in disk as a string
vector<string> Files::getLinesFromFile(string filename)
{
    string line;
    ifstream inputFile(filename);
    vector<string> lines;

    // Open text file and copy all lines that aren't comments
    if (inputFile.is_open())
    {
        while (getline(inputFile, line))
        {
            if (line[0] != '/' || line[1] != '/') { lines.push_back(line); }
        }
    }
    else
    {
        cout << "File IO error when opening " << filename << endl;
    }

    return lines;
}

// Split string into contiguous chunks
vector<string> Files::splitLine(string line) {
    vector<string> pieces;
    string buffer = "";
    line.append(" ");

    for (char c : line)
    {
        if (!isspace(c))
        {
            buffer.append(1, c);
        }
        else
        {
            pieces.push_back(buffer);
            buffer = "";
        }
    }

    return pieces;
}
