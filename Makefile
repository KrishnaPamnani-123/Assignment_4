# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Executable names
EXEC1 = q1
EXEC2 = q2
EXEC3 = q3

# Source files
SRC1 = q1.cpp
SRC2 = q2.cpp
SRC3 = q3.cpp

# Default target
all: $(EXEC1) $(EXEC2) $(EXEC3)

# Compile q1.cpp
$(EXEC1): $(SRC1)
	$(CXX) $(CXXFLAGS) -o $(EXEC1) $(SRC1)

# Compile q2.cpp
$(EXEC2): $(SRC2)
	$(CXX) $(CXXFLAGS) -o $(EXEC2) $(SRC2)

# Compile q3.cpp
$(EXEC3): $(SRC3)
	$(CXX) $(CXXFLAGS) -o $(EXEC3) $(SRC3)

# Run q1
run1: $(EXEC1)
	./$(EXEC1)

# Run q2
run2: $(EXEC2)
	./$(EXEC2)

# Run q3 with AUTHOR argument
run3: $(EXEC3)
	./$(EXEC3) "$(AUTHOR)"

# Clean up build artifacts
clean:
	rm -f $(EXEC1) $(EXEC2) $(EXEC3)
