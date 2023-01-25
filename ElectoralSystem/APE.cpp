#include "APE.h"

default_random_engine randEng(time(0));
uniform_real_distribution<> trig(0,1);

//Generic functions
void setPopularity(vector<Electorate>* electorates, vector<pair<Electorate,double>>* popularity){
       popularity->clear();
    for(int i=0;i<electorates->size();i++){
        double d = 21;
        while(d>20)
        d = lround(trig(randEng)*20)+trig(randEng);
        pair<Electorate, double> p(electorates->at(i),d);
        popularity->push_back(p);
    }

}

//String Manipulation Functions
string padString(string s, int n){
    string str = s;
    int strLength = s.length();
    if(n-strLength>0)
    for(int i = strLength; i<n;i++)
        str+=" ";
    else
        str+=" ";
    return str;
}

string addBorder(string s, char c, bool fullBorder){
    string str ="";
    int l = s.length();
    if(fullBorder){
        l+=2;
        for(int i = 0; i<l;i++)
            str+=c;
        str+="\n";
        str+=c+s+c+"\n";
    }
    else
        str=s+"\n";
    for(int i =0; i<l;i++)
        str+=c;
    return str+"\n";
}

//Election functions
void Election::initialize(){
//Issue Intialization    
issues.push_back(Issue("Climate Change",7));
issues.push_back(Issue("Wealth Inequality",5));
issues.push_back(Issue("Cat Shelters",10));
issues.push_back(Issue("Universal Basic Income",2));
issues.push_back(Issue("Pandemic Response",9));

//Electorate Initialization
for(int i=0;i<electorateCount;i++){
    int p =0;
    while(p<9999)
    p = lround(trig(randEng)*1000000);
    electorates.push_back(Electorate(lround(trig(randEng)*90000)+i,p));
    for(int j=0;j<5;j++){
        electorates.at(i).addViews(&(issues.at(j)));
}
}

//Party Initialization
for(int i =0;i<3;i++){
    Party party = Party();
    setPopularity(&electorates, party.getPopularity());
    party.setPartyLeader(PartyLeader());
    setPopularity(&electorates, party.getPartyLeader()->getPopularity());
    for(int j=0;j<5;j++)
        party.addViews(&(issues.at(j)));
    for(int j=0;j<electorates.size();j++){
        party.addCandidate(Candidate(&(electorates.at(j))));
    }
    parties.push_back(party);
    }
}



void Election::startCampaign(){
    for(int i =0;i<campaignLength;i++){
        event.changeDay(i+1);
    for(int j =0;j<electorateCount;j++){
    int ctoss = lround(trig(randEng)*9);
    if(ctoss%2==0)
        event.toggleTrigger();
    if(event.getTrigger()){
        int a = lround(trig(randEng)*7)/2;
        event.setType(a);
        double b = j;//stores the electorate index
        double c =b;
        while(c==b){
        c = electorates.size()-1;//stores the secondary electorate index
        c*=trig(randEng);
        c=lround(c);
        }
        int d = lround(trig(randEng)*2);//stores party index
        int e = d;//stores secondary party index
        while(e==d)
        e = lround(trig(randEng)*2);
        int f = lround(trig(randEng)*(issues.size()-1));//stores Issue index
        int g = f;
        while(f==g)
            g = lround(trig(randEng)*(issues.size()-1));//stores secondary Issue index
        if(a==0){
            event.startDebate(parties.at(d).getCandidate(b),parties.at(e).getCandidate(c));
        }
        if(a==1){
            ctoss = lround(trig(randEng)*10);
            if(ctoss%2==0)
                event.startCandidateEvent(parties.at(d).getCandidate(b),&parties.at(d));                 
            else{
            Candidate* candidate1 = parties.at(d).getCandidate(b);
            Candidate* candidate2 = parties.at(e).getCandidate(c);
            event.startCandidateEvent(candidate1,candidate2);
            }
        }
        if(a==2){
            ctoss = lround(trig(randEng)*10);
            if(ctoss%2==0)
                event.startLeaderEvent(&parties.at(d), parties.at(d).getCandidate(b));
            else 
                event.startLeaderEvent(&parties.at(d), &issues.at(f));

        }
        if(a==3){
            ctoss = lround(trig(randEng)*10);
            if(ctoss%2==0)
                event.startIssueEvent(&issues.at(f));
            else
                event.startIssueEvent(&issues.at(f), &electorates.at(b));
        }
    }
    }
    }
}

string Election::logData(int stage){
    string str;
    if(stage == 1){
        str = addBorder("Pre-Campaign Party Report",'#',true)+"\n";
        for(int i =0;i<parties.size();i++)
            str+=parties.at(i).logData()+"\n";

        str += addBorder("Pre-Campaign Nation Report",'#',true)+"\n";
            for(int i=0;i<electorates.size();i++)
                str+=electorates.at(i).logData()+"\n";
    }
    
    if(stage == 2){
        str = addBorder("Campaign Report",'#',true)+"\n";
        str+=event.logData()+"\n";

        str += addBorder("Pre-Election Party Report",'#',true)+"\n";
            for(int i =0;i<parties.size();i++)
                str+=parties.at(i).logData()+"\n";

        str += addBorder("Pre-Election Nation Report",'#',true)+"\n";
            for(int i=0;i<electorates.size();i++)
                str+=electorates.at(i).logData()+"\n";
    }
    
    if(stage == 3){
        str = addBorder("Election Report",'#',true)+"\n";
        string elstr=padString("Electorate",16)+" |";
        for(int i =0;i<parties.size();i++)
            elstr+=padString(parties.at(i).getNameInitials(),25)+" |";
        elstr+=padString("Winning Candidate",25);
        str+=addBorder(elstr,'-',false);
        for(int i =0;i<electorates.size();i++){
            str+=padString(to_string(electorates.at(i).getNum()),16);
            Candidate* winningCandidate=nullptr;
            for(int j=0;j<parties.size();j++){
                tuple<Electorate*,int,double,bool>* thisStat = &stats.at(j).electorateData.at(i);
                str+=" |"+
                padString(to_string(get<1>(*thisStat))+
                "("+to_string((int)get<2>(*thisStat))+"%)"+
                +(get<3>(*thisStat)?" WON":""),25);
                if(get<3>(*thisStat))
                    winningCandidate =parties.at(j).getCandidate(get<0>(*thisStat));
            }
            str+=" |" +winningCandidate->getNameInitials();
            str+="\n";
        }

        str+="\n\n"+addBorder(padString("Party",16)+"Overall Percentage",'-',false);
        int victoryIndex = -1;
        for(int i =0;i<parties.size();i++){
            if(stats.at(i).willFormGovernment)
                victoryIndex = i;
            str+=padString(parties.at(i).getNameInitials(),16)+to_string(stats.at(i).overallVictoryPercentage).substr(0,5)+"%\n";
        }
        string pstr = "\n"+addBorder("Hung Parliament",'+',true);
        if(victoryIndex>-1)
            pstr="\n"+addBorder(parties.at(victoryIndex).getName()+" will form government.",'+',true);
        str+=pstr;
    } 

     return str;   

}

void Election::startElection(){
    for(int i =0;i<parties.size();i++)
        stats.push_back(PostPollingPartyStats(&parties.at(i)));
    for(int i =0;i<electorates.size();i++){
        Electorate* el = &electorates.at(i);
        uniform_real_distribution<> viewDistribution;
        int electorateVoteShare[3] = {0,0,0};

        //Total Electorate Vote Calculation
        int turnout = trig(randEng)*100;
        while(turnout<40)
            turnout = trig(randEng)*100;
        int totalElectorateVotes = el->getPopulation()*turnout/100;

        //Electorate View Calculation
        double electorateViewValues[5];
        double electorateApproach[5];
        for(int j =0;j<5;j++){
            int mean = el->getViews(&issues.at(j))->mean;
            int sd = el->getViews(&issues.at(j))->sd;
            uniform_real_distribution<> viewDistribution(mean,sd);
            electorateViewValues[j]=viewDistribution(randEng);
            electorateApproach[j] = el->getViews(&issues.at(j))->approach;
        }
    
        //Candidate Popularity
        double candidatePopularityValues[3];
        for(int j=0;j<3;j++)
            candidatePopularityValues[j]=parties.at(j).getCandidate(el)->getPopularity();

        //Party View Calculation
        double partyViewValues[5][3];
        double partyApproach[5][3];
        for(int j=0;j<3;j++){
            for(int k=0;k<5;k++){
                int mean = parties.at(j).getViews(&issues.at(k))->mean;
                int sd = parties.at(j).getViews(&issues.at(k))->sd;
                uniform_real_distribution<> viewDistribution(mean,sd);
                partyViewValues[k][j]= viewDistribution(randEng);
                partyApproach[k][j] = parties.at(j).getViews(&issues.at(k))->approach;
            }
        }
        
        //Party Popularity
        double partyPopularityValues[3];
        for(int j =0;j<3;j++)
            partyPopularityValues[j]=parties.at(j).getPopularity(el);

        //Party-Candidate Popularity equivalence
        double popularityEquivalence[3];
        for(int j=0;j<3;j++){
            double mean = 0;
            for(int k=0;k<5;k++){
                int approach = abs(partyApproach[k][j]-electorateApproach[k]);
                mean += abs((candidatePopularityValues[j])*(10-approach))+abs(partyPopularityValues[j]*approach);
            }
        popularityEquivalence[j]=mean/10;
        }

        //Party-Electorate View Significance Factor
        double viewWeightage[3];
        for(int j=0;j<3;j++){
            double mean=0;
            for(int k=0;k<5;k++){
                mean += abs(100-abs(electorateViewValues[k]-partyViewValues[k][j]))*issues.at(k).getSignificance();
            }
            viewWeightage[j]=mean/50;
        }

        //Issue/Popularity based Voteshare Calculation
        double issueVotes[3],popularityVotes[3];
        double nonrandomVoteShare;
        double randomVotes;
        for(int j=0;j<3;j++){
            issueVotes[j] = viewWeightage[j]*0.7*totalElectorateVotes/100;
            popularityVotes[j] = ((totalElectorateVotes*0.7)-issueVotes[j])*popularityEquivalence[j]/100;
        }
        int issueMean = issueVotes[0]+issueVotes[1]+issueVotes[2]/3;
        int popularityMean = popularityVotes[0]+popularityVotes[1]+popularityVotes[2];
        int voteCount=0;
        for(int j =0;j<3;j++){
            nonrandomVoteShare=((issueVotes[j]/issueMean)+(popularityVotes[j]/popularityMean))/2;
            electorateVoteShare[j]= nonrandomVoteShare*0.7*totalElectorateVotes;
            voteCount+=lround(nonrandomVoteShare*0.7*totalElectorateVotes);
        }
        randomVotes = totalElectorateVotes - voteCount;
 
        //Random Voteshare Calculation
        double randomFactor;
        double randomVoteShare;
        double cumulative=0;
        for(int j=0;j<3;j++){
            randomFactor=trig(randEng);
            while((randomFactor+cumulative)>1&&j<2)
                randomFactor=trig(randEng);
            if(j==2)
                randomFactor=1-cumulative;
            cumulative+=randomFactor;
            randomVoteShare = randomFactor*randomVotes;
            electorateVoteShare[j]+=randomVoteShare;
        }

        //Electorate VoteShare Assignment
        int maxVotesIndex =0;
        for(int j=1;j<3;j++)
            if(electorateVoteShare[j]>electorateVoteShare[maxVotesIndex])
                maxVotesIndex = j;
        for(int j =0;j<3;j++){
            bool victoryFlag = false;
            if(j==maxVotesIndex)
            victoryFlag = true;
            stats.at(j).electorateData.push_back(make_tuple(el,electorateVoteShare[j],electorateVoteShare[j]*100/totalElectorateVotes,victoryFlag));
        }
    }

        //Determining Overall Victory
        int electorateVictories[3]={0,0,0};
        bool governmentFormationFlag;
        for(int i = 0;i<electorates.size();i++)
            for(int j=0;j<3;j++)
                if(get<3>(stats.at(j).electorateData.at(i)))
                    electorateVictories[j]++;
        int victoryRequirement = ceil(electorateCount*0.51);
        for(int i=0;i<3;i++){     
            governmentFormationFlag = false;
            if(electorateVictories[i]>=victoryRequirement)
                governmentFormationFlag = true;
            stats.at(i).overallVictoryPercentage = (double)electorateVictories[i]*100/(double)electorates.size();
            stats.at(i).willFormGovernment = governmentFormationFlag;
        }       
}
