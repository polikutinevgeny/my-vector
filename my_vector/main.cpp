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
        test = { 5, 6, 7 };
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
    }
}