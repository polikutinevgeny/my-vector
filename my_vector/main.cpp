#include "my_vector.hpp"
#include <vector>
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace my;

TEST_CASE ("Constructors' tests", "[constructors]") {
    SECTION("Default constructor") {
        vector<int> test;
        REQUIRE(test.empty());
    }
    SECTION("Amount constructor") {
        vector<int> test(10);
        REQUIRE(test.size() == 10);
        REQUIRE(test.capacity() >= 10);
    }
    SECTION("Amount constructor with default value") {
        vector<int> test(10U, 673);
        REQUIRE(test.size() == 10);
        REQUIRE(test.capacity() >= 10);
        bool ok = true;
        for (int i = 0; i < test.size(); ++i) {
            ok = ok && test[i] == 673;
        }
        REQUIRE(ok);
    }
    SECTION("Iterator constructor") {
        std::vector<int> temp(10, 673);
        vector<int> test(temp.begin(), temp.end());
        REQUIRE(test.size() == 10);
        REQUIRE(test.capacity() >= 10);
        bool ok = true;
        for (int i = 0; i < test.size(); ++i) {
            ok = ok && test[i] == 673;
        }
        REQUIRE(ok);

        vector<int> temp2(10U, 673);
        vector<int> test2(temp2.begin(), temp2.end());
        REQUIRE(test2.size() == 10);
        REQUIRE(test2.capacity() >= 10);
        bool ok2 = true;
        for (int i = 0; i < test2.size(); ++i) {
            ok2 = ok2 && test2[i] == 673;
        }
        REQUIRE(ok2);
    }
    SECTION("Copy constructor") {
        vector<int> test(10U, 673);
        vector<int> test2(test);
        REQUIRE(test.size() == 10);
        REQUIRE(test.capacity() >= 10);
        REQUIRE(test2.size() == 10);
        REQUIRE(test2.capacity() >= 10);
        bool ok = true;
        for (int i = 0; i < test.size(); ++i) {
            ok = ok && test[i] == 673;
        }
        REQUIRE(ok);
        bool ok2 = true;
        for (int i = 0; i < test2.size(); ++i) {
            ok2 = ok2 && test2[i] == 673;
        }
        REQUIRE(ok2);
    }
    SECTION("Move constructor") {
        vector<int> test(10U, 673);
        vector<int> test2(std::move(test));
        REQUIRE(test.size() == 0);
        REQUIRE(test.capacity() == 0);
        REQUIRE(test2.size() == 10);
        REQUIRE(test2.capacity() >= 10);
        bool ok = true;
        for (int i = 0; i < test2.size(); ++i) {
            ok = ok && test2[i] == 673;
        }
        REQUIRE(ok);
    }
    SECTION("Init list constructor") {
        vector<int> test = { 0, 1, 2, 3, 4, 5 };
        REQUIRE(test.size() == 6);
        REQUIRE(test.capacity() >= 6);
        bool ok = true;
        for (int i = 0; i < test.size(); ++i) {
            ok = ok && test[i] == i;
        }
        REQUIRE(ok);
    }
}

TEST_CASE ("Operators", "[operators]") {
    SECTION("Copy assignment") {
        vector<int> test;
        vector<int> test2 = { 0, 1, 2, 3, 4, 5 };
        test = test2;
        REQUIRE(test.size() == 6);
        REQUIRE(test.capacity() >= 6);
        REQUIRE(test2.size() == 6);
        REQUIRE(test2.capacity() >= 6);
        bool ok = true;
        for (int i = 0; i < test.size(); ++i) {
            ok = ok && test[i] == i;
        }
        REQUIRE(ok);
        ok = true;
        for (int i = 0; i < test2.size(); ++i) {
            ok = ok && test2[i] == i;
        }
        REQUIRE(ok);
    }
    SECTION("Move assignment") {
        vector<int> test;
        vector<int> test2 = { 0, 1, 2, 3, 4, 5 };
        test = std::move(test2);
        REQUIRE(test.size() == 6);
        REQUIRE(test.capacity() >= 6);
        REQUIRE(test2.size() == 0);
        REQUIRE(test2.capacity() == 0);
        bool ok = true;
        for (int i = 0; i < test.size(); ++i) {
            ok = ok && test[i] == i;
        }
        REQUIRE(ok);
    }
    SECTION("Init list assignment") {
        vector<int> test;
        test = { 0, 1, 2, 3, 4, 5 };
        REQUIRE(test.size() == 6);
        REQUIRE(test.capacity() >= 6);
        bool ok = true;
        for (int i = 0; i < test.size(); ++i) {
            ok = ok && test[i] == i;
        }
        REQUIRE(ok);
    }
}

TEST_CASE("Assigns") {
    SECTION("Iterator assign") {
        vector<int> test = { 0, 1, 2, 3, 4, 5 };
        vector<int> test2 = { 5, 6, 7 };
        test.assign(test2.begin(), test2.end());
        REQUIRE(test.size() == 3);
        REQUIRE(test.capacity() >= 3);
        bool ok = true;
        for (int i = 0; i < test.size(); ++i) {
            ok = ok && test[i] == i + 5;
        }
        REQUIRE(ok);
    }
    SECTION("Filling assign") {
        vector<int> test = { 0, 1, 2, 3, 4, 5 };
        test.assign(10U, 573);
        REQUIRE(test.size() == 10);
        REQUIRE(test.capacity() >= 10);
        bool ok = true;
        for (int i = 0; i < test.size(); ++i) {
            ok = ok && test[i] == 573;
        }
        REQUIRE(ok);
    }
    SECTION("Init list assign") {
        vector<int> test = { 0, 1, 2, 3, 4, 5 };
        test.assign({ 5, 6, 7 });
        REQUIRE(test.size() == 3);
        REQUIRE(test.capacity() >= 3);
        bool ok = true;
        for (int i = 0; i < test.size(); ++i) {
            ok = ok && test[i] == i + 5;
        }
        REQUIRE(ok);
    }
}

TEST_CASE("Iterators") {
    SECTION("Vector with elements") {
        vector<int> test = { 0, 1, 2, 3, 4, 5 };
        auto it = test.begin();
        REQUIRE(*it == 0);
        it = test.end() - 1;
        REQUIRE(it == test.end() - 1);
        {
            bool ok = true;
            int c = 0;
            for (auto i = test.begin(); i != test.end(); ++i, ++c) {
                ok = ok && *i == c;
            }
            REQUIRE(ok);
        }
        {
            bool ok = true;
            int c = 5;
            for (auto i = test.end() - 1;; --i, --c) {
                ok = ok && *i == c;
                if (i == test.begin()) {
                    break;
                }
            }
            REQUIRE(ok);
        }
        REQUIRE(test.begin() < test.end());
        REQUIRE(test.end() > test.begin());
        REQUIRE(test.begin() <= test.end());
        REQUIRE(test.end() >= test.begin());
        REQUIRE(it == test.begin() + 5);
        REQUIRE(it != test.end());
        REQUIRE(it++ == test.end() - 1);
        REQUIRE(it-- == test.end());
        it -= 2;
        REQUIRE(*it == 3);
        it += 2;
        REQUIRE(*it == 5);
        it -= 10;
        REQUIRE(*it == 0);
        it += 10;
        REQUIRE(it == test.end());
        REQUIRE(*--it == 5);
        REQUIRE(++it == test.end());
        it = 3 + test.begin();
        REQUIRE(*it == 3);
        it = test.end();
        REQUIRE(++it == test.end());
    }
    SECTION("Empty vector") {
        vector<int> test;
        REQUIRE(test.begin() == test.end());
    }
    SECTION("Misc") {
        vector<int>::iterator it;
        REQUIRE(it.operator->() == nullptr);
        vector<int> test = { 0, 1, 2, 3, 4, 5 };
        REQUIRE(test.begin()[1] == 1);
    }
}

TEST_CASE("Size management") {
    vector<int> test(10U, 13);
    REQUIRE(test.max_size() == std::numeric_limits<vector<int>::size_type>::max());
    REQUIRE(test.size() == 10);
    REQUIRE(test.capacity() >= 10);
    test.resize(15);
    REQUIRE(test.size() == 15);
    REQUIRE(test.capacity() >= 15);
    {
        bool ok = true;
        for (int i = 0; i < 10; ++i) {
            ok = ok && test[i] == 13;
        }
        REQUIRE(ok);
    }
    test.resize(100);
    REQUIRE(test.size() == 100);
    REQUIRE(test.capacity() >= 100);
    {
        bool ok = true;
        for (int i = 0; i < 10; ++i) {
            ok = ok && test[i] == 13;
        }
        REQUIRE(ok);
    }
    test.resize(5);
    REQUIRE(test.size() == 5);
    REQUIRE(test.capacity() >= 5);
    {
        bool ok = true;
        for (int i = 0; i < 5; ++i) {
            ok = ok && test[i] == 13;
        }
        REQUIRE(ok);
    }
    test.resize(10, 13);
    REQUIRE(test.size() == 10);
    REQUIRE(test.capacity() >= 10);
    {
        bool ok = true;
        for (int i = 0; i < 10; ++i) {
            ok = ok && test[i] == 13;
        }
        REQUIRE(ok);
    }
    test.resize(100, 13);
    REQUIRE(test.size() == 100);
    REQUIRE(test.capacity() >= 100);
    {
        bool ok = true;
        for (int i = 0; i < 100; ++i) {
            ok = ok && test[i] == 13;
        }
        REQUIRE(ok);
    }
    test.resize(0);
    REQUIRE(test.empty());
    {
        auto a = test.capacity();
        test.reserve(test.capacity() + 1);
        REQUIRE(a < test.capacity());
        a = test.capacity();
        test.reserve(test.capacity() - 1);
        REQUIRE(a == test.capacity());
    }
    {
        auto a = test.capacity();
        test.shrink_to_fit();
        REQUIRE(a > test.capacity());
    }
    test.resize(500, 10);
    REQUIRE(test.size() == 500);
    REQUIRE(test.capacity() >= 500);
    {
        bool ok = true;
        for (int i = 0; i < 500; ++i) {
            ok = ok && test[i] == 10;
        }
        REQUIRE(ok);
    }
}

TEST_CASE("Data access") {
    {
        const vector<int> test = { 0,1,2,3,4,5 };
        REQUIRE(test[1] == 1);
        REQUIRE(test.at(2) == 2);
        REQUIRE_THROWS_AS(test.at(6), std::out_of_range);
        REQUIRE(test.front() == 0);
        REQUIRE(test.back() == 5);
        REQUIRE(*test.data() == 0);
    }
    {
        vector<int> test = { 0,1,2,3,4,5 };
        REQUIRE(test[1] == 1);
        REQUIRE(test.at(2) == 2);
        REQUIRE_THROWS_AS(test.at(6), std::out_of_range);
        REQUIRE(test.front() == 0);
        REQUIRE(test.back() == 5);
        REQUIRE(*test.data() == 0);
    }
}

TEST_CASE("Data management") {
    SECTION("Push-pop") {
        vector<int> test = { 0,1,2,3,4,5 };
        test.push_back(6);
        REQUIRE(test.size() == 7);
        REQUIRE(test.capacity() >= 7);
        {
            bool ok = true;
            for (int i = 0; i < 7; ++i) {
                ok = ok && test[i] == i;
            }
            REQUIRE(ok);
        }
        int a = 7;
        test.push_back(a);
        REQUIRE(test.size() == 8);
        REQUIRE(test.capacity() >= 8);
        {
            bool ok = true;
            for (int i = 0; i < 8; ++i) {
                ok = ok && test[i] == i;
            }
            REQUIRE(ok);
        }
        test.pop_back();
        REQUIRE(test.size() == 7);
        REQUIRE(test.capacity() >= 7);
        {
            bool ok = true;
            for (int i = 0; i < 7; ++i) {
                ok = ok && test[i] == i;
            }
            REQUIRE(ok);
        }
        test.push_back(7);
        test.push_back(8);
        REQUIRE(test.size() == 9);
        REQUIRE(test.capacity() >= 9);
        {
            bool ok = true;
            for (int i = 0; i < 9; ++i) {
                ok = ok && test[i] == i;
            }
            REQUIRE(ok);
        }
        test.clear();
        int p = 8;
        test.push_back(p);
        REQUIRE(test.size() == 1);
        REQUIRE(test.capacity() >= 1);
        REQUIRE(test[0] == 8);
    }
    SECTION("Insert") {
        vector<int> test = { 0,1,2,3,4,5 };
        test.insert(test.begin(), -1);
        int t = -2;
        test.insert(test.begin(), t);
        REQUIRE(test.size() == 8);
        REQUIRE(test.capacity() >= 8);
        {
            bool ok = true;
            for (int i = 0; i < 8; ++i) {
                ok = ok && test[i] == i - 2;
            }
            REQUIRE(ok);
        }
        test.insert(test.begin() + 1, 3U, 13);
        REQUIRE(test.size() == 11);
        REQUIRE(test.capacity() >= 11);
        {
            vector<int> r = { -2,13,13,13,-1,0,1,2,3,4,5 };
            bool ok = true;
            for (int i = 0; i < 11; ++i) {
                ok = ok && test[i] == r[i];
            }
            REQUIRE(ok);
        }
        vector<int> tt = { 12, 13 };
        test.insert(test.end(), tt.begin(), tt.end());
        REQUIRE(test.size() == 13);
        REQUIRE(test.capacity() >= 13);
        {
            vector<int> r = { -2,13,13,13,-1,0,1,2,3,4,5,12,13 };
            bool ok = true;
            for (int i = 0; i < 13; ++i) {
                ok = ok && test[i] == r[i];
            }
            REQUIRE(ok);
        }
        test.insert(test.begin(), {67, 23});
        REQUIRE(test.size() == 15);
        REQUIRE(test.capacity() >= 15);
        {
            vector<int> r = { 67,23,-2,13,13,13,-1,0,1,2,3,4,5,12,13 };
            bool ok = true;
            for (int i = 0; i < 15; ++i) {
                ok = ok && test[i] == r[i];
            }
            REQUIRE(ok);
        }
        test.insert(test.begin(), { 67, 23 });
        REQUIRE(test.size() == 17);
        REQUIRE(test.capacity() >= 17);
        {
            vector<int> r = { 67,23,67,23,-2,13,13,13,-1,0,1,2,3,4,5,12,13 };
            bool ok = true;
            for (int i = 0; i < 17; ++i) {
                ok = ok && test[i] == r[i];
            }
            REQUIRE(ok);
        }
        test.clear();
        test.insert(test.begin(), 10);
        REQUIRE(test.size() == 1);
        REQUIRE(test.capacity() >= 1);
        REQUIRE(test[0] == 10);
        test.clear();
        int a = 10;
        test.insert(test.begin(), a);
        REQUIRE(test.size() == 1);
        REQUIRE(test.capacity() >= 1);
        REQUIRE(test[0] == 10);
        test.clear();
        test.insert(test.begin(), tt.begin(), tt.end());
        REQUIRE(test.size() == 2);
        REQUIRE(test.capacity() >= 2);
        REQUIRE(test[0] == 12);
        REQUIRE(test[1] == 13);
    }
    SECTION("Erase") {
        vector<int> test = { 0,1,2,3,4,5 };
        test.erase(test.begin() + 2);
        REQUIRE(test.size() == 5);
        REQUIRE(test.capacity() >= 5);
        {
            vector<int> r = { 0,1,3,4,5 };
            bool ok = true;
            for (int i = 0; i < 5; ++i) {
                ok = ok && test[i] == r[i];
            }
            REQUIRE(ok);
        }
        test.erase(test.begin() + 2, test.end() - 1);
        REQUIRE(test.size() == 3);
        REQUIRE(test.capacity() >= 3);
        {
            vector<int> r = { 0,1,5 };
            bool ok = true;
            for (int i = 0; i < 3; ++i) {
                ok = ok && test[i] == r[i];
            }
            REQUIRE(ok);
        }
    }
}

TEST_CASE("Misc") {
    vector<int> test = { 0,1,2,3,4 };
    vector<int> test2 = { 5,6,7,8,9 };
    test.swap(test2);
    REQUIRE(test.size() == 5);
    REQUIRE(test.capacity() >= 5);
    REQUIRE(test2.size() == 5);
    REQUIRE(test2.capacity() >= 5);
    {
        vector<int> r = { 0,1,2,3,4 };
        bool ok = true;
        for (int i = 0; i < 5; ++i) {
            ok = ok && test2[i] == r[i];
        }
        REQUIRE(ok);
    }
    {
        vector<int> r = { 5,6,7,8,9 };
        bool ok = true;
        for (int i = 0; i < 5; ++i) {
            ok = ok && test[i] == r[i];
        }
        REQUIRE(ok);
    }
    test.clear();
    REQUIRE(test.size() == 0);
    REQUIRE(test.capacity() == 0);
}