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
        Environment();
        std::string get(EnvironmentBank bank, std::string key);
        void set(EnvironmentBank bank, std::string key, std::string value);
    private:
        std::map<std::string, std::string> execution;
        std::map<std::string, std::string> variables;
        std::map<std::string, std::string> *getBankPointer(EnvironmentBank bank);
};

Environment::Environment() {
    execution["echo"] = "on";
}

std::map<std::string, std::string> *Environment::getBankPointer(EnvironmentBank bank) {
    switch (bank) {
        case EnvironmentBank::EXECUTION:
            return &execution;
            break;
        case EnvironmentBank::VARIABLES:
            return &variables;
        default:
            throw;
    }
}

std::string Environment::get(EnvironmentBank bank, std::string key) {
    std::map<std::string, std::string> *p_chosenBank = getBankPointer(bank);
    return (*p_chosenBank)[key];
}

void Environment::set(EnvironmentBank bank, std::string key, std::string value) {
    std::map<std::string, std::string> *p_chosenBank = getBankPointer(bank);
    (*p_chosenBank)[key] = value;
}

Environment environment;

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
    if (!vector.empty()) {
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
    if (command[0] == '@') {
        std::string key = command;
        key.erase(key.begin());
        environment.set(EnvironmentBank::EXECUTION, key, args[0]);
        return;
    }
    if (environment.get(EnvironmentBank::EXECUTION, "echo") == "on") {
        std::cout << command << ' ';
        printVector(args, " ", "");
        std::cout << '\n';
    }
    if (command == "echo") {
        printVector(args);
    } else if (command == "rem") {} else if (command == "somecommand") {
        // ...
    } else {
        std::cout << "Bad command!\n";
        throw;
    }
}

int main(int argc, char *argv[]) {
    if (std::string_view(argv[1]) == "-i") {
        environment.set(EnvironmentBank::EXECUTION, "echo", "off");
    }
    while (!std::cin.eof()) {
        std::string line;
        getline(std::cin, line, '\n');
        std::vector<std::string> args = split(line, ' ');
        if (!args.empty()) {
            std::string command = args[0];
            args.erase(args.begin());
            execute(command, args);
        }
    }
}