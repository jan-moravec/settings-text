#include <iostream>
#include "settingstext.h"

int main()
{
    SettingsText settings;

    settings.setValue("0", "Test 0");
    settings.setValue("1", std::string("Test 1"));
    settings.setValue("2", 2);
    settings.setValue("3", 3.123456789);

    settings.save("settings.txt");

    std::cout << "All Ok" << std::endl;
    return 0;
}
