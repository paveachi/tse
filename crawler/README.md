# Crawler - Paul Chirkov (github: paulchir)

# Assumptions and Implementation

No assumtions beyond those outlined in the specs.

Implementation follows the implementation spec.

# Exit codes for crawler

Code, Error:

1, incorrect number of arguments

2, URL is not internal

3, pageDirectory is not a valid path to existing directory

4, depth argument is not a number

5, depth argument is not in correct range

# Testing

Tested with valgrind to ensure there are no memory leaks (make valgrind runs valgrind on all test cases in testing.sh). The bash script Testing.sh tests a variety of cases where parameters are invalid. It also tests the crawler on letters, toscrape and dictionary to depths 0 and 1. The crawler passes all of these tests. 