#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

#define infinity -1

int debug_i = 1;
bool debug_mode = true;
static void debug(string message, int level){
        if(debug_mode){
        string str = "";
        for(int i=0;i<level;i++)
            str+="\t";
        cout<<str<<"Running "<<debug_i++<<": "<<message<<endl;
    }
}

template<typename T, typename U, typename V>
struct Triple{
    public:
        T first;
        U second;
        V third;

        Triple(){
            first = -1;
            second = -1;
            third = -1;
        }

        Triple(T a, U b, V c){
            first = a;
            second = b;
            third = c;
        }

        static Triple<T, U, V> makeTriple(T a1, U a2, V a3){
            return Triple<T,U,V>(a1, a2, a3);
        }

        void operator=(const Triple &a){
            this->first = a.first;
            this->second = a.second;
            this->third = a.third;
        }

        bool operator==(const Triple &a){
            bool flag = true;
            if(this->first!=a.first)
                flag = false;
            if(this->second!=a.second)
                flag = false;
            if(this->third!=a.third)
                flag = false;
            return flag;
        }

        friend ostream &operator<<(ostream &output, const Triple<T, U, V> &t){
            output<<t.first<<"\t"<<t.second<<"\t"<<t.third;
            return output;
        }
};

typedef Triple<int, double, double> Vertex;
//<Label, X, Y>
typedef Triple<int, int, double> Edge;
//<Start, End, Weight>
typedef Triple<int, double, double> Connector;
//<End, Cost, Distance>
typedef Triple<int, int, double> ListNode;
//<Previous Node, Current Node, Total Cost>

struct Node{
    public:
        Vertex* vertex;
        Connector* connector;
        int cindex = 0;

        Node(){
            vertex = nullptr;
            connector = (Connector *) malloc(sizeof(Connector)*20);
        }       

        void initialize(Vertex* _vertex){
            vertex = _vertex;
        }

        void addConnection(int label,double cost, double distance){
            connector[cindex++] = Connector::makeTriple(label, cost, distance);
        }

        void removeConnection(int label){
            int cndx;
            for(int i =0;i<cindex;i++){
                Connector* c= &connector[i];
                if(c->first == label){
                    cndx=i;
                    c->first=-1;
                    break;
                }
            }

            for(int i = cndx;i<cindex-1;i++){
                Connector* c1 = &connector[i];
                Connector* c2 = &connector[i+1];
                if(c1->first==-1){
                    Connector temp = *c2;
                    *c2 = *c1;
                    *c1 = temp;
                }
            }
            cindex--;
        }

        ~Node(){
            free (connector);
        }
};

struct List{
    ListNode *listNode;
    ListNode empty = ListNode::makeTriple(-1,-1,-1);
    int rear, maxSize, totalElements;

    List(){
        listNode = new ListNode[100];
        maxSize=100;
        for(int i=0;i<100;i++)
            listNode[i] = empty;
        rear=-1;
        totalElements = 0;
    }

    void initialize(int size){
        listNode = new ListNode[size];
        for(int i=0;i<size;i++)
            listNode[i] = empty;
        maxSize= size;
    }

    void push(int previousNode, int currentNode, double totalCost){
        if(rear==-1)
            rear = 0;
        if(rear<maxSize){
            listNode[rear++]=ListNode::makeTriple(previousNode, currentNode, totalCost);
            totalElements++;
        }
        else
            throw overflow_error("List is full");
    }

    void pushSort(int previousNode, int currentNode, double totalCost){
        push(previousNode, currentNode, totalCost);
        int count = 0;
        for(int i = rear-1;i>0;i--){
            ListNode* l1 = &listNode[i];
            ListNode* l2 = &listNode[i-1];
            if(isEmpty(*l2)||l2->third<l1->third){
                ListNode temp = *l2;
                *l2 = *l1;
                *l1 = temp;
            }
        }
    }

    void clear(){
        delete listNode;
        listNode = new ListNode[maxSize];
        for(int i=0;i<maxSize;i++)
            listNode[i] = empty;
        rear=-1;
        totalElements = 0;
    }

    bool isEmpty(ListNode l){
        if(l.first==-1&&l.second==-1&&l.third==-1)
            return true;
        else
            return false;
    }

    ListNode* get(int label){
        ListNode* temp;
        bool exists = false;
        for(int i= rear-1;i>-1;i--){
            temp = &listNode[i];
            if(temp->second==label){
                exists = true;
                break;
            }
        }
        if(!exists)
            throw invalid_argument("ListNode::get : Object does not exist");
        return temp;
    }

    ListNode remove(int label, double totalCost){
        bool hasRemoved = false;
        ListNode temp = empty;
        int indx = 0;
        if(rear==-1)
            throw underflow_error("List is empty.");
        for(int i=rear-1;i>-1;i--){
            if(!isEmpty(listNode[i])){
                ListNode* l = &listNode[i];
                if(l->second==label&&l->third==totalCost){
                    temp = *l;
                    *l = empty;
                    indx = i;
                    hasRemoved = true;
                    totalElements--;
                    break;
                }
            }
        }

        for(int i=indx; i<rear-1;i++){
            ListNode* v1 = &listNode[i];
            ListNode* v2 = &listNode[i+1];
            if(isEmpty(*v1)){
                ListNode temp = *v2;
                *v2 = *v1;
                *v1 = temp;
            }
        }

        if(hasRemoved)
            rear--;

        if(isEmpty(temp)){
            string str = "(PN, "+to_string(label)+", "+to_string(totalCost)+")";
            throw invalid_argument("ListNode "+str+ " does not exist.");
        }
        return temp;
    }
    
    bool checkMinimum(int currentNode, double cost){
        bool isMinimum = false;
        bool exists = false;
        for(int i=0;i<rear;i++){
            ListNode* l = &listNode[i];
            if(l->second==currentNode){
                if(l->third>cost)
                    isMinimum = true;
                exists = true;
            }
        }
        if(!exists){
            isMinimum = true;
        }
        return isMinimum;

    }

    bool checkMinimum2(int currentNode, double cost){
        bool isMinimum = false;
        ListNode* firstValue;
        ListNode* secondValue;
        bool hasFirstValue = false;
        bool hasSecondValue = false;
        for(int i=0;i<rear;i++){
            if(!hasFirstValue){
                firstValue=&listNode[i];
                if(firstValue->second==currentNode){
                    hasFirstValue = true;
                    i++;
                }
            }
            if(!hasSecondValue&&hasFirstValue){
                secondValue=&listNode[i];
                if(secondValue->second==currentNode){
                    hasSecondValue = true;
                    break;
                }
            }
        }
        if(hasFirstValue&&hasSecondValue){
            ListNode* maxValue = firstValue->third>secondValue->third?firstValue:secondValue;
            if(maxValue->third>cost)
                isMinimum = true;
        }
        if(hasFirstValue&&!hasSecondValue){
            if(firstValue->third>cost)
                isMinimum = true;
        }
        if(!hasFirstValue&&!hasSecondValue)
            isMinimum = true;
        return isMinimum;

    }

    void print_debug(){
        for(int i=0;i<rear;i++){
            cout<<listNode[i]<<endl;
        }
    }

    void operator=(const List &list){
        this->totalElements = list.totalElements;
        this->initialize(list.maxSize);
        this->rear = -1;
        for(int i=0;i<list.rear;i++){
            ListNode l = list.listNode[i];
            this->push(l.first, l.second, l.third);
        }
    }

    ~List(){
        delete listNode;
    }
};

class Graph{
    private:
        Vertex *vertex;
        Edge *edge;
        Node *node;
        List openList, closedList, minPath, minSecondPath;
        int edgeIndex=0, vertexIndex=0;
        int start, goal;
    public:
        Graph(int maxVertices, int maxEdges){
            vertex = new Vertex[maxVertices];
            edge = new Edge[maxEdges];
            node = new Node[maxVertices];
            openList.initialize(maxEdges);
            closedList.initialize(maxVertices*10);
            minSecondPath.initialize(maxVertices);
            minPath.initialize(maxVertices);
        }

        double getDistance(int n){
            Vertex v1= vertex[n-1];
            Vertex v2= vertex[goal-1];
            double x = v1.second-v2.second;
            x*=x;
            double y = v1.third-v2.third;
            y*=y;
            return sqrt(x+y);
        }

        void addVertex(int a1, double a2, double a3){
            vertex[vertexIndex++]=(Vertex::makeTriple(a1, a2, a3));         
        }

        void addEdge(int a1, int a2, double a3){
            edge[edgeIndex++]=(Edge::makeTriple(a1,a2,a3));
        }

        void setTerminals(int _start, int _goal){
            start = _start;
            goal = _goal;
            plot();
        }

        void plot(){
            int EI=0;
            for(int i=0;i<vertexIndex;i++){
                Vertex* v= &vertex[i];
                node[v->first-1].initialize(v);
            }
            for(int i=0;i<edgeIndex;i++){
                Edge* e = &edge[i];
                Node* n1 = &node[e->first-1];
                Node* n2 = &node[e->second-1];
                bool existsN1 = false;
                bool existsN2 = false;
                for(int j = 0;j<n1->cindex;j++){
                    Connector* c = &n1->connector[j];
                    if(c->first==e->second){
                        existsN1 = true;
                        c->second = c->second>e->third?e->third:c->second;
                    }
                }
                if(!existsN1)
                    n1->addConnection(e->second, e->third, getDistance(e->second));
                for(int j = 0;j<n2->cindex;j++){
                    Connector* c = &n2->connector[j];
                    if(c->first==e->first){
                        existsN2 = true;
                        c->second = c->second>e->third?e->third:c->second;
                    }
                }
                if(!existsN2)
                    n2->addConnection(e->first, e->third, getDistance(e->first));
            }
            //print_debug();
            findSecondShortestPath();
        }

        void findShortestPath(){
            openList.clear();
            closedList.clear();
            minPath.clear();
            openList.push(0, start, getDistance(start));
            double minPathCost=-1;
            int previousLabel = 0;
            //While nodes are still present in Open List
            while(openList.totalElements>0){
                //Stores the node with minimum possible traversal cost
                ListNode current;
                int currentLabel;
                double currentCost;
                //Calculates parent
                for(int i = openList.rear-1;i>-1;i--){
                    current = openList.listNode[i];
                    if(previousLabel==current.first){
                        currentLabel = current.second;
                        currentCost = current.third;
                        openList.remove(current.second,current.third);
                        break;
                    }
                }
                //cout<<"Current Node: "<<current<<endl;
                //Stores every connection from currentNode
                Node* currentNode = &node[currentLabel-1];
                //Stores the next minimum cost node from currentNode
                Connector next; 
                int nextLabel;
                double nextCost;
                int listCount = 0;
                //For every connection from currentNode 
                for(int i=0;i<currentNode->cindex;i++){
                    //Terminates the loop if the current node is the goal
                    if(currentLabel==goal)
                        break;
                    //Temporary storage for each connection
                    Connector eachConnector = currentNode->connector[i];
                    nextLabel = eachConnector.first;
                    //Total Cost = Actual Path Cost + Heuristic Path Cost
                    //Therefore, Actual Path Cost for next = 
                    //  Weight of next + Actual Path Cost for next + Actual Path Cost upto current - Heuristic Path Cost for previous
                    if(currentCost == 0)
                        nextCost = eachConnector.second+eachConnector.third;
                    else
                        nextCost = eachConnector.second+eachConnector.third+currentCost-getDistance(currentLabel);
                    //Checks if this is the lowest value of node cost with respect to open list
                    if(openList.checkMinimum(nextLabel, nextCost)&&nextLabel!=start){
                        //Checks if this is the lowest value of node cost with respect to closed list
                        if(closedList.checkMinimum(nextLabel, nextCost)){
                            if(nextCost<minPathCost||minPathCost==-1){
                                listCount++;
                                openList.pushSort(currentLabel, nextLabel, nextCost);
                            }
                        }
                    }                    
                }
                //cout<<"Open List Nodes: "<<endl;
                //openList.print_debug();
                if(listCount==0&&previousLabel==start&&currentLabel!=goal)
                    break;
                closedList.push(previousLabel, currentLabel, currentCost);
                if(currentLabel==goal){
                    if(currentCost<minPathCost||minPathCost==-1){
                        minPathCost = currentCost;
                        //cout<<"Minimum Path Cost: "<<minPathCost<<endl;
                        for(int i=0;i<openList.rear;i++){
                            ListNode *l = &openList.listNode[i];
                            if(l->third>minPathCost)
                                openList.remove(l->second, l->third);
                            currentLabel = start;
                        }
                    }
                }
                previousLabel = currentLabel;
            }
            bool flag = false;
            for(int i = closedList.rear-1;i>-1;i--){
                ListNode l = closedList.listNode[i];
                if(l.second==goal&&l.third==minPathCost)
                    flag= true;
                if(flag){
                    minPath.push(l.first, l.second, l.third);
                }
                if(flag&&l.second==goal){
                    break;
                }
            }
        }

        void findSecondShortestPath(){
            openList.clear();
            closedList.clear();
            minPath.clear();
            minSecondPath.clear();
            findShortestPath();
            openList.clear();
            closedList.clear();

            openList.push(0, start, getDistance(start));
            double minSecondPathCost=-1;
            int previousLabel = 0;
            //While nodes are still present in Open List
            while(openList.totalElements>0){
                //Stores the node with minimum possible traversal cost
                ListNode current;
                int currentLabel;
                double currentCost;
                //Calculates parent
                for(int i = openList.rear-1;i>-1;i--){
                    current = openList.listNode[i];
                    if(previousLabel==current.first){
                        currentLabel = current.second;
                        currentCost = current.third;
                        openList.remove(current.second,current.third);
                        break;
                    }
                }
                if(previousLabel==current.first){
                    //cout<<"Current Node: "<<current<<endl;
                    //Stores every connection from currentNode
                    Node* currentNode = &node[currentLabel-1];
                    //Stores the next minimum cost node from currentNode
                    Connector next; 
                    int nextLabel;
                    double nextCost;
                    int listCount = 0;
                    //For every connection from currentNode 
                    for(int i=0;i<currentNode->cindex;i++){
                        //Terminates the loop if the current node is the goal
                        if(currentLabel==goal)
                            break;
                        //Temporary storage for each connection
                        Connector eachConnector = currentNode->connector[i];
                        nextLabel = eachConnector.first;
                        //Total Cost = Actual Path Cost + Heuristic Path Cost
                        //Therefore, Actual Path Cost for next = 
                        //  Weight of next + Actual Path Cost for next + Actual Path Cost upto current - Heuristic Path Cost for previous
                        if(currentCost == 0)
                            nextCost = eachConnector.second+eachConnector.third;
                        else
                            nextCost = eachConnector.second+eachConnector.third+currentCost-getDistance(currentLabel);
                        //If node exists in minimum path then check whether next node has a higher cost or not
                        bool existInMinPath = false;
                        for(int j=0;j<minPath.rear;j++){
                            ListNode l = minPath.listNode[j];
                            if(l.first == currentLabel&&l.second == nextLabel&&l.third==nextCost){
                                existInMinPath = true;
                                break;
                            }
                        }
                        //Checks if this is the lowest value of node cost with respect to open list
                        if(!existInMinPath){
                            if(openList.checkMinimum(nextLabel, nextCost)&&nextLabel!=start){
                                //Checks if this is the lowest value of node cost with respect to closed list
                                if(closedList.checkMinimum(nextLabel, nextCost)){
                                    if(nextCost<minSecondPathCost||minSecondPathCost==-1){
                                        listCount++;
                                        openList.pushSort(currentLabel, nextLabel, nextCost);
                                    }
                                }
                            }              
                        }      
                    }
                   //cout<<"Open List Nodes: "<<endl;
                    //openList.print_debug();
                    if(listCount==0&&(previousLabel==start)&&currentLabel!=goal)
                        break;
                    closedList.push(previousLabel, currentLabel, currentCost);
                    if(currentLabel==goal){
                        if(currentCost<minSecondPathCost||minSecondPathCost==-1){
                            minSecondPathCost = currentCost;
                            //cout<<"Minimum Path Cost: "<<minSecondPathCost<<endl;
                            for(int i=0;i<openList.rear;i++){
                                ListNode *l = &openList.listNode[i];
                                if(l->third>minSecondPathCost)
                                    openList.remove(l->second, l->third);
                                currentLabel = start;
                            }
                        }
                    }
                    previousLabel = currentLabel;
                }
                else
                    previousLabel = start;
            }
            bool flag = false;
            for(int i = closedList.rear-1;i>-1;i--){
                ListNode l = closedList.listNode[i];
                if(l.second==goal&&l.third==minSecondPathCost)
                    flag= true;
                if(flag){
                    minSecondPath.push(l.first, l.second, l.third);
                }
                if(flag&&l.first==start){
                    break;
                }
            }

        }

        void print(){
            cout<<"Find the shortest path between vertices "<<start<<" and "<<goal<<endl;
            cout<<"The shortest path has a length of "<<minPath.listNode[0].third<<endl;
            cout<<"\tThe vertices on this path are: "<<start;
            for(int i=minPath.rear-1;i>-1;i--)
                cout<<" "<<minPath.listNode[i].second;
            cout<<endl;


            cout<<"\n\nFind the second shortest path between "<<start<<" and "<<goal<<endl;
            cout<<"The second shortest path has a length of "<<minSecondPath.listNode[0].third<<endl;
            cout<<"\tThe vertices on this path are: "<<start;
            for(int i=minSecondPath.rear-1;i>-1;i--)
                cout<<" "<<minSecondPath.listNode[i].second;
            cout<<endl;

            cout<<"\n\nThe Euclidean distance between vertices "<<start<<" and "<<goal<<" is "<<getDistance(start)<<endl;
        } 
        
        void print_debug(){
            ofstream out("output.txt");
            for(int i=0;i<vertexIndex;i++){
                out<<*node[i].vertex<<endl;
                for(int j=0;j<node[i].cindex;j++){
                    out<<"\t"<<node[i].connector[j]<<endl;
                }
            }
        }

        ~Graph(){
            delete vertex;
            delete edge;
            delete node;
        }
};

double toDouble(string str){
    double d1 = 0;
    double d2 = 0;
    int n =0;
    bool flag = false;
    for(int i=0;i<str.size();i++){
        char a = str[i];
        if(a=='.'){
            flag = true;
        }
        if(a>47&&a<58){
            int b = a-48;
            if(!flag){
                d1 = d1*10+b;
            }
            else{
                d2 = d2*10+b;
                if(d2>0){
                    if(n==0)
                        n=10;
                    else
                        n*=10;
                }
            }
        }
    }
    if(d2>0)
        d2 = d2 / n;
    return d1+d2;
}

string substring(string &str, int start, int end){
    string out="";
    for(int i = start;i<end;i++)
        out+=str[i];
    return out;
}

int main(){
    string str;
    cout<<"Please enter the name of the input file: ";
    cin>>str;
    ifstream file(str);

    //Extracting the first line for total edges and vertices    
    string firstLine;
    getline(file, firstLine);
    int n =0;
    int maxVertices =0 , maxEdges;
    for(int i=0;i<firstLine.size();i++){
        if(firstLine[i]=='\t'){
            maxVertices = stoi(firstLine.substr(0,i));
            maxEdges = stoi(firstLine.substr(i+1,firstLine.size()));
            break;
        }
    }

    //Creating Graph Object
    Graph graph(maxVertices, maxEdges);

    //Extracting rest of the data from file
    string line;
    int nextVertex = 1;
    int j=0;

    //Extracting all vertices
    for(int i=0;i<maxVertices+maxEdges;i++){
        double a1, a2, a3;
        getline(file, line);
        bool isFirstTab = true;
        int firstTab, secondTab;
        
        for(int j=0;j<line.size();j++){
            char a = line[j];
            if(a=='\t'){
                if(isFirstTab){
                    isFirstTab = false;
                    firstTab = j;
                }
                else{
                    secondTab = j;
                    break;
                }
            }
        }
        a1=toDouble(substring(line,0,firstTab));                  
        a2=toDouble(substring(line,firstTab+1,secondTab));
        a3=toDouble(substring(line,secondTab+1,line.size()));
        if(i<maxVertices)
            graph.addVertex(a1,a2,a3);
        else
            graph.addEdge(a1,a2,a3);
    }

    //Extracting Last Line
    getline(file,line);
    int firstTab;
    for(int i=0;i<line.size();i++){
        char a = line[i];
        if(a=='\t'){
            firstTab = i;
            break;
        }
    }
    int start = toDouble(substring(line,0,firstTab));
    int end = toDouble(substring(line,firstTab+1,line.size()));
    graph.setTerminals(start,end);
    graph.findShortestPath();
    graph.findSecondShortestPath();
    graph.print();
}