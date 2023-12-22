#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <fstream>
#include <cassert>
using namespace std;

class Domain {
public:
    
    Domain(const string& domain)
        :domain_(domain)
    {
        std::reverse(domain_.begin(), domain_.end());
    }
    bool operator==(const Domain& other) const{
        return domain_ == other.domain_;
    }
    bool operator<(const Domain& other) const{
        return lexicographical_compare(domain_.begin(), domain_.end(), other.domain_.begin(), other.domain_.end());
    }

    bool operator>(const Domain& other) const{
        return other < *this;
    }

    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    
    bool IsSubdomain(const Domain& other) const{
        if (other.domain_.size() >= domain_.size()) {
            return false;
        }    
        else if (domain_.substr(0, other.domain_.size()) == other.domain_ && domain_[other.domain_.size()] == '.') {
            return true;
        }
        else
        return false;
    }

    string Get() {
        return domain_;
    }
private:
    string domain_;
};

class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template <typename It>
    DomainChecker(It first, It last)
        :domains_(first, last)
    {
        SortUnique();
    }
    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& domain) {

        auto result = upper_bound(domains_.begin(), domains_.end(), domain, [&domain](Domain lhs, Domain rhs) {
            if (domain.IsSubdomain(lhs) || lhs == domain) {
                lhs < rhs;
            }
            return lhs < rhs;
            });
        ;
        if (result == domains_.begin()) {
            return false;
        }

        else
        {
            auto res = result - 1;
            if (domain.IsSubdomain(*res) || *res == domain) {
                return true;

            }
            else return false;

        }
    }
private:
    std::vector<Domain> domains_;

    void SortUnique() {
        sort(domains_.begin(), domains_.end(), [](Domain lhs, Domain rhs) {
            return lhs < rhs;
            });

        domains_.erase(unique(domains_.begin(), domains_.end(), [](Domain lhs, Domain rhs) {
            if (rhs.IsSubdomain(lhs)) {
                lhs = rhs;
            }
            return lhs == rhs;
            }), domains_.end());
    }
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

const std::vector<Domain> ReadDomains(istream& input, int number) {
    std::vector<Domain> domains_;

    for (int i = 0; i != number; ++i) {
        string line;
        getline(input, line);
        Domain domain(line);
        domains_.push_back(domain);
    }
   
    return domains_;
}

void Test() {
    Domain bad("oper.ru");
    Domain subdomen("goblin.oper.ru");
    Domain not_sub("autor.ru");
    Domain not_sub2("coper.ru");
    Domain not_sub3("ru");
    assert(subdomen.IsSubdomain(bad) == true);
    assert(not_sub.IsSubdomain(bad) == false);
    assert(not_sub2.IsSubdomain(bad) == false);
    assert(not_sub3.IsSubdomain(bad) == false);
}

int main() {
    Test();

     
     const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
     DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

     const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
     for (const Domain& domain : test_domains) {
         cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
     }
}
    









    

