#ifndef SETTINGSTEXT_H
#define SETTINGSTEXT_H

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unordered_map>

#include <iostream>

class SettingsText
{
public:
    SettingsText();

    template<typename T> void setValue(const std::string &key, const T &value);
    void setValue(const std::string &key, const std::string &value);
    void setValue(const std::string &key, const char *value);

    template<typename T> bool getValue(const std::string &key, T &value);
    bool getValue(const std::string &key, std::string &value);

    bool load(const std::string &file_name);
    bool save(const std::string &file_name);

private:
    std::unordered_map<std::string, std::string> settings;

    void set(std::stringstream &sstream);
};

SettingsText::SettingsText()
{
}

template<typename T>
void SettingsText::setValue(const std::string &key, const T &value)
{
    std::stringstream sstream;
    set(sstream);
    sstream << value;
    settings[key] = sstream.str();
}

void SettingsText::setValue(const std::string &key, const std::string &value)
{
    settings[key] = value;
}

void SettingsText::setValue(const std::string &key, const char *value)
{
    settings[key] = value;
}

template<typename T>
bool SettingsText::getValue(const std::string &key, T &value)
{
    if (settings.find(key) == settings.end()) {
        return false;
    }

    std::stringstream sstream(settings[key]);
    set(sstream);
    if (sstream >> value) {
        return true;
    }

    return false;
}

bool SettingsText::getValue(const std::string &key, std::string &value)
{
    if (settings.find(key) == settings.end()) {
        return false;
    }

    value = settings[key];

    return true;
}

bool SettingsText::load(const std::string &file_name)
{
    std::ifstream file(file_name);

    if (!file.is_open()) {
        return false;
    }

    settings.clear();
}

bool SettingsText::save(const std::string &file_name)
{
    std::ofstream file(file_name);

    if (!file.is_open()) {
        return false;
    }

    for (const std::pair<std::string, std::string> &pair: settings) {
        std::cout << pair.first << " " << pair.second << std::endl;
    }
}

void SettingsText::set(std::stringstream &sstream)
{
    sstream << std::boolalpha << std::setprecision(6) << std::fixed;
}

#endif // SETTINGSTEXT_H
