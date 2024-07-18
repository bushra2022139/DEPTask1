#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Function to read the contents of a file
string readFile(const string &filePath) {
    ifstream file(filePath, ios::in | ios::binary);
    if (!file) {
        throw runtime_error("Could not open file for reading.");
    }
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    return content;
}

// Function to write contents to a file
void writeFile(const string &filePath, const string &data) {
    ofstream file(filePath, ios::out | ios::binary);
    if (!file) {
        throw runtime_error("Could not open file for writing.");
    }
    file.write(data.c_str(), data.size());
}

// Function to compress data using Run-Length Encoding (RLE)
string compressRLE(const string &data) {
    string compressed;
    int n = data.size();
    for (int i = 0; i < n; ++i) {
        int count = 1;
        while (i + 1 < n && data[i] == data[i + 1]) {
            ++i;
            ++count;
        }
        compressed += data[i];
        compressed += to_string(count);
    }
    return compressed;
}

// Function to decompress data using Run-Length Encoding (RLE)
string decompressRLE(const string &data) {
    string decompressed;
    int n = data.size();
    for (int i = 0; i < n; ++i) {
        char ch = data[i];
        string countStr;
        while (i + 1 < n && isdigit(data[i + 1])) {
            countStr += data[++i];
        }
        int count = stoi(countStr);
        decompressed.append(count, ch);
    }
    return decompressed;
}

int main() {
    string inputFilePath, outputFilePath;
    char choice;
    
    cout << "Enter the path of the input file: ";
    cin >> inputFilePath;
    
    cout << "Choose operation: (c)ompress or (d)ecompress: ";
    cin >> choice;
    
    if (choice == 'c' || choice == 'C') {
        try {
            string data = readFile(inputFilePath);
            string compressedData = compressRLE(data);
            outputFilePath = inputFilePath + ".rle";
            writeFile(outputFilePath, compressedData);
            cout << "File compressed successfully. Output file: " << outputFilePath << endl;
        } catch (const exception &e) {
            cerr << "Error: " << e.what() << endl;
        }
    } else if (choice == 'd' || choice == 'D') {
        try {
            string data = readFile(inputFilePath);
            string decompressedData = decompressRLE(data);
            outputFilePath = inputFilePath + ".decompressed";
            writeFile(outputFilePath, decompressedData);
            cout << "File decompressed successfully. Output file: " << outputFilePath << endl;
        } catch (const exception &e) {
            cerr << "Error: " << e.what() << endl;
        }
    } else {
        cerr << "Invalid choice. Please enter 'c' for compress or 'd' for decompress." << endl;
    }

    return 0;
}
