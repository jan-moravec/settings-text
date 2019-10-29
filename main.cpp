#include <iostream>
#include "settingstext.h"

enum class Test { aaa, bbb, ccc };

std::ostream& operator<<(std::ostream& str, Test value)
{
    switch (value) {
    case Test::aaa:
        return str << "aaa";
    case Test::bbb:
        return str << "bbb";
    case Test::ccc:
        return str << "ccc";
    }

    return str << "error";
}

std::istream& operator>>(std::istream& stream, Test &value)
{
    std::string text;
    if (stream >> text) {
        if (text == "aaa") {
            value = Test::aaa;
        } else if (text == "bbb") {
            value = Test::bbb;
        } else if (text == "ccc") {
            value = Test::ccc;
        } else {
            stream.setstate(std::ios::failbit);
        }
    }

    return stream;
}

int main()
{
    SettingsText settings;

    settings.setValue("0", "Test 0");
    settings.setValue("1", std::string("Test 1"));
    settings.setValue("2", 2);
    settings.setValue("3", 3.123456789);

    settings.setValue("4", Test::ccc);

    settings.save("settings.txt");

    Test test;
    if (settings.getValue("4", test)) {
        std::cout << test <<std::endl;
    } else {
        std::cout << "error" <<std::endl;
    }

    std::cout << "All Ok" << std::endl;
    return 0;
}
