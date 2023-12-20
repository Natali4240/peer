#include <iostream>
#include <vector>
#include <iomanip> 
#include <algorithm>
#include<execution>

using namespace std;

const int MAX_USERS_COUNT = 100000;
const int MAX_PAGES_COUNT = 1000;

struct Reader {
    Reader()
        : user_to_pages(MAX_USERS_COUNT + 1, 0),
        readed_by_users(MAX_PAGES_COUNT + 1, 0)
    {}
    vector<int> user_to_pages;
    vector<int> readed_by_users;
    int count_users = 0;
};

void AddPagesToUser(int id, int page, Reader& read) {

    if (read.user_to_pages[id] != 0) {
        
        auto it = read.readed_by_users.begin() + read.user_to_pages[id] + 1;
        auto it1 = read.readed_by_users.begin() + page + 1;
        std::for_each(std::execution::par, it, it1, [](int& n) { n++; });
       
    }

    else { 
        read.count_users += 1;
        auto it = read.readed_by_users.begin() + page + 1;
        std::for_each(std::execution::par, read.readed_by_users.begin() + 1, it, [](int& n) { ++n; });
       
    }
    read.user_to_pages[id] = page;
}

double ComputeToCheer(int id, const Reader& read) {
    double res = 0.0;
    int i = read.user_to_pages[id];

    if (i == 0 ) {
        res = 0.0;
    }
    
    else if (read.count_users == 1 && i != 0) {
        res = 1.0;
    }
    else {
        double  readed_less = read.count_users - read.readed_by_users[i];
        double other_users = (read.count_users - 1) * 1.0;
        res = readed_less / other_users;
    }
    
    return res;
}

int main() {
   
    Reader read;
    int count;
    cin >> count;
    for (int i = 0; i < count; ++i) {
        string action;
        cin >> action;

        if (action == "READ") {
            int id;
            int page;
            cin >> id >> page;
            AddPagesToUser(id, page, read);

        }
        else if (action == "CHEER") {
            int id_;
            cin >> id_;
            double res = ComputeToCheer(id_, read);
            cout << setprecision(6) << res << endl;
        }
    }
}

