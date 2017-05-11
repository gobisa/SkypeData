#include "SkypeData.h"
#include "csvstream.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <set>



////////////////////////////////////////
////////SkypeData///////////////////////
////////////////////////////////////////

/*
REQUIRES: valid csv_data
MODIFIES: 
	SkypeData: posts_count, user_data, rows, user_names
	UserSkypeData: name, rows
EFFECTS:
	SkypeData: updates posts_count to number of posts in the csv_data, creates a 
		new UserSkypeData object for each unique author in csv_data, stores each unique
		author in user_names, and stores all the rows of the csv_data in rows
	UserSkypeData: for each user in SkypeData.user_data, sets the name, rows, 
		num_posts, num_edits
*/
SkypeData::SkypeData(csvstream& csv_data) {
	csvstream::row_type row;

	//bool new_name;
	int user_data_index = -1;

	//read in all rows from csv_data, and temporarily store data in row
	while (csv_data >> row) {
		//returns pair, where second element is true if 
		//element is a new element and is inserted
		if ((user_names.insert(row["author"])).second) {
			//create new user, sets name
			user_data.push_back(new UserSkypeData(row["author"]));
			++user_data_index;
		}
		//add rows of csv_data to this.rows
		rows.push_back(row);
		//add rows of csv_data to correct UserSkypeData object
		user_data[user_data_index]->rows.push_back(row);
	}
	//set posts_count
	posts_count = rows.size();
	



	//update/initialize all users

	//num_posts, num_edits, vocabulary_count, word_count, and vocab_size
	for (auto user : user_data) {
		//update num_posts
		user->num_posts = (user->rows).size();

		//go through every row
		for (auto row : user->rows) {
			//update num_edits
			if (row["edited_by"] != "") {
				//FIXME, may not work correctly
				++(user->num_edits);
			}
		}
	}

}



void SkypeData::update_users_vocabulary_count() {

	for (auto user : user_data) {
		//update num_posts
		user->num_posts = (user->rows).size();

		//go through every row
		for (auto row : user->rows) {
			//update num_edits
			if (row["edited_by"] != "") {
				//FIXME, may not work correctly
				++(user->num_edits);
			}
		}
	}

}



SkypeData::~SkypeData() {
	for (auto ptr_datum : user_data) {
		delete ptr_datum;
	}
}
////////////////////////////////////////
////////SkypeData///////////////////////
////////////////////////////////////////




////////////////////////////////////////
////////UserData////////////////////////
////////////////////////////////////////
UserSkypeData::UserSkypeData(const string& name_in)
	: name(name_in), num_posts(0), num_edits(0), word_count(0), vocab_size(0),
	bad_words_count(0), punctuation_count(0), skype_emoji_count(0) {}