#include <string>
#include <fstream>
#include <random>

// chance the better seed wins if no data is available
#define DEFAULT_PROBABILITY 90

class Node
{
	public:
	Node(int seed, Node* rc, Node* lc, int level, std::string name="Team");
	Node* getLC() {return lc;};
	Node* getRC() {return rc;};
	int getSeed() {return seed;};
    int getLevel() {return level;};
	int getPercent() {return percent;};
    std::string getTeam() {return team;};
	void setLC(Node* newLC) {lc = newLC;};
	void setRC(Node* newRC) {rc = newRC;};
	void setSeed(int newSeed) {seed = newSeed;};
    void setTeam(std::string newTeam) {team = newTeam;};
	void setPercent(int newPercent) {percent = newPercent;};
		
	private:
	std::string team;
	int seed;
	Node* rc;
	Node* lc;
	int level;
	int percent;
};

class Tree
{
	public:
	Tree();
	Node* createNode(int level);
    void printTree();
	
	private:
	Node* root;
	static int leaves;
	std::ifstream seeds;
	int stats[6][16][16];
	std::default_random_engine generator;
	
	void determineWinner(Node* currentNode);
	void getStats();
    void printLevel(int level, Node* currentNode);
};