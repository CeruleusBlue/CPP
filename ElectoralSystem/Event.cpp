#include "APE.h"

//Event functions
Event::Event(){
    trigger = false;
}

string Event::getName(){
    return name;
}

void Event::setType(int n){
    if(n<4)
        type = typeArr[n];
}

void Event::toggleTrigger(){
    if(!trigger)
        trigger=true;
    else
        trigger=false;
}

bool Event::getTrigger(){
    return trigger;
}

void Event::changeDay(int n){
    report+="\nDay "+to_string(n)+"\n";
}

void Event::startDebate(Candidate* candidate1, Candidate* candidate2){
    this->name = "Debate between "+candidate1->getName()+" and "+candidate2->getName();
    this->report+=name+". ";
    uniform_real_distribution<> urd(0.5,1);
    double c1 = (candidate1->getInfluence())*urd(randEng)+(candidate1->getPopularity()/10);
    double c2 = (candidate2->getInfluence())*urd(randEng)+(candidate2->getPopularity()/10);
    candidate1->setInfluence(c1);
    candidate2->setInfluence(c2);
    if(c1>c2){
        candidate2->setPopularity(candidate1->getPopularity()-(c1/10));
        this->report+=candidate1->getName()+" wins debate and their popularity increases. ";
        this->report+=candidate2->getName()+" popularity decreases.\n";
    }
    else{
        candidate1->setPopularity(candidate2->getPopularity()-(c2/10));
        this->report+=candidate2->getName()+" wins debate and their popularity increases. ";
        this->report+=candidate1->getName()+" popularity decreases.\n";
    }
}

void Event::startCandidateEvent(Candidate* candidate, Party* party){
    this->name = candidate->getName()+" does Social Work. ";
    this->report+=name;
    double influence = ((double)(((int)candidate->getInfluence())%10)*10) + trig(randEng)*10;
    influence += party->getTeam().size()/2;
    if(influence>candidate->getInfluence()){
        candidate->setPopularity(candidate->getPopularity()-(influence/10));
        this->report+= candidate->getName()+"'s influence increases.\n";
    }
    else{
        candidate->setPopularity(candidate->getPopularity()+(influence/10));
        this->report+= candidate->getName()+"'s influence decreases.\n";
    }
    candidate->setInfluence(influence);
}

void Event::startCandidateEvent(Candidate* candidate1, Candidate* candidate2){
    this->name = candidate1->getName()+" gives a Speech. ";    
    int ctoss = lround(trig(randEng)*10);
    double change = 21;
    while(change>20)
    change = trig(randEng)*20;
    if(ctoss%2==0){
        this->name+= candidate2->getName()+"'s popularity is affected by "+candidate1->getName()+  " statements";
        candidate2->setPopularity(change); 
    }
    else{
        this->name+= candidate1->getName()+"'s popularity is affected by "+candidate1->getName()+  " statements";
        candidate1->setPopularity(change); 
    }
     this->report+=name+"\n";
}

void Event::startLeaderEvent(Party* party, Candidate* candidate){
    this->name= party->getPartyLeader()->getName()+ " visits "+candidate->getName()+"'s Electorate";
    this->report+=name+". Their popularity in the electorate increases."+"\n";
    double influence = ((double)(((int)party->getPartyLeader()->getInfluence())%10)*10) + trig(randEng)*10;
    candidate->setInfluence(candidate->getInfluence()+influence/10);
    party->setPopularity(candidate->getElectorate(),influence/20);
}

void Event::startLeaderEvent(Party* party, Issue* issue){
    this->name=party->getPartyLeader()->getName()+" raises concern about "+issue->getName();
    this->report+=name+". The stance of the party towards the issue changes."+"\n";
    Stance* stance = party->getViews(issue);
    double approach = stance->approach+issue->getApproach();
    stance->mean = stance->mean+approach;
}

void Event::startIssueEvent(Issue* issue){
    this->name="Situation surrounding "+issue->getName()+" worsens. c";
    this->report+=name+"The significance of the issue is raised by 1."+"\n";
    if(issue->getSignificance()<10)
    issue->getSignificance()+1;
}

void Event::startIssueEvent(Issue* issue, Electorate* electorate){
    this->name="Electorate "+to_string(electorate->getNum()) +" changes attitude towards "+issue->getName();
    this->report+=name+". The electorate changes its stance towards the issue."+"\n";
    Stance* stance = electorate->getViews(issue);
    stance->mean = lround(trig(randEng)*100);
    stance->sd = trig(randEng)*20;
}

string Event::logData(){
    return report;
}