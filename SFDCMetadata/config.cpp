//
//  config.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 22/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#include "config.hpp"
#include <iostream>
#include <fstream>
//
//
std::string config::username;
std::string config::password;
std::string config::securitytoken {};
std::string config::clientid;        // REST connected app
std::string config::clientsecret;    // REST connected app secret
std::string config::domain;
std::string config::apiversion;
int config::defaultAuthorizedObjectNumber {0};
std::map<std::string,int> config::pslicenseauthorizationsmap;
std::map<std::string,int> config::licenseauthorizationsmap;

bool config::isASandbox {false};
bool config::dsisASandbox {false};

std::string config::dsclientid;        // REST connected app
std::string config::dsclientsecret;    // REST connected app secret
std::string config::dsdomain;
std::string config::dsusername;
std::string config::dspassword;
std::string config::dssecuritytoken {};

const std::vector<config::tokenDesc> config::tokenDescriptions = {
    {config::token::TOKENDOM, "_domain_:"},
    {config::token::CLIENTID, "_clientid_:"},
    {config::token::CLIENTSECRET, "_clientsecret_:"},
    {config::token::USERNAME, "_username_:"},
    {config::token::PASSWORD, "_password_:"},
    {config::token::SECURITYTOKEN, "_token_:"},
    {config::token::ISPROD, "_isprod_:"},
    {config::token::APIVERSION, "_apiversion_:"},
    {config::token::DEFAULT, "_default_:"},
    {config::token::DSDOMAIN, "_datasetdomain_:"},
    {config::token::DSCLIENTID, "_datasetclientid_:"},
    {config::token::DSCLIENTSECRET, "_datasetclientsecret_:"},
    {config::token::DSUSERNAME, "_datasetusername_:"},
    {config::token::DSPASSWORD, "_datasetpassword_:"},
    {config::token::DSSECURITYTOKEN, "_datasetsecuritytoken_:"},
    {config::token::DSISPROD, "_datasetisprod_:"},
    {config::token::PSLICENSE, "_pslicense_:"},
    {config::token::LICENSE, "_license_:"}
};
//
//
//
void config::addlicensevalue(const std::string & token) {
    size_t firstColon = token.find_first_of(':');
    if (firstColon != std::string::npos) {
        std::string licenselabel = token.substr(0,firstColon);
        licenseauthorizationsmap.insert(std::pair<std::string,int>(licenselabel, std::stoi(token.substr(firstColon+1))));
    }
}
//
//
//
void config::addpslicensevalue(const std::string & token) {
    //_pslicense_:Field Service Dispatcher:120
    size_t firstColon = token.find_first_of(':');
    if (firstColon != std::string::npos) {
        std::string licenselabel = token.substr(0,firstColon);
        pslicenseauthorizationsmap.insert(std::pair<std::string,int>(licenselabel, std::stoi(token.substr(firstColon+1))));
    }
}
//
//
//
bool config::getIsSandbox(const std::string& line) {
    size_t firstColon = line.find_first_of(':');
    if (firstColon != std::string::npos) {
        std::string value = line.substr(firstColon+1);
        if (value.compare("true") == 0)
            return false;
        else if (value.compare("false") == 0)
            return true;
    }
    return true;
}
//
//
//
config::token config::getTokenValue(const std::string& line, std::string& value) {
    for (auto it = tokenDescriptions.begin(); it != tokenDescriptions.end(); it++) {
        if (line.find(it->literal) != std::string::npos) {
            value = line.substr(it->literal.size());
            return it->theToken;
        }
    }
    return token::UNKNOWN;
}

//
void config::processLine(const std::string& line) {
    if (line[0] == '#') return; // skip comments
    std::string value;
    token theToken = getTokenValue(line, value);
    switch (theToken) {
        case token::TOKENDOM:
            domain = value;
            break;
        case token::CLIENTID:
            clientid = value;
            break;
        case token::CLIENTSECRET:
            clientsecret = value;
            break;
        case token::USERNAME:
            username = value;
            break;
        case token::PASSWORD:
            password = value;
            break;
        case token::APIVERSION:
            apiversion = value;
            break;
        case token::SECURITYTOKEN:
            securitytoken = value;
            break;
        case token::ISPROD:
            isASandbox = getIsSandbox(line);
            break;
        case token::DEFAULT:
            defaultAuthorizedObjectNumber = std::stoi(value);
            break;
        case token::PSLICENSE:
            addpslicensevalue(value);
            break;
        case token::LICENSE:
            addlicensevalue(value);
            break;
        case token::DSDOMAIN:
            dsdomain = value;
            break;
        case token::DSCLIENTID:
            dsclientid = value;
            break;
        case token::DSCLIENTSECRET:
            dsclientsecret = value;
            break;
        case token::DSUSERNAME:
            dsusername = value;
            break;
        case token::DSPASSWORD:
            dspassword = value;
            break;
        case token::DSSECURITYTOKEN:
            dssecuritytoken = value;
            break;
        case token::DSISPROD:
            dsisASandbox = getIsSandbox(line);
            break;
        default:
            break;
    }
}
//
bool config::getConfig(const std::string filename) {
    std::ifstream configFile {filename};
    std::string currentLine;
    
    int nbline {0};
    
    while (getline(configFile,currentLine)) {
        nbline++;
        processLine(currentLine);
    }
    
    return (nbline > 0);
}
//
//
//
bool config::checkConfig() {

    bool status {true};
    
    if (username == "") {
        std::cerr << "config file: username parameter is mandatory" << std::endl;
        status = false;
     }
    if (password == "") {
        std::cerr << "config file: password parameter is mandatory" << std::endl;
        status = false;
     }
    if (apiversion == "") {
        std::cerr << "config file: apiversion parameter is mandatory" << std::endl;
        status = false;
     }

    return status;
}
