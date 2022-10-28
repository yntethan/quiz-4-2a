#include <iostream>
#include <thread>
#include <string>
#include <condition_variable>
#include <mutex>
// include additional necessary headers
using namespace std;

mutex m;
condition_variable q,
                   r;
int n = 0;

void query(/*add necessary parameters*/ int c) {
    // Should print: the print number (starting from 0), "SYN", and the three dots "..."
    for (; n < c; n++) {
        unique_lock<mutex> lock(m);
        q.wait(lock, [&]{return n % 2 == 0;});
        cout << "[" << n/2 << "] SYN ... ";
        cout.flush();
        lock.unlock();
        r.notify_one();
    }
}

void response(/*add necessary parameters*/ int c) {
    // Should print "ACK"
    for (; n <= c; n++) {
        unique_lock<mutex> lock(m);
        r.wait(lock, [&]{return n % 2 == 1;});
        cout << "ACK";
        cout.flush();
        lock.unlock();
        q.notify_one();
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: ./handshake <count>\n");
        exit(1);
    }
    /**
     * Steps to follow:
     * 1. Get the "count" from cmd args
     * 2. Create necessary synchronization primitive(s)
     * 3. Create two threads, one for "SYN" and the other for "ACK"
     * 4. Provide the threads with necessary args
     * 5. Update the "query" and "response" functions to synchronize the output
    */
    int count = stoi(argv[1]) * 2;

    thread qthread (query, count),
           rthread (response, count);
    qthread.join();
    rthread.join();
   
    return 0;
}
