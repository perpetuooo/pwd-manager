#ifndef FILES_HPP
#define FILES_HPP

# include <fstream>
# include <string>
using namespace std;

class FileWriter {
private:
    fstream file;

public:
    bool fileExists(const string& filename);
    void readFile(const string& filename);
    void writeFile(const string& filename, const string& data);
};

#endif
