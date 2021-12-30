//
//  dataset.hpp
//  SFDCMetadata
//
//  Created by Vincent Brenet on 30/12/2021.
//  Copyright © 2021 Vincent Brenet. All rights reserved.
//

#ifndef dataset_hpp
#define dataset_hpp

#include <stdio.h>
#include <string>
#include "globals.hpp"

class dataset {
private:
    static bool firstTime;      // used by the callback providing data to libcurl
    static std::string body;    // buffer containing POST request bodies
    static size_t read_callback(void *dest, size_t size, size_t nmemb, void *userp); // callback called by libcurl to send data in POST requests
    static size_t read_callback_inject(void *dest, size_t size, size_t nmemb, void *userp); // callback called by libcurl to send data in POST requests
    static size_t sizeleft;
    static int currindex;

    std::string appName;
    std::string datasetName;
    std::string datasetLabel;
    std::string intputFileName;
    std::string json;
    
    std::string InsightsExternalDataId;
    
    bool createHeader(datasetOperation op);
    bool putData() const;
    bool doExport() const;
    
    std::string extractId(const std::string);

public:
    dataset() {};
    dataset(const std::string& app, const std::string& name, const std::string& label, const std::string& inputfile) : appName {app}, datasetName {name}, datasetLabel {label}, intputFileName {inputfile} {}
    dataset(const std::string& app, const std::string& name, const std::string& label, const std::string& inputfile, const std::string& thejson) : appName {app}, datasetName {name}, datasetLabel {label}, intputFileName {inputfile}, json{thejson} {}

    bool exportDataset(datasetOperation op) ;
};
#endif /* dataset_hpp */
