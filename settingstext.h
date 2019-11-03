#ifndef SETTINGSTEXT_H
#define SETTINGSTEXT_H

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <unordered_map>
#include <algorithm>

#include <iostream>

class SettingsText
{
public:
    SettingsText();

    template<typename T> bool setValue(const std::string &key, const T &value);
    bool setValue(const std::string &key, const std::string &value);
    bool setValue(const std::string &key, const char *value);
    template<typename T, typename F> bool setValue(const std::string &key, const T &value, F &&convert);

    template<typename T> bool getValue(const std::string &key, T &value);
    bool getValue(const std::string &key, std::string &value);
    template<typename T, typename F> bool getValue(const std::string &key, T &value, F &&convert);

    std::string value(const std::string &key);
    std::string operator[](const std::string &key);

    void setDescription(const std::string &description);

    bool load(const std::string &file_name);
    bool save(const std::string &file_name);

private:
    std::unordered_map<std::string, std::string> settings;
    const std::string delimiter = " ";
    const std::string comment = "#";
    const std::string whitespace = " \n\r\t\f\v";

    std::string description;

    void set(std::stringstream &sstream);
    bool isComment(const std::string &line);
    std::string trimLeft(const std::string& s);
    std::string trimRight(const std::string& s);
    std::string trim(const std::string& s);
};

SettingsText::SettingsText()
{
}

template<typename T>
bool SettingsText::setValue(const std::string &key, const T &value)
{
    std::stringstream sstream;
    set(sstream);
    if (sstream << value) {
        settings[key] = sstream.str();
        return true;
    }

    return false;
}

bool SettingsText::setValue(const std::string &key, const std::string &value)
{
    settings[key] = value;
    return true;
}

bool SettingsText::setValue(const std::string &key, const char *value)
{
    settings[key] = value;
    return true;
}

template<typename T, typename F>
bool SettingsText::setValue(const std::string &key, const T &value, F &&convert)
{
    std::string result;
    if (convert(value, result)) {
        settings[key] = result;
        return true;
    }

    return false;
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

std::string SettingsText::value(const std::string &key)
{
    return settings[key];
}

std::string SettingsText::operator[](const std::string &key) {
    return value(key);
}

template<typename T, typename F>
bool SettingsText::getValue(const std::string &key, T &value, F &&convert)
{
    if (settings.find(key) == settings.end()) {
        return false;
    }

    if (convert(settings[key], value)) {
        return true;
    }

    return false;
}

void SettingsText::setDescription(const std::string &descr)
{
    description = "# " + descr;
    std::size_t index = 0;
    while (true) {
        index = description.find("\n", index);
        if (index == std::string::npos) {
            break;
        }

        description.replace(index, 1, "\n" + comment + " ");

        /* Advance index forward so the next iteration doesn't pick it up as well. */
        index += (2 + comment.size());
    }

    description += "\n\n";
}

bool SettingsText::load(const std::string &file_name)
{
    std::ifstream file(file_name);

    if (!file.is_open()) {
        return false;
    }

    settings.clear();

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);

        if (line.empty() || isComment(line)) {
            continue;
        }

        std::size_t position_delimiter = line.find(delimiter);
        if (position_delimiter == std::string::npos) {
            settings.clear();
            return false;
        }

        std::string key = trimRight(line.substr(0, position_delimiter));
        std::string value = trimLeft(line.substr(position_delimiter + delimiter.size()));

        settings[key] = value;
    }

    return true;
}

bool SettingsText::save(const std::string &file_name)
{
    std::ofstream file(file_name);

    if (!file.is_open()) {
        return false;
    }

    file << description;

    std::map<std::string, std::string> ordered(settings.begin(), settings.end());
    for (const auto &pair: ordered) {
        file << pair.first << delimiter << pair.second << std::endl;
    }

    return true;
}

void SettingsText::set(std::stringstream &sstream)
{
    sstream << std::boolalpha << std::setprecision(6) << std::fixed;
}

bool SettingsText::isComment(const std::string &line)
{
    if (line.substr(0, comment.size()) == comment) {
        return true;
    }

    return false;
}

std::string SettingsText::trimLeft(const std::string &s)
{
    size_t start = s.find_first_not_of(whitespace);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string SettingsText::trimRight(const std::string &s)
{
    size_t end = s.find_last_not_of(whitespace);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string SettingsText::trim(const std::string &s)
{
    return trimRight(trimLeft(s));
}

#endif // SETTINGSTEXT_H
