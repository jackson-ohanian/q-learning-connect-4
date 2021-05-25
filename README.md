# q-learning-connect-4 #
Q RL applied to the connect 4 game. 

## Usage ## 

  Run driver.cpp with c++11 or higher. The AI#.txt files hold sample training data of that filter size. 
  
## About the Training ##

  Double Q Learning, two AIs compete for given epochs, focused on training one and using the other to influence that training. 

  A convulation-like filter is put over the board (stride=1, padding=0) the larger the size the more computational power and memory is needed.. to do a full 7x6 
  board is pretty signifigant (some billions of boards from quick math), anything under 4 is borderline useless. The downside here is he will miss blocks sometimes
  chasing a high reward play on a different side of the board. More info in header documentation.
  
  Training is epsilon greedy to encourage exploratory action. High decay rate to give the AI more freedom to make multi-turn plays. 
  
  While focused on training the dominant AI (red), I have been increasing the chance of non-greedy action in the second AI to ensure some different plays come up 
  through randomness. 
  
  Meaningful results appear quickly, the AI becomes mostly competent around a million epochs. 
  
## Loading Training Data ##

  Command line arguments enable loading a file with Q table information. Giving a filename arg automatically loads from and saves to that file. If the convulation
  size is changed, the board hashes will become useless, and the new save will overwrite with a mix of sized hashes and rewards, making the save file useless.

## Human Match ## 

  A board allows human input against the trained AI. Numeric 1-7 to drop a piece. 
