#include "dictionary.h"


using namespace std;
const char * DICT_PATH = "../dict/dict.txt.big";
const char * HMM_PATH = "../dict/hmm_model.utf8";
const char * USER_DICT_PATH = "../dict/user.dict.utf8";
const char * IDF_PATH = "../dict/idf.utf8";
const char * STOP_WORD_PATH = "../dict/stop_words.utf8";


vector<string> SplitChinese(string& chinese_text) {
	//Credits to deviantfan
	//https://stackoverflow.com/questions/40054732/c-iterate-utf-8-string-with-mixed-length-of-characters/40054802#40054802
	vector<string> chinese_chars;
	for(int i = 0; i < chinese_text.length();) {
		int cplen = 1;
		if((chinese_text[i] & 0xf8) == 0xf0) cplen = 4;
		else if((chinese_text[i] & 0xf0) == 0xe0) cplen = 3;
		else if((chinese_text[i] & 0xe0) == 0xc0) cplen = 2;
		if((i + cplen) > chinese_text.length()) cplen = 1;
		chinese_chars.push_back(chinese_text.substr(i, cplen));
		i += cplen;
	}
	return chinese_chars;
}

Dictionary::Dictionary() {
}

Dictionary::Dictionary(const std::string& variety, bool traditional) {
	BuildDictionary(variety, traditional);
}

vector<vector<string>> Dictionary::TranslateToEnglish(std::string& chinese_text) {
	cppjieba::Jieba jieba(
		DICT_PATH,
		HMM_PATH,
		USER_DICT_PATH,
		IDF_PATH,
		STOP_WORD_PATH);
	vector<string> words;
	vector<cppjieba::Word> jiebawords;

	jieba.Cut(chinese_text, words, true);
	vector<vector<string>> translation_key;
	for(string word : words) {
		vector<string> word_entry = dict[word];
		vector<string> trans_entry;
		trans_entry.push_back(word);
		for(int i = 0; i < word_entry.size(); i++) {
			trans_entry.push_back(word_entry[i]);
		}
		if( trans_entry.size() <= 1 || trans_entry[1] == "") {
			vector<string> chinese_chars = SplitChinese(word);
			for(auto chinese_char : chinese_chars) {
				trans_entry.clear();
				word_entry = dict[chinese_char];
				trans_entry.push_back(chinese_char);
				for(int i = 0; i < word_entry.size(); i++) {
					trans_entry.push_back(word_entry[i]);
				}
				translation_key.push_back(trans_entry);
			}   
		} else { 
			translation_key.push_back(trans_entry);
		}
	}

	return translation_key;
}

unordered_map<string, string> Dictionary::BuildPronunciations(string& pronunciation_filename, string& pronunciation_bounds, bool traditional) {
	unordered_map<string, string> pronunciations;
	ifstream pronunciation_file(pronunciation_filename);
	if( pronunciation_file.is_open()) {
		string line;
		while( getline(pronunciation_file, line)) {
			if( line.find("#") != 0) {
				string chinese;
				if(traditional) {
					chinese = line.substr(0, line.find(" "));
				} else {
					chinese = line.substr(line.find(" ") + 1, line.find("[") - line.find(" ") - 2);
				}
				string pronunciation = line.substr(line.find(pronunciation_bounds[0]) + 1, line.find(pronunciation_bounds[1]) - line.find(pronunciation_bounds[0]) - 1);
				pronunciations[chinese] = pronunciation;
			}
		}
	}
	pronunciation_file.close();
	return pronunciations;
}

void Dictionary::BuildDictionaryPart(string& dict_filename, string &pronunciation_filename, string& pronunciation_bounds, bool traditional) {
	unordered_map<string, string> pronunciations;
	bool separate_files = dict_filename != pronunciation_filename;
	
	if(separate_files) {
		pronunciations = BuildPronunciations(pronunciation_filename, pronunciation_bounds, traditional);
	}

	ifstream dict_file(dict_filename);
	if( dict_file.is_open()) {
		string line;
		while ( getline (dict_file, line) ) {
			if(line.find("#") != 0){ 
				string chinese;
				if(traditional) {
					chinese = line.substr(0, line.find(" "));
				} else {
					chinese = line.substr(line.find(" ") + 1, line.find("[") - line.find(" ") - 2);
				}
				if(dict.count(chinese) == 0) {
					vector<string> dict_entry;
					string pronunciation;
					if(separate_files) {
						pronunciation = pronunciations[chinese];
					} else {
						pronunciation = line.substr(line.find(pronunciation_bounds[0]) + 1, line.find(pronunciation_bounds[1]) - line.find(pronunciation_bounds[0]) - 1);
					}
					dict_entry.push_back(pronunciation);

					stringstream english(line.substr(line.find("/") + 1, line.size() - line.find("/") - 2));
					string segment;
					while(std::getline(english, segment, '/')) {
						dict_entry.push_back(segment);
					}
					dict[chinese] = dict_entry;
				} else {
					stringstream english(line.substr(line.find("/") + 1, line.size() - line.find("/") - 2));
					string segment;
					while(std::getline(english, segment, '/')) {
						dict[chinese].push_back(segment);
					}
				}
				
			}
		}
		dict_file.close();
	} 
}

void Dictionary::BuildDictionary(const string& variety, bool traditional) {
	try{
		string dict_filename;
		string pronunciation_filename;
		string pronunciation_bounds;
		if(variety == "cantonese") {
			dict_filename = "../bin/data/cccanto-webdist.txt";
			pronunciation_filename = dict_filename;
			pronunciation_bounds = "{}";
			BuildDictionaryPart(dict_filename, pronunciation_filename, pronunciation_bounds, traditional);

			dict_filename = "../bin/data/cedict_ts.u8";
			pronunciation_filename = "../bin/data/cccanto-readings.txt";
			BuildDictionaryPart(dict_filename, pronunciation_filename, pronunciation_bounds, traditional);
		} else if(variety == "mandarin") {
			dict_filename = "../bin/data/cedict_ts.u8";
			pronunciation_filename = dict_filename;
			pronunciation_bounds = "[]";
			BuildDictionaryPart(dict_filename, pronunciation_filename, pronunciation_bounds, traditional);

		} else if(variety == "korean") {
			dict_filename = "../bin/data/cedict_ts.u8";
			pronunciation_filename = "../bin/data/hanja_pronunciations.txt";
			pronunciation_bounds = "()";
			traditional = true;
			BuildDictionaryPart(dict_filename, pronunciation_filename, pronunciation_bounds, traditional);
		} else {
			throw;
		}
	} catch(...) {
		cout << "Variety not supported." << endl;
	}
}

unordered_map<string, vector<string>> Dictionary::get_dict() {
	return dict;
}

