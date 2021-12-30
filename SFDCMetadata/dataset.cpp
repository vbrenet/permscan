//
//  dataset.cpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 30/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#include "dataset.hpp"
#include "SalesforceSession.hpp"
#include "config.hpp"
#include <sstream>
#include <istream>
#include <fstream>
#include <curl/curl.h>
#include <iostream>
#include <cstring>

bool dataset::firstTime = true;
std::string dataset::body {};
size_t dataset::sizeleft;
int dataset::currindex;
//
//
//
static std::string base64_encode(const std::string &in) {

    std::string out;

    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
            valb -= 6;
        }
    }
    if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
    while (out.size()%4) out.push_back('=');
    return out;
}

extern size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  bulkInject::read_callback
//      callback used by libcurl to fill bodies
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t dataset::read_callback(void *dest, size_t size, size_t nmemb, void *userp)
{
    char *thedest = (char *)dest;
    if (dataset::firstTime) {
        dataset::firstTime = false;
        for (auto i=0; i < dataset::body.size(); i++)
            thedest[i] = dataset::body[i];
        return dataset::body.size();
    }
    return 0;
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  dataset::read_callback_inject
//      callback used by libcurl to fill bodies
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t dataset::read_callback_inject(void *dest, size_t size, size_t nmemb, void *userp)
{
    char *thedest = (char *)dest;
    size_t maxtocopy = size*nmemb;
    
    if (sizeleft > 0) {
        size_t sizeToCopy = sizeleft;
        if (sizeToCopy > maxtocopy)
            sizeToCopy = maxtocopy;
        for (auto j=0; j < sizeToCopy; j++) {
            thedest[j] = body[currindex];
            currindex++;
        }
        sizeleft -= sizeToCopy;
        return sizeToCopy;
    }
    
    return 0;
}
//
//
std::string dataset::extractId(const std::string buffer) {
    // {"id":"06V1X000000UxZZUA0","success":true,"errors":[]}

size_t beginindex = buffer.find("id") + 5;
size_t endindex = buffer.find('"', beginindex);
        
return buffer.substr(beginindex,endindex-beginindex);
}
//
//
//
bool dataset::exportDataset(datasetOperation op) {
    
    std::ifstream ifs(intputFileName);
    
    if (!ifs) {
        std::cerr << "Warning : " << intputFileName << " does not exist or is not accessible" << std::endl;
        return true;    // not considered as a error
    }

    if (createHeader(op)) {
        if (putData()) {
            return doExport();
        }
    }
    
    return false;
}
//
//
//
bool dataset::createHeader(datasetOperation op)  {
            
    std::string operation;
    if (op == datasetOperation::OVERWRITE)
        operation = "Overwrite";
    else if (op == datasetOperation::APPEND)
        operation = "Append";
    else
        operation = "Upsert";
    
    if (globals::verbose) {
        std::cout << "Creating " << datasetName << " from " << intputFileName << " operation : " << operation << std::endl;
        std::cout << "Analytics App : " << appName  << " dataset API name : " << datasetName << " dataset API label : " << datasetLabel << std::endl;
    }
    
    std::stringstream headerJson;
    
    headerJson
    << "{"
    << "\"format\" : \"Csv\"" << ","
    << "\"EdgemartAlias\" : \"" << datasetName << "\"" << ","
    << "\"EdgemartContainer\" : \"" << appName << "\"" << ","
    << "\"EdgemartLabel\" : \"" << datasetLabel << "\"" << ","
    << "\"Operation\" : \"" << operation << "\"" << ",";
    
    if (json.size() > 0)
        headerJson << "\"MetadataJson\" : \"" << base64_encode(json) << "\",";
    
    headerJson
    << "\"Action\" : \"None\""
    << "}"
    ;
    
    if (globals::veryverbose) {
        std::cout << "header json : " << headerJson.str() << std::endl;
        std::cout << "json descriptor : " << json << std::endl;
    }
    
    body = headerJson.str();
    dataset::firstTime = true;
    
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    
    curl = curl_easy_init();
    
    if (curl) {
        
        curl_easy_setopt(curl, CURLOPT_URL, ("https://" + SalesforceSession::getDomain() + "/services/data/v" + config::getApiVersion() + "/sobjects/InsightsExternalData").c_str());
        
        if (globals::veryverbose)
            std::cout << "dataset::createHeader POST URL: " << "https://" + SalesforceSession::getDomain() + "/services/data/v" + config::getApiVersion() + "/sobjects/InsightsExternalData" << std::endl;

        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        // set header
        struct curl_slist *list = NULL;
        list = curl_slist_append(list, "Content-Type: application/json; charset=UTF-8");
        list = curl_slist_append(list, "Accept: application/json");
        list = curl_slist_append(list, ("Authorization: Bearer " + SalesforceSession::getConnectedAppToken()).c_str());

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        
        /* Now specify we want to POST data */
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        
        /* we want to use our own read function */
        firstTime = true;
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        
        /* pointer to pass to our read function */
        curl_easy_setopt(curl, CURLOPT_READDATA, headerJson.str().c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(headerJson.str().c_str()));
        
        res = curl_easy_perform(curl);
        curl_slist_free_all(list); /* free the list  */
        
        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code >= 400) {
            std::cerr << "dataset::createHeader : http error: " << http_code << std::endl;
            std::cout << "dataset::createHeader : received buffer: " << readBuffer << std::endl;
            return false;
        }
        
        curl_easy_cleanup(curl);
    }
    else {
        std::cerr << "dataset::createHeader : curl_easy_init failure" << std::endl;
        return false;
    }
    
    if (res != CURLE_OK) {
        std::cerr << "dataset::createHeader : curl_easy_perform error: " << curl_easy_strerror(res) << std::endl;
        return false;
    }
    
    if (globals::verbose)
        std::cout << "dataset::createHeader : received buffer: " << readBuffer << std::endl;
    
// extraire id objet créé
    InsightsExternalDataId = extractId(readBuffer);
    
    return true;
}
//
//
//
bool dataset::putData() const {
    std::ifstream ifs(intputFileName);

    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                           (std::istreambuf_iterator<char>()    ) );
    
    // convert to Base64
    std::string base64EncodedData;
    
    //std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
    base64EncodedData = base64_encode(content);

    if (globals::veryverbose) {
        std::cout << "dataset::putData encoded file length : " << base64EncodedData.size() << std::endl;
    }
    
    std::stringstream InsightsExternalDataPartBody;
    
    InsightsExternalDataPartBody
    << "{"
    << "\"DataLength\" : \"" << base64EncodedData.size() << "\"" << ","
    << "\"InsightsExternalDataId\" : \"" << InsightsExternalDataId << "\"" << ","
    << "\"PartNumber\" : \"1\"" << ","
    << "\"DataFile\" : \"" << base64EncodedData << "\""
    << "}"
    ;
    
    body = InsightsExternalDataPartBody.str();
    dataset::firstTime = true;
    firstTime = true;
    sizeleft = body.size();
    currindex = 0;

    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    
    curl = curl_easy_init();
    
    if (curl) {
        
        curl_easy_setopt(curl, CURLOPT_URL, ("https://" + SalesforceSession::getDomain() + "/services/data/v" + config::getApiVersion() + "/sobjects/InsightsExternalDataPart").c_str());
        
        if (globals::veryverbose)
            std::cout << "dataset::putData POST URL: " << "https://" + SalesforceSession::getDomain() + "/services/data/v" + config::getApiVersion() + "/sobjects/InsightsExternalDataPart" << std::endl;

        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt (curl, CURLOPT_FAILONERROR, 1L);
        
        // set header
        struct curl_slist *list = NULL;
        list = curl_slist_append(list, "Content-Type: application/json; charset=UTF-8");
        list = curl_slist_append(list, "Accept: application/json");
        list = curl_slist_append(list, "Expect: 100-continue");
        list = curl_slist_append(list, ("Authorization: Bearer " + SalesforceSession::getConnectedAppToken()).c_str());

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        
        /* Now specify we want to POST data */
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        
        /* we want to use our own read function */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback_inject);
        
        /* pointer to pass to our read function */
        curl_easy_setopt(curl, CURLOPT_READDATA, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(body.c_str()));
        
        res = curl_easy_perform(curl);
        curl_slist_free_all(list); /* free the list  */
        
        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code >= 400) {
            std::cerr << "dataset::putData : http error: " << http_code << std::endl;
            std::cerr << "dataset::putData : received buffer: " << readBuffer << std::endl;
            return false;
        }
        
        curl_easy_cleanup(curl);
    }
    else {
        std::cerr << "dataset::putData : curl_easy_init failure" << std::endl;
        return false;
    }
    
    if (res != CURLE_OK) {
        std::cerr << "dataset::putData : curl_easy_perform error: " << curl_easy_strerror(res) << std::endl;
        return false;
    }
    
    if (globals::verbose)
        std::cout << "dataset::putData : received buffer: " << readBuffer << std::endl;
    
    return true;
}
//
//
//
bool dataset::doExport() const {
    
    std::stringstream headerJson;
    
    headerJson
    << "{"
    << "\"Action\" : \"Process\""
    << "}"
    ;
    
    body = headerJson.str();
    dataset::firstTime = true;

    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    
    curl = curl_easy_init();
    
    if (curl) {
        
        curl_easy_setopt(curl, CURLOPT_URL, ("https://" + SalesforceSession::getDomain() + "/services/data/v" + config::getApiVersion() + "/sobjects/InsightsExternalData/" + InsightsExternalDataId).c_str());
        
        if (globals::veryverbose)
            std::cout << "dataset::doExport PATCH URL: " << "https://" + SalesforceSession::getDomain() + "/services/data/v" + config::getApiVersion() + "/sobjects/InsightsExternalData/" + InsightsExternalDataId << std::endl;

        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        // set header
        struct curl_slist *list = NULL;
        list = curl_slist_append(list, "Content-Type: application/json; charset=UTF-8");
        list = curl_slist_append(list, "Accept: application/json");
        list = curl_slist_append(list, ("Authorization: Bearer " + SalesforceSession::getConnectedAppToken()).c_str());

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        
        /* Now specify we want to POST data */
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
        
        /* we want to use our own read function */
        firstTime = true;
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        
        /* pointer to pass to our read function */
        curl_easy_setopt(curl, CURLOPT_READDATA, headerJson.str().c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(headerJson.str().c_str()));
        
        res = curl_easy_perform(curl);
        curl_slist_free_all(list); /* free the list  */
        
        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code >= 400) {
            std::cerr << "dataset::doExport : http error: " << http_code << std::endl;
            std::cout << "dataset::doExport : received buffer: " << readBuffer << std::endl;
            return false;
        }
        
        curl_easy_cleanup(curl);
    }
    else {
        std::cerr << "dataset::doExport : curl_easy_init failure" << std::endl;
        return false;
    }
    
    if (res != CURLE_OK) {
        std::cerr << "dataset::doExport : curl_easy_perform error: " << curl_easy_strerror(res) << std::endl;
        return false;
    }
    
    if (globals::verbose)
        std::cout << "dataset::doExport : received buffer: " << readBuffer << std::endl;
    
    return true;
}
