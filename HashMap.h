#include <string>
#include <vector>
#include <utility>
#include <list>
#include <iostream>
#ifndef HASHMAP_H
#define HASHMAP_H
using namespace std;

template <class T>
class Hash_Map
{
private:
    vector<vector<T>> data;

    int hash(string date) //Turns string date into an int to be hashed
    {
        int day = stoi(date.substr(8, 2));
        int month = stoi(date.substr(5, 2));
        int year = stoi(date.substr(0, 4));


        return ((day - 1) + (month - 1) * 31 + (year - 2012) * 416) - 27;
    }


public:
    Hash_Map() //Default constructor, resizes hashmap vector
    {
        data.resize(4000);
    }

    void insert(T value, string key) //Inserts new element into hashmap
    {
        int hKey = hash(key);
        data[hKey].push_back(value);
    }

    vector<T>& operator[](string key) //Allows [] to access a value when a string is used
    {
        return data[hash(key)];
    }

    vector<T>& operator[](int index) { //Allows [] to access a value when an int is used
        return data[index];
    }
    
};
#endif