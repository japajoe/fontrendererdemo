#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <functional>
#include <vector>

template<typename T>
class EventHandler {
public:       
    void operator += (T callback) {
        callbacks.push_back(callback);
    }

    void add(T callback) {
        callbacks.push_back(callback);
    }

    template<typename ... Param>
    void operator () (Param ... param) {
        if(callbacks.size() == 0)
            return;
        for(size_t i = 0; i < callbacks.size(); i++) {
            callbacks[i](param...);
        }
    }
private:
    std::vector<T> callbacks;
};

#endif