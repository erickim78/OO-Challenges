/******************************************************************************

This class maintains Security objects and provides methods for their lookup
via Security attributes.

    • Every Security object has a unique id (id_)
    • Every Security object has a unique symbol (symbol_)
    • Every Security object has a (potentially non-unique) channel id (channel_)


    Security GetSecurity(int id) const

        Return Security object with given id or default (invalid) Security object if not found

    Security GetSecurity(const std::string& symbol) const

        Return Security object with given symbol or default (invalid) Security object if not found

    Securities GetSecurities(int channel) const

        Returns all Security objects with given channel value
        or an empity List of Security objects, if not matching securities are found

    void SaveSecurity(const Security& sec)

        Add a Security object to the lookup instance

        If a Security with a given id already exists:
            • Update Security with new channel id

                Before state:
                    Security: 1, "MSFT", 1
                    Security: 2, "TSLA", 1
                    Security: 3, "GOOG", 1

                Mutations:
                    Save Security: 1, "MSFT, 2

                After state:
                    Security: 1, "MSFT", 2
                    Security: 2, "TSLA", 1
                    Security: 3, "GOOG", 1

            • Update Security with new symbol
                
                Before state:
                    Security: 1, "MSFT", 1
                    Security: 2, "TSLA", 1
                    Security: 3, "GOOG", 1

                Mutations:
                    Save Security: 1, "AAPL", 1

                After state:
                    Security: 1, "AAPL", 1
                    Security: 2, "TSLA", 1
                    Security: 3, "GOOG", 1

            • Throw std::runtime_error if symbol is being used by an another Security

                Before state:
                    Security: 1, "MSFT", 1
                    Security: 2, "TSLA", 1
                    Security: 3, "GOOG", 1

                Mutations:
                    Save Security 1, "GOOG", 1
                    <<< Exception! >>>

                After state:
                    Security: 1, "MSFT", 1
                    Security: 2, "TSLA", 1
                    Security: 3, "GOOG", 1

            • Do not throw std::runtime_error if symbol is in use by the same Security (NOOP)

                Before state:
                    Security: 1, "MSFT", 1
                    Security: 2, "TSLA", 1
                    Security: 3, "GOOG", 1

                Mutations:
                    Save Security 1, "MSFT", 1

                After state:
                    Security: 1, "MSFT", 1
                    Security: 2, "TSLA", 1
                    Security: 3, "GOOG", 1

        If a Security with a given id does not exist:
            • Save new Security

                Before state:
                    Security: 1, "MSFT", 1

                Mutations:
                    Save Security: 2, "TSLA", 2

                After state:
                    Security: 1, "MSFT", 1
                    Security: 2, "TSLA", 2

******************************************************************************/

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

//Testing
#include <iostream>

struct Security
{
    uint32_t id_;
    std::string symbol_;
    uint16_t channel_;

    Security()
        : id_{ 0 }
        , channel_{ 0 }
    {}

    Security(uint32_t id, std::string symbol, uint16_t channelId)
        : id_(id)
        , symbol_(symbol)
        , channel_(channelId)
    {}

    bool IsValid() const { return id_ != 0; }
};

using Securities = std::vector<Security>;

class SecurityLookup
{
public:
    Security GetSecurity(int id) const
    {
        // TODO

        //If key 'id' exists, return the associated security
        if(securitytable.count(id) > 0) 
            return securitytable.at(id);

        return Security();
    }

    Security GetSecurity(const std::string& symbol) const
    {
        // TODO

        //Iterate through map, checking each security if it matches symbol
        for( auto i = securitytable.begin(); i != securitytable.end(); i++)
        {
            if( (i->second).symbol_ == symbol)
                return i->second;
        }

        return Security();
    }

    Securities GetSecurities(int channel) const
    {
        // TODO

        //If channel has at least one Security, return that vector
        Securities answer;
        for( auto i = securitytable.begin(); i != securitytable.end(); i++)
        {
            if( (i->second).channel_ == channel)
                answer.push_back( i->second );
        }

        return answer;
    }

    void SaveSecurity(const Security& sec)
    {
        // TODO

        //If key already exists
        try {
            if(securitytable.count( sec.id_ ) > 0)
            {
                if( symboltable.count( sec.symbol_) > 0 && symboltable.at( sec.symbol_).id_ != sec.id_ )
                    throw std::runtime_error("Exception!");     //Throws exception if symbol exists, and is not being used by same security 
                else    //otherwise, update the data with new security
                {
                    securitytable[ sec.id_ ] = sec;
                    symboltable[ sec.symbol_ ] = sec;
                }
                
            }
            else    //If key doesnt exists yet
            {
                securitytable[ sec.id_ ] = sec;
                symboltable[ sec.symbol_ ] = sec;
            }
        }
        catch( std::runtime_error ) {
            std::cout << "<<< Exception! >>>" << std::endl;
        }

        return;
    }

    //Helper Functions for Testing
    void printSecurity( Security input ) 
    {
        std::cout << input.id_ << ", " << input.symbol_ << ", " << input.channel_ << std::endl;
        return;
    }

    void printSecurities( Securities input)
    {
        for( int i = 0; i < input.size(); i++ )
            std::cout << input[i].id_ << ", " << input[i].symbol_ << ", " << input[i].channel_ << std::endl;
    }

private:
    // TODO add necessary data structures

    //Unordered map hashed by id_
    std::unordered_map<int, Security> securitytable;

    //Hashed by symbol_
    std::unordered_map<std::string, Security> symboltable;
    
};
    
int main ()
{
    SecurityLookup lookup;
}