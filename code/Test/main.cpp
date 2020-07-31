// stl library
#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include <bitset>
#include <random>
#include <map>
#include <cstddef>

// testing
#include "UmaTests.h"
#include "VictoriaTests.h"
#include "InvalidMutationTests.h"

// genomes
#include "AbstractGenome.h"
#include "TestGenome.h"
#include "GenomeLite.h"
#include "UmaGenome.h"

// misc
#include "utilities.h"


/** main function for running tests **/
int main() 
{
    /**     Uma's Testing Suite         **/
    /* Pass in false(0) to just see pass/fail results of all tests.
       Pass in true(1) to see pass/fail results AND debug output of all tests.
       Pass in <GenomeName> based on which genome class you want to test. */
    UmaTests::runAllTests<UmaGenome>(0);

    /**     Victoria's Testing Suite         **/
    /* Pass in false(0) to just see pass/fail results of all tests.
       Pass in true(1) to see pass/fail results AND debug output of all tests.
       Pass in <GenomeName> based on which genome class you want to test. */
     VictoriaTests::TestAll<UmaGenome>(0);

    /**     Invalid Mutations Test Suite     **/
    /* The tests below test the out of bounds checks in the mutation methods.
       Run each test by itself, not all three together, since each test should exit.
       Pass in <GenomeName> based on which genome class you want to test.
       Pass in true(1) to enable debug mode. */
    InvalidMutationTests::invalidOverwriteTest<UmaGenome>(0);
    //InvalidMutationTests::invalidInsertTest<UmaGenome>(0);
    //InvalidMutationTests::invalidRemoveTest<UmaGenome>(0);

    return(0);
}