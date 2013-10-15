/*
 * ShellCommandManager.h
 *
 *  Created on: Oct 14, 2013
 *      Author: yann291
 */

#ifndef SHELLCOMMANDMANAGER_H_
#define SHELLCOMMANDMANAGER_H_

#include "Util/Singleton.h"

#include <sys/types.h>
#include <iostream>
#include <sstream>

namespace Shell
{

class ShellCommandManager : public Util::Singleton<ShellCommandManager>
{
    friend Util::Singleton<ShellCommandManager>;
public:
    void HandleShellInput();
private:
    ShellCommandManager();
    ~ShellCommandManager();
    void HandleShellCommand(std::wstring message);

    fd_set rfds;
    struct timeval tv;
    int retval;
    char buffer[256];
};

} /* namespace Shell */
#endif /* SHELLCOMMANDMANAGER_H_ */
