#ifndef SETTINGSTEXT_H
#define SETTINGSTEXT_H

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
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
    template<typename T, typename F> bool setValue(const std::string &key, const T &value, F &&convert);

    template<typename T> bool getValue(const std::string &key, T &value);
    bool getValue(const std::string &key, std::string &value);
    template<typename T, typename F> bool getValue(const std::string &key, T &value, F &&convert);

    std::string value(const std::string &key);
    std::string operator[](const std::string &key);

    void setDescription(const std::string &description);
    void setCategory(const std::string &category = "");

    bool load(const std::string &file_name);
    bool save(const std::string &file_name);
    void clear();

private:
    struct Value {
        std::string value;
        std::string category;
        std::string description;
    };
    /// Main object containing all settings - key, values
    std::unordered_map<std::string, Value> settings;
    /// Current category name
    std::string current_category;

    /// Settings file description
    std::string description;

    /// Settings file properties
    const std::string delimiter = " ";
    const std::string comment = "#";
    const std::string whitespace = " \n\r\t\f\v";

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
        return setValue(key, sstream.str());
    }

    return false;
}

bool SettingsText::setValue(const std::string &key, const std::string &value)
{
    if (settings.find(key) == settings.end()) {
        settings[key].value = value,
        settings[key].category = current_category;
        return true;
    }

    settings[key].value = value;
    return true;
}

template<typename T, typename F>
bool SettingsText::setValue(const std::string &key, const T &value, F &&convert)
{
    std::string result;
    if (convert(value, result)) {
        return setValue(key, result);
    }

    return false;
}

template<typename T>
bool SettingsText::getValue(const std::string &key, T &value)
{
    if (settings.find(key) == settings.end()) {
        return false;
    }

    std::stringstream sstream(settings[key].value);
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

    value = settings[key].value;

    return true;
}

std::string SettingsText::value(const std::string &key)
{
    return settings[key].value;
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

    if (convert(settings[key].value, value)) {
        return true;
    }

    return false;
}

void SettingsText::setDescription(const std::string &descr)
{
    description = comment + " " + descr;
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

    description += "\n";
}

void SettingsText::setCategory(const std::string &category)
{
    current_category = category;
}

bool SettingsText::load(const std::string &file_name)
{
    std::ifstream file(file_name);

    if (!file.is_open()) {
        return false;
    }

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

        settings[key].value = value;
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

    // Category, key
    std::map<std::string, std::set<std::string>> ordered;
    for (const auto &pair: settings) {
        ordered[pair.second.category].emplace(pair.first);
    }

    for (const auto &category: ordered) {
        file << std::endl;
        if (category.first.size()) {
            file << comment << comment << comment << comment << " " << category.first << std::endl;
        }
        for (const std::string &key: category.second) {
            file << key << delimiter << settings[key].value << std::endl;
        }
    }

    return true;
}

void SettingsText::clear()
{
    settings.clear();
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
