#include "APE.h"

//Personnel functions
Personnel::Personnel(){
    ifstream inFile("names.txt");
    int n1 =0;
    int n2 =0;
    while(n1==0)
    n1 = lround(trig(randEng)*257999); 
    while(n2==0)
    n2 = lround(trig(randEng)*151670);
    double a = n1>n2?n1:n2;
    int n3 = lround(a)%3==0?lround(trig(randEng)*a):-1;
    while(n3==n1||n3==n2)
    n3 = lround(trig(randEng)*a);
    string firstName,middleName,lastName;
    middleName = " ";
    for(int i=1;i<=(n1>n2?n1:n2);i++){
        string str;
        getline(inFile, str);
        if(i==n1)
            firstName = str.substr(0,str.find(','));
        if(i==n2)
            lastName = str.substr(str.find(',')+1,str.length());
        if(i==n3)
            middleName = " "+str.substr(0,str.find(','))+" ";
    }
    name = firstName+middleName+lastName;
    inFile.close();
}

void Personnel::setName(string _name){
    name = _name;
}

string Personnel::getName(){
    return name;
}

string Personnel::getNameInitials(){
    string str =name.substr(0, 1)+".";
    for(int i=name.find_first_of(' ');i<name.find_last_of(' ');i++)
        if(name.at(i)==' '){
            str.push_back(name.at(i+1));
            str+=".";
        }
    str+=name.substr(name.find_last_of(' '),name.size());
    return str;
}