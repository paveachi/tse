# EXIT CODES

Code: error

1: invalid number of arguments
2: invalid pageDirectory
3: invalid indexFile
4: empty query
5: query contains invalid character
6: error allocating memory for word array
7: query cannot start with operator
8: query cannot end with operator
9: query cannot have two adjacent operators

# Assumptions 

No assumptions in implementation beyond those outlined in the specs. 
Testing assumes crawer directories exist for wikipedia and toscrape for depths 0 and 1 in a 'data' subdirectory of tse. 

# Testing/known errors

The querier has passed all test cases I have tried and is fully functional. 