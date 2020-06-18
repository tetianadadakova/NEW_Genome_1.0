#pragma once
#include <vector>
#include <stdint.h>
#include "AbstractGenome.h"
#include <cstddef>

class Uma_NKGenome : public AbstractGenome {
private:
    // custom properties & functions
    std::vector<std::byte> sites;
public:
    Uma_NKGenome(size_t _size);
    ~Uma_NKGenome() override {
        //std::cout << "done" << std::endl;
    }

    std::byte* data() override;

    virtual void resize(size_t new_size) override;

    virtual int initGeneSet(std::vector<GeneDefinition> geneInfo) override {
        // geneInfo is a struct of GeneDefinition. Genome will localize the geneInfo and
        // return a key that can be used to access these genes in the future

        // temporary return
        return(1);
    }

    virtual std::unordered_map<std::string, std::vector<size_t>> getGenePositions(int key) override {
        // return a map of vector where each map key is the name indicated in GeneDefinition,
        //and the vector is the list of start codons associated with that name.

        // temporary return
        std::unordered_map<std::string, std::vector<size_t>> geneMap = { {"det", {0}},{"prob",{ 10,20}} };
        return(geneMap);
    }

    virtual void mutate() override; 

};

namespace NK {

    // Represents an organism (genome, score) in a population of organisms
    struct Organism {
        AbstractGenome* genome;
        double score;
    };
}
