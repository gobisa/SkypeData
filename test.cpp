#include "SkypeData.h"
#include "csvstream.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <regex>
#include <sstream>
#include <cassert>

using std::string;
using std::cout;
using std::map;
using std::set;
using std::vector;
using std::ifstream;
using std::regex;
using std::regex_replace;
using std::stringstream;


int main() {

	SkypeUser test("test");
	assert(test.XMLToStringConverter("My <b raw_pre = ""*"" raw_post = ""*"">intro< / b> wow") == "wow");
	assert(test.XMLToStringConverter("<ss type=""sad"">:(</ss>") == ":(");
	assert(test.XMLToStringConverter("<a href=""https://www.youtube.com"">https://www.youtube.com</a>") == "");
	assert(test.XMLToStringConverter("hello <a href=""https://www.youtube.com"">https://www.youtube.com</a> goodbye") == "hello  goodbye");
	assert(test.XMLToStringConverter("<i raw_pre=""_"" raw_post=""_"">hello</i>") == "hello");
	assert(test.XMLToStringConverter("lets see what <s raw_pre = ""~"" raw_post = ""~"">this< / s> says") == "lets see what this says");
	assert(test.XMLToStringConverter("Be one of first to try out the new Skype Preview for Android <ss type=""wink"">wink</ss><br/><b>What&apos;s new?</b><br/>-Add live emoticons, text and photos to your video and voice calls<br/>-Quickly swipe to capture a photo <ss type=""selfie"">selfie</ss> and add emoticons, stickers and annotations<br/>-Add your reaction to any message. Vote <ss type=""yes"">yes</ss> or show your excitement <ss type=""sarcastic"">sarcastic</ss><br/>-Easily find news, sports results, restaurants, weather <ss type=""rain"">rain</ss> and quickly add to the chat without leaving the app<br/><a href=""https://go.skype.com/blog.skypepreview-express?WT.mc_id=ICCO_2491_skypepreview_all_list_learn"">Learn more</a>") == "Be one of first to try out the new Skype Preview for Android wink What&apos;s new? -Add live emoticons, text and photos to your video and voice calls -Quickly swipe to capture a photo selfie and add emoticons, stickers and annotations -Add your reaction to any message. Vote yes or show your excitement sarcastic -Easily find news, sports results, restaurants, weather rain and quickly add to the chat without leaving the app ");
	assert(test.XMLToStringConverter("<quote author=""andrius.gobis"" authorname=""Andrius Gobis"" conversation=""19:6231efe89fc74b3884799e8c68c75912@thread.skype"" guid=""x50b30ee4e6b8d6e7038c989abecb44260fcf24b5ca7d84e8f9d80255d0ca1081"" timestamp=""1486322839""><legacyquote>[2:27:19 PM] Andrius Gobis: </legacyquote>my quote<legacyquote> &lt; &lt; &lt; < / legacyquote>< / quote>the text") == "my quote the text");

	cout << "TESTS PASSED";
}