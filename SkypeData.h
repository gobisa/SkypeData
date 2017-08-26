#ifndef SKYPEDATA_H
#define SKYPEDATA_H

#include "csvstream.h"
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <cstdlib>
#include <vector>


using std::vector;
using std::map;
using std::string;
using std::set;
using std::regex;
using std::stringstream;


//each of these objects stores data by person
//each person has their own object of this class
class SkypeUser {
public:
	SkypeUser(const string& name_in);
	void addRow(csvstream::row_type row);
	//fills in raw_xml_messages, parsed_messages, num_posts, num_edits
	void sortData();
	//fills in vocabulary_count, emoji_count, word_count, vocab_size, link_count, skype_emoji_count
	void analyzeData(const set<string>& neg_words, const set<string>& pos_words, const set<string>& bad_words);  
	void outputData(std::ofstream& output_file);
	string XMLToStringConverter(const string& xml);
	string XMLSpecialCharToString(const string& xml_word);
	string personalTextFormatter(const string& unformatted_string);
	string removePunctuation(const string& text);
	vector<std::pair<string, int>> getTop3FromMap(const map<string, int>& m);
	string getName() { return name; }

private:

	//filled using sortData()
	string name; 
	vector<csvstream::row_type> rows; 
	vector<string> raw_xml_messages; 
	vector<string> parsed_messages;  
	int num_posts; 
	int num_edits; 

	//filled using analyzeData()
	map<string, int> vocabulary_count; //unique word frequencies 
	map<string, int> emoji_count; //unique emoji frequencies 
	int word_count; //total words, non unique 
	int vocab_size; //number of unique words
	int punctuation_count; 
	int link_count; 
	int skype_emoji_count; 
	int negative_words_count; 
	int positive_words_count; 
	int bad_words_count; //ie swear words 
	int negative_message_count; 
	int positive_message_count; 
	int bad_word_message_count; 
	
};




#endif