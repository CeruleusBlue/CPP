#include "APE.h"

//Issue functions
void Issue::setApproach(){
    approach = trig(randEng)*10;
}

void Issue::setApproach(double _approach){
    approach = _approach;
}

void Issue::setSignificance(double _significance){
    significance = _significance;
}

string Issue::getName(){
    return name;
}

int Issue::getSignificance(){
    return significance;
}

double Issue::getApproach(){
    return approach;
}

string Issue::logData(){
    string str="Issue Name:"+name;
    string s;
    s=to_string(significance);
    if(significance<10)
        s="0"+s;
    str+="\nSignificance: "+s+"\tApproach: "+to_string(approach)+"\n";
    return str;
}

//Stance functions
Stance::Stance(Issue* _issue){
issue = _issue;
mean = 100;
sd = 20;
while((mean+sd)>100||(mean-sd)<0){
    mean = trig(randEng)*100;
    sd = trig(randEng)*20;
}
approach = lround(trig(randEng)*10);
}

string Stance::logData(){
    string str ="Mean: "+to_string(mean)+"\tS.D.: "+to_string(sd)+"\nST_Approach: "+to_string(approach)+"\n";
    return str;
}