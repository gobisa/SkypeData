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

//http://www.sqlapi.com/ use this to get sql into this program so that only the .db file has to be used
//https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm THIS ONE LOOKS BETTER

int main(int argc, char** argv) {

	//read in second argument as file name
	//string file_name = argv[1];

	
	xml_document<> doc;
	xml_node<> * root_node;
	ifstream my_skype_data("skype_data.csv");
	
	
	//use this one to 
	csvstream skype_data_with_xml("skype_data.csv");
	csvstream skype_data_parsed("skype_data_parsed.csv");

	string xml_text;
	string parsed_text;

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
string XMLToStringConverter(const string& xml) {
	//no xml tags
	if (xml[0] != '"') return xml;
	
	//partlist type
	if (xml.find("\"<partlist type=\"\"") != xml.npos) return "";

	//<URIObject type
	if (xml.find("\"<URIObject type=\"\"") != xml.npos) return "";

	//ss (when analyzing the return of this function, if word is in emoji list, add it to both vocabulary and list of emojis)


	//a href
	size_t href_pos = xml.find("<a href=\"\"");
	while (href_pos != xml.npos) {
		//a href tag is found

		//FIXME, TEST IF WORKING CORRECTLY
		regex href_format("(<a href)\\S+(/a>)"); //matches all text between (inclusive) "<a href" and "/a>"
		//(<a href)\S+(/a>) is the code on regex101, but I think two \ are needed for the S because

		//count instances of href, could be sending multiple links
	}


	//string is in quotes, but has no tags
	return xml;
}



//FIXME, WRITE TEST CASES
/*
REQUIRES:	xml_word is a single word, with no spaces
MODIFIES:	null
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
-Removes quotes
-if there is a /, treat the string as two words
*/
string personalTextFormatter(const string& unformatted_string) {

}
