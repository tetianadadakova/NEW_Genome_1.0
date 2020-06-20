#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include <bitset>
#include "AbstractGenome.h"
#include "StephanieGenome.h"
#include <cstddef>
#include "utilities.h"
#include "random.h"
#include <iomanip>
#include <algorithm>
#include <cstdlib>

struct genomeChangelog {
	std::byte* address;
	size_t index;		//is this really needed?
	int indexDiff;
};


void stephanie_runNKFitnessTests(AbstractGenome* genome) {
	double score = { 0.0 };
	int n = 10;
	int k = 3;
	int k_exp = std::pow(2, k);
	int genome_position = 0; 
	std::string genomeString = {};

	//initialize genome to 5 sites
	//generate values for genome
	genome->resize(n);
	auto readHead = genome->data();
	for (size_t i(0); i < genome->size(); ++i)
		readHead[i] = (std::byte)(i % 2);

	//build grid
	std::cout << std::fixed;
	std::cout << std::setprecision(1);
	std::vector<std::vector<double>> grid = std::vector<std::vector<double>>(k_exp, std::vector<double>(n, 0.0));
	for (int i = 0; i < grid.size(); i++) { //rows
		for (int j = 0; j < grid[i].size(); j++) { //columns
			double randomValue = Random::getDouble(1);
			grid[i][j] = randomValue;
		}
	}

	//loop through genome
	for (int i = 0; i < genome->size(); i++) {
		/*//print grid
		for (int i = 0; i < grid.size(); i++) { //rows
			for (int j = 0; j < grid[i].size(); j++) { //columns
				std::cout << grid[i][j] << " ";
			}
			std::cout << std::endl;
		}*/
		genome_position = i;
		//check if at last position in genome
		if (i == n - 1) {
			auto value = std::to_integer<int>(GN::genomeRead<std::byte>(genome, genome_position));
			genomeString.append(std::to_string(value));
			genome_position = genome_position % genome_position;
			for (int j = genome_position; j < k - 1; j++) {
				auto value = std::to_integer<int>(GN::genomeRead<std::byte>(genome, genome_position));
				++genome_position;
				genomeString.append(std::to_string(value));
			}
		} 
		else {
			for (int j = 0; j < k; j++) {
				auto value = std::to_integer<int>(GN::genomeRead<std::byte>(genome, genome_position));
				++genome_position;
				genomeString.append(std::to_string(value));
			}
		}
		std::bitset<1000> genomePair(genomeString);
		unsigned long genomeKValue = genomePair.to_ulong();
		score += grid[genomeKValue][i] / n;

		std::cout << "\ngenome: ";
		for (size_t i(0); i < genome->size(); ++i)
			std::cout << GN::genomeRead<std::byte>(genome, i) << " ";
		std::cout << std::endl;
		std::cout << "genome site: " << genomeString << std::endl;
		std::cout << "k row: " << genomeKValue << std::endl;
		std::cout << "n column: " << i << std::endl;
		std::cout << "grid[" << genomeKValue << "][" << i << "]: " << grid[genomeKValue][i] << std::endl;
		std::cout << "score: " << score << std::endl;
		genomeString.clear();
		std::cout << std::endl;
	}
}

void stephanie_runChangeLogStruct(AbstractGenome* genome) {
	//std::vector of a genomeChangelog struct
	std::vector<genomeChangelog> changelog;
	genome->resize(100);									//initialize genome to 100
	auto readHead = genome->data();							//return a pointer to the first element address of the vector
	for (size_t i(0); i < genome->size(); i++) {			//populate the genome with values
		readHead[i] = (std::byte)(Random::getInt(99));
	}

	//delete three sites
	changelog.push_back(genomeChangelog());
	changelog[0].address = &(GN::genomeRead<std::byte>(genome, 3));
	changelog[0].index = 3;
	changelog[0].indexDiff = -3;
	size_t modifiedSize = genome->size() + changelog[0].indexDiff;
	size_t counter = 0;
	/*for (size_t i(0); i < modifiedSize; i++) {
		for (auto& itr : changelog) {
			if (i == itr.index) {
				i += std::abs(itr.indexDiff);
			}
		}
		std::cout << counter << " " << i << " " << GN::genomeRead<std::byte>(genome, i) << std::endl;
		counter++;
	}*/

	for (size_t i(0); i < genome->size(); i++) {
		for (auto& itr : changelog) {
			if (i == itr.index) {
				counter += std::abs(itr.indexDiff);
			}
		}
		std::cout << i << " " << counter << " " << GN::genomeRead<std::byte>(genome, i) << std::endl;
		counter++;
	}

	//delete one site
/*	changelog.push_back(genomeChangelog());
	changelog[0].address = &(GN::genomeRead<std::byte>(genome, 3));
	changelog[0].index = 3;
	changelog[0].indexDiff = -1;

	size_t modifiedSize = genome->size() + changelog[0].indexDiff;
	for (auto& itr : changelog) {
		std::cout << itr.index << std::endl;
	}
	std::vector<genomeChangelog>::iterator itr;
	size_t counter = 0;
	for (size_t i(0); i <= modifiedSize; i++) {
		for (auto& itr : changelog) {
			if (i == itr.index)
				i++;
		}

		std::cout << counter << " " << GN::genomeRead<std::byte>(genome, i) << std::endl;
		counter++;
	}*/
}

void stephanie_runChangeLogBytePtr(AbstractGenome* genome) {
	//std::vector storing site position and std::byte pointers
    /*	std::vector<std::pair<size_t, std::byte*>> changelog;
	genome->resize(100);									//initialize genome to 100
	auto readHead = genome->data();							//return a pointer to the first element address of the vector
	for (size_t i(0); i < genome->size(); i++) {			//populate the genome with values
		readHead[i] = (std::byte)(Random::getInt(99));
	}
	//site 3 will be deleted
	//get pointer to site 3 in genome
	std::byte* deletedSiteAddress = &(GN::genomeRead<std::byte>(genome, 3)); 
	size_t deletedSiteIndex = 3;
	changelog.push_back(std::make_pair(deletedSiteIndex, deletedSiteAddress));
	//get pointer to site 4
	std::byte* nextSiteAddress = deletedSiteAddress + 1;
	size_t nextSiteIndex = deletedSiteIndex + 1;
	std::cout << deletedSiteIndex << " " << deletedSiteAddress << " " << *deletedSiteAddress << std::endl;
	std::cout << nextSiteIndex << " " << nextSiteAddress << " " << *nextSiteAddress << std::endl;

	//how to have genome print values and then print the other section?

	auto genomeStart = genome->data();
	size_t index = 0;
	size_t counter = 0;
	std::vector<std::pair<size_t, std::byte*>>::iterator itr;

	while (genomeStart <= &readHead[100-1]) {
		for (auto& itr : changelog) {
			//if address of site is in the changelog vector
			//increment the index
			if (itr.second == &(GN::genomeRead<std::byte>(genome, counter))) {
				index++;
			}
		}
		if (index < 100) {
			std::cout << counter << " " << GN::genomeRead<std::byte>(genome, index) << " " << &(GN::genomeRead<std::byte>(genome, index)) << std::endl;

		}
		genomeStart++;
		counter++;
		index++;
	}
*/
    /*	std::byte* address = &(GN::genomeRead<std::byte>(genome, 3));
	size_t index = 3;
	while (address <= &readHead[100-1]) {
		std::cout << address << " [" << index << "]: " << *address << std::endl;
		index++;
		address++;
	}

	std::cout << address << " " << *address << std::endl; 	// address is a pointer of std::byte, *address gives you the value at that pointer address
	++address;
	std::cout << address << " " << *address << std::endl;
	++address;
	std::cout << address << " " << *address << std::endl;*/

	//std::vector storing std::byte pointers
    /*	std::vector<std::byte*> testVector;
	genome->resize(100);									//initialize genome to 100
	auto readHead = genome->data();							//return a pointer to the first element address of the vector
	for (size_t i(0); i < genome->size(); i++) {			//populate the genome with values
		readHead[i] = (std::byte)(Random::getInt(99));
		testVector.push_back(&readHead[i]);					//populate the vector with values
	}
	size_t counter = 0;
	for (std::vector<std::byte*>::iterator it = testVector.begin(); it != testVector.end(); ++it) {
		std::cout << counter << " " << *(*it) << std::endl;
			counter++;
	}*/ 

}

void stephanie_runChangeLogMap(AbstractGenome* genome) {

	//map of size_t, and vector of bytes -- implemented, needs more complexity
	/*
	std::map<size_t, std::vector<std::byte>> changeLog;

	for (size_t i(0); i < genome->size(); i++) {
		std::byte value = (std::byte)(Random::getInt(99));
		readHead[i] = value;
		changeLog.insert(std::pair<size_t, std::vector<std::byte> >(i, std::vector<std::byte>()));
		changeLog[i].push_back(value);
	}
//	GN::genomeWrite(genome, 3, (std::byte)(92));
	changeLog[3].push_back((std::byte)92);
	changeLog[1].push_back((std::byte)75);
	changeLog.erase(2);

	for (auto& t : changeLog)
		std::cout << t.first << " " << t.second << std::endl;
	//	for (size_t i(0); i < genome->size(); ++i)
	//		std::cout << "[" << i << "]: " << GN::genomeRead<std::byte>(genome, i) << "\n";*/

	//map of size_t, and vector pair of index and bytes -- sorta implemented
	/*std::map < size_t, std::vector<std::pair<size_t, std::byte>> > changeLog;

	for (size_t i(0); i < genome->size(); i++) {
		std::byte value = (std::byte)(Random::getInt(99));
		readHead[i] = value;
		changeLog[i].push_back(std::make_pair(0, value));
	}
	changeLog[3].push_back(std::make_pair(1, (std::byte)92));
	std::cout << changeLog[3][0].first << std::endl;
	std::cout << changeLog[3][0].second << std::endl;
	size_t my_index = 3;
	std::cout << changeLog[my_index][0].first << std::endl;
	std::cout << changeLog[my_index][0].second << std::endl;
	std::cout << changeLog[my_index][1].first << std::endl;
	std::cout << changeLog[my_index][1].second << std::endl;*/

	//map of size_t, and map of index and bytes -- implemented
	/*	std::map< size_t, std::map<size_t, std::byte> > changeLog;
		for (size_t i(0); i < genome->size(); i++) {
			std::byte value = (std::byte)(Random::getInt(99));
			readHead[i] = value;

			changeLog.insert(make_pair(i, std::map<size_t, std::byte>()));
			changeLog[i].insert(make_pair(0, value));
		}

		changeLog[3].insert(make_pair(1, (std::byte)92)); //point mutation
		changeLog[1].insert(make_pair(2, (std::byte)75)); //point mutation


		std::map< size_t, std::map<size_t, std::byte> >::iterator itr;	// For accessing outer map
		std::map<size_t, std::byte>::iterator ptr;	// For accessing inner map

		//print through all items
		for (itr = changeLog.begin(); itr != changeLog.end(); itr++) {
			for (ptr = itr->second.begin(); ptr != itr->second.end(); ptr++) {
				std::cout << "[" << itr->first
					<< "][" << ptr->first
					<< "]: " << ptr->second << std::endl;
			}
		}
		//print only the last head of the sub map
		for (itr = changeLog.begin(); itr != changeLog.end(); itr++) {
			auto test = itr->second.rbegin();
			std::cout << "[" << itr->first << "][" << test->first << "]: " << test->second << std::endl;
		}

		//insertion mutation
		itr = changeLog.find(2);
		auto test = itr->second.rbegin();
		auto changeLogValue = test->second;
		std::cout << itr->first << " " << changeLogValue  << std::endl;
		changeLog[2].insert(make_pair(3, (std::byte)75)); //point mutation


		for (itr = changeLog.begin(); itr != changeLog.end(); itr++) {
			auto test = itr->second.rbegin();
			auto changeLogValue = test->second;
		}*/

}

void runTests(AbstractGenome* genome) {
	//initialize genome to 8 sites
	genome->resize(8);
	auto readHead = genome->data();
	for (size_t i(0); i < genome->size(); ++i)
		readHead[i] = (std::byte)(i + 1);
	readHead[7] = (std::byte)(0b10000001); // 129, or -127

	/* this test demonstrates no time is spent copying into num if we use references.
	   * Note: genomeRead<>() is defined in AbstractGenome.h
	   */
	{
		std::byte& num = GN::genomeRead<std::byte>(genome, 0);
		assert((int)num == 1); // same as above
		genome->data()[0] = (std::byte)3;
		assert((int)num == 3);
		num = (std::byte)1; // reset it to 1
	}

	int num;

	// 1-byte read
	num = (int)GN::genomeRead<std::byte>(genome, 0);
	assert(num == 0b00000001);
	assert(num == 1); // same as above

	// 1-byte read (with named parameters for readability)
	num = (int)GN::genomeRead<std::byte>({ .genome = genome, .index = 0 });
	assert(num == 0b00000001);
	assert(num == 1); // same as above

	// 2-byte read
	num = GN::genomeRead<uint16_t>(genome, 0);
	assert(num == 0b00000010'00000001);
	assert(num == 513); // same as above

	// 4-byte read
	num = GN::genomeRead<uint32_t>(genome, 0);
	assert(num == 0b00000100'00000011'00000010'00000001);
	assert(num == 67'305'985); // same as above

	// 1-double read
	double& dbl = GN::genomeRead<double>(genome, 0);
	uint64_t bitanswer = 0b10000001'00000111'00000110'00000101'00000100'00000011'00000010'00000001; // roughly, = -1.04917e-303
	assert(dbl == reinterpret_cast<double*>(&bitanswer)[0]);

	// 2's complement
	// left-most bit interpretation:
	// unsigned vs signed
	num = GN::genomeRead<uint8_t>(genome, 7); // range [0,255] [00000000, 11111111]
	assert(num == 0b10000001);
	assert(num == 129); // same as above
	num = GN::genomeRead<int8_t>(genome, 7); // range [-128,127] [10000000,01111111]
	assert(num == static_cast<int8_t>(0b10000001));
	assert(num == -127); // same as above

	// byte-array read with no copy
	{
		std::array<uint8_t, 4>& arr = GN::genomeRead<std::array<uint8_t, 4>>(genome, 0);
		for (size_t i(0); i < arr.size(); i++)
			assert(arr[i] == i + 1);
	}

	// int32-array read with no copy
	{
		std::array<unsigned int, 2>& arr = GN::genomeRead<std::array<unsigned int, 2>>(genome, 0);
		assert(arr[0] == 67'305'985);
		assert(arr[1] == 2'164'721'157);
	}

	std::cout << "all read tests passed" << std::endl;

	// write single byte
	GN::genomeWrite<uint8_t>(genome, 0, 255);
	assert(GN::genomeRead<uint8_t>(genome, 0) == 255);

	// write several bytes
	for (size_t i = 0; i < 4; i++) { // rewrite first 4 bytes with (2,4,6,8)
		GN::genomeWrite<uint8_t>({ .genome = genome, .index = i, .value = uint8_t((i + 1) * 2) });
		assert(GN::genomeRead<uint8_t>(genome, i) == (i + 1) * 2);
	}

	// write array of integers (and let compiler guess the type!)
	{
		std::array<uint32_t, 2> arr = { 42,213 };
		GN::genomeWrite(genome, 0, arr);
		auto& result = GN::genomeRead<std::array<uint32_t, 2>>(genome, 0);
		assert(result[0] == 42);
		assert(result[1] == 213);
	}

	std::cout << "all write tests passed" << std::endl;
}

void runGeneTest(AbstractGenome* genome) {

	std::cout << "\n\nrunGeneTest...\nThis shows an example of loading\na struct into the genome and\nhow memory is affected.\n" << std::endl;

	// define payload and GeneDefinition for deterministic gate
	struct MyGenePayload {
		int num_in, num_out;
		std::array<uint8_t, 7> addresses;
		std::bitset<17> logic; // will use 4 bytes, bitset uses 4 bytes at a time
		double answer;
	};

	AbstractGenome::GeneDefinition MyGeneDef = { "MyGene",{(std::byte)42,(std::byte)212},sizeof(MyGenePayload) };
	//GeneDefinition name, start codon, size of gene

	std::cout << "gene contains:\n  ints  'num_in', 'num_out'\n  array<uint8_t, 7> 'addresses'\n  bitset<17> 'logic'\n  double 'answer'";
	
	// write a MyGenomePayload into the genome at index 0
    MyGenePayload gene_instance;
    clean(gene_instance); // zero out memory
    gene_instance.num_in = 255+10;
    gene_instance.num_out = 20;
    gene_instance.addresses = {30,31,32,33,34,35,36};
    gene_instance.logic = 0b111100101100101101;
    gene_instance.answer = 42.42;
    GN::genomeWrite(genome, 0, gene_instance);
	// note that the bitstring will print in reverse order below (least signifigent diget first)

	// set up genes set in genome
	// the list can contain more then one GeneDefinition
	// key is a value used to get these genes
	int key = genome->initGeneSet({ MyGeneDef });

	// get the list of start positions for the genes
	auto startPos = genome->getGenePositions(key);

	// read a MyGene from start of first deterministic gate
	const auto& MyGene_data = GN::genomeRead<MyGenePayload>(genome, 0);

	// print sizes
	std::cout << "Det MyGene_data sizeof() " << sizeof(MyGene_data) << std::endl;
	std::cout << "Det MyGene_data.num_in sizeof() " << sizeof(MyGene_data.num_in) << std::endl;
	std::cout << "Det MyGene_data.num_out sizeof() " << sizeof(MyGene_data.num_out) << std::endl;
	std::cout << "Det MyGene_data.addresses sizeof() " << sizeof(MyGene_data.addresses) << std::endl;
	std::cout << "Det MyGene_data.logic sizeof() " << sizeof(MyGene_data.logic) << std::endl;
	std::cout << "Det MyGene_data.answer sizeof() " << sizeof(MyGene_data.answer) << std::endl;

	std::cout << "Det Gate:" << std::endl;
	std::cout << "  num_in:  " << MyGene_data.num_in << std::endl;
	std::cout << "  num_out: " << MyGene_data.num_out << std::endl;
	std::cout << "  addresses: ";
	for (auto& v : MyGene_data.addresses) {
		std::cout << " " << int(v);
	}
	std::cout << "\n  logic: ";
	for (size_t v(0); v < MyGene_data.logic.size(); v++) {
		std::cout << " " << MyGene_data.logic[v];
	}
	std::cout << std::endl;
	std::cout << "  val: " << MyGene_data.answer << std::endl;

	// iterate over MyGene_data size locations in genome
	auto head = genome->data();
	std::cout << "genome byte values for MyGene:" << std::endl;
	for (int i(0); i < sizeof(MyGenePayload); i++) {
		std::cout << " " << i << ":" << (int)head[i] << std::endl;
	}
	std::cout << std::endl;
	std::cout << "0-3 = num_in (int), 4-7 = num_out (int),\n"
		"8-14 = addresses (vect of uint8_t),"
		"\n15 is skipped so bitstring can start on word,\n"
		"16-23 logic (bitset), 24-31 anwser (double)\n" << std::endl;
}

int main() {
	AbstractGenome* genome = new StephanieGenome(100);

	stephanie_runChangeLogStruct(genome);

	delete genome;

	{
		//AbstractGenome  & y = *(new StephanieGenome);
		//AbstractGenome* secondGenome = new StephanieGenome(200);
		//runGeneTest(secondGenome);
		//delete secondGenome;
	}
	return(0);
}
