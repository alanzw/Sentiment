#ifndef SENTIMENT_ISYSTEM_H
#define SENTIMENT_ISYSTEM_H

class ISystem
{

};

typedef void (*SYSTEM_CONSTRUCTOR)(std::shared_ptr<ISystem> & systemObj);

#endif
