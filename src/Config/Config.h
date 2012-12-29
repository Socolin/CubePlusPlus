/*
 * Config.h
 *
 *  Created on: 28 nov. 2012
 *      Author: poogly
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <iostream>
#include <libconfig.h++>

namespace Config {

class Config {
private:
	Config();
	~Config();
public:
	static libconfig::Config& getConfig();
};

} /* namespace Config */
#endif /* CONFIG_H_ */
