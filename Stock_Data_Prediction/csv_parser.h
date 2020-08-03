#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <string>
#include <fstream>
#include <vector>

class CSV_Parser
{
public:

    CSV_Parser(std::string file_dir);
    std::vector<std::string> Get_Next_Row();
    std::string Get_First_Line();
    std::string Get_Last_Line();


    bool Has_Next();
private:

    std::vector<std::string> split (const std::string &s, char delim);
    bool moveToStartOfLine(std::ifstream & file_stream);

    std::string file_dir;
    std::ifstream * file_stream;

};

#endif // CSV_PARSER_H
