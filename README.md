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


## Running
- copy the `shibabot` executable to a folder of your liking or leave it in `build/`  
- make a file called `config.json` in the same directory as `shibabot`  
- modify and put the content below inside the file:  
```json
{
    "prefix": "shiba",
    "chanlist": "enabledchans.json",
    "userlist": "usermgr.json",
    "miscConfigs": {
        "discordToken": "<your discord token here>"
    }
}
```
Note: the prefix works in the following way:  
`prefix [command] [arguments]`

## TO-DO
- automatic saving of necessary files after changes
- more frontends (such as IRC)
- better permission system
  - tree permissions