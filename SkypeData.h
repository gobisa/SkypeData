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

class SkypeData {
public:
	SkypeData(csvstream& csv_data);
private:
	friend class SkypeUser;

	//void update_users_num_posts();
	//void update_users_num_edits();
	void update_users_vocabulary_count();

	//Modifies:	user_data
	//Effects:	deletes every pointed-to objectin user_data
	~SkypeData();

	int posts_count;
	vector<SkypeUser*> user_data; //stores data for each user
	vector<csvstream::row_type> rows; //stores each row of the csv in a vector
	set<string> user_names;
};

//each of these objects stores data by person
//each person has their own object of this class
class SkypeUser {
public:
	SkypeUser(const string& name_in);
	void addRow(csvstream::row_type row);
	void sortData(); //fills in raw_xml_messages, parsed_messages, num_posts, num_edits
	void analyzeData();  //does vocab count
	string XMLToStringConverter(const string& xml);
	string XMLSpecialCharToString(const string& xml_word);
	string personalTextFormatter(const string& unformatted_string);
	string removePunctuation(const string& text);

private:

	string name; //
	vector<csvstream::row_type> rows; //
	vector<string> raw_xml_messages; //
	vector<string> parsed_messages;  //
	int num_posts; //
	int num_edits; //

	map<string, int> vocabulary_count; //unique word frequencies //use plain text
	map<string, int> emoji_count; //unique emoji frequencies //should use xml
	int word_count; //total words, non unique 
	int vocab_size; //number of unique words, = vocabulary_count.size()
	int bad_words_count;
	int punctuation_count;
	int link_count;
	int skype_emoji_count;
	int negative_words_count;
	int positive_words_count;
};




#endif