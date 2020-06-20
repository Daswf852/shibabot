#pragma once

#include <string>

namespace Shiba{

    //An abstract class that contains information about a user
    class User {
        public:
            virtual ~User() = default;

            //Returns a human readable username which tries not to ping the respective user.
            //IRC: daswf852 (pings)
            //Discord: daswf852 (doesn't ping)
            virtual std::string GetUsername() const = 0;

            //Returns a string which tries to ping a user. Might or might not be readable.
            //IRC: daswf852 (pings, readable)
            //Discord: <@254628026380320768> (pings, not readable by itself)
            virtual std::string GetPingableUsername() const = 0;

            //Returns a string which uniquely identifies a user
            //IRC: IRC_freenode_daswf852
            //Discord: Discord_254628026380320768
            virtual std::string GetIdentifier() const = 0;

            //Returns wheter or not a user is a bot
            //Might not always be accurate or possible (to get)
            virtual bool GetBot() const = 0;
    };  

}