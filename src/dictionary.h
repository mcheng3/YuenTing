#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <unordered_map>
#include <codecvt>
#include <locale>
#include <iostream>
#include <string>
#include <vector>
#include "../libs/cppjieba/include/cppjieba/Jieba.hpp"


extern const char* DICT_PATH;
extern const char* HMM_PATH;
extern const char* USER_DICT_PATH;
extern const char* IDF_PATH;
extern const char* STOP_WORD_PATH;

std::vector<std::string> SplitChinese(std::string& chinese_text);

class Dictionary {
private:
	std::unordered_map<std::string, std::vector<std::string>> dict;
	void BuildDictionary(const std::string& variety, bool traditional);
	void BuildDictionaryPart(std::string& dict_filename, std::string& pronunciation_filename, std::string& pronunciation_bounds, bool traditional);
	std::unordered_map<std::string, std::string> BuildPronunciations(std::string& pronunciation_filename, std::string& pronunciation_bounds, bool traditional);
public:
	Dictionary();
	Dictionary(const std::string& dict_filename, bool traditional);
	std::vector<std::vector<std::string>> TranslateToEnglish(std::string& chinese_text);
	std::unordered_map<std::string, std::vector<std::string>> get_dict();
};

#endif //DICTIONARY_H