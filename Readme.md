# Assignment 4

## Overview

This assignment contains three C++ programs focused on publication management, BibTeX parsing, and analysis. Each program demonstrates advanced C++ programming concepts such as inheritance, regular expressions, and STL usage.

---

## Program Details

### **Q1.cpp**
- **Description**: Manages academic publications for an institute.
- **Features**:
  - Base class `PublicationBase` and derived class `ResearchPublication`.
  - Adds contributors and filters publications by year.
  - Displays publication details in a structured format.

### **Q2.cpp**
- **Description**: Parses a BibTeX file and groups publications by authors.
- **Features**:
  - Extracts metadata such as title, authors, venue, year, and DOI.
  - Uses regular expressions to parse BibTeX entries.
  - Outputs publications grouped by each author.

### **Q3.cpp**
- **Description**: Enhances BibTeX parsing with error handling and statistical analysis.
- **Features**:
  - Validates brace matching in BibTeX entries.
  - Prevents duplicate author entries.
  - Displays co-author counts and calculates average co-authors per paper.

---

## Makefile

The provided `Makefile` automates the compilation, execution, and cleanup of the programs. 

- **Targets**:
  - `all`: Compiles all three programs.
  - `run1`: Runs the program in `Q1.cpp`.
  - `run2`: Runs the program in `Q2.cpp`.
  - `run3`: Runs the program in `Q3.cpp` with an author name as input.
  - `clean`: Removes compiled executables.

### Example Usage
To build and run a program:
make run1
make run2
make run3 AUTHOR="Author Name"


