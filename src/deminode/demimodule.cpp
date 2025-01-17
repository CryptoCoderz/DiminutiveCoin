// Copyright (c) 2021-2023 The Espers Project/CryptoCoderz Team
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "demimodule.h"
#include "util.h"

bool fDemiFound = false;

std::string GetDemiConfigFile()
{
    //TODO: include GetArg for user defined directories
    //example: ConfigFileAlias(GetArg("-demiconf", "Demi.conf"))
    std::string pathConfigFile = GetDataDir().string().c_str();
    std::string ConfigFileAlias = "/Demi.conf";
    pathConfigFile += ConfigFileAlias.c_str();

    return pathConfigFile;
}

void ReadDemiConfigFile(std::string peerReadAddr)
{
    fDemiFound = false;
    std::ifstream streamConfig(GetDemiConfigFile().c_str());
    if (!streamConfig.good())
    {
               std::string ConfPath = GetDataDir().string().c_str();
               std::string ConfigFileAlias = "/Demi.conf";
               ConfPath += ConfigFileAlias.c_str();
               FILE* ConfFile = fopen(ConfPath.c_str(), "w");
               fprintf(ConfFile, "75.119.140.224:49139\n");
               fprintf(ConfFile, "38.242.255.229:49139\n");
               fprintf(ConfFile, "176.57.189.38:49139\n");
               fclose(ConfFile);
    }

    // Open requested config file
    LogPrintf("ReadDemiConfigFile - INFO - Loading Demi-nodes from: %s \n", GetDemiConfigFile().c_str());
    std::ifstream file;
    file.open(GetDemiConfigFile().c_str());
    if(!file.is_open()) {
        // Print for debugging
        LogPrintf("ReadDemiConfigFile - ERROR 00 - Cannot open file!\n");
        return;
    } else {
        // Print for debugging
        LogPrintf("ReadDemiConfigFile - INFO - File successfully opened!\n");
    }

    // Read data
    //
    // Print for debugging
    LogPrintf("ReadDemiConfigFile - INFO - Reading file...\n");
    std::string line;
    while(file.good()) {
        // Loop through lines
        std::getline(file, line);
        // Print for debugging
        LogPrintf("ReadDemiConfigFile - INFO - Got line data...\n");
        if (!line.empty()) {
            if (line[0] != '#') {
                // Print for debugging
                LogPrintf("ReadDemiConfigFile - INFO - Read data success!\n");
                // Combine input string
                if(peerReadAddr == line) {
                    // Print for debugging
                    LogPrintf("ReadDemiConfigFile - INFO - Set data success!\n");
                    fDemiFound = true;
                    break;
                }
            } else {
                // Print for debugging
                LogPrintf("ReadDemiConfigFile - WARNING - Comment detected! \n");
            }
        } else {
            // Print for debugging
            LogPrintf("ReadDemiConfigFile - WARNING - Empty line detected! \n");
        }
    }

    file.close();
}
