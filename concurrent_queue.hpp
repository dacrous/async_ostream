/*
 * Copyright (c) 2012 Mateusz Pusz
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <queue>
#include <thread>
#include <condition_variable>


template<typename T>
class concurrent_queue {
    std::queue<T> _queue;
    std::condition_variable _newItemReady;
    std::mutex _mutex;
public:
    void push(T &&msg){

        std::lock_guard<std::mutex> lock(_mutex);

        const bool wasEmpty = _queue.empty();

        _queue.emplace(std::move(msg));

        if(wasEmpty){
            _newItemReady.notify_one();
        }
    }

    T pop(){

        std::unique_lock<std::mutex> lock(_mutex);

        _newItemReady.wait(lock, [&]{ return !_queue.empty(); });

        T msg = std::move(_queue.front());
        _queue.pop();

        return msg;
    }
};
