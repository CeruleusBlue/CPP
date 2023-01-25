#include "APE.h"

//PartyLeader functions
PartyLeader::PartyLeader(){
    Personnel();
    influence=lround(trig(randEng)*100);
}

vector<pair<Electorate, double>>* PartyLeader::getPopularity(){
    return &popularity;
}

double PartyLeader::getInfluence(){
    return influence;
}

PartyLeader PartyLeader::operator=(const PartyLeader& leader){
        this -> name = leader.name;
        this -> influence = leader.influence;
        this -> popularity.clear();
        for(int i =0;i<leader.popularity.size();i++)
        this -> popularity.push_back(leader.popularity.at(i));
        return *(this);
    }
    
PartyLeader PartyLeader::operator=(Candidate& candidate){
        this -> name = candidate.getName();
        this -> influence = candidate.getInfluence();
        for(int i=0;i<this->popularity.size();i++){
        if(this->popularity.at(i).first.getNum()==candidate.getElectorate()->getNum())
            this -> popularity.at(i).second = candidate.getPopularity();
        double d = 21;
        while(d>20)
        d = lround(trig(randEng)*20)+trig(randEng);
        this -> popularity.at(i).second = d;
        }
        return *(this);
    }

string PartyLeader::logData(){
    string str = "Leader Name: "+name+"\nInfluence: "+to_string(influence)+"\n";
    str +=addBorder("Electorate Popularity",'-',true)+"Electorate| ";
    for(int i=0;i<popularity.size();i++)
        str+=padString(to_string(popularity.at(i).first.getNum()),10);
    str+="\nPopularity| ";
    for(int i=0;i<popularity.size();i++)
        str+=padString(to_string(popularity.at(i).second),10);
    str +=addBorder("                       ",'-',false);;
    return str;
}
