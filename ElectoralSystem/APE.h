#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <random>
#include <math.h>
#include <tuple>

using namespace std;

class Issue{
    private:
        string name;
        int significance;
        double approach;
    public:
        Issue();
        Issue(string _issueName, int _significance): name(_issueName),significance(_significance){setApproach();}
        void setApproach();
        void setApproach(double);
        void setSignificance(double);
        string getName();
        int getSignificance();
        double getApproach();
        string logData();
};

struct Stance{
    public:
        Issue* issue;
        double mean, sd;
        double approach;
        Stance(Issue*);
        string logData();
};

class Electorate{
    private:
        int num, population;
        vector<Stance> views;
    public:
        Electorate(){}
        Electorate(int _num, int _population): num(_num), population(_population){}
        int getNum();
        int getPopulation();
        Stance* getViews(Issue* issue);
        void addViews(Issue*);
        vector<pair<Electorate,double>> setPopularity(Electorate*, int);
        string logData();
};

class Personnel{
    protected:
        string name;
    public:
        Personnel();
        void setName(string);
        string getName();
        string getNameInitials();
};

class Candidate: public Personnel{
    private:
        double influence;
        double popularity;
        Electorate* electorate;
    public:
        Candidate(Electorate*);
        double getInfluence();
        double getPopularity();
        Electorate* getElectorate();
        void setInfluence(double);
        void setPopularity(double);
        string logData();
};

class PartyLeader: public Personnel{
    private:
        double influence;
        vector<pair<Electorate, double>> popularity;
    public:
        PartyLeader();
        double getInfluence();
        vector<pair<Electorate, double>>* getPopularity();
        PartyLeader operator=(const PartyLeader&);
        PartyLeader operator=(Candidate&);
        string logData();
};

class Party{
private:
    string name;
    PartyLeader leader;
    vector<Candidate> candidates;
    vector<Personnel> team;
    vector<Stance> views;
    vector<pair<Electorate, double>> popularity;
public:
    Party();
    string getName();
    string getNameInitials();
    Candidate* getCandidate(int);
    Candidate* getCandidate(Electorate*);
    vector<pair<Electorate, double>>* getPopularity();
    double getPopularity(Electorate*);
    PartyLeader* getPartyLeader();
    Stance* getViews(Issue*);
    vector<Personnel> getTeam();
    void setPartyLeader(PartyLeader);
    void setPartyLeader(Candidate);
    void setPopularity(Electorate*,double);
    void addCandidate(Candidate);
    void addViews(Issue*);
    string logData();
};

struct PostPollingPartyStats{
    Party* party;
    vector<tuple<Electorate*,int,double,bool>> electorateData;
    double overallVictoryPercentage;
    bool willFormGovernment;
    PostPollingPartyStats(Party* _party): party(_party){}
    string logData();
};

class Event{
    private:
        string name;
        bool trigger;
        string report="";
        string type;
        string typeArr[4]={"eventDebate","eventCandidate","eventLeader","eventIssue"};
    public:
        Event();
        string getName();
        void setType(int);
        void toggleTrigger();
        bool getTrigger();
        void changeDay(int);
        void startDebate(Candidate*,Candidate*);
        void startCandidateEvent(Candidate*,Party*);
        void startCandidateEvent(Candidate*,Candidate*);
        void startLeaderEvent(Party*,Candidate*);
        void startLeaderEvent(Party*,Issue*);
        void startIssueEvent(Issue*);
        void startIssueEvent(Issue*, Electorate*);
        string logData();
};

void setPopularity(vector<Electorate>*,vector<pair<Electorate,double>>*);

class Election{ 
    private:
        int electorateCount, campaignLength;
        Event event;
        vector<Issue> issues;
        vector<Electorate> electorates;
        vector<Party> parties;
        vector<PostPollingPartyStats> stats;
public:
        Election(int EC, int CL): electorateCount(EC), campaignLength(CL) {}
        void initialize();
        void startCampaign();
        void startElection();
        string logData(int);
};

extern default_random_engine randEng;
extern uniform_real_distribution<> trig;

//String Manipulation Functions
extern string padString(string s, int n);
extern string addBorder(string s, char c, bool fullBorder);



