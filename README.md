**SkypeData**
===============

## Features
### Analyzes a csv file of skype data found in main.db, extracted using SQLite.
### Calculates edit frequency, word count, unique words, punctuation frequency, link frequency, emoji frequency, negative and positive emotional word use, bad/dirty word use.
### Outputs data as csv.

## Sample Output
![alt text](https://github.com/gobisa/SkypeData/blob/master/SkypeDataResultsSample.JPG "Spreadsheet of 'SkypeDataResults.csv'")

## Instructions
1. Find your Skype "main.db" file
2. Query "main.db" using SQLiteStudio3 and export the query results to a csv file named "main.csv"
3. Move "main.csv" to the folder containing the program files
4. Run "main.cpp"
5. Data written to "SkypeDataResults.csv"

#### Skype message data found in Users\\"YourWindowsUsername"\\AppData\\Roaming\\Skype\\"YourSkypeUsername"\\main.db
#### SQL query used on main.db: "SELECT author,edited_by,replace(body_xml,CHAR(10),'') FROM messages ORDER BY author"
#### SQLiteStudio3 Export Settings for exporting SQL query results from main.db to a compatible csv:
 * Export format: CSV
 * Output: File (name should be "main.csv")
 * Export format options: select "Column names in first row", select ", (comma)" for Column separator, and Export NULL values as Empty string

 ![alt text](https://github.com/gobisa/SkypeData/blob/master/SQLiteStudioCSVSettings.PNG "SQLiteStudio CSV Settings")

## References
 * negative_words.txt and positive_words.txt:

	Minqing Hu and Bing Liu. "Mining and Summarizing Customer Reviews."
		Proceedings of the ACM SIGKDD International Conference on Knowledge
		Discovery and Data Mining (KDD-2004), Aug 22-25, 2004, Seattle,
		Washington, USA.
 * bad_words.txt: http://fffff.at/googles-official-list-of-bad-words/
 * csvstream.h by Andrew DeOrio <awdeorio@umich.edu>, https://github.com/awdeorio/csvstream
