//Copyright 2019 <VladDarb2000>
#ifndef INCLUDE_JSON_HPP_
#define INCLUDE_JSON_HPP_

#include <any>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <typeinfo>
#include <fstream>


using std::cout;
using std::cin;
using std::string;
using std::any;
using std::vector;
using std::map;
using std::any_cast;

class Json {
private:
    map <string, any> json_map;
    vector <any> json_arr;

    bool is_json_array = false;
    bool is_json_object = false;

    void create_vector(const string& s);
    void create_map(const string& s);

public:
    explicit Json(const string& s);

    bool is_array() const;

    bool is_object() const;

    std::any& operator[](const string& key);

    std::any& operator[](int index);

    static Json parse(const string& s);

    static Json parseFile(const string& path_to_file);

    static unsigned int pass (unsigned int i, const string& s);

    static string word_check (unsigned int &i, const string& s);

    static unsigned int closing_bracket (unsigned int i, const string& s);

    static string read_number (unsigned int i, const string& s);
};

#endif // INCLUDE_JSON_HPP_
