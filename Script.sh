#!/bin/bash

# Create the main folder
mkdir -p "Assignment_4"

# Change to the new folder
cd "Assignment_4" || exit

# Create the files listed in the image (excluding the BibTeX file)
touch Makefile
touch Q1.cpp
touch Q2.cpp
touch Q3.cpp
touch README.md

echo "Folder 'Assignment_4' and files created successfully!"