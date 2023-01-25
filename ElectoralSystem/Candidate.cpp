#include "APE.h"

//Candidate functions
Candidate::Candidate(Electorate* _electorate){
    Personnel();
    electorate = _electorate;
    influence=lround(trig(randEng)*100);
    popularity = trig(randEng)*20;
    //while(popularity>20);
    //popularity = +trig(randEng);
}

double Candidate::getInfluence(){
    return influence;
}

double Candidate::getPopularity(){
    return popularity;
}

Electorate* Candidate::getElectorate(){
    return electorate;
}

Stance* Electorate::getViews(Issue* issue){
    Stance* stance = nullptr;
    for(int i=0;i<views.size();i++){
        if(issue->getName()==views.at(i).issue->getName())
        {
            stance = &views.at(i);
            break;
        }
    }
    return stance;
}


void Candidate::setInfluence(double _influence){
    influence = _influence;
}

void Candidate::setPopularity(double _popularity){
    popularity = _popularity;
}

string Candidate::logData(){
    string str = "Candidate Name: "+name;
    str+=padString("\nInfluence: "+to_string(influence),15)+padString("\tPopularity: "+to_string(abs(popularity)),15);
    str=str+"\nElectorate no.: "+to_string(electorate->getNum())+"\n";
    return str;
}
