#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> split(std::string string, char splitChar) {
    std::vector<std::string> splitedString;
    std::string currentSubstring;
    for (int position = 0; position <= string.length(); ++position) {
        char currentChar = string[position];
        if (currentChar != splitChar && position < string.length()) {
            currentSubstring += currentChar;
        } else {
            if (currentSubstring != "") {
                splitedString.push_back(currentSubstring);
                currentSubstring = "";
            }
        }
    }
    return splitedString;
}

template <typename Vector>
void printVector(Vector vector, std::string comma = " ", std::string end = "\n") {
    int size = vector.size();
    for (int i = 0; i < size; i++) {
        std::cout << vector[i];
        if (i < size -1) {
            std::cout << comma;
        }
    }
    std::cout << end;
}

int main() {
    while (!std::cin.eof()) {
        std::string line;
        getline(std::cin, line, '\n');
        std::vector<std::string> args = split(line, ' ');
        std::cout << line << '\n';
        std::string command = args[0];
        args.erase(args.begin());
        printVector(args);
    }
}