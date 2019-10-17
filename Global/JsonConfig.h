#ifndef JSONCONFIG_H
#define JSONCONFIG_H
#include "sysconfig.h"

class JsonConfig : public SysConfig
{
	friend class SysConfig;
private:
    JsonConfig();
public:
    virtual ~JsonConfig();
	void readConfig();
	void writeConfig();  
};

#endif // JSONCONFIG_H
