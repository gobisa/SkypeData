#include "SkypeData.h"
#include "csvstream.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <regex>
//#include <iterator>
//#include <regex.h>
#include <sstream>

using std::regex;


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
/*
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
			user_data.push_back(new SkypeUser(row["author"]));
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


//FIXME
//CURRENTLY ONLY UPDATES num_posts and num_edits
//PROBABLY WANT IT TO UPDATE VOCAB AND VOCAB COUNTS
void SkypeData::update_users_vocabulary_count() {

	for (auto user : user_data) {
		//update num_posts
		user->num_posts = (user->rows).size();

		//go through every row
		for (auto row : user->rows) {
			//update num_edits
			if (row["edited_by"] != "") { //FIXME, MAY NEED TO CHECK IF == user->name?
				//FIXME, may not work correctly
				++(user->num_edits);
			}


			//FIXME
			//UPDATE VOCAB
		}
	}

}



SkypeData::~SkypeData() {
	for (auto ptr_datum : user_data) {
		delete ptr_datum;
	}
}
*/
////////////////////////////////////////
////////SkypeData///////////////////////
////////////////////////////////////////




////////////////////////////////////////
////////UserData////////////////////////
////////////////////////////////////////
SkypeUser::SkypeUser(const string& name_in)
	: name(name_in), num_posts(0), num_edits(0), word_count(0), vocab_size(0),
	bad_words_count(0), punctuation_count(0), skype_emoji_count(0) {}

void SkypeUser::addRow(csvstream::row_type row) {
	rows.push_back(row);
}

void SkypeUser::sortData() {
	/*
	vector<string> raw_xml_messages;
	vector<string> parsed_messages;
	int num_posts; // = rows.size();
	int num_edits;

	map<string, int> vocabulary_count; //unique word frequencies //use plain text
	map<string, int> emoji_count; //unique emoji frequencies //should use xml
	int word_count; //total words, non unique //use multiplication in vocabulary_count
	int vocab_size; //number of unique words, = vocabulary_count.size()
	int bad_words_count;
	int punctuation_count;
	int link_count;
	int skype_emoji_count;
	*/

	for (auto row : rows) {
		//std::cout << "raw xml: " << row["replace(body_xml, CHAR(10), '')"] << std::endl;
		raw_xml_messages.push_back(row["replace(body_xml, CHAR(10), '')"]);
		string parsed_text = XMLToStringConverter(row["replace(body_xml, CHAR(10), '')"]);
		//std::cout << "parsed_text: " << parsed_text << std::endl;
		parsed_messages.push_back(parsed_text);
		
		if (row["edited_by"] == name) {
			++num_edits;
		}
	}
	
	num_posts = rows.size();
	
}

void SkypeUser::analyzeData() {

	string word;

	//vocabulary_count, word_count
	for (const auto& msg : parsed_messages) {
		std::cout << msg << std::endl;
		//vocabulary_count, word_count
		stringstream message(msg);
		while (message >> word) {
			//FIXME, need to fix to make sure punctuation is not included when adding to vocab, also need to remove XMLspecialChars
			//parse xmlspecialchartostring, then remove punctuation 
			//must be in this order so that xml isn't removed as punct
			word = XMLSpecialCharToString(word);
			word = removePunctuation(word);
			if (vocabulary_count.find(word) == vocabulary_count.end()) { //word not found
				vocabulary_count[word] = 0;
			}
			++vocabulary_count[word];
			++word_count;
		}
	}

	//vocab_size
	vocab_size = vocabulary_count.size();


	//emoji_count, skype_emoji_count, link_count
	for (const auto& msg : raw_xml_messages) {

		//http://en.cppreference.com/w/cpp/regex/regex_iterator
		//https://stackoverflow.com/questions/12908534/retrieving-a-regex-search-in-c

		//has three groups: the opening tag, the emoji in plain text, and the closing tag
		regex emoji_format("(<ss type=[a-z]+>)(\\S+(?=</ss>))(</ss>)");

		//iterates through the different matches of emojis
		auto first_emoji_match = std::sregex_iterator(msg.begin(), msg.end(), emoji_format);
		auto last_emoji_match = std::sregex_iterator(); //end iterator

		//distance finds number of matches (ie, if 1 emoji found, then it would return the distance between 0(the emoji) and 1(end iterator)
		skype_emoji_count += std::distance(first_emoji_match, last_emoji_match);

		//loops through each match, and adds only the emoji group of the match to the emoji_count map
		for (std::sregex_iterator i = first_emoji_match; i != last_emoji_match; ++i) {
			std::smatch emoji_match = *i;
			if (emoji_count.find(emoji_match[2]) == emoji_count.end()) {
				emoji_count[emoji_match[2]] = 1;
			}
			else ++emoji_count[emoji_match[2]];
		}


		//link_count
		regex link_format("<a href=");

		auto first_link_match = std::sregex_iterator(msg.begin(), msg.end(), link_format);
		auto last_link_match = std::sregex_iterator();

		link_count += std::distance(first_link_match, last_link_match);

	}



	
	



}




/*
Skype xml formatting:
-messages with xml formatting all begin with double quotes
-if the message does not begin with double quotes,
the message is in plain text NOT TRUE
-xml special characters: &lt; (<), &amp; (&), &gt; (>), &quot; ("), and &apos; (').

FOR THIS FUNCTION, ONLY REMOVE THE TAGS
SPECIAL CHARACTERS CAN BE DEALT WITH ONCE THE VOCAB TREES ARE ESTABLISHED
-the xml string will begin with quotes if it has tags
-if the string has quotes and there are no tags, then the quotes are from the user, and should be removed
-xml tags: <ss, <partlist type, <a href, <URIObject type, <quote author
-note: partlist type is a tag that contains nonsense data, so just ignore these messages
-note: <URIObject type contains an href type, so search for URIObject first
-URIObject is a file
-note: for ss tag, record that user used an emoji and record what kind
-note: format for text with a link
andrew.lukasiewicz,,"i came out of the womb singing this <a href=""https://www.youtube.com/watch?v=l7KEuKKuuas"">https://www.youtube.com/watch?v=l7KEuKKuuas</a>"
*/
//THIS FUNCTION IS ONLY FOR CONVERTING THE STRING TO TEXT, OTHER ANALYSIS WILL BE DONE BY OTHER FUNCTIONS
//THIS FUNCTION SHOULD PROBABLY ONLY BE USED FOR COMPUTING VOCABULARY AND WORDCOUNT
//FIXME, NEW TAG: ajmatvekas,ajmatvekas,"My <b raw_pre=""*"" raw_post=""*"">intro</b> to excel teacher sucks so badly....he goes I don&apos;t even use excel after messing up showing us something lol"
/*
All tags found using python script:
['</i>', '<i r', '<b>W', '</s>', '<s r', '<br/', '<Des', '</De', '<b r', '</Ti', '<Tit', '</b>', '<Fil', '<quo', '</qu',
'<met', '<Ori', '</UR', '<URI', '</le', '<leg', '<ss ', '</ss', '<dur', '</du', '</na', '<nam', '</pa', '<par', '</a>', '<a h']
Tags account for: </i>, <i r>,
*/
string SkypeUser::XMLToStringConverter(const string& xml) {

	//FIXME, when message has a comma, it starts with quotes in the csv

	//std::cout << "xml: " << xml << std::endl;

	//no xml tags
	//FIXME, MAY NEED TO UNREMOVE
	//if (xml[0] != '"') return xml;

	//partlist type
	if (xml.find("<partlist type=") != xml.npos) return ""; //GOOD

	//<URIObject type
	if (xml.find("<URIObject type=") != xml.npos) return ""; //GOOD


	string plain_text_string = xml; //use this string to replace things

	//std::cout << "plain_text_string declared" << std::endl;

	/*
	//http://www.cplusplus.com/reference/regex/
	*/

	//CORRECT REGEX
	regex ss_opening_tag("<ss type=\\w*>");
	regex ss_closing_tag("</ss>");
	//regex ss_format("(<ss type=)\\S+(</ss>)"); //unused
	plain_text_string = regex_replace(plain_text_string, ss_opening_tag, "");
	plain_text_string = regex_replace(plain_text_string, ss_closing_tag, "");



	////CORRECT REGEX
	regex href_opening_tag("(<a href=)\\S+(\"\">)"); //matches opening href tag, from "<a href=" to '"">'
	regex href_closing_tag("</a>"); //matches closing href tag, "</a>"
	regex href_format("(<a href=).*(</a>)"); //matches all text between (inclusive) "<a href=" and "/a>"
	//GOOD									   //(<a href)\S+(/a>) is the code on regex101, but I think two \ are needed for the S because

											   //FIXME, for now, we'll just ignore links when converting to text
	plain_text_string = regex_replace(plain_text_string, href_format, "");
	//remove opening and closing tags
	//plain_text_string = regex_replace(plain_text_string, href_opening_tag, "");
	//plain_text_string = regex_replace(plain_text_string, href_closing_tag, "");


	//CORRECT REGEX
	//when user adds the bold, takes form: <b raw_pre=* raw_post=*></b>
	//when the skype bot adds bold, takes form: <b><  / b>
	//regex b_user_opening_tag("(<b raw_pre).*(post = \\*>)");
	regex b_user_opening_tag("(<b raw_pre).*(post=\\*>)");
	regex b_bot_opening_tag("<b>");
	regex b_bot_closing_tag("</b>");
	regex b_user_closing_tag("< / b>");
	plain_text_string = regex_replace(plain_text_string, b_user_opening_tag, "");
	plain_text_string = regex_replace(plain_text_string, b_bot_opening_tag, "");
	plain_text_string = regex_replace(plain_text_string, b_bot_closing_tag, "");
	plain_text_string = regex_replace(plain_text_string, b_user_closing_tag, "");


	//CORRECT REGEX
	//should always take form: <i raw_pre=_ raw_post=_>
	//not encountered, but skype bot may use <i></i>
	regex i_opening_tag("(<i raw_pre).*(post=_>)");
	regex i_bot_opening_tag("<i>");
	regex i_closing_tag("</i>");
	plain_text_string = regex_replace(plain_text_string, i_opening_tag, "");
	plain_text_string = regex_replace(plain_text_string, i_bot_opening_tag, "");
	plain_text_string = regex_replace(plain_text_string, i_closing_tag, "");

	/*
	All tags found using python script :
	['</i>', '<i r', '<b>W', '</s>', '<s r', '<br/', '<Des', '</De', '<b r', '</Ti', '<Tit',
	'</b>', '<Fil', '<quo', '</qu',
	'<met', '<Ori', '</UR', '<URI', '</le', '<leg', '<ss ', '</ss', '<dur', '</du',
	'</na', '<nam', '</pa', '<par', '</a>', '<a h']
	Tags account for :	</i>, <i r, <b>, </s>, <s r>, <br/, <Des, </De, <b r, </Ti, <Tit, </b>
	<Fil, <quo, </qu, </le, <leg, <met, <Ori, '</UR', '<URI', '</le', '<leg',
	'<ss ', '</ss', '<dur', '</du','</na', '<nam', '</pa', '<par', '</a>', '<a h'
	Tags ignored:	<Des and </De are description tags nested within the <URIObject tags
	</Ti and <Tit are title tags nested within the <URIObject tags
	<Fil is a single tag for filesize nested within the URIObject tags
	<met is a meta tag for object type within the URIObject tags
	<Ori is the OriginalName tag for object name within the URIObject tags
	</UR and <URI are the URIObject tags
	<dur and </du are the duration tags within the partlist tags
	</na and <nam are the name tags within the partlist tags
	</pa and <par are the start of tags for both <part> and <partlist>, and <part> tags are within partlist tags
	*/

	//GOOD REGEX
	//regex s_opening_tag("(<s raw_pre).*(= ~>)");
	regex s_opening_tag("(<s raw_pre).*(=~>)");
	regex s_bot_opening_tag("<s>");
	regex s_closing_tag("</s>");
	plain_text_string = regex_replace(plain_text_string, s_opening_tag, "");
	plain_text_string = regex_replace(plain_text_string, s_bot_opening_tag, "");
	plain_text_string = regex_replace(plain_text_string, s_closing_tag, "");


	//GOOD REGEX
	//br doesn't have opening and closing tags, it only acts as a line break
	regex br_tag("<br/>");
	plain_text_string = regex_replace(plain_text_string, br_tag, " ");


	//quo is a quote tag, when you copy and paste someone else
	/*"<quote author=""ajmatvekas"" authorname=""Audrius Matvekas""
	conversation=""19:6231efe89fc74b3884799e8c68c75912@thread.skype""
	guid=""x25fbbca08aefaa0253f83b3c7289fac113955099789439e0294209cd68ac8f1e""
	timestamp=""1486522229""><legacyquote>[9:50:29 PM]
	Audrius Matvekas: </legacyquote>Audrius Matvekas has been promoted to conversation host.<legacyquote>
	*/
	/*
	ajmatvekas,,"<quote author=""andrius.gobis"" authorname=""Andrius Gobis"" conversation=""19:6231efe89fc74b3884799e8c68c75912@thread.skype"" guid=""x50b30ee4
	e6b8d6e7038c989abecb44260fcf24b5ca7d84e8f9d80255d0ca1081"" timestamp=""1486322839""><legacyquote>[2:27:19 PM] Andrius Gobis: </legacyquote>it takes painfully
	long and you hate each other more than catan when you wait for people to finish their turn<legacyquote>
	&lt;&lt;&lt; </legacyquote></quote>matai this is literally the reason you said you wouldn&apos;t play multiplayer with me"
	*/

	//GOOD REGEX
	//NOTE: DOES NOT FULLY DEAL WITH THE TEXT WITHIN THE QUOTE TAGS
	//FIXME, NOT SURE IF THIS IS RIGHT
	//FIXME, I AM COUNTING QUOTES TOWARD THE QUOTER'S VOCABULARY
	regex quote_opening_tag("<quote(.*)</legacyquote>"); //matches anything from <quote to >
	/*
	matches:
	<quote author=""andrius.gobis"" authorname=""Andrius Gobis"" conversation=""19:6231efe89fc74b3884799e8c68c75912@thread.skype"" guid=""x50b30ee4
	e6b8d6e7038c989abecb44260fcf24b5ca7d84e8f9d80255d0ca1081"" timestamp=""1486322839"">
	*/
	plain_text_string = regex_replace(plain_text_string, quote_opening_tag, "");
	//deal with /qu
	//regex quote_closing_tag("<legacyquote> &lt; &lt; &lt; < / legacyquote>< / quote>");
	regex quote_closing_tag("<legacyquote>\\s\\s&lt;&lt;&lt; </legacyquote></quote>");
	plain_text_string = regex_replace(plain_text_string, quote_closing_tag, " ");

	//deal with legacyquote
	/*
	<legacyquote>[1:06:02 AM] Erikas Anužis: </legacyquote>glob, duki and I are planning a
	trip to arizona<legacyquote>&lt;&lt;&lt; </legacyquote></quote>we are?"
	*/
	regex legacyquote_tags("(<legacyquote>).*(</legacyquote>)");
	plain_text_string = regex_replace(plain_text_string, legacyquote_tags, "");


	//FIXME, NOT SURE IF THIS SHOULD BE DONE HERE
	//string is in quotes, but has no tags
	/*
	stringstream ss_plain_text_string(plain_text_string);
	string word;

	while (ss_plain_text_string >> word) {

	}
	*/

	return plain_text_string;
}



//FIXME, WRITE TEST CASES, this doesnt work
/*
REQUIRES:	xml_word is a single word, with no spaces
MODIFIES:	nothing
EFFECTS:	returns a string based on xml_word,
with all xml special characters replaced by their plain text characters
*/
string SkypeUser::XMLSpecialCharToString(const string& xml_word) {
	vector<string> xml_special_chars = { "&lt;", "&amp;", "&gt;", "&quot;", "&apos;" };

	string parsed_word = xml_word;

	//check word for all special characters
	for (const string& special_char : xml_special_chars) {
		size_t pos = parsed_word.find(special_char);
		//loop until all occurrences are found
		while (pos != parsed_word.npos) {
			if (special_char == "&lt;") {
				parsed_word.replace(pos, 4, "<");
			}
			else if (special_char == "&amp;") {
				parsed_word.replace(pos, 5, "&");
			}
			else if (special_char == "&gt;") {
				parsed_word.replace(pos, 4, ">");
			}
			else if (special_char == "&quot;") {
				parsed_word.replace(pos, 6, "\"");
			}
			else if (special_char == "&apos;") {
				parsed_word.replace(pos, 6, "'");
			}
			pos = parsed_word.find(special_char, pos); //try to find next instance in the word
		}
	}


	return parsed_word;
}
//remove this in vocab count

/*
-Removes quotes that the user adds
-if there is a /, treat the string as two words
*/
string SkypeUser::personalTextFormatter(const string& unformatted_string) {
	regex quote("\"");
	string removed_quotes_string = unformatted_string;
	return regex_replace(removed_quotes_string, quote, "");
}


//removes any prefix or postfix punctuation
//works correctly
string SkypeUser::removePunctuation(const string& word) {
	regex punct("^[[:punct:]]+|[[:punct:]]+$");
	return regex_replace(word, punct, "");
}