#ifndef FILES_HPP
#define FILES_HPP

# include <fstream>
using namespace std;

class FileWriter {
    private:
        fstream file;
    
    public:
        bool fileExists(string filename);
        void readFile(string filename);
        void writeFile(string filename, string& data);
};

#endif