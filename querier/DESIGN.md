# DESIGN.md for Querier
As according to the requirements spec, the querier is a standalone program that reads an index file and answers queries submitted via stdin.

## User interface
The querier's only interface with the user is the command line; it must always have two arguments.
`querier pageDirectory indexFilename`
for example if we were running a query on the a `letters` pagedirectory and index file:
`querier ../data/letters ../data/letters.index`
since it accepts input from stdin we can pipe in input as well:
`echo test query | querier ../data/letters ../data/letters.index`

## Inputs and Outputs
Inputs: the querier reads from stdin.
Outputs the querier writes a ranked list of matching documents to stdout.

## Functinal decomposition into module
The code of querier is broken down into 7 main functions, 2 utility functions and 5 helper functions for iterators. The details of the functions are in comments before each one and are not repeated here. The core functions are 
- main 
- parseArgs
- parseQuery
- validateQueryStruct
- tokenize
- calulateScores
- rankScores

The utility functinos are
- countersUnion
- countersIntersection

The iterator helper functions are 
- countersUnionHelper
- countersSameKeys
- countersIntersectionHelper
- rankHelper
- countMatches


## Pseudo Code for logic/algorithmic flow
The overall flow for the querier is:

        Read query from stdin
        make sure it only contains letters and spaces
            tokenize it and normalize each word
            validate structure of tokenized query
                if valid, calculate scores for the query in a counters data structure
                from above counters object rank the counts and print them.


## Testing plan

The querier is tested with bad command line inputs, bad querier and random correct queries using fuzzquery.c. 
It will test: incorrect number of arguments, invalid indexFile, invalid page directory, invalid query (starts/ends with operator), invalid query (two queries adjacent), invalid query (invalid characters), random correct queries with fuzzquery.c.