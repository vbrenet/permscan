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
#include "contractRule.hpp"


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
                        LICENSE,
                        DSDOMAIN,       // Dataset org domain
                        DSCLIENTID,     // Dataset connected app client id
                        DSCLIENTSECRET, // Dataset connected app client secret
                        DSUSERNAME,     // Dataset org username
                        DSPASSWORD,     // Dataset org user password
                        DSSECURITYTOKEN,// Dataset org user security token
                        DSISPROD,       // Dataset org type
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
    static std::map<std::string,int> licenseauthorizationsmap;
    //
    //
    static contractRule defaultContractRules;
    static std::map<std::string,contractRule> pslicenseContractRules;

    //  dataset org access parameters
    //
    static std::string dsclientid;
    static std::string dsclientsecret;
    static std::string dsdomain;
    static std::string dsusername;
    static std::string dspassword;
    static std::string dssecuritytoken;
    static bool dsisASandbox;
    //
    //
    static bool getIsSandbox(const std::string&);
    static void getUseFileForAttributeList(const std::string&);
    static void updateExcludedAttributes(const std::string&);
    static token getTokenValue(const std::string&, std::string&);
    static void processLine(const std::string&);
    static void addpslicensevalue(const std::string&);
    static void addlicensevalue(const std::string&);

public:
    
    static bool getConfig(const std::string filename);
    static bool getContractRules(const std::string filename);
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
    static const std::map<std::string,int>& getlicensemap() {return licenseauthorizationsmap;};

    static bool isSandbox() {return isASandbox;};
    //
    //  dataset accessors
    //
    //
    static std::string& getDsClientId() {return dsclientid;};
    static std::string& getDsClientSecret() {return dsclientsecret;};
    static std::string& getDsDomain() {return dsdomain;};
    static std::string& getDsUsername() {return dsusername;};
    static std::string& getDsPassword() {return dspassword;};
    static std::string& getDsSecurityToken() {return dssecuritytoken;};
    static bool getDsIsASandbox() {return dsisASandbox;};

    static bool checkConfig();
};
#endif /* config_hpp */
