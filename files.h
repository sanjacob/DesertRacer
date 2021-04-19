/**
 * @file files.cpp
 * Helper class to simplify file operations
 * Mainly used to read scene setup table
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_FILES_H
#define DESERT_RACER_FILES_H

#include <string>

namespace desert
{
    class Files
    {
    public:
        /**
        * Retrieve contents of file as lines
        * The line will be ignored if it starts with '//'
        */
        static std::vector<std::string> Files::getLinesFromFile(std::string filename);
        // Split string into contiguous chunks
        static std::vector<std::string> Files::splitLine(std::string line);
    };
}

#endif
