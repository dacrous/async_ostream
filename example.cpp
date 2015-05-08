#include "async_ostream.hpp"

#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

void thread0(int i){
    for(int i = 0; i < 25; ++i){
        async_cout("Hi from ", i, "\n");
    }
}

int main() {

    for(int i=0; i<125; i++){

        boost::thread(boost::bind(thread0, i));
    }

    std::cout << "Done!\n";

}
