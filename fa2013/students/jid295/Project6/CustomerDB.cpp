#include <iostream>
#include <cassert>
#include "String.h"
#include "CustomerDB.h"

using namespace std;

const int default_capacity = 1;

Customer::Customer(String name) {
    this->bottles = 0;
    this->rattles = 0;
    this->diapers = 0;
    this->name = name;
}

CustomerDB::CustomerDB(void) {
    this->capacity = default_capacity;
    this->data = new Customer[this->capacity];
    this->length = 0;
}

int CustomerDB::size(void) {
    return this->length;
}

CustomerDB::~CustomerDB(void) {
    delete[] this->data;
}

void CustomerDB::clear(void) {
    delete[] this->data;
    this->capacity = default_capacity;
    this->data = new Customer[this->capacity];
    this->length = 0;
}

Customer& CustomerDB::operator[](int k) {
    assert(k >= 0 && k < this->length);
    return this->data[k];
}

Customer& CustomerDB::operator[](String name) {
    int i;
    for (i = 0; i < this->length; ++i) {
        if (this->data[i].name == name) {
            return this->data[i];
        }
    }
    Customer new_member;
    new_member.name = name;
    new_member.bottles = 0;
    new_member.rattles = 0;
    new_member.diapers = 0;
    if ((this->length)+1 > this->capacity) {
        /* Double database size. */
        this->capacity = this->capacity*2+1;
        Customer* temp_member = new Customer[this->capacity];;
        for (i = 0; i < this->length; ++i) {
            temp_member[i] = this->data[i];
        }
        this->~CustomerDB();
        this->data = temp_member;
    }
    this->data[this->length] = new_member;
    ++(this->length);
    return this->data[this->length-1];
}

bool CustomerDB::isMember(String name) {
    for (int i = 0; i < this->length; ++i) {
        if (this->data[i].name == name) {
            return true;
        }
    }
    return false;
}
