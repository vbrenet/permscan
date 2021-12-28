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
    enum class token  {
                        TOKENDOM,
                        CLIENTID,
                        CLIENTSECRET,
                        USERNAME,
                        PASSWORD,
                        ISPROD,
                        APIVERSION,
                        SECURITYTOKEN,
                        DEFAULT,
                        PSLICENSE,
                        UNKNOWN};
    struct tokenDesc {
        token theToken;
        std::string literal;
    };
    static const std::vector<tokenDesc> tokenDescriptions;
    static std::string clientid;        // REST connected app
    static std::string clientsecret;    // REST connected app secret
    static std::string domain;
    static std::string username;
    static std::string password;
    static std::string securitytoken;
    static std::string apiversion;
    static bool isASandbox;
    static int defaultAuthorizedObjectNumber;
    static std::map<std::string,int> pslicenseauthorizationsmap;
    //
    static bool getIsSandbox(const std::string&);
    static void getUseFileForAttributeList(const std::string&);
    static void updateExcludedAttributes(const std::string&);
    static token getTokenValue(const std::string&, std::string&);
    static void processLine(const std::string&);
    static void addpslicensevalue(const std::string&);
    
public:
    
    static bool getConfig(const std::string filename);
    static void getAttributeList(const std::string dirname, const std::string object, std::vector<std::string>&);
    //
    //  config accessors
    //
    static const std::string& getClientId() {return clientid;};
    static const std::string& getClientSecret() {return clientsecret;};
    static const std::string& getDomain() {return domain;};
    static const std::string& getUsername() {return username;};
    static const std::string& getPassword() {return password;};
    static const std::string& getSecurityToken() {return securitytoken;};
    static const std::string& getApiVersion() {return apiversion;};
    static int getDefaultAuthorizedObjectNumber() {return defaultAuthorizedObjectNumber;};
    static const std::map<std::string,int>& getpslicensemap() {return pslicenseauthorizationsmap;};

    static bool isSandbox() {return isASandbox;};
    //
    static bool checkConfig();
};
#endif /* config_hpp */
