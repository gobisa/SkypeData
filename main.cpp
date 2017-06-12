/*
Analyzes a csv file of skype data found in main.db,
extracted using SQLite.
Calculates edit frequency, 
positive and negative words, 
total words, 
number of curses,
vocabulary (#unique words and average unique words per word)?????,
punctuation (",.;:!?") [,.;:!?] //only count punctuation if it is preceded by a letter/number,
number of links, (should probably be done first, because it can count the xml tags '<a href=""http'
number of skype emojis



References:
negative_words.txt and positive_words.txt:
	Minqing Hu and Bing Liu. "Mining and Summarizing Customer Reviews."
		Proceedings of the ACM SIGKDD International Conference on Knowledge
		Discovery and Data Mining (KDD-2004), Aug 22-25, 2004, Seattle,
		Washington, USA.
bad_words.txt: http://fffff.at/googles-official-list-of-bad-words/
csvstrea.h by Andrew DeOrio <awdeorio@umich.edu>, https://github.com/awdeorio/csvstream
rapidxml*.hpp: http://rapidxml.sourceforge.net/, http://rapidxml.sourceforge.net/license.txt
*/


#include "csvstream.h"
//#include "tinyxml2.h"
#include "rapidxml-1.13\rapidxml.hpp"
#include "rapidxml-1.13\rapidxml_print.hpp"
#include "SkypeData.h"
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <regex>

using std::string;
using std::cout;
using std::map;
using std::set;
using std::vector;
using rapidxml::xml_document;
using rapidxml::xml_node;
using std::ifstream;
using std::regex;
using std::regex_replace;
using std::stringstream;

//http://www.sqlapi.com/ use this to get sql into this program so that only the .db file has to be used
//https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm THIS ONE LOOKS BETTER

/*
string XMLToStringConverter(const string& xml);
string XMLSpecialCharToString(const string& xml_word);
string personalTextFormatter(const string& unformatted_string);
void testFunctions();
string storeDataFromXMLString(const string& xml_message);
*/

int main(int argc, char** argv) {

	//read in second argument as file name
	//string file_name = argv[1];

	/*
	xml_document<> doc;
	xml_node<> * root_node;
	ifstream my_skype_data("skype_data.csv");
	*/
	
	//use this one to 
	try {
		//column names are: author,edited_by,body_xml
		csvstream skype_data("skype_data.csv");
		csvstream::row_type row;

		set<string> authors;
		vector<SkypeUser*> skype_users;


		//create all users and fill vector rows
		int user_index = -1;
		while (skype_data >> row) {
		
			if (authors.insert(row["author"]).second) {//new author
				skype_users.push_back(new SkypeUser(row["author"]));
				++user_index;
			}

			skype_users[user_index]->addRow(row);
		}


		//analyze data



		//delete dynamic memory
		for (SkypeUser* user : skype_users) {
			delete user;
		}
	}
	catch (csvstream_exception e) {
		cout << "Unable to open file... quitting\n";
		exit(EXIT_FAILURE);
	}

	//ifstream file_with_xml("")

	//doc.parse<>(test_c);

	//open "negative_words.txt"
	//open "positive_words.txt"
	//open "bad_words.txt"







	/*
	tinyxml2::XMLDocument doc;
	doc.LoadFile("skype_data.csv");
	doc.SaveFile("skype_data_parsed.csv");
	*/


	/*
	try {
		tinyxml2::XMLDocument doc;
		doc.LoadFile("skype_data.csv");
		doc.SaveFile("skype_data_parsed.csv");


		csvstream skype_data("skype_data_parsed.csv");

		//collect data
	}
	catch (csvstream_exception e) {
		cout << "Unable to open file... quiting\n";
		exit(EXIT_FAILURE);
	}
	*/

	cout << "Finished\n";
	return 0;
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
string XMLToStringConverter(const string& xml) {
	//no xml tags
	if (xml[0] != '"') return xml;
	
	//partlist type
	if (xml.find("\"<partlist type=\"\"") != xml.npos) return "";

	//<URIObject type
	if (xml.find("\"<URIObject type=\"\"") != xml.npos) return "";


	string plain_text_string = xml; //use this string to replace things

	/*

	//ss (when analyzing the return of this function, if word is in emoji list, add it to both vocabulary and list of emojis)
	//Example: "Send a message to your Valentine <ss type=""heart"">(heart)</ss>, friends and family"
	size_t ss_pos = xml.find("<ss type=\"");
	while (ss_pos != xml.npos) {
		//ss tag is found


		//FIXME, NOT SURE IF THESE WORK
		regex ss_opening_tag("(<ss type=)\\S+(\"\">)");
		regex ss_closing_tag("</ss>");
		regex ss_format("(<ss type=)\\S+(</ss>)");

		//FIXME, MAY NOT NEED THE WHILE LOOP, OR ANY xml.find
		//remove opening and closing tags
		plain_text_string = regex_replace(plain_text_string, ss_opening_tag, "");
		plain_text_string = regex_replace(plain_text_string, ss_closing_tag, "");

	}

	//a href
	size_t href_pos = xml.find("<a href=\"\"");
	while (href_pos != xml.npos) {
		//a href tag is found

		//FIXME, TEST IF WORKING CORRECTLY
		regex href_opening_tag("(<a href=)\\S+(\"\">)"); //matches opening href tag, from "<a href=" to '"">'
		regex href_closing_tag("</a>"); //matches closing href tag, "</a>"
		regex href_format("(<a href=)\\S+(</a>)"); //matches all text between (inclusive) "<a href=" and "/a>"
		//(<a href)\S+(/a>) is the code on regex101, but I think two \ are needed for the S because


		//http://www.cplusplus.com/reference/regex/

		//size_t href_start = xml.find(

		//count instances of href, could be sending multiple links in one message


		//FIXME, MAY NOT NEED THE WHILE LOOP, OR ANY xml.find
		//remove opening and closing tags
		plain_text_string = regex_replace(plain_text_string, href_opening_tag, "");
		plain_text_string = regex_replace(plain_text_string, href_closing_tag, "");
	}

	*/

	//FIXME, NOT SURE IF THESE WORK
	regex ss_opening_tag("(<ss type=)\\S+(\"\">)");
	regex ss_closing_tag("</ss>");
	regex ss_format("(<ss type=)\\S+(</ss>)"); //unused
	plain_text_string = regex_replace(plain_text_string, ss_opening_tag, "");
	plain_text_string = regex_replace(plain_text_string, ss_closing_tag, "");

	//FIXME, TEST IF WORKING CORRECTLY
	regex href_opening_tag("(<a href=)\\S+(\"\">)"); //matches opening href tag, from "<a href=" to '"">'
	regex href_closing_tag("</a>"); //matches closing href tag, "</a>"
	regex href_format("(<a href=)\\S+(</a>)"); //matches all text between (inclusive) "<a href=" and "/a>"
	//(<a href)\S+(/a>) is the code on regex101, but I think two \ are needed for the S because

	//FIXME, for now, we'll just ignore links when converting to text
	plain_text_string = regex_replace(plain_text_string, href_format, "");
	//remove opening and closing tags
	//plain_text_string = regex_replace(plain_text_string, href_opening_tag, "");
	//plain_text_string = regex_replace(plain_text_string, href_closing_tag, "");


	//when user adds the bold, takes form: <b raw_pre=""*"" raw_post=""*""></b>
	//when the skype bot adds bold, takes form: <b></b>
	regex b_user_opening_tag("(<b raw_pre=).*(post=\"\"*\"\">)");
	regex b_bot_opening_tag("<b>");
	regex b_user_closing_tag("</b>");
	plain_text_string = regex_replace(plain_text_string, b_user_opening_tag, "");
	plain_text_string = regex_replace(plain_text_string, b_bot_opening_tag, "");
	plain_text_string = regex_replace(plain_text_string, b_user_closing_tag, "");
	
	//should always take form: <i raw_pre=""_"" raw_post=""_"">
	//not encountered, but skype bot may use <i></i>
	regex i_opening_tag("(<i raw_pre=).*(\"\">)");
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

	regex s_opening_tag("(<s raw_pre=)\\S+(\"\">)");
	regex s_bot_opening_tag("<s>");
	regex s_closing_tag("</s>");
	plain_text_string = regex_replace(plain_text_string, s_opening_tag, "");
	plain_text_string = regex_replace(plain_text_string, s_bot_opening_tag, "");
	plain_text_string = regex_replace(plain_text_string, s_closing_tag, "");
	
	//br doesn't have opening and closing tags, it only acts as a line break
	regex br_tag("<br/>");
	plain_text_string = regex_replace(plain_text_string, br_tag, "");


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

	//NOTE: DOES NOT FULLY DEAL WITH THE TEXT WITHIN THE QUOTE TAGS
	//FIXME, NOT SURE IF THIS IS RIGHT
	//FIXME, I AM COUNTING QUOTES TOWARD THE QUOTER'S VOCABULARY
	regex quote_opening_tag("<quote.*>"); //matches anything from <quote to >
	/*
	matches:
	<quote author=""andrius.gobis"" authorname=""Andrius Gobis"" conversation=""19:6231efe89fc74b3884799e8c68c75912@thread.skype"" guid=""x50b30ee4
	e6b8d6e7038c989abecb44260fcf24b5ca7d84e8f9d80255d0ca1081"" timestamp=""1486322839"">
	*/
	plain_text_string = regex_replace(plain_text_string, quote_opening_tag, "");
	//deal with /qu
	regex quote_closing_tag("</quote>");
	plain_text_string = regex_replace(plain_text_string, quote_closing_tag, "");

	//deal with legacyquote
	/*
	<legacyquote>[1:06:02 AM] Erikas Anu�is: </legacyquote>glob, duki and I are planning a 
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



//FIXME, WRITE TEST CASES
/*
REQUIRES:	xml_word is a single word, with no spaces
MODIFIES:	nothing
EFFECTS:	returns a string based on xml_word, 
			with all xml special characters replaced by their plain text characters
*/
string XMLSpecialCharToString(const string& xml_word) {
	vector<string> xml_special_chars = { "&lt;", "&amp;", "&gt;", "&quot;", "&apos;" };
	
	string parsed_word = xml_word;

	//check word for all special characters
	for (const string& special_char : xml_special_chars) {
		size_t pos = parsed_word.find(special_char);
		//loop until all occurrences are found
		while (pos != parsed_word.npos) { 
			if (special_char == "&lt;") {
				parsed_word.replace(pos, pos + 4, "<");
			}
			else if (special_char == "&amp;") {
				parsed_word.replace(pos, pos + 5, "&");
			}
			else if (special_char == "&gt;") {
				parsed_word.replace(pos, pos + 4, ">");
			}
			else if (special_char == "&quot;") {
				parsed_word.replace(pos, pos + 6, "\"");
			}
			else if (special_char == "&apos;") {
				parsed_word.replace(pos, pos + 6, "'");
			}
			pos = parsed_word.find(special_char, pos); //try to find next instance in the word
		}
	}


	return parsed_word;
}


/*
-Removes quotes that the user adds
-if there is a /, treat the string as two words
*/
string personalTextFormatter(const string& unformatted_string) {
	regex quote("\"");
	string removed_quotes_string = unformatted_string;
	return regex_replace(removed_quotes_string, quote, "");
}


void testFunctions() {

}


//FIXME
string storeDataFromXMLString(const string& xml_message) {

	stringstream ss_xml_message(xml_message);
	return "";
}
