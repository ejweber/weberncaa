#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "classes.hpp"

int Tree::leaves = 0;

Node::Node(int seed, Node* rc, Node* lc, int level, std::string team)
	: team(team)
	, seed(seed)
	, rc(rc)
	, lc(lc)
	, level(level)
	{/* left intentionally blank */}
	
Tree::Tree()
{
    seeds.open("seeds.csv");
	if (seeds.is_open() == false) {
		exit(1);
	}
	getStats();
	unsigned int seed = (unsigned int)std::chrono::system_clock::now()
		.time_since_epoch().count();
	generator.seed(seed);
	//std::cout << seed << std::endl;
	
    // championship is at round 6
    // individual teams are at "round" 0
    root = createNode(6);
}

Node* Tree::createNode(int level)
{
	Node* currentNode = new Node(0, NULL, NULL, level);
    if (level > 0) {
		if (currentNode->getLC() == NULL) {
			currentNode->setLC(createNode(level - 1));
		}
		if (currentNode->getRC() == NULL) {
			currentNode->setRC(createNode(level - 1));
		}
		determineWinner(currentNode);
	}
	else {
		std::string currentTeam;
		std::string currentSeed;
		std::string currentRegion;
		getline(seeds, currentTeam, ',');
		getline(seeds, currentSeed, ',');
		getline(seeds, currentRegion);
		currentNode->setSeed(std::stoi(currentSeed));
        currentNode->setTeam(currentTeam);
		Tree::leaves++;
	}
	return currentNode;
}

void Tree::getStats()
{
	std::ifstream file;
	file.open("stats.csv");
	char trash[10];
	int round;
	int upper;
	int lower;
	int percent;
    
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 16; j++) {
            for (int k= 0; k < 16; k++) {
                if (j == k)
					// always a 50% chance for equal seeds
					stats[i][j][k] = 50;
				else
					stats[i][j][k] = -1;
            }
        }
    }
	
	file >> round;
    file.ignore();
    file >> upper;
    file.ignore();
    file >> lower;
    file.ignore();
    file >> percent;
    file.getline(trash, 100);
    while (file.eof() == false) {
        // stats array indexes start at 0
        stats[round - 1][upper][lower] = percent;
        file >> round;
        file.ignore();
        file >> upper;
        file.ignore();
        file >> lower;
        file.ignore();
        file >> percent;
    }
}

void Tree::determineWinner(Node* currentNode)
{
    Node* temp;
    int percent;
    Node* upper = currentNode->getLC();
    Node* lower = currentNode->getRC();
	Node* winner;
	int success;
    
    if (upper->getSeed() > lower->getSeed()) {
        temp = upper;
        upper = lower;
        lower = temp;
    }
	
    percent = stats[currentNode->getLevel() - 1][upper->getSeed()][lower->getSeed()];
	if (percent == -1) {
		// handle situations for which we have no data
		upper->setPercent(-1);
		lower->setPercent(-1);
		percent = DEFAULT_PROBABILITY;
	}
	else {
		upper->setPercent(percent);
		lower->setPercent(100 - percent);
	}
	
	std::binomial_distribution<int> distribution(1, (double)percent/100);
	//std::cout << "Percent: " << ((double)percent/100) << std::endl;
	success = distribution(generator);
	//std::cout << success;
    
	if (success == 1)
		winner = upper;
	else
		winner = lower;
    currentNode->setSeed(winner->getSeed());
    currentNode->setTeam(winner->getTeam());
}

void Tree::printTree()
{
    for (int i = 1; i < 7; i++) {
        std::cout << "Round " << i << std::endl
                  << "-------" << std::endl;
        printLevel(i, root);
    }
}

void Tree::printLevel(int level, Node* currentNode)
{
    Node* lc = currentNode->getLC();
    Node* rc = currentNode->getRC();
    
    if (level == currentNode->getLevel()) {
        printf("%-2d ", lc->getSeed());
		if (lc->getPercent() == -1)
			std::cout << "??% ";
		else
			std::cout << lc->getPercent() << "% ";
		std::cout << lc->getTeam() << std::endl;
        printf("%-2d ", rc->getSeed());
        if (rc->getPercent() == -1)
			std::cout << "??% ";
		else
			std::cout << rc->getPercent() << "% ";
		std::cout << rc->getTeam() << std::endl;
        printf("%-2d ", currentNode->getSeed());
        std::cout << currentNode->getTeam() << std::endl << std:: endl;
    }
    else {
        printLevel(level, lc);
        printLevel(level, rc);
    }
}