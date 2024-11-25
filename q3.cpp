#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <regex>
using namespace std;

class BasePublication {
protected:
    string title;
    string year;

public:
    BasePublication() = default;

    void setTitle(const string& t) { title = t; }
    void setYear(const string& y) { year = y; }

    virtual void display() const {
        cout << "Title: " << title << endl;
        cout << "Year: " << year << endl;
    }

    virtual ~BasePublication() = default;
};

class Publication : public BasePublication {
private:
    string venue;
    vector<string> authors;
    string doi;

public:
    void setVenue(const string& v) { venue = v; }
    void setAuthors(const vector<string>& a) { authors = a; }
    void setDOI(const string& d) { doi = d; }

    const string& getVenue() const { return venue; }
    const vector<string>& getAuthors() const { return authors; }
    const string& getDOI() const { return doi; }

    void display() const override {
        BasePublication::display();
        cout << "Venue: " << venue << endl;
        cout << "DOI: " << doi << endl;
        cout << "Authors: ";
        for (const auto& author : authors) {
            cout << author << "; ";
        }
        cout << endl;
    }

    static string sanitizeString(const string& input) {
        string result = regex_replace(input, regex("[{},]"), "");
        result.erase(0, result.find_first_not_of(" \t"));
        result.erase(result.find_last_not_of(" \t") + 1);
        return result;
    }

    static bool areBracesBalanced(const string& content) {
        int count = 0;
        for (char ch : content) {
            if (ch == '{') ++count;
            if (ch == '}') --count;
            if (count < 0) return false;
        }
        return count == 0;
    }

    static vector<string> extractAuthors(const string& authorData) {
        vector<string> authorList;
        set<string> uniqueAuthors;
        stringstream ss(authorData);
        string author;
        size_t position = 0, start = 0;

        while ((position = authorData.find(" and ", start)) != string::npos) {
            author = sanitizeString(authorData.substr(start, position - start));
            start = position + 5;
            if (uniqueAuthors.count(author)) {
                cerr << "Error: Duplicate author found." << endl;
                exit(EXIT_FAILURE);
            }
            uniqueAuthors.insert(author);
            authorList.push_back(author);
        }

        author = sanitizeString(authorData.substr(start));
        if (uniqueAuthors.count(author)) {
            cerr << "Error: Duplicate author found." << endl;
            exit(EXIT_FAILURE);
        }
        uniqueAuthors.insert(author);
        authorList.push_back(author);

        return authorList;
    }

    static Publication parseEntry(const string& entry) {
        if (!areBracesBalanced(entry)) {
            cerr << "Error: Mismatched braces in the following entry:\n" << entry << endl;
            exit(EXIT_FAILURE);
        }

        Publication publication;
        stringstream stream(entry);
        string line;

        while (getline(stream, line)) {
            size_t position;
            if ((position = line.find("title=")) != string::npos) {
                publication.setTitle(sanitizeString(line.substr(position + 6)));
            } else if ((position = line.find("venue=")) != string::npos) {
                publication.setVenue(sanitizeString(line.substr(position + 6)));
            } else if ((position = line.find("author=")) != string::npos) {
                string authorData = sanitizeString(line.substr(position + 7));
                publication.setAuthors(extractAuthors(authorData));
            } else if ((position = line.find("year=")) != string::npos) {
                publication.setYear(sanitizeString(line.substr(position + 5)));
            } else if ((position = line.find("doi=")) != string::npos) {
                publication.setDOI(sanitizeString(line.substr(position + 4)));
            }
        }

        return publication;
    }
};

unordered_map<string, vector<Publication>> processBibFile(const string& filePath) {
    ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open the file: " << filePath << endl;
        exit(EXIT_FAILURE);
    }

    unordered_map<string, vector<Publication>> authorToPubs;
    string line, entry;

    while (getline(inputFile, line)) {
        if (line.find("@") == 0) {
            if (!entry.empty()) {
                Publication pub = Publication::parseEntry(entry);
                for (const string& author : pub.getAuthors()) {
                    authorToPubs[author].push_back(pub);
                }
                entry.clear();
            }
        }
        entry += line + '\n';
    }

    if (!entry.empty()) {
        Publication pub = Publication::parseEntry(entry);
        for (const string& author : pub.getAuthors()) {
            authorToPubs[author].push_back(pub);
        }
    }

    return authorToPubs;
}

void showPublicationsForAuthor(const unordered_map<string, vector<Publication>>& authorToPubs, const string& authorName) {
    string normalizedAuthor = Publication::sanitizeString(authorName);
    auto it = authorToPubs.find(normalizedAuthor);

    if (it == authorToPubs.end()) {
        cout << "No publications found for author: " << authorName << endl;
        return;
    }

    const vector<Publication>& publications = it->second;
    cout << "Publications for Author: " << authorName << endl;
    int totalCoauthors = 0;

    for (const Publication& pub : publications) {
        pub.display();
        int coauthors = pub.getAuthors().size() - 1;
        totalCoauthors += coauthors;
        cout << "  Number of Co-authors: " << coauthors << endl << endl;
    }

    double avgCoauthors = (publications.size() > 0) ? static_cast<double>(totalCoauthors) / publications.size() : 0.0;
    cout << "Average Co-authors per Paper: " << avgCoauthors << endl;
    cout << "----------------------------------------" << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Error: Please provide an author name as an argument." << endl;
        return EXIT_FAILURE;
    }

    string filename = "publist.bib";
    auto authorToPubs = processBibFile(filename);

    for (int i = 1; i < argc; ++i) {
        string authorName = argv[i];
        showPublicationsForAuthor(authorToPubs, authorName);
    }

    return 0;
}
