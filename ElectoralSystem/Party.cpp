#include "APE.h"

//Party functions
Party::Party(){
    string pName[]={"Party","Alliance","Collective","Commune","Federation","Organisation"};
    string str="";
    int arr[3]={-1,-1,-1};
    for(int i=0;i<3;i++){
        int a =0;
        bool f=false;
        while (a==0)
        a=lround(trig(randEng)*15);       
        arr[i]= a;
        for(int j=0;j<i;j++){
            if(arr[i]==arr[j]){
                f=true;
                break;
            }
        }
        if(a%3==0&&i>0)
            break;
        if(f)
            --i;
    }

    for(int i =1;i<=3;i++){

        if(arr[i]==-1)
        break;
        ifstream inFile("parties.txt");
        for(int j =1;j<=16;j++){
            string s;
            getline(inFile,s);
            if(arr[i]==j){
                str=str+s+" ";
            }
        }
        inFile.close();
    }
    int a = 6;
    while(a>5)
        a = lround(trig(randEng)*6);
    str=str+pName[a];
    name = str;
    int n = 0;
    while(n<6)
        n = lround(trig(randEng)*20);
    for(int i=0;i<n;i++)
      team.push_back(Personnel()); 
    
}

string Party::getName(){
    return name;
}

string Party::getNameInitials(){
    string str;
    str.push_back(name.at(0));
    for(int i=1;i<(name.size()-1);i++){
        char a = name.at(i);
        if(i==name.find_first_of(' '))
                str.push_back(toupper(name.at(i-1)));
        if(a==' '||a=='-'){
            str.push_back(name.at(i+1)); 
        }   
    }
    str.push_back(toupper(name.at((name.size()-1))));
    return str;
}

PartyLeader* Party::getPartyLeader(){
    return &leader;
}

Candidate* Party::getCandidate(int n){
    return &candidates.at(n);
}

Candidate* Party::getCandidate(Electorate* electorate){
    for(int i=0;i<candidates.size();i++){
        if(candidates.at(i).getElectorate()->getNum()==electorate->getNum())
            return &candidates.at(i);
    }
    return nullptr;
}

vector<pair<Electorate, double>>* Party::getPopularity(){
    return &popularity;
}

double Party::getPopularity(Electorate* electorate){
    for(int i =0;i<popularity.size();i++){
        if(popularity.at(i).first.getNum() == electorate->getNum())
            return popularity.at(i).second;
    }
    return -1;
}

Stance* Party::getViews(Issue* issue){
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

vector<Personnel> Party::getTeam(){
    return team;
}

void Party::setPartyLeader(PartyLeader _leader){
    leader = _leader;
}

void Party::setPartyLeader(Candidate _candidate){
    leader = _candidate;
}

void Party::setPopularity(Electorate* _electorate, double _pvalue){
    for(int i =0;i<popularity.size();i++){
        if(popularity.at(i).first.getNum()==_electorate->getNum()){
            popularity.at(i).second = _pvalue;
            break;
        }
    }
}

void Party::addCandidate(Candidate candidate){
        candidates.push_back(candidate);
}

void Party::addViews(Issue* issue){
        views.push_back(Stance(issue));
}

string Party::logData(){
    string str = "Party Name: "+name+"("+this->getNameInitials()+")"+"\nLeader-\n"+leader.logData(); 
    str+="\n"+addBorder("Managerial Team",'-',false);
    for(int i=0;i<team.size();i++){
        str+=padString(team.at(i).getNameInitials(),20);
        if((i+1)%5==0&&i!=0)
        str+="\n";
    }
       int padl =25;
    str+=addBorder("Party Views",'-',true);
    str+=padString("Views",20)+"->|";
    for(int i = 0;i<views.size();i++){
        str+=padString("View "+to_string(i+1),padl)+" |";
    }
    str+="\n"+padString("Issue Name",20)+"->|";
    for(int i=0;i<views.size();i++){
        str+=padString(views.at(i).issue->getName(),padl)+" |";
    }
    str+="\n"+padString("Mean, SD",20)+"->|";
        for(int i=0;i<views.size();i++){
        str+=padString(to_string(views.at(i).mean)+", "+to_string(views.at(i).sd),padl)+" |";
    }
    str+="\n"+padString("Electorate Approach",20)+"->|";
        for(int i=0;i<views.size();i++){
            str+=padString(to_string(views.at(i).approach),padl)+" |";
        }
    str+="\n"+addBorder("               ",'-',false);
    str+="\nCandidates: \n";
    for(int i = 0; i<candidates.size();i++)
    str+=candidates.at(i).logData()+"\n";
    return str;
}



//PostPollingPartyStats Functions
string PostPollingPartyStats::logData(){
    string columnHeads[3] = {"Electorate Number", "Vote Percentage","Won Electorate"};
    string str=addBorder(party->getName(),'-',false)+"\n";
    str+=addBorder("Electorate Info",'*',true);
    str+=padString(columnHeads[0],20)+padString(columnHeads[1],20)+padString(columnHeads[2],10)+"\n";
    for(int i =0;i<electorateData.size();i++){
        tuple<Electorate*,int,double,bool>* ed = &electorateData.at(i);
        str+=padString(to_string(get<0>(*ed)->getNum()),20)+padString(to_string((int)get<2>(*ed)),20);
        string s = "NO";
        if(get<3>(*ed))
            s="YES";
        str+=s+"\n";
    }

    return str;
}