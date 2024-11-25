#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <regex>
#include <set>
using namespace std;

class Publication {
private:
    string title;
    string venue;
    vector<string> authors;
    string year;
    string doi;
    string affiliation;

public:
    
    void setTitle(const string& t) { title = t; }
    void setVenue(const string& v) { venue = v; }
    void setAuthors(const vector<string>& a) { authors = a; }
    void setYear(const string& y) { year = y; }
    void setDOI(const string& d) { doi = d; }
    void setAffiliation(const string& a) { affiliation = a; }

    const string& getTitle() const { return title; }
    const string& getVenue() const { return venue; }
    const vector<string>& getAuthors() const { return authors; }
    const string& getYear() const { return year; }
    const string& getDOI() const { return doi; }
    const string& getAffiliation() const { return affiliation; }

    void display() const {
        cout << "  Title: " << title << endl;
        cout << "  Venue: " << venue << endl;
        cout << "  Year: " << year << endl;
        cout << "  DOI: " << doi << endl;
        if (!affiliation.empty()) {
            cout << "  Affiliation: " << affiliation << endl;
        }
        cout << "  Authors: ";
        for (const auto& author : authors) {
            cout << author << "; ";
        }
        cout << endl;
    }
};

vector<string> extractAuthors(const string& authorLine) {
    vector<string> authors;
    regex authorRegex(R"(([^,]+?)(?: and |$))");
    auto start = sregex_iterator(authorLine.begin(), authorLine.end(), authorRegex);
    auto finish = sregex_iterator();

    for (auto it = start; it != finish; ++it) {
        string author = it->str(1);
        author.erase(author.find_last_not_of(" \t") + 1); 
        authors.push_back(author);
    }

    return authors;
}

Publication processEntry(const string& entryContent) {
    Publication pub;
    regex fieldRegex(R"((\w+)\s*=\s*\{(.+?)\},?)");
    auto start = sregex_iterator(entryContent.begin(), entryContent.end(), fieldRegex);
    auto finish = sregex_iterator();

    for (auto it = start; it != finish; ++it) {
        string key = it->str(1);
        string value = it->str(2);

        if (key == "title") {
            pub.setTitle(value);
        } else if (key == "venue" || key == "journal") {
            pub.setVenue(value);
        } else if (key == "author") {
            pub.setAuthors(extractAuthors(value));
        } else if (key == "year") {
            pub.setYear(value);
        } else if (key == "doi") {
            pub.setDOI(value);
        } else if (key == "affiliation") {
            pub.setAffiliation(value);
        }
    }

    return pub;
}

unordered_map<string, vector<Publication>> analyzeBibFile(const string& filePath) {
    ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open the file " << filePath << endl;
        exit(EXIT_FAILURE);
    }

    unordered_map<string, vector<Publication>> authorToPubs;
    string line, entry;

    cout << "Reading BibTeX file: " << filePath << endl;

    while (getline(inputFile, line)) {
        if (line.find('@') == 0) {
            if (!entry.empty()) {
                Publication pub = processEntry(entry);
                for (const auto& author : pub.getAuthors()) {
                    authorToPubs[author].push_back(pub);
                }
                entry.clear();
            }
        }
        entry += line + '\n';
    }

    if (!entry.empty()) {
        Publication pub = processEntry(entry);
        for (const auto& author : pub.getAuthors()) {
            authorToPubs[author].push_back(pub);
        }
    }

    cout << "Finished parsing BibTeX file." << endl;

    return authorToPubs;
}

void displayPublications(const unordered_map<string, vector<Publication>>& authorToPubs) {
    cout << "Publications Grouped by Author:" << endl;

    for (const auto& pair : authorToPubs) {
        cout << "Author: " << pair.first << endl;
        for (const auto& pub : pair.second) {
            pub.display();
        }
        cout << "----------------------------------------" << endl;
    }
}

int main() {
    string filePath = "publist.bib"; 
    auto authorToPubs = analyzeBibFile(filePath);
    displayPublications(authorToPubs);

    return 0;
}
