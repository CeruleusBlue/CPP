#include "APE.h"
#include <iostream>

int main(int argle, char* args[]){
    int ec = 10;
    int cl = 30;
    if(ec>10||ec<1){
        cerr<<"Error: Invalid number of electorates";
        exit(-1);
    }
    if(cl>30||cl<1){
        cerr<<"Error: Invalid number of campaign days";
        exit(-1);
    }
    Election election(ec,cl);
    election.initialize();
    cout<<election.logData(1);
    election.startCampaign();
    cout<<election.logData(2);
    election.startElection();
    cout<<election.logData(3);
    cout<<"end"<<endl;
    return 0;
}