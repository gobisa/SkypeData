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

class SkypeData {
public:
	SkypeData(csvstream& csv_data);
private:
	friend class UserSkypeData;

	//void update_users_num_posts();
	//void update_users_num_edits();
	void update_users_vocabulary_count();

	//Modifies:	user_data
	//Effects:	deletes every pointed-to objectin user_data
	~SkypeData();

	int posts_count;
	vector<UserSkypeData*> user_data; //stores data for each user
	vector<csvstream::row_type> rows; //stores each row of the csv in a vector
	set<string> user_names;
};

//each of these objects stores data by person
//each person has their own object of this class
class UserSkypeData {
public:
	UserSkypeData(const string& name_in);

private:
	friend class SkypeData;

	//FIXME: DELETE
	//variables with "//" are done
	string name; //
	vector<csvstream::row_type> rows;
	int num_posts; // = rows.size();
	int num_edits;

	map<string, int> vocabulary_count; //unique word frequencies
	int word_count; //total words, non unique
	int vocab_size; //number of unique words, = vocabulary_count.size()
	int bad_words_count;
	int punctuation_count;
	int link_count;
	int skype_emoji_count;
};