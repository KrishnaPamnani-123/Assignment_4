#include <iostream>
#include <vector>
#include <string>

using namespace std;

class PublicationBase {
protected:
    string title;
    int year;

public:
    PublicationBase(const string& t, int y) : title(t), year(y) {}

    virtual void showDetails() const {
        cout << "Title: " << title << endl;
        cout << "Year: " << year << endl;
    }

    int getYear() const { return year; } 

    virtual ~PublicationBase() = default;
};


class ResearchPublication : public PublicationBase {
private:
    string event;
    vector<string> contributors;
    string referenceID;

public:
    ResearchPublication(const string& t, const string& e, int y, const string& doi = "")
        : PublicationBase(t, y), event(e), referenceID(doi) {}

    void addContributor(const string& contributor) {
        contributors.push_back(contributor);
    }

    bool hasContributorFrom(const string& org) const {
        for (const auto& contributor : contributors) {
            if (contributor == org) {
                return true;
            }
        }
        return false;
    }

    void showDetails() const override {
        PublicationBase::showDetails();
        cout << "Event: " << event << endl;
        if (!referenceID.empty()) {
            cout << "DOI: " << referenceID << endl;
        }
        cout << "Contributors:" << endl;
        for (const auto& contributor : contributors) {
            cout << "  " << contributor << endl;
        }
    }
};


class AcademicInstitute {
private:
    string name;
    vector<ResearchPublication> catalog;

public:
    AcademicInstitute(const string& n) : name(n) {}

    void addPublication(const ResearchPublication& publication) {
        catalog.push_back(publication);
    }

    void showAllPublications() const {
        cout << "Research Publications by " << name << ":" << endl;
        for (const auto& pub : catalog) {
            pub.showDetails();
        }
    }

    void showPublicationsByYear(int year) const {
        cout << "Publications by " << name << " in " << year << ":" << endl;
        for (const auto& pub : catalog) {
            if (pub.getYear() == year) { 
                pub.showDetails();
            }
        }
    }
};

int main() {
    AcademicInstitute university("Tech University");

    ResearchPublication publication1("Ethics in AI", "AI Conference", 2023, "10.1000/ai123");
    publication1.addContributor("Vinayak Abrol");
    publication1.addContributor("Nikil Lad");

    ResearchPublication publication2("Healthcare and Machine Learning", "AI Journal", 2022);
    publication2.addContributor("Irani Bhattacharya");

    ResearchPublication publication3("Advances in Quantum Computing", "Quantum Journal", 2023, "10.2000/qc789");
    publication3.addContributor("Vinayak Abrol");
    publication3.addContributor("Irani Bhattacharya");

    university.addPublication(publication1);
    university.addPublication(publication2);
    university.addPublication(publication3);

    university.showAllPublications();
    university.showPublicationsByYear(2023);

    return 0;
}
