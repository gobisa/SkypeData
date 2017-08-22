# SkypeData

### Analyzes a csv file of skype data found in main.db, extracted using SQLite.
### Calculates edit frequency, word count, unique words, punctuation frequency, link frequency, emoji frequency, negative and positive emotional word use, bad/dirty word use.
### Outputs data as csv.


#####Skype Messages found in Users\\"YourWindowsUsername"\\AppData\\Roaming\\Skype\\"YourSkypeUsername"\\main.db
#####SQL query used on main.db: SELECT author,edited_by,replace(body_xml,CHAR(10),'') FROM messages ORDER BY author
#####SQLiteStudio3 Export Settings for exporting SQL query results from main.db to a compatible csv:
 * Export format: CSV
 * Output: File (name should be "main.csv")
 * Export format options: select "Column names in first row", select ", (comma)" for Column separator, and Export NULL values as Empty string
