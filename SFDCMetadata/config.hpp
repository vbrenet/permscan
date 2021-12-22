//
//  config.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 22/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef config_hpp
#define config_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <vector>


class config {

private:
    enum class token  { USERNAME,
                        PASSWORD,
                        ISPROD,
                        APIVERSION,
                        SECURITYTOKEN,
                        UNKNOWN};
    struct tokenDesc {
        token theToken;
        std::string literal;
    };
    static const std::vector<tokenDesc> tokenDescriptions;
    static std::string username;
    static std::string password;
    static std::string securitytoken;
    static std::string apiversion;
    static bool isASandbox;
    //
    static bool getIsSandbox(const std::string&);
    static void getUseFileForAttributeList(const std::string&);
    static void updateExcludedAttributes(const std::string&);
    static token getTokenValue(const std::string&, std::string&);
    static void processLine(const std::string&);
    
public:
    
    static bool getConfig(const std::string filename);
    static void getAttributeList(const std::string dirname, const std::string object, std::vector<std::string>&);
    //
    //  config accessors
    //
    static std::string& getUsername() {return username;};
    static std::string& getPassword() {return password;};
    static std::string& getSecurityToken() {return securitytoken;};
    static std::string& getApiVersion() {return apiversion;};

    static bool isSandbox() {return isASandbox;};
    //
    static bool checkConfig();
};
#endif /* config_hpp */
