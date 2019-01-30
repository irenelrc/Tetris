#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include "observer.h"
#include "info.h"
#include <memory>
using namespace std;


// DO NOT MODIFY THIS FILE!



class Subject {
    std::vector<shared_ptr<Observer>> observers;
public:
    void attach(std::shared_ptr<Observer> o);
    void detach();
    void notifyObservers();
    virtual Info getInfo() const = 0;
    
};





#endif
