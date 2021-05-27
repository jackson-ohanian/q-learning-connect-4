# q-learning-connect-4 #
Q RL applied to the connect 4 game. 

## Usage ## 

  Run driver.cpp with c++17 minimum required. The AI#.txt files hold sample training data of that filter size. 
  
## About the Training ##

  Double Q Learning, two AIs compete for given epochs, focused on training one and using the other to influence that training. 

  A convulation-like filter is put over the board (stride=1, padding=0) the larger the size the more computational power and memory is needed.. to do a full 7x6 
  board is pretty signifigant (some trillions of boards from quick math), anything under 4 is borderline useless. The downside here is he will miss blocks sometimes
  chasing a high reward play on a different side of the board. More info in header documentation.
  
  Even a 4x4 filter, which is of limited use, has over 20,000,000 possible states, a lot of which can be removed by considering cases such as a
  completely full board as non-unique. For filter size n this reduces the amount of state vectors needed by 2^(2n). Overall, the computation and storage needed to
  make this learner very smart are high - I don't think this task is necesarily better suited to deep Q Learning, however, as precise choices are needed, not
  descisions. 
  
  Training is epsilon greedy to encourage exploratory action. High decay rate to give the AI more freedom to make multi-turn plays. 
  
  While focused on training the dominant AI (red), I have been increasing the chance of non-greedy action in the second AI to ensure some different plays come up 
  through randomness. 
  
  Meaningful results appear quickly, the AI becomes mostly competent around a million epochs. Untrained (random) models compete with a win rate around 55% for red,
  around a hundred thousand epochs this approaches 65%, the tie rate also rises signifigantly. 
  
## Loading Training Data ##

  Command line arguments enable loading a file with Q table information. Giving a filename arg automatically loads from and saves to that file. If the convulation
  size is changed, the board hashes will become useless, and the new save will overwrite with a mix of sized hashes and rewards, making the save file useless.

## Human Match ## 

  A board allows human input against the trained AI. Numeric 1-7 to drop a piece. 
