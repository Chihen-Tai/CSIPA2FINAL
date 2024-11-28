#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED

#include <string>
#include <map>
#include "../Object.h"
enum class BallState
{
    Normal
};

class Ball : public Object
{
    public:
    void init();    
    void update();
    void draw();
    
    private:
    double speed =5.0;
    std::map<BallState,std::string>  pngpath;
};

#endif // BALL_H_INCLUDED