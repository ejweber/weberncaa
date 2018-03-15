weberncaa v1.0

weberncaa is a small hobby project that attempts to fill out a viable NCAA tournament bracket using a probabilistic calculations based on the win percentage of past tournament seeds in each tournament round.

Input:
weberncaa requires two files as input:
	1)  stats.csv represents historical tournament data with the following schema:
			round (1-6)
			seed of first team (1-16)
			seed of second team (1-16)
			win percentage (0-100)
	2)	seeds.csv represents the current year's tournament teams with the following schema:
			seed (1-16)
			team name (string)
			region (string) (not used by weberncaa)
NOTE: Historical data is only represented for matchups that have occurred at least four times!
			
Output:
weberncaa outputs a list of every tournament game by round. The output for each game shows the seed, win probability, and name of the competing teams as well as the seed and name of the winning team. If historical data is not available for a particular round and seed combination, each team's win probability is represented as "??%".

How It Works:
weberncaa constructs a binary tree representing the NCAA tournament bracket. Each internal node represents a game and its winner. Its children are the two previous games and their winners. Each leaf node represents a single NCAA tournament team. After constructing the tree, weberncaa attempts to determine the winner of each game using the output of a Binomial Distribution with k = 1 trials and p = win probability as given by stats.csv. If no data is available, weberncaa defaults to p = 90%. This behavior can be changed by modifying DEFAULT_PROBABILITY in ncaa_functions.hpp.

How To Build:
weberncaa compiles on Windows with Visual C++ Native Build Tools using the command: "cl /W4 /EHs classes.cpp weberncaa.cpp". It should work with any C++ compiler conforming to C++99 or later.

Issues:
While technically complete (given its stated limitations), the dataset fails to provide probabilities for a great number of games. In the vast majority of complete runs, weberncaa predicts a number of matchups where the specific round and seed combination has not occurred four times in history, causing a default probability to be used.