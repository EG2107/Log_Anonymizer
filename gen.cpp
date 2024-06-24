#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <random>
#include <unordered_map>
#include <vector>

const int MSG_AMOUNT = 1e4;

std::mt19937 rng(42239777);
std::string alphabet;
std::vector<std::string> phone_numbers, surnames, first_names, last_names;

std::string gen_message(){
    std::uniform_int_distribution<int> dis(32, 126);
    std::string res = "";
    int len = rng() % 100 + 1;
    for (int i = 0; i < len; ++i){
        res += char(dis(rng));
    }
    return res;
}

std::string gen_ip(){
    std::string res;
    for (int i = 0; i < 4; ++i){
        int block = rng() % 256;
        res += std::to_string(block);
        if (i < 3){
            res += '.';
        }
    }
    return res;
}

std::string gen_user_data(){
    return surnames[rng() % surnames.size()] + ' ' 
        + first_names[rng() % first_names.size()] + ' ' 
        + last_names[rng() % last_names.size()] + ' ' 
        + phone_numbers[rng() % phone_numbers.size()] + ' ' 
        + gen_ip();
}

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    for (char c = 'a'; c <= 'z'; ++c){
        alphabet += c;
    }
    for (char c = 'A'; c <= 'Z'; ++c){
        alphabet += c;
    }
    for (char c = '0'; c <= '9'; ++c){
        alphabet += c;
    }
    alphabet += ' ';

    std::fstream fout("in.log");

    phone_numbers = {"+7911-123-45-67", "+7921-239-42-69", "567-89-10", "8(931)523-65-67", "+79541234567", "8921-123-45-69"};
    first_names = {"Иван", "Василий", "Константин", "Петр", "Алексей", "Дмитрий"};
    surnames = {"Иванов", "Петров", "Сидоров", "Алексеев", "Авербах"};
    last_names = {"Иванович", "Петрович", "Константинович", "Евгеньевич"};

    for (int i = 0; i < MSG_AMOUNT; ++i){
        fout << "'" << gen_message() << "' was sent from " << gen_user_data() << " to " << gen_user_data() << '\n';
    }

    return 0;
}