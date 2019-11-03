#include <iostream>
#include <cassert>
#include "settingstext.h"

enum class Test { aaa, bbb, ccc };

std::ostream& operator<<(std::ostream& stream, Test value)
{
    switch (value) {
    case Test::aaa:
        return stream << "aaa";
    case Test::bbb:
        return stream << "bbb";
    case Test::ccc:
        return stream << "ccc";
    }

    stream.setstate(std::ios::failbit);
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

bool to_string(const Test &value, std::string &result)
{
    switch (value) {
    case Test::aaa:
        result = "aaa";
        return true;
    case Test::bbb:
        result = "bbb";
        return true;
    case Test::ccc:
        result = "ccc";
        return true;
    default:
        return false;
    }
}

bool from_string(const std::string &text, Test &value)
{
    if (text == "aaa") {
        value = Test::aaa;
    } else if (text == "bbb") {
        value = Test::bbb;
    } else if (text == "ccc") {
        value = Test::ccc;
    } else {
        return false;
    }

    return true;
}

int main()
{
    SettingsText settings;
    settings.setDescription("Testing settings.\n\nAuthor: Jan Moravec");

    settings.setValue("0", "Test 0");
    settings.setValue("1", std::string("Test 1"));
    settings.setValue("2", 2);
    settings.setValue("3", 3.123456789);

    settings.setValue("4", Test::bbb);
    settings.setValue("5", Test::ccc, &to_string);

    assert(settings.save("settings.txt"));
    assert(settings.load("settings.txt"));

    Test test;
    if (settings.getValue("4", test)) {
        std::cout << test <<std::endl;
    } else {
        std::cout << "error" <<std::endl;
    }

    if (settings.getValue("5", test, &from_string)) {
        std::cout << test <<std::endl;
    } else {
        std::cout << "error" <<std::endl;
    }

    std::cout << settings.value("0") << std::endl;

    std::cout << "All Ok" << std::endl;
    return 0;
}
