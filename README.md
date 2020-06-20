# Shiba Bot
A (possibly) multi-platform bot, 'nuff said  
  
## Building
`git clone --recurse-submodules git@github.com:Daswf852/shibabot.git`
`cd submodules`  
`cd sleepy-discord && mkdir build && cd build`  
`cmake .. && make -j`  
`cd .. && cd spdlog && mkdir build && cd build`  
`cmake .. && make -j`  
`cd ../.. && mkdir build && cd build`  
`cmake .. && make -j`  
By default only a discord frontend exists and to run that you don't need to change main.cpp, just put your bot token into a file called `token` and run `./shibabot`  
  
## TO-DO
- automatic saving of necessary files after changes
- more frontends (such as IRC)
- better permission system
  - tree permissions