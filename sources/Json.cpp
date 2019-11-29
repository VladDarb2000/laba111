//Copyright 2019 <VladDarb2000>
#include <Json.hpp>
Json::Json(const string& s) {
    unsigned int i = 0;
    i = pass (i, s);
    if (s[i] == '[') {
        is_json_array = true;
        create_vector(s);
    }else if (s[i] == '{') {
        is_json_object = true;
        create_map(s);
    } else {
        throw std::logic_error("NOT SUITABLE");
    }
}
bool Json::is_array() const{
    return is_json_array;
}

bool Json::is_object() const{
    return is_json_object;
}

std::any& Json::operator[](const string& key) {
    if (Json::is_object()) {
        return this->json_map[key];
    } else {
        throw std::logic_error(" not object");
    }
}

std::any& Json::operator[](int index) {
    if (is_array()) {
        return this->json_arr[index];
    } else {
        throw std::logic_error("not array");
    }
}
Json Json::parse(const std::string& s) {
    Json obj(s);
    return obj;
}

Json Json::parseFile(const std::string& path_to_file){
    std::ifstream json;
    json.open(path_to_file);
    string s = "";
    string line;
    while (!json.eof()) {
        std::getline(json, line);
        s += line;
    }
    Json obj(s);
    return obj;
}

unsigned int Json::pass (unsigned int i, const string& s) {

    if(s[i] == ' '){
        i++;
    }
    else if(s[i] == ',')
    {
        i++
    }
    return i;
}

string Json:: word_check (unsigned int &i, const string& s) {
    unsigned int symbols;
    string word;
    i = pass (i, s);
    if (s[i] == '\"') {
        i++;
        symbols = i;
    } else {
        throw std::logic_error("not our line");
    }
    while (s[i] != '"') i++;
    word = s.substr(symbols, i - symbols);
    i++;
    return word;
}

unsigned int Json:: closing_bracket (unsigned int i, const string& s) {
    unsigned int open_bracket = 1 , close_bracket = 0;
    char first, second;
    if(s[i] == '{') {
        first = '{';
        second = '}';
    }
    else if (s[i] == '[') {
        first = '[';
        second = ']';
    }
    while ( close_bracket != open_bracket && i < s.length()) {
        i++;
        if (s[i] == first) open_bracket++;
        else if (s[i] == second) close_bracket++;
    }
    if (close_bracket != open_bracket) return s.length();
    return i;
}

string Json::read_number (unsigned int i, const string& s) {
    unsigned int st = i;
    string num;
    while (isdigit(static_cast<unsigned char>(s[i]))) i++;
    num = s.substr(st, i - st);
    return num;
}
void Json::create_map(const string& s) {
    unsigned int i = 1;
    while (i < s.find_last_of('}')) {
        string key;
        pass (i, s);
        key = word_check (i, s);
        i = pass (i, s);
        if (s[i] != ':')
            throw std::logic_error("not our line");
        i++;
        i = pass(i, s);

        if (s[i] == '\"') {
            string word;
            word = word_check (i, s);
            i = pass (i, s);
            this->json_map[key] = word;
        }
        else if (s[i] == '{') {
            string wd;
            unsigned int n = closing_bracket (i, s);
            if (n == s.length())
                throw std::logic_error("not our line");
            wd = s.substr(i, n - i + 1);
            Json obj(wd);
            this->json_map[key] = obj.json_map;
            i += wd.length();
        }
        else if (std::isdigit(static_cast<unsigned char>(s[i])) ||
                 (s[i] == '-' && std::isdigit(static_cast<unsigned char>(s[i + 1]))))
        {
            string num = read_number (i, s);
            i += num.length();
            double d = stod(num);
            if (d - static_cast<int>(d) == 0) {
                int n = static_cast<int>(d);
                this->json_map[key] = n;
            } else {
                this->json_map[key] = d;
            }
        }
        else if (s[i] == '[') {
            string wd;
            unsigned int n = closing_bracket (i, s);
            if (n == s.length())
                throw std::logic_error("not our line");
            wd = s.substr(i, n - i + 1);
            Json obj(wd);
            this->json_map[key] = obj.json_arr;
            i += wd.length();
        }
        else if ((s[i] == 't' && s[i + 3] == 'e')
                 || (s[i] == 'f' && s[i + 3] == 's' && s[i + 4] == 'e')) {
            bool x;
            if (s[i] == 't') {
                i += 4;
                x = true;
            } else if (s[i] == 'f'){
                i += 5;
                x = false;
            }
            this->json_map[key] = x;
        }
        i = pass(i, s);
    }
}
void Json::create_vector(const string& s) {
    unsigned int i = 1;
    while (i < s.find_last_of(']')) {
        pass (i, s);

        else if (s[i] == '\"') {
            string word;
            word = read_word(i, s);
            i = skip_spaces(i, s);
            this->json_arr.emplace_back(word);
        }
        else if (s[i] == '{') {
            string wd;
            unsigned int n = closing_bracket (i, s);
            if (n == s.length())
                throw std::logic_error("not our line");
            wd = s.substr(i, n - i + 1);
            Json obj(wd);
            this->json_arr.emplace_back(obj.json_map);
            i += wd.length();
        }
        else if (std::isdigit(static_cast<unsigned char>(s[i]))||
                 (s[i] == '-' && std::isdigit(static_cast<unsigned char>(s[i + 1])))){
            string num = read_number (i, s);
            i += num.length();
            double d = stod(num);
            if (d - static_cast<int>(d) == 0) {
                int n = static_cast<int>(d);
                this->json_arr.emplace_back(n);
            } else {
                this->json_arr.emplace_back(d);
            }
        }
        else if (s[i] == '[') {
            string wd;
            unsigned int n = closing_bracket (i, s);
            if (n == s.length())
                throw std::logic_error("not our line");
            wd = s.substr(i, n - i + 1);
            Json obj(wd);
            this->json_arr.emplace_back(obj.json_arr);
            i += wd.length();
        }
        else if ((s[i] == 't' && s[i + 3] == 'e') ||
                 (s[i] == 'f' &&  s[i + 3] == 's' && s[i + 4] == 'e')) {
            bool x;
            if (s[i] == 't') {
                i += 4;
                x = true;
            }
            else if( s[i] == 'f') {
                i += 5;
                x = false;
            }
            this->json_arr.emplace_back(x);
        }

        i = pass (i, s);
    }
}
