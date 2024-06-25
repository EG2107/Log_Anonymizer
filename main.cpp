#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> phone_number_templates;

struct UTF8Char {
    char a, b;
    UTF8Char(char a, char b): a(a), b(b) {}
    bool isRus(){
        return (((int) a + 256) == 208) && ((((int) b + 256) >= 144 && ((int) b + 256) <= 191) || (((int) b + 256) == 129)) ||
            (((int) a + 256) == 209) && ((((int) b + 256) >= 128 && ((int) b + 256) <= 143) || (((int) b + 256) == 145));
    }
};

void input_templates(){
    std::ifstream fin_templates("phone_number_templates.txt");
    std::string phone_template;
    while (std::getline(fin_templates, phone_template)){
        phone_number_templates.push_back(phone_template);
    }
}

bool is_digit(char c){
    return '0' <= c && c <= '9';
}

unsigned int ip_to_ul(std::string ip){
    ip += '.';
    unsigned int ans = 0, block = 0;
    for (char c : ip){
        if (c == '.'){
            ans *= 256;
            ans += block;
            block = 0;
        } else {
            block *= 10;
            block += c - '0';
        }
    }
    return ans;
}

std::string ul_to_ip(unsigned int ul_ip){
    std::vector<std::string> ip;
    for (int i = 0; i < 4; ++i){
        ip.push_back(std::to_string(ul_ip % 256));
        ul_ip /= 256;
    }
    std::string res;
    for (int i = ip.size() - 1; i >= 0; --i){
        res += ip[i];
        if (i > 0){
            res += '.';
        }
    }
    return res;
}

long long phone_to_ll(std::string phone){
    long long res = 0;
    for (char c : phone){
        if (is_digit(c)){
            res *= 10;
            res += c - '0';
        }
    }
    return res;
}

bool is_phone_number(std::string str){
    for (std::string phone_template : phone_number_templates){
        if (str.size() != phone_template.size()){
            continue;
        }
        bool flag = true;
        for (int i = 0; i < str.size(); ++i){
            if (str[i] != phone_template[i]){
                if (is_digit(str[i])){
                    if ((i == 0 && phone_template[0] == '8' && phone_template.size() > 9) || (i == 1 && phone_template[0] == '+')){
                        flag = false;
                        break;
                    }
                } else {
                    flag = false;
                    break;
                }
            }
        }
        if (flag){
            return true;
        }
    }
    return false;
}

bool is_ip(std::string str){
    if (str.size() > 15){
        return false;
    }
    std::string cur_block;
    int cnt_dots = 0, cnt_digits_in_block = 0;
    for (char c : str){
        if (c == '.'){
            cnt_dots++;
            cnt_digits_in_block = 0;
            cur_block = "";
        } else if (is_digit(c)){
            cur_block += c;
            cnt_digits_in_block++;
            if (cnt_digits_in_block > 3 || std::stoll(cur_block) >= 256){
                return false;
            }
        } else {
            return false;
        }
    }
    return cnt_dots == 3;
}

bool is_russian(std::string str){
    for (int i = 0; i < str.size(); i += 2){
        if (!UTF8Char(str[i], str[i + 1]).isRus()){
            return false;
        }
    }
    return true;
}

int main(){
    setlocale(LC_ALL, "Russian");

    input_templates();

    std::ifstream fin_logs("in.log");
    std::ofstream fout_logs("out.log");
    std::ofstream fout_map("convert_map.txt");

    std::unordered_map<long long, unsigned int> mp_phones;
    std::vector<long long> all_phones;
    std::unordered_map<unsigned int, unsigned int> mp_ips;
    std::vector<unsigned int> all_ips;
    std::unordered_map<std::string, unsigned int> mp_names;
    std::vector<std::string> all_names;

    std::string line;
    while (std::getline(fin_logs, line)){
        std::stringstream sstream(line);
        std::string word;
        while (sstream >> word){
            if (is_phone_number(word)){
                long long phone = phone_to_ll(word);
                if (!mp_phones.count(phone)){
                    mp_phones[phone] = mp_phones.size();
                    all_phones.push_back(phone);
                }
                fout_logs << "phone" << mp_phones[phone] << " ";
            } else if (is_ip(word)){
                unsigned int ip = ip_to_ul(word);
                if (!mp_ips.count(ip)){
                    mp_ips[ip] = mp_ips.size();
                    all_ips.push_back(ip);
                }
                fout_logs << "ip" << mp_ips[ip] << " ";
            } else if (is_russian(word)){
                if (!mp_names.count(word)){
                    mp_names[word] = mp_names.size();
                    all_names.push_back(word);
                }
                fout_logs << "name" << mp_names[word] << " ";
            } else {
                fout_logs << word << " ";
            }
        }
        fout_logs << "\n";
    }

    fout_map << "Phone numbers:\n";
    for (int i = 0; i < all_phones.size(); ++i){
        fout_map << all_phones[i] << " " << i << "\n";
    }
    fout_map << "\n";

    fout_map << "IPs:\n";
    for (int i = 0; i < all_ips.size(); ++i){
        fout_map << ul_to_ip(all_ips[i]) << " " << i << "\n";
    }
    fout_map << "\n";

    fout_map << "Names:\n";
    for (int i = 0; i < all_names.size(); ++i){
        fout_map << all_names[i] << " " << i << "\n";
    }
    fout_map << "\n";

    return 0;
}