#include <iostream>
#include <cassert>
#include "settingstext.h"

enum class TestEnum { aaa, bbb, ccc };

void save();
void load();

std::ostream& operator<<(std::ostream& stream, TestEnum value);
std::istream& operator>>(std::istream& stream, TestEnum &value);

bool to_string(const TestEnum &value, std::string &result);
bool from_string(const std::string &text, TestEnum &value);

int main()
{
    save();
    load();

    std::cout << "All Ok" << std::endl;
    return 0;
}

void save()
{
    std::cout << "Testing saving values." << std::endl;

    SettingsText settings;
    settings.setDescription("Testing settings.\n\nAuthor: Jan Moravec");

    settings.setValue("0", "Test 0");
    settings.setValue("1", std::string("Test 1"));

    settings.setCategory("CATEGORY 2");
    settings.setValue("2", 2);
    settings.setValue("3", 3.123456789);

    settings.setCategory("CATEGORY 1");
    settings.setValue("4", TestEnum::bbb);
    settings.setValue("5", TestEnum::ccc, &to_string);

    settings.setCategory();
    settings.setValue("6", 6);
    settings.setDescription("6", "Testing value description comment.");

    bool result = settings.save("settings.txt");
    assert(result);
}

void load()
{
    std::cout << "Testing loading values." << std::endl;

    SettingsText settings;
    bool result = settings.load("settings.txt");
    assert(result);

    TestEnum test;
    assert(settings.getValue("4", test) && test == TestEnum::bbb);
    assert(settings.getValue("5", test, &from_string) && test == TestEnum::ccc);
}

std::ostream& operator<<(std::ostream& stream, TestEnum value)
{
    switch (value) {
    case TestEnum::aaa:
        return stream << "aaa";
    case TestEnum::bbb:
        return stream << "bbb";
    case TestEnum::ccc:
        return stream << "ccc";
    }

    stream.setstate(std::ios::failbit);
}

std::istream& operator>>(std::istream& stream, TestEnum &value)
{
    std::string text;
    if (stream >> text) {
        if (text == "aaa") {
            value = TestEnum::aaa;
        } else if (text == "bbb") {
            value = TestEnum::bbb;
        } else if (text == "ccc") {
            value = TestEnum::ccc;
        } else {
            stream.setstate(std::ios::failbit);
        }
    }

    return stream;
}

bool to_string(const TestEnum &value, std::string &result)
{
    switch (value) {
    case TestEnum::aaa:
        result = "aaa";
        return true;
    case TestEnum::bbb:
        result = "bbb";
        return true;
    case TestEnum::ccc:
        result = "ccc";
        return true;
    default:
        return false;
    }
}

bool from_string(const std::string &text, TestEnum &value)
{
    if (text == "aaa") {
        value = TestEnum::aaa;
    } else if (text == "bbb") {
        value = TestEnum::bbb;
    } else if (text == "ccc") {
        value = TestEnum::ccc;
    } else {
        return false;
    }

    return true;
}
