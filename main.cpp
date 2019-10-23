#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//词法分析总程序
//1     自定义标识符
//2     实数
//3     关键字
//4     运算符
//5     界限符

//关键字
string keyWords[11] = {"begin", "end", "if", "then", "while", "do", "const", "var", "call", "procedure", "odd"};
//运算符
string operationKeys[11] = {"+", "-", "*", "/", "=", ":=", "#", "<", "<=", ">", ">="};

struct Info {
    int type;       //记录种类
    string words;   //记录单词
};

bool contain(char b) {
    for (int i = 0; i < 11; i++) {
        if (b == operationKeys[i].at(0)) {
            return true;
        }
    }
    return false;
}

int main() {
    vector<Info> records;
    Info tmp;
    char currentChar;
    ifstream fin("input.txt");
    while (fin.peek() != EOF) {
        tmp.type = -1;
        tmp.words = "";
        fin >> noskipws >> currentChar;
        //跳过空格、制表符和换行符
        if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n') {
            continue;
        }
        //如果是数字,判断是不是一个实数
        if (currentChar >= '0' && currentChar <= '9') {
            //TODO 判断是不是实数
            tmp.type = 2;
            tmp.words = currentChar;
            fin >> currentChar;
            //获取整数部分
            while (currentChar >= '0' && currentChar <= '9') {
                tmp.words += currentChar;
                fin >> currentChar;
            }
            //判断是'.' 'e' 还是其它字符导致的循环退出
            //如果是'.',获取小数部分
            if (currentChar == '.') {
                tmp.words += currentChar;
                fin >> currentChar;
                //如果'.'后面的不是数字,出错
                if (currentChar >= '0' && currentChar <= '9') {
                    //TODO 继续获取小数部分
                    while (currentChar >= '0' && currentChar <= '9') {
                        tmp.words += currentChar;
                        fin >> currentChar;
                    }
                    //判断导致退出循环的是'e'还是其它字符
                    if (currentChar == 'e') {
                        tmp.words += currentChar;
                        //TODO 获取指数部分
                        fin >> currentChar;
                        if (currentChar == '-') {
                            tmp.words += currentChar;
                            fin >> currentChar;
                        }
                        if (currentChar >= '0' && currentChar <= '9') {
                            while (currentChar >= '0' && currentChar <= '9') {
                                tmp.words += currentChar;
                                fin >> currentChar;
                            }
                        } else {
                            cout << "实数出错" << endl;
                        }
                        //保存小数指数类型实数
                        fin.putback(currentChar);
                        records.push_back(tmp);
                        continue;
                    } else {
                        //保存小数类型实数
                        fin.putback(currentChar);
                        records.push_back(tmp);
                        continue;
                    }
                }
            } else if (currentChar == 'e') {
                tmp.words += currentChar;
                //TODO 获取指数部分
                fin >> currentChar;
                if (currentChar == '-') {
                    tmp.words += currentChar;
                    fin >> currentChar;
                }
                if (currentChar >= '0' && currentChar <= '9') {
                    while (currentChar >= '0' && currentChar <= '9') {
                        tmp.words += currentChar;
                        fin >> currentChar;
                        if (fin.eof()) {
                            break;
                        }
                    }
                } else {
                    cout << "实数出错" << endl;
                }
                //保存整数指数类型实数
                fin.putback(currentChar);
                records.push_back(tmp);
                continue;

            } else {
                //保存整数实数
                records.push_back(tmp);
                fin.putback(currentChar);
                continue;
            }

        }
            //如果开头是'_'或者字母,判断是自定义标识符还是关键字
        else if (currentChar == '_' || (currentChar >= 'a' && currentChar <= 'z')
                 || (currentChar >= 'A' && currentChar <= 'Z')) {
            //TODO 判断是不是自定义标识符或者关键字
            while ((currentChar >= 'a' && currentChar <= 'z') || (currentChar >= 'A' && currentChar <= 'Z') ||
                   (currentChar >= '0' && currentChar <= '9') || currentChar == '_') {
                tmp.words += currentChar;
                fin >> currentChar;
            }
            fin.putback(currentChar);
            tmp.type = 1;   //自定义标识符
            for (int i = 0; i < 11; i++) {
                if (tmp.words == keyWords[i]) {
                    tmp.type = 3;     //关键字
                    break;
                }
            }
            records.push_back(tmp);
            continue;
        }
            //如果开头是单个运算符,判断是不是运算符
        else if (contain(currentChar)) {
            //TODO 判断是不是运算符
            tmp.type = 4;
            tmp.words += currentChar;
            if (tmp.words == ":" || tmp.words == ">" || tmp.words == "<") {
                fin >> currentChar;
                if (currentChar == '=') {
                    tmp.words += currentChar;
                } else {
                    fin.putback(currentChar);
                    if (tmp.words == ":") {
                        cout << "运算符出错" << endl;
                    }
                }
            }
            records.push_back(tmp);
            continue;
        } else if (currentChar == ';' || currentChar == '(' || currentChar == ')' || currentChar == ',' ||
                   currentChar == '.') {
            //TODO 判断是不是界符
            tmp.type = 5;
            tmp.words = currentChar;
            records.push_back(tmp);
            continue;
        }
    }

    for (Info tmp:records) {
        cout << tmp.type << "\t" << tmp.words << endl;
    }

    return 0;
}