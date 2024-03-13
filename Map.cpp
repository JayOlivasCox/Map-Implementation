#include <iostream>
#include <string>
#include "Map.h"

using namespace std;

using KeyType = std::string;
using ValueType = double;

int Convert_Key(string key)
{
    long int i = 0;

    for (unsigned int j = 0; j < key.length(); j++) {
        i += static_cast<int>(key[j]);
    }

    return i % 5;
}

int Convert_Key(double key)
{
    int i = static_cast<int>(key) % 5;

    return i;
}



Map::Map() {
    for (int i = 0; i < 5; i++) {
        arr[i] = new Node;
    }
}
Map::~Map() {

    for (int i = 0; i < 5; i++) {
        Node* tmp = arr[i];
        Node* behindTmp = nullptr;

        while (behindTmp != nullptr) {
            behindTmp = tmp;
            tmp = tmp->next;
            delete behindTmp;
        }
    }
}

bool Map::empty() const {
    return (sizeOfMap == 0) ? true : false;
}

int Map::size() const { return sizeOfMap; }

bool Map::insert(const KeyType& key, const ValueType& value) {
    int idx = Convert_Key(key);

    //In case the hashed idx's list in the array is empty, add it there.
    if (arr[idx]->next == nullptr) {
        arr[idx]->next = new Node(value, key);
        sizeOfMap++;
        return true;
    }

    Node* temp = arr[idx]->next;

    while (temp != nullptr) {
        // I used this if statement to know if that temp would be at the last value of the list here.
        if (temp->next == nullptr) { 
            temp->next = new Node(value, key);
            sizeOfMap++;
            return true;
        }
        // I made this line so if the key/value pair is already in the list, it returns false
        if (temp->key == key && temp->data == value) return false; 
        temp = temp->next;
    }
    return false;

}

bool Map::update(const KeyType& key, const ValueType& value) {
    int idx = Convert_Key(key);

    Node* temp = arr[idx]->next;

    while (temp != nullptr) {
        if (temp->key == key) {
            temp->data = value;
            return true;
        }
        temp = temp->next;
    }
    // This is for the case that no Node in the list at the correct index has the same key as what was provided.
    return false; 
}


bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    int idx = Convert_Key(key);

    Node* temp = arr[idx]->next;

    while (temp != nullptr) {
        if (temp->key == key) {
            temp->data = value;
            return true;
        }

        if (temp->next == nullptr) {
            temp->next = new Node(value, key);
            sizeOfMap++;
            return true;
        }
        temp = temp->next;

    }
    return false;
}
//This function goes through all the values in the hash table looking for the "value" variabl.
// I used a temporary variable to traverse the hash table so I wouldn't change the value in the actual values in the array.



bool Map::erase(const KeyType& key) {
    int idx = Convert_Key(key);

    Node* tmp1 = arr[idx]->next;
    Node* tmp2 = nullptr;

    while (tmp1 != nullptr) {
        if (tmp1->key == key) {
            if (tmp1 == arr[idx]->next) {
                arr[idx]->next = tmp1->next;
            }
            else {
                tmp2->next = tmp1->next;
            }
            delete tmp1;
            sizeOfMap--;
            return true;

        }
        tmp2 = tmp1;
        tmp1 = tmp1->next;
    }
    return false;
}
//This function pretty much uses an erase function similar to a link-list's to erase a node.
//The difference is that this one uses the hash function to see which list needs to be 
//  checked for the value to be erased.


bool Map::contains(const KeyType& key) const {
    int idx = Convert_Key(key);

    for (Node* tmp = arr[idx]->next; tmp != nullptr; tmp = tmp->next) {
        if (tmp->key == key) {
            return true;
        }
    }
    return false;
}
//This function simply checks in every list to see if a Node with the key value "key" is in the hash table.


bool Map::get(const KeyType& key, ValueType& value) const {
    int idx = Convert_Key(key);

    for (Node* tmp = arr[idx]->next; tmp != nullptr; tmp = tmp->next) {
        if (tmp->key == key) {
            value = tmp->data;
            return true;
        }
    }

    return false;
}
//This function checks to see if there is a Node with the key "key" and does it
//it simlpy uses a temporary node to traverse the linked-list to see if it has the said Node.


bool Map::get(int i, KeyType& key, ValueType& value) const {
    int counter = 0;

    for (int j = 0; j < 5; j++) {
        for (Node* tmp = arr[j]->next; tmp != nullptr; tmp = tmp->next) {
            if (counter == i) {
                value = tmp->data;
                key = tmp->key;
                return true;
            }
            counter++;
        }
    }
    return false;
}
//This second get function does the same thing as the other but instead of looking for 
//a key it loooks for the ith node in the hash table.


void Map::swap(Map& other) {
    for (int i = 0; i < 5; i++) {
        Node* tmp = other.arr[i];
        other.arr[i] = arr[i];
        arr[i] = tmp;
    }
}
//This function swaps the values in two different hash tables by setting each 
//linked list equals to the other by using a temporary list. It goes to all the
//lists by using the hash values.


bool combine(const Map& m1, const Map& m2, Map& result) {
    bool combOrNot = true;
    for (int i = 0; i < (m1.size() < m2.size() ? m2.size() : m1.size()); i++) {
        ValueType tmpVal;
        KeyType tmpKey;
        bool bul;
        if (m1.size() < i)
            bul = m1.get(i, tmpKey, tmpVal);

        if (m2.contains(tmpKey)) {
            ValueType tmpVal2;
            bul = m2.get(tmpKey, tmpVal2);
            if (tmpVal2 == tmpVal) {
                result.insert(tmpKey, tmpVal);
            }
            else {
                combOrNot = false;
            }
        }
        else {
            result.insert(tmpKey, tmpVal);
        }
        if (m1.size() < i)
            bul = m2.get(i, tmpKey, tmpVal);
        result.insert(tmpKey, tmpVal);
    }
    return combOrNot;
}
//This function uses a loop looking at the bigger table between two of them 
//for the boundary. It then finds values that are different between the hash tables
//if the values have different values but same keys it throws them off. 
//All the values that fit the conditions are added onto the "result" hash table.


void subtract(const Map& m1, const Map& m2, Map& result) {
    for (int i = 0; i < m1.size(); i++) {
        KeyType tmpKey;
        ValueType tmpVal;
        bool bul;
        m1.get(i, tmpKey, tmpVal);
        if (!m2.contains(tmpKey))
            result.insert(tmpKey, tmpVal);
    }
}
//This function just adds values from "m1" into "result" if they don't appear in 
//"m2", it does this by looping through all the values in "m1", then checks if 
//they are in "m2", and if they aren't, it adds them to "result".




