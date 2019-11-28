#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

// Globals
bool globalEcho;
std::map<std::string, std::string> variables;
std::vector<std::string> code;
std::map<std::string, int> gotoSections;
int lineNumber = 0;

// Enums
enum CommandType {
    COMMAND,
    GOTO_SECTION
};

// Structs

struct Switch {
    std::string type;
    std::string param;
};


std::string upper(std::string source) {
    std::string result;
    for (int i = 0; i < source.length(); i++) {
        result += toupper(source[i]);
    }
    return result;
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
std::string joinVector(Vector vector, std::string comma = " ", std::string end = "\n") {
    int size = vector.size();
    std::string str;
    if (!vector.empty()) {
        for (int i = 0; i < size; i++) {
            str += vector[i];
            if (i < size - 1) {
                str += comma;
            }
        }
        str += end;
    }
    return str;
}

void output(std::string str) {
    std::cout << str << '\n';
}

namespace builtins {

void echo(std::string str) {
    if (upper(str) == "ON") {
        globalEcho = 1;
    } else if (upper(str) == "OFF") {
        globalEcho = 0;
    } else {
        output(str);
    }
}

void rem() {
    // Do nothing
}

void goto_(std::string gotoSection) {
    /*for (auto i: gotoSections) {
        std::cout << std::get<0>(i) << ' ' << std::get<1>(i) << '\n';
    }*/
    lineNumber = gotoSections[gotoSection];
}

void set(std::vector<Switch> switches/*, std::string setString*/) {
    for (auto i: switches) {
        std::cout << i.type << ' ' << i.param << '\n';
    }
}

}

std::vector<Switch> parseSwitches(std::vector<std::string> args) {
    std::vector<Switch> switches;
    for (int i = 0; i < args.size(); i++) {
        std::string arg = args[i];
        if (arg[0] == '/') {
            Switch switch_;
            std::string switchType = arg;
            switchType.erase(0, 1);
            for (int j = 2; j < arg.length(); j++) {
                if (arg[j] == ':') {
                    std::string switchParam = arg;
                    switchParam.erase(0, j+1);
                    switchType.erase(j-1, switchParam.back());
                    switch_.param = switchParam;
                    break;
                }
            }
            switch_.type = switchType;
            switches.push_back(switch_);
        }
    }
    return switches;
}

void parse(std::string line, bool &echo, CommandType &commandType, std::string &command, std::vector<std::string> &args) {
    // Variables
    echo = globalEcho;
    //command;
    commandType = COMMAND;
    //args;
    // Ignore empty lines
    if (!line.empty()) {
        // Parsing
        // First character checks (goto sections and no echo)
        char firstChar = line[0];
        std::string noFirstChar = line;
        noFirstChar.erase(0, 1);
        bool isSpecial = 0;
        if (firstChar == '@') {
            echo = 0;
            isSpecial = 1;
        } else if (firstChar == ':') {
            commandType = GOTO_SECTION;
            isSpecial = 1;
        }
        if (isSpecial) {
            line = noFirstChar;
        }
        // Split command and args
        if (commandType == COMMAND) {
            args = split(line, ' ');
            if (!args.empty()) {
                command = args[0];
                args.erase(args.begin());
            }
            command = upper(command);
        } else {
            command = line;
        }
    }
}

int execute(std::string line) {
    bool echo;
    CommandType commandType;
    std::string command;
    std::vector<std::string> args;
    parse(line, echo, commandType, command, args);
    if (echo) {
        std::cout << line << '\n';
    }
    if (commandType == COMMAND) {
        if (command.empty()) {
            builtins::rem();
        } else if (command == "ECHO") {
            builtins::echo(joinVector(args, " ",  ""));
        } else if (command == "REM") {
            builtins::rem();
        } else if (command == "GOTO") {
            builtins::goto_(args[0]);
        } else if (command == "EXIT") {
            return 0;
        } else if (command == "SET") {
            builtins::set(parseSwitches(args));
        } else {
            std::cout << command << "is not recognized as an internal or external command.\n";
        }
    } else if (commandType == GOTO_SECTION) {
        gotoSections.insert(std::pair<std::string, int>(command, lineNumber));
    }
    return 1;
}

int main(int argc, char *argv[]) {
    globalEcho = 1;
    bool bufferedMode = 0;
    while (!std::cin.eof()) {
        std::string line;
        getline(std::cin, line, '\n');
        if (bufferedMode) {
            code.push_back(line);
        } else {
            if (!execute(line)) {
                return 0;
            }
        }
    }
    if (bufferedMode) {
        lineNumber = 0;
        while (lineNumber < code.size()) {
            if (!execute(code[lineNumber])) {
                return 0;
            }
            lineNumber++;
        }
    }
}
