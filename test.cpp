#include "SkypeData.h"
#include "csvstream.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <regex>
//#include <regex.h>
#include <sstream>
#include <cassert>

using std::string;
using std::cout;
using std::map;
using std::set;
using std::vector;
//using rapidxml::xml_document;
//using rapidxml::xml_node;
using std::ifstream;
using std::regex;
using std::regex_replace;
using std::stringstream;
//using std::assert;

int main() {

	//DISCLAIMER: THESE TESTS ARE NOT ACCURATE, BUT THE CURRENT REGEX IN SkypeData.cpp IS GOOD AND TESTED USING VISUAL ANALYSIS OF OUTPUT


	SkypeUser test("test");
	//cout << "COUT: " << test.XMLToStringConverter("My <b raw_pre = ""*"" raw_post = ""*"">intro< / b> to excel teacher sucks so badly....he goes I don&apos; t even use excel after messing up showing us something lol");
	assert(test.XMLToStringConverter("My <b raw_pre = ""*"" raw_post = ""*"">intro< / b> to excel teacher sucks so badly....he goes I don&apos; t even use excel after messing up showing us something lol") == "My intro to excel teacher sucks so badly....he goes I don&apos; t even use excel after messing up showing us something lol");
	//cout << "cout<<: " << test.XMLToStringConverter("<ss type=""sad"">:(</ss>");
	assert(test.XMLToStringConverter("<ss type=""sad"">:(</ss>") == ":(");
	//cout << "cout<<: " << test.XMLToStringConverter("<a href=""https://www.youtube.com/watch?v=9IlwIUmOoZQ"">https://www.youtube.com/watch?v=9IlwIUmOoZQ</a>");
	assert(test.XMLToStringConverter("<a href=""https://www.youtube.com/watch?v=9IlwIUmOoZQ"">https://www.youtube.com/watch?v=9IlwIUmOoZQ</a>") == "");
	assert(test.XMLToStringConverter("hello <a href=""https://www.youtube.com/watch?v=9IlwIUmOoZQ"">https://www.youtube.com/watch?v=9IlwIUmOoZQ</a> goodbye") == "hello  goodbye");
	assert(test.XMLToStringConverter("<i raw_pre=""_"" raw_post=""_"">hello</i>") == "hello");
	assert(test.XMLToStringConverter("lets see what <s raw_pre = ""~"" raw_post = ""~"">women< / s> feminists have to say about that") == "lets see what women feminists have to say about that");
	//cout << "COUT: " << test.XMLToStringConverter("Be one of first to try out the new Skype Preview for Android <ss type=""wink"">wink</ss><br/><b>What&apos;s new?</b><br/>-Add live emoticons, text and photos to your video and voice calls<br/>-Quickly swipe to capture a photo <ss type=""selfie"">selfie</ss> and add emoticons, stickers and annotations<br/>-Add your reaction to any message. Vote <ss type=""yes"">yes</ss> or show your excitement <ss type=""sarcastic"">sarcastic</ss><br/>-Easily find news, sports results, restaurants, weather <ss type=""rain"">rain</ss> and quickly add to the chat without leaving the app<br/><a href=""https://go.skype.com/blog.skypepreview-express?WT.mc_id=ICCO_2491_skypepreview_all_list_learn"">Learn more</a>");
	//TESTS MANY, BUT MAINLY <br/>
	assert(test.XMLToStringConverter("Be one of first to try out the new Skype Preview for Android <ss type=""wink"">wink</ss><br/><b>What&apos;s new?</b><br/>-Add live emoticons, text and photos to your video and voice calls<br/>-Quickly swipe to capture a photo <ss type=""selfie"">selfie</ss> and add emoticons, stickers and annotations<br/>-Add your reaction to any message. Vote <ss type=""yes"">yes</ss> or show your excitement <ss type=""sarcastic"">sarcastic</ss><br/>-Easily find news, sports results, restaurants, weather <ss type=""rain"">rain</ss> and quickly add to the chat without leaving the app<br/><a href=""https://go.skype.com/blog.skypepreview-express?WT.mc_id=ICCO_2491_skypepreview_all_list_learn"">Learn more</a>") == "Be one of first to try out the new Skype Preview for Android wink What&apos;s new? -Add live emoticons, text and photos to your video and voice calls -Quickly swipe to capture a photo selfie and add emoticons, stickers and annotations -Add your reaction to any message. Vote yes or show your excitement sarcastic -Easily find news, sports results, restaurants, weather rain and quickly add to the chat without leaving the app ");
	cout << "COUT: " << test.XMLToStringConverter("<quote author=""andrius.gobis"" authorname=""Andrius Gobis"" conversation=""19:6231efe89fc74b3884799e8c68c75912@thread.skype"" guid=""x50b30ee4e6b8d6e7038c989abecb44260fcf24b5ca7d84e8f9d80255d0ca1081"" timestamp=""1486322839""><legacyquote>[2:27:19 PM] Andrius Gobis: </legacyquote>it takes painfully long and you hate each other more than catan when you wait for people to finish their turn<legacyquote> &lt; &lt; &lt; < / legacyquote>< / quote>matai this is literally the reason you said you wouldn&apos; t play multiplayer with me");
	cout << "\n\n";
	assert(test.XMLToStringConverter("<quote author=""andrius.gobis"" authorname=""Andrius Gobis"" conversation=""19:6231efe89fc74b3884799e8c68c75912@thread.skype"" guid=""x50b30ee4e6b8d6e7038c989abecb44260fcf24b5ca7d84e8f9d80255d0ca1081"" timestamp=""1486322839""><legacyquote>[2:27:19 PM] Andrius Gobis: </legacyquote>it takes painfully long and you hate each other more than catan when you wait for people to finish their turn<legacyquote> &lt; &lt; &lt; < / legacyquote>< / quote>matai this is literally the reason you said you wouldn&apos; t play multiplayer with me") == "it takes painfully long and you hate each other more than catan when you wait for people to finish their turn matai this is literally the reason you said you wouldn&apos; t play multiplayer with me");
	/*	assert(test.XMLToStringConverter("<quote author=""andrius.gobis"" authorname=""Andrius Gobis"" conversation=""19:6231efe89fc74b3884799e8c68c75912@thread.skype"" guid=""x50b30ee4e6b8d6e7038c989abecb44260fcf24b5ca7d84e8f9d80255d0ca1081"" timestamp=""1486322839""><legacyquote>[2:27:19 PM] Andrius Gobis: </legacyquote>it takes painfully long and you hate each other more than catan when you wait for people to finish their turn<legacyquote> &lt; &lt; &lt; < / legacyquote>< / quote>matai this is literally the reason you said you wouldn&apos; t play multiplayer with me")				"it takes painfully long and you hate each other more than catan when you wait for people to finish their turn<legacyquote>		&lt; &lt; &lt; < / legacyquote>< / quote>matai this is literally the reason you said you wouldn&apos; t play multiplayer with me") == )
	*/
	cout << "TESTS PASSED";
}