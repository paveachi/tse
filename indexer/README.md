# EXIT CODES

Code: error

1: improper number of parameters

2: invalid pageDirectory

3: invalid indexFile path

# Assumptions... 

No assumptions beyond those outlined in the specs. 
Testing assumes crawer directories exist for letters, wikipedia and toscrape for depths 0 and 1. it also assumes there exists a `data` folder in the tse directory for the outputs of crawler and indexer.

# Testing/known errors

Indexer and indextest have passed all testing they have undergone. Testing.sh runs through many various test cases, and the command (make valgrind) will run valgrind on all of those cases. 