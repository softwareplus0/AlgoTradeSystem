#include "csv_parser.h"
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>


CSV_Parser::CSV_Parser(std::string file_dir)
{
    this->file_dir = file_dir;
    file_stream = new std::ifstream(file_dir);
}

std::vector<std::string> CSV_Parser::Get_Next_Row()
{

    std::string row;
    getline(*file_stream, row);
    std::vector<std::string> row_strings = split(row, ',');

    return row_strings;

}

bool CSV_Parser::Has_Next()
{

    if(file_stream->eof())
    {
        return false;
    } else {
        return true;
    }

}

std::vector<std::string> CSV_Parser::split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

bool CSV_Parser::moveToStartOfLine(std::ifstream & file_stream)
{
    file_stream.seekg(-1, std::ios_base::cur);
    for(long i = file_stream.tellg(); i > 0; i--)
    {
        if(file_stream.peek() == '\n')
        {
            file_stream.get();
            return true;
        }
        file_stream.seekg(i, std::ios_base::beg);
    }
    return false;
}

std::string CSV_Parser::Get_First_Line()
{

    file_stream->seekg(0, std::ios::beg);
    std::string firstLine = "";
    getline(*file_stream, firstLine);
    return firstLine;


}

std::string CSV_Parser::Get_Last_Line()
{
    // Go to the last character before EOF

    file_stream->seekg(-1, std::ios_base::end);
    if (!moveToStartOfLine(*file_stream)) {
        return "";
    }

    std::string lastline = "";
    getline(*file_stream, lastline);
    return lastline;
}
