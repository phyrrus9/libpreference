//
//  libpreferences.cpp
//  libpreferences
//
//  Created by Ethan Laur on 7/7/13.
//  Copyright (c) 2013 Ethan Laur. All rights reserved.
//

#import "libpreferences.h"

preferences::preferences(const char * appname)
{
    //printf("constructor enter\n");
    mkdir("/Library/prefs", 0644); //just in case
    sprintf(path, "/Library/prefs/%s.pref", appname); //change this to fit your OS
    prefdb = fopen(path, "ab");
    appcache = new preferences("imod.app");
    appcache->writePreferenceByKey(appname, path);
    //printf("constructor exit\n");
}

bool preferences::checkIfKeyExists(const char * key) //determine if we need to update
{
    bool found = false;
    char * buffer = (char *)malloc(70); //keys can not be longer than 65 chars
    char read = 0; //value read from the fs
    close();
    prefdb = fopen(path, "r");
    while (read != EOF && !found)
    {
        read = fscanf(prefdb, "%s", buffer);
        if (strcmp(buffer, key) == 0)
        {
            return true;
        }
    }
    return found;
}

char * preferences::getPreferenceByKey(const char * key)
{
    //printf("get enter\n");
    char * buffer = (char *)malloc(70); //keys can not be longer than 65 chars
    char read = 0; //value read from the fs
    int sp = 0; //string pointer
    bool found = false;
    close();
    prefdb = fopen(path, "r");
    while (read != EOF && !found)
    {
        fscanf(prefdb, "%s", buffer);
        if (strcmp(buffer, key) == 0)
        {
            found = true;
            sp = 0;
            fscanf(prefdb, "%c", &read);
            fscanf(prefdb, "%c", &read);
            while (read != ';')
            {
                //printf("Read character: %c\n", read);
                //sleep(1);
                buffer[sp++] = read;
                fscanf(prefdb, "%c", &read);
            }
            fscanf(prefdb, "%c", &read);
            buffer[sp] = 0;
        }
    }
    //printf("get exit\n");
    if (!found)
        return NULL;
    return buffer;
}

void preferences::writePreferenceByKey(const char * key, char * data)
{
    //printf("write enter\n");
    if (!checkIfKeyExists(key))
    {
        close();
        prefdb = fopen(path, "ab");
        fprintf(prefdb, "%s %s;\n", key, data);
        fflush(prefdb);
    }
    else
    {
        close();
        FILE * out = fopen("/tmp/out.pref", "wb");
        prefdb = fopen(path, "rb");
        char tmpkey[70];
        char * buffer = (char *)malloc(70); //keys can not be longer than 65 chars
        char read = 0; //value read from the fs
        int sp = 0; //string pointer
        bool nowrite = false;
        prefdb = fopen(path, "r");
        while (read != EOF)
        {
            nowrite = false;
            if (fscanf(prefdb, "%s", tmpkey) == EOF)
                break;
            if (strcmp(tmpkey, key) == 0)
                nowrite = true;
            sp = 0;
            fscanf(prefdb, "%c", &read);
            fscanf(prefdb, "%c", &read);
            while (read != ';')
            {
                buffer[sp++] = read;
                fscanf(prefdb, "%c", &read);
            }
            fscanf(prefdb, "%c", &read);
            buffer[sp] = 0;
            if (!nowrite)
                fprintf(out, "%s %s;\n", tmpkey, buffer);
        }
        fprintf(out, "%s %s;\n", key, data);
        fflush(out);
        fclose(out);
        close();
        unlink(path);
        rename("/tmp/out.pref", path);
    }
    //printf("write exit\n");
}

void preferences::close()
{
    fclose(prefdb);
}
