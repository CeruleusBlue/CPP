#include "APE.h"

//Electorate functions
int Electorate::getNum(){
    return num;
}

int Electorate::getPopulation(){
    return population;
}

void Electorate::addViews(Issue* issue){
    Stance stance = Stance(issue);
    views.push_back(stance);
}

string Electorate::logData(){
    string str = "Electorate no.: "+to_string(num)+"\tPop: "+to_string(population)+"\n\n";
    int padl =25;
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
    str+="\n";
    return str;
}