#include <iostream>
#include <string>
#include <vector>
#include <map>

enum class EnvironmentBank {
    EXECUTION,
    VARIABLES
};

class Environment {
    public:
        std::string get(EnvironmentBank bank, std::string key);
        void set(EnvironmentBank bank, std::string key, std::string value);
    private:
        std::map<std::string, std::string> execution;
        std::map<std::string, std::string> variables;
};

std::string Environment::get(EnvironmentBank bank, std::string key) {
    std::map<std::string, std::string> *p_chosenBank;
    switch (bank) {
        case EnvironmentBank::EXECUTION:
            p_chosenBank = &execution;
            break;
        case EnvironmentBank::VARIABLES:
            p_chosenBank = &variables;
        default:
            throw;
    }
    return p_chosenBank->operator[](key);
}

void Environment::set(EnvironmentBank bank, std::string key, std::string value) {

}

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
    if (size > 1) {
        for (int i = 0; i < size; i++) {
            std::cout << vector[i];
            if (i < size - 1) {
                std::cout << comma;
            }
        }
        std::cout << end;
    }
}

void execute(std::string command, std::vector<std::string> args) {
    if (1) {
        std::cout << command << ' ';
        printVector(args);
    }
}

int main() {
    while (!std::cin.eof()) {
        std::string line;
        getline(std::cin, line, '\n');
        std::vector<std::string> args = split(line, ' ');
        std::string command = args[0];
        args.erase(args.begin());
        execute(command, args);
    }
}