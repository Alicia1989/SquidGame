#include <iostream> //cin,cout
#include <conio.h> //getch()
#include <windows.h> //set screen size + disable cursor
using namespace std;

class list{
private:
    struct node{    //linklist node
        char data;  //data of node
        node *next; //pointer to next node
    };
    node *head; //start node of linklist
public:
    list(){ //default constructor
        head=NULL;  //initially no node in list     
    }
    void push_front(int new_data){ 
        if(head==NULL){ //if no node in list
            node *new_node = new node;  //create new node
            new_node->data = new_data;  //fill data
            new_node->next = NULL;  // no next node because this is supposed to be last node
            head = new_node;    //add firest node to list
        }else{
            node *last_node = head; //otherwise find last node nad attach new node after this last node
            while(last_node->next!=NULL)
            {
                last_node = last_node->next;
            }
            node *new_node = new node;
            new_node->data = new_data;
            new_node->next = NULL;
            last_node->next = new_node;
        }
    }

    void push_back(int new_data){
        if(head==NULL){
            node *new_node = new node;
            new_node->data = new_data;
            new_node->next = NULL;
            head = new_node;
        }else{  //this is supposed to be first node so attach is before head
            node *new_node = new node;
            new_node->data = new_data;
            new_node->next = head; //and then make it head of list
            head = new_node;
        }
    }

    char pop_front()//[d|n]->[d|n]->[d|N]
    {
         node *last_node = head;   //start looking for last node
         node *sec_last_node = head;
        while(last_node->next!=NULL)
        {
            sec_last_node = last_node;
            last_node = last_node->next;
        }
        char this_data = last_node->data;   //copy data of last node
        sec_last_node->next=NULL;   //delete last node
        return this_data;   //retun data of last node
    }

    char pop_back()//[d|n]->[d|n]->[d|N]
    {
        char this_data = head->data;    //copy data of head and then delete head
        head=head->next;
        return this_data;
    }

    void print_list(){
        node *current_node = head; //start from head and print all nodoes
        while(current_node!=NULL)
        {
            cout<<current_node->data;
            current_node = current_node->next;
        }
        cout<<endl;
    }

    int move_bullet()
    {
        node *current_node = head;
        int kills = 0;
        while(current_node->next!=NULL)
        {
            if(current_node->data=='-')
            {
                if(current_node->next!=NULL)
                {
                    if(current_node->next->data==(char)157)
                    {
                        current_node->data=' ';
                        current_node->next->data = ' ';
                        kills++;
                    }else{
                        if(current_node->next->next!=NULL)
                        {
                            current_node->data = ' ';
                            current_node->next->next->data = '-';
                            current_node = current_node->next;
                            current_node = current_node->next;
                        }else{
                            current_node->data=' ';
                        }
                    }
                }else{
                    current_node->data=' ';
                }
            }
            current_node = current_node->next;
        }
        return kills;
    }
};

void gotoxy(int x, int y) { //move terminal cursor to position x,y for new output
    COORD pos = {x, y};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

int main()
{
    list squid_list[25];
    char ship_arr[25];
    char new_squid;
    int temp;
    int kills = 0;
    int shooting_delay = 0;
    int score = 0;
    int curr_ship_pos = 0; // current ship's position: 0 when game starts
    int threshold = 3;  //percent squids on screen
    char squid;
    system("CLS");  //clear screen
    ship_arr[0] = '>';  //ship initially
    for(int i=1;i<25;i++)   //other than index 0 no ship so store ' '
    {
        ship_arr[i] = ' ';
    }
    cout << "\e[8;28;125t"; //set size of terminal
    printf("\e[?25l");  //hide cursor
    //120 per line
    for(int i=0;i<117;i++)  //size of horizontal squid list
    {
        for(int j=0;j<25;j++) //total 25 squid lists
        {
            squid_list[j].push_front(' ');  // initially no squids on screen when game starts
        }
    }
    char key;
    key = ' ';
    while(true)
    {
        gotoxy(0,0);    // to start printing fron start of terminal screen
        cout << "Use the UP\\DOWN arrow keys to move the ship.... Space bar to fire. ESC to quit" << endl;
        cout << "Score: " << score << ", Kills: " << kills << endl;
        for(int j=0;j<25;j++)   //start printing squids and ship location on screen
        {
            if(kbhit()) //check if any key is pressed
            {
                key = (char)getch();    //for up and down get key
                if(key==' ')
                {
                    if(shooting_delay>2)
                    {
                        char bullet;
                        bullet = '-';
                        squid_list[curr_ship_pos].pop_front();
                        squid_list[curr_ship_pos].push_back(bullet);
                        shooting_delay=0;
                    }
                }else if(key == (char)27){
                    system("CLS");
                    cout<< "BYE" <<endl;
                    return 0;
                }else{
                    key = (char)getch();
                    //UP:H
                    //DOWN:P
                    //Right:M
                    if(key == 'H')  //If Up
                    {
                        if(curr_ship_pos>0) 
                        {
                            ship_arr[curr_ship_pos] = ' ';  //remove ship icon from old position
                            curr_ship_pos--;    //decrememnt ship position
                            ship_arr[curr_ship_pos] = '>';  //add ship icon on new position
                        }
                    }else if(key == 'P') // If DOWN
                    {
                        if(curr_ship_pos<24)
                        {
                            ship_arr[curr_ship_pos] = ' ';
                            curr_ship_pos++;//same
                            ship_arr[curr_ship_pos] = '>';
                        }
                    }
                }
            }
            cout<<" "<<ship_arr[j]; //print ship location on screen
            temp = rand()%100;
            if(temp>threshold)  //check probability of squid
            {
                new_squid = ' ';
            }else{
                new_squid = (char)157;
            }
            kills+=squid_list[j].move_bullet();
            squid_list[j].push_front(new_squid);//add squid/no squid in lsit
            squid_list[j].print_list(); //print squid list
            squid = squid_list[j].pop_back();   //remove left squid
            if(squid == (char)157 && j==curr_ship_pos)    //if squid/ship crash game end
            {
                system("CLS");
                cout << "GAME END!"<<endl;
                cout << "Score:" << score << endl;
                cout << "Kill Bonus: " << kills*3 <<endl;
                cout << "Total: " << score+(kills*3)<<endl;
                return 0;
            }
            if(squid!=' ')
            {
                score++;
            }
        }
        _sleep(250);
        shooting_delay++;
    }
    return 0;
}