//
//  libpreferences.h
//  libpreferences
//
//  Created by Ethan Laur on 7/7/13.
//  Copyright (c) 2013 Ethan Laur. All rights reserved.
//

//#import <Foundation/Foundation.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

class preferences
{
private:
    FILE * prefdb;
    char path[70];
    bool checkIfKeyExists(const char * key);
    preferences *appcache;
public:
    preferences(const char * appname);
    char * getPreferenceByKey(const char * key);
    void writePreferenceByKey(const char * key, char * data);
    void close();
};

