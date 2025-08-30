# include <iostream>
# include <fstream>
# include <filesystem>
# include <string>

using namespace std;

class FileWriter {
    private:
        fstream file;

    public:
        bool fileExists(string filename) {
            return filesystem::exists(filename);
        }

        void readFile(string filename) {
            string line;

            file.open(filename, ios::in);

            if (!file.is_open()) {
                throw runtime_error("Could not open " + filename);
            }
            
            cout << "--- Passwords ---\n";
            while (getline(file, line)) {
                cout << line << endl;
            }

            file.close();
        }

        void writeFile(string filename ,string& data) {
            file.open(filename, ios::out | ios::app);

            if (!file.is_open()) {
                throw runtime_error("Could not open " + filename);
            }

            file << data << endl;
            file.close();
        }
};
