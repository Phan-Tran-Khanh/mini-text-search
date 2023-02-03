#include "features.h"

#include <chrono>
using namespace std::chrono;

int main() {
    //PreProc::exec();
    Features f;
    int choice = 0;
    while (true) {
        std::cout << "1. Search\n";
        std::cout << "2. Exit\n";
        std::cout << "Your choice = ";
        std::cin >> choice;
        if (choice == 1) {
            std::string line;
            std::cout << "Enter your query: ";
            std::getline(std::cin, line);

            auto start = high_resolution_clock::now();
            std::map<float, std::string, std::greater<float>> docs = f.Searching(f.Parse(line));
            auto stop = high_resolution_clock::now();
            
            /*
                First ten relevant articles 
            */
            int i = 0;
            for (auto doc : docs) {
                std::cout << doc.first << ' ' << doc.second << std::endl;
                ++i;
                if (i == 10) {
                    break;
                }
            }

            auto duration = duration_cast<microseconds>(stop - start);
            std::cout <<  "Time cost = " << duration.count() << std::endl;
        }
        else {
            break;
        }
    }
    return 0;
}