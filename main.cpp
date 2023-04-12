#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

#define KB_ESCAPE 27
#define KB_ENTER 13
#define KB_BACKSPACE 8

struct colors_t{
    HANDLE hstdout;
    int initial_colors;

    colors_t(){
        hstdout        = GetStdHandle( STD_OUTPUT_HANDLE );
        initial_colors = getcolors();
    }

    ~colors_t(){
        setcolors( initial_colors );
    }

    int getcolors() const{
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo( hstdout, &csbi );
        return csbi.wAttributes;
    }

    void setcolors(int color){
        SetConsoleTextAttribute( hstdout, color );
    }

    void setfg(int color){
        int current_colors = getcolors();
        setcolors( (current_colors & 0xF0) | (color & 0x0F) );
    }

    void setbg(int color){
        int current_colors = getcolors();
        setcolors( ((color & 0x0F) << 4) | (current_colors & 0x0F) );
    }

    int getfg() const { return  getcolors()    & 0x0F; }
    int getbg() const { return (getcolors() >> 4) & 0x0F; }
};
struct user{ // UNTUK MENAMPUNG DATA PLAYER
    string user_name;
    string user_level;
    double user_score;
    double user_words;
    double user_speed;
    double user_accuracy;
    int user_combo;
    int user_time;
};
enum {
    black,
    dark_blue,
    dark_green,
    dark_cyan,
    dark_red,
    dark_magenta,
    dark_yellow,
    light_gray,
    dark_gray,
    light_blue,
    light_green,
    light_cyan,
    light_red,
    light_magenta,
    light_yellow,
    white
};
unsigned int x_hours=0;
unsigned int x_minutes=0;
unsigned int x_seconds=0;
unsigned int x_milliseconds=0;
unsigned int totaltime=0,count_down_time_in_secs=0,time_left=0;

int xInput; // POSISI KETIK SEKARANG
int input_count=0; // JUMLAH HURUF YANG SUDAH DIKETIK
clock_t x_startTime,x_countTime; // VARIABLE DENGAN TIPE DATA clock_t
colors_t colors; // TEMP UNTUK MEMANGGIL STRUCT color_t

void gotoxy(int x, int y){
    COORD k = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), k);
}
void mainmenu(){
    cout<<"======================"<<endl;
    cout<<"|";colors.setfg( light_magenta );cout<<"   10 FAST FINGER   ";colors.setcolors( colors.initial_colors );cout<<"|"<<endl;
    cout<<"======================"<<endl;
    cout<<"|  1. PLAY GAME      |"<<endl;
    cout<<"|  2. SET TIMER      |"<<endl;
    cout<<"|  3. LIBRARY        |"<<endl;
    cout<<"|  4. HIGHSCORE      |"<<endl;
    cout<<"|  5. TUTORIAL BOOK  |"<<endl;
    cout<<"|  6. EXIT GAME      |"<<endl;
    cout<<"======================"<<endl;
    cout<<">>>";
}
void read(string nama_file,vector<string> &words){
    ifstream read(nama_file.c_str());
    string temp_baris="";
    string temp_kata="";

    if(read.is_open()){
        while(!read.eof()){
            getline(read,temp_baris);
            for(int i=0;i<temp_baris.size();i++){
                if(temp_baris[i]=='|'){
                    words.push_back(temp_kata);
                    temp_kata="";
                }
                else{
                    temp_kata+=temp_baris[i];
                }
            }
        }
        read.close();
    }
    else{
        system("cls");
        cout<<"========================"<<endl;
        cout<<"|THE FILE DOESN'T EXIST|"<<endl;
        cout<<"|     CREATING FILE    |"<<endl;
        cout<<"========================"<<endl;getch();
        ofstream write(nama_file.c_str());
        write.close();
    }
}
void write(string nama_file,vector<string> words,int check){
    ofstream write(nama_file.c_str());
    if(write.is_open()){
        for(int i=0;i<words.size();i++){
            write<<words[i];
            if(check==1 && i==words.size()){write<<"";}
            else{write<<"|";}
            if(i%10==9){write<<endl;}
        }
        write.close();
    }
    else{
        system("cls");
        cout<<"========================"<<endl;
        cout<<"|THE FILE DOESN'T EXIST|"<<endl;
        cout<<"========================"<<endl;getch();
    }
}
void list_words(vector<string> words){
    int ctr=0,tambah=0;
    for(int i=0;i<words.size()/2;i++){
        if(i<9){cout<<"0"<<i+1;}
        else{cout<<i+1;}
        cout<<". "<<words[i]<<endl;
        ctr++;
    }
    gotoxy(20,3);
    for(int i=words.size()/2;i<words.size();i++){
        if(i<9){cout<<"0"<<i+1;}
        else{cout<<i+1;}
        cout<<". "<<words[i]<<endl;tambah++;
        gotoxy(20,3+tambah);
        ctr++;
    }
}
void library(string nama_file,vector<string> words,int menu2,int menu3,int check){
    do{
        string kata;
        words.clear();
        read(nama_file,words);
        system("cls");
        cout<<"==============="<<endl;
        if(menu2==1){cout<<"|    EXPERT   |"<<endl;}
        else if(menu2==2){cout<<"|     HARD    |"<<endl;}
        else if(menu2==3){cout<<"|    NORMAL   |"<<endl;}
        else if(menu2==4){cout<<"|     EASY    |"<<endl;}
        cout<<"==============="<<endl;
        list_words(words);
        cout<<endl<<"============================================="<<endl;
        cout<<"1. ADD WORD"<<endl;
        cout<<"2. DELETE WORD"<<endl;
        cout<<"0. BACK"<<endl;
        cout<<">>>";cin>>menu3;
        if(menu3==1){ //ADD
            cout<<"INPUT WORDS : ";cin>>kata;
            if(menu2==1){
                if(kata.size()>=8){
                    words.push_back(kata);
                    write(nama_file,words,check);
                }
                else{cout<<"PANJANG KATA UNTUK EXPERT HARUS >=8 ";getch();}
            }
            else if(menu2==2){
                if(kata.size()>=6){
                    words.push_back(kata);
                    write(nama_file,words,check);
                }
                else{cout<<"PANJANG KATA UNTUK HARD HARUS >=6 ";getch();}
            }
            else if(menu2==3){
                if(kata.size()>=2 && kata.size()<=8){
                    words.push_back(kata);
                    write(nama_file,words,check);
                }
                else{cout<<"PANJANG KATA UNTUK NORMAL HARUS 2-8 ";getch();}
            }
            else if(menu2==4){
                if(kata.size()<=5){
                    words.push_back(kata);
                    write(nama_file,words,check);
                }
                else{cout<<"PANJANG KATA UNTUK EASY HARUS <=5 ";getch();}
            }
        }
        else if(menu3==2){ // DELETE
            int temp;
            cout<<"DELETE WORDS : ";cin>>kata;
            for(int i=0;i<words.size();i++){
                if(kata==words[i]){temp=i;check=1;}
            }
            if(check==1){
                words.erase(words.begin()+temp);
                colors.setfg( light_magenta );
                cout<< kata;
                colors.setcolors( colors.initial_colors );
                cout<< " DELETED SUCCESSFULY..."<<endl;getch();
                write(nama_file,words,check);
                check=0;
            }
            else{
                cout<< "WORDS NOT FOUND..."<<endl;
                getch();
            }
        }
    }while(menu3!=0);
}
void write_user(vector<user> player){
    ofstream write("user.txt");
    if(write.is_open()){
        for(int i=0;i<player.size();i++){
            write<<player[i].user_name<<endl;
            write<<player[i].user_level<<endl;
            write<<player[i].user_score<<endl;
            write<<player[i].user_words<<endl;
            write<<player[i].user_speed<<endl;
            write<<player[i].user_accuracy<<endl;
            write<<player[i].user_combo<<endl;
            write<<player[i].user_time<<endl;
        }
        write.close();
    }
    else{
        cout<<"FILE DOESN'T EXIST";
    }
}
void read_user(vector<user> &player){
    ifstream read("user.txt");
    string temp_bar;
    double temp_double;
    int temp_integer;
    user temp;

    if(read.is_open()){
        while(!read.eof()){
            getline(read,temp_bar);
            if(temp_bar!=""){
                temp.user_name=temp_bar;

                getline(read,temp_bar);
                temp.user_level=temp_bar;

                getline(read,temp_bar);
                temp_integer=stoi(temp_bar);
                temp.user_score=temp_integer;

                getline(read,temp_bar);
                temp_integer=stoi(temp_bar);
                temp.user_words=temp_integer;

                getline(read,temp_bar);                temp_double=stod(temp_bar);
                temp.user_speed=temp_double;

                getline(read,temp_bar);
                temp_double=stod(temp_bar);
                temp.user_accuracy=temp_double;

                getline(read,temp_bar);
                temp_integer=stoi(temp_bar);
                temp.user_combo=temp_integer;

                getline(read,temp_bar);                temp_integer=stoi(temp_bar);
                temp.user_time=temp_integer;

                player.push_back(temp);
            }
        }
        read.close();
    }
    else{
        ofstream write("user.txt");
        write.close();
    }
}
void start_timer(){//PROCEDURE UNTUK MENGAMBIL WAKTU SAAT INI UNTUK DIJADIKAN SUMBU
    x_startTime=clock();  // start clock
}
void delta_time_update_timer(int xInput){//UPDATE TIMER
    x_countTime=clock(); // update timer difference
    x_milliseconds=x_countTime-x_startTime;
    x_seconds=(x_milliseconds/(CLOCKS_PER_SEC))-(x_minutes*60);
    x_minutes=(x_milliseconds/(CLOCKS_PER_SEC))/60;
    x_hours=x_minutes/60;
    time_left=count_down_time_in_secs-x_seconds;   // update timer

    gotoxy(13,7);
    printf( "%d", time_left,count_down_time_in_secs); //Menampilkan sisa waktu ke UI
    if(time_left<10){gotoxy(14,7);cout<<" ";}
    gotoxy(11+xInput,7);
}
void status(string level,string name,int score,int xInput,double jum_huruf,int cek_warna[10],int ctr,int &combo,int &max_combo){
    cout<<"========================"<<endl;
    cout<<"|        STATUS        |"<<endl;
    cout<<"========================"<<endl;
    cout<<"| LEVEL    : ";colors.setfg( light_red );cout<<level;colors.setcolors( colors.initial_colors );gotoxy(23,3);cout<<"|"<<endl;
    cout<<"| NAME     : "<<name;gotoxy(23,4);cout<<"|"<<endl;
    cout<<"| SCORE    : ";colors.setfg( light_cyan );cout<<score;colors.setcolors( colors.initial_colors );gotoxy(23,5);cout<<"|"<<endl;
    cout<<"| WORDS    : "<<jum_huruf;gotoxy(23,6);cout<<"|======================="<<endl;
    cout<<"| TIMER    : ";delta_time_update_timer(xInput);gotoxy(23,7);cout<<"|";
    if(cek_warna[ctr-1]==1){
        gotoxy(24,7);
        colors.setfg( light_yellow );
        cout<<" EXCELENT";
        colors.setcolors( colors.initial_colors );
        combo++;
        if(combo>max_combo){max_combo=combo;}
    }
    else if(cek_warna[ctr-1]==2){
        gotoxy(24,7);
        colors.setfg( dark_magenta );
        cout<<"    BAD";
        colors.setcolors( colors.initial_colors );
        combo=0;
    }
    gotoxy(34,7);cout<<"| COMBO : ";colors.setfg( light_magenta );cout<<combo;colors.setcolors( colors.initial_colors );
    gotoxy(46,7);cout<<"|"<<endl;
}
void gameplay(int difficulty,string nama_file,vector<string> words,string name,int xInput,double secs,vector<user> &player){
    string soal[10],level;
    vector<char> ketik; //MENANMPUNG JAWABAN USER
    double benar=0,salah=0,speed,accuracy;
    int HT1=0,cek_warna[10],combo=0,ctr=0,max_combo=0,score=0,jum_huruf=0;

    for(int i=0;i<10;i++){ //KOSONGIN WARNA
        cek_warna[i]=0;
    }

    if(difficulty==1){level="EXPERT";}
    else if(difficulty==2){level="HARD";}
    else if(difficulty==3){level="NORMAL";}
    else if(difficulty==4){level="EASY";}

    read(nama_file,words);
    srand(time(0));
    system("cls");
    int x[10];

    for(int i=0;i<10;i++){// RANDOM SOAL
        for(int j=0;j<10;j++){
            if(x[i]==x[j]){
                x[i]=rand()%words.size();
            }
        }
        soal[i]=words[x[i]];
    }

    // START TIMER
    count_down_time_in_secs= secs;
    start_timer();
    delta_time_update_timer(xInput);

    do{ //GAME START
        system("cls");
        status(level,name,score,xInput,jum_huruf,cek_warna,ctr,combo,max_combo);
        for(int i=0;i<10;i++){
            for(int j=0;j<soal[i].size();j++){
                cout<<"=";
            }
            cout<<"=";
        }
        cout<<"="<<endl<<"|";
        for(int i=0;i<10;i++){
            if(cek_warna[i]==1){
                colors.setfg( light_green );
                for(int j=0;j<soal[i].size();j++){
                    cout<<soal[i][j];
                }
                colors.setcolors( colors.initial_colors );
            }
            else if(cek_warna[i]==2){
                colors.setfg( light_red );
                for(int j=0;j<soal[i].size();j++){
                    cout<<soal[i][j];
                }
                colors.setcolors( colors.initial_colors );
            }
            else{
                cout<<soal[i];
            }
            if(i<9) {cout<<" ";}
        }
        cout<<"|"<<endl<<"=";
        for(int i=0;i<10;i++){
            for(int j=0;j<soal[i].size();j++){
                cout<<"=";
            }
            cout<<"=";
        }
        cout<<endl;
        cout<<"TYPE NOW : ";
        bool submit = false;
        do {
            delta_time_update_timer(xInput);
            if (kbhit()) {
                int KeyBoard_code;
                KeyBoard_code = getch();

                if(KeyBoard_code!=8 && KeyBoard_code!=13){
                    xInput++;
                    gotoxy(11+xInput,11);
                    cout <<(char)KeyBoard_code;
                    ketik.push_back((char)KeyBoard_code);
                }
                else if(KeyBoard_code==8){
                    if(ketik.size()==0){
                        ketik.clear();
                    }
                    else{
                        ketik.erase(ketik.end()-1);
                    }
                }
                else if(KeyBoard_code==13 && ketik.size()>0){
                    ketik.erase(ketik.end()-1);
                }
                else if(KeyBoard_code==13 && ketik.size()==0){
                    ketik.push_back((char)KeyBoard_code);
                }

                switch(KeyBoard_code)
                {
                    case KB_ENTER: // KETIKA TEKAN ENTER
                        xInput=0;
                        input_count=0;
                        delta_time_update_timer(xInput);

                        gotoxy(11+xInput,11);
                        cout << "                               ";
                        gotoxy(11+xInput,11);
                        submit = true;
                        break;

                    case KB_BACKSPACE: // KETIKA TEKAN BACKSPACE
                        if(ketik.size()>=0){
                            gotoxy(11+xInput,11);
                            cout<<" ";
                            if(ketik.size()>0){
                                xInput--;
                            }
                        }
                        break;
                }
            }
        }while(time_left>0 && submit==false);

        for(int i=0;i<soal[ctr].size();i++){// CHECKING BENAR SALAH
            if(ketik[i]==soal[ctr][i]){
                cout<<ketik[i];
                benar+=1;
                HT1+=1;
                if(HT1==soal[ctr].size()){
                    cek_warna[ctr]=1;
                    score+=soal[ctr].size();
                    jum_huruf+=1;
                    HT1=0;
                }
                else if(HT1<0 && i==soal[ctr].size()-1){HT1=0;}
            }
            else{
                cek_warna[ctr]=2;
                salah+=1;
                HT1=-99;
                if(HT1<0 && i==soal[ctr].size()-1){HT1=0;}
            }
        }
        /* PENGECEKAN ISI INPUTAN
        gotoxy(11+xInput,14);
        for(int i=0;i<=ketik.size();i++){
            cout << ketik[i];
        }getch();
        */
        ketik.clear();
        ctr++;

        if(ctr==10){// JIKA SUDAH 10 HURUF
            for(int i=0;i<10;i++){// RANDOM SOAL
                cek_warna[i]=0;
                for(int j=0;j<10;j++){
                    if(x[i]==x[j]){
                        x[i]=rand()%words.size();
                    }
                }
                soal[i]=words[x[i]];
            }
            ctr=0;
        }
    }while(time_left>0);

    speed=score/((secs/60)*5); // 5 CPM = 1 WPM
    accuracy=(benar/(benar+salah))*100;
    int tekan;
    do{ //RESULT
        system("cls");
        cout<<"=========================="<<endl;
        cout<<"|         RESULT         |"<<endl;
        cout<<"=========================="<<endl;
        cout<<"|  LEVEL    : ";colors.setfg( light_red );cout<<level;colors.setcolors( colors.initial_colors );gotoxy(25,3);cout<<"|"<<endl;
        cout<<"|  NAME     : "<<name;gotoxy(25,4);cout<<"|"<<endl;
        cout<<"|  SCORE    : ";colors.setfg( light_cyan );cout<<score;colors.setcolors( colors.initial_colors );gotoxy(25,5);cout<<"|"<<endl;
        cout<<"|  WORDS    : ";colors.setfg( light_magenta );cout<<jum_huruf;colors.setcolors( colors.initial_colors );gotoxy(25,6);cout<<"|"<<endl;
        cout<<"|  TIME     : "<<secs<<" seconds";gotoxy(25,7);cout<<"|"<<endl;
        cout<<"|  SPEED    : ";colors.setfg( light_green );cout<<setprecision(3)<<speed<<" WPM";colors.setcolors( colors.initial_colors );gotoxy(25,8);cout<<"|"<<endl;
        cout<<"|  ACCURACY : ";colors.setfg( light_yellow );cout<<accuracy<<" %";colors.setcolors( colors.initial_colors );gotoxy(25,9);cout<<"|"<<endl;
        cout<<"=========================="<<endl;
        cout<<"Press 'ESCAPE' to continue..."<<endl;
        tekan=getch();
    }while(tekan!=27);

    // SAVE SCORE
    user temp;
    temp.user_name=name;
    temp.user_level=level;
    temp.user_score=score;
    temp.user_words=jum_huruf;
    temp.user_speed=speed;
    temp.user_accuracy=accuracy;
    temp.user_combo=max_combo;
    temp.user_time=secs;
    player.push_back(temp);
    write_user(player);
    // PENGOSONGAN VARIABEL
    name="";score=0;jum_huruf=0;secs=60;ctr=0;benar=0;salah=0;accuracy=0;combo=0;
    for(int i=0;i<10;i++){
        cek_warna[i]=0;
    }
}
void intro(){
    ifstream read("intro.txt");
    vector<string> text;
    string temp;

    if(read.is_open()){
        while(!read.eof()){
            getline(read,temp);
            text.push_back(temp);
        }
        read.close();
        colors.setfg( light_yellow );cout<<"                  Hello there user! :)"<<endl;
        for(int i=0;i<text.size();i++){
            for(int j=0;j<text[i].size();j++){
                cout<< text[i][j];
                Sleep(10);
            }
            cout<<endl;
        }
        colors.setcolors( colors.initial_colors );
        cout<<endl<<endl<<"Loading Game..."<<endl;Sleep(1200);
        cout<<"Press any key to continue...";getch();
    }
    else{
        ofstream write("intro.txt");
        write.close();
    }
}
void highscore_list(vector<user> player){
    cout<<"========================================================================================================="<<endl;
    cout<<"|                                            LEADERBOARD                                                |"<<endl;
    cout<<"========================================================================================================="<<endl;
    cout<<"|  RANK  |    NAME    |  LEVEL  |  SCORE  |  WORDS  |   SPEED   |  ACCURACY  |  MAX COMBO  |    TIMER   |"<<endl;
    cout<<"========================================================================================================="<<endl;

    for(int i=0;i<player.size();i++){
        cout<<"     "<<i+1;gotoxy(16-player[i].user_name.size()/2,5+i);
        cout<<player[i].user_name<<" ";gotoxy(28-player[i].user_level.size()/2,5+i);
        colors.setfg( light_red );cout<<player[i].user_level;gotoxy(36,5+i);colors.setcolors( colors.initial_colors );
        colors.setfg( light_cyan );cout<<player[i].user_score;gotoxy(47,5+i);colors.setcolors( colors.initial_colors );
        colors.setfg( light_magenta );cout<<player[i].user_words;gotoxy(55,5+i);colors.setcolors( colors.initial_colors );
        colors.setfg( light_green ); cout<<player[i].user_speed<<" WPM";gotoxy(69,5+i);colors.setcolors( colors.initial_colors );
        colors.setfg( light_yellow );cout<<setprecision(4)<<player[i].user_accuracy<<" %";gotoxy(84,5+i);colors.setcolors( colors.initial_colors );
        colors.setfg( dark_magenta );cout<<player[i].user_combo;gotoxy(95,5+i);colors.setcolors( colors.initial_colors );
        colors.setfg( dark_cyan );cout<<player[i].user_time<<" secs"<<endl;colors.setcolors( colors.initial_colors );
    }
    cout<<"========================================================================================================="<<endl;
}
void sorting_speed(vector<user> &player){
    for(int i=0;i<player.size()-1;i++) {
        for(int j=i+1; j<player.size();j++){
            if(player[i].user_speed < player[j].user_speed){
                swap(player[i], player[j]);
            }
        }
    }
}
void sorting_score(vector<user> &player){
    for(int i=0;i<player.size()-1;i++) {
        for(int j=i+1; j<player.size();j++){
            if(player[i].user_score < player[j].user_score){
                swap(player[i], player[j]);
            }
        }
    }
}
void sorting_words(vector<user> &player){
    for(int i=0;i<player.size()-1;i++) {
        for(int j=i+1; j<player.size();j++){
            if(player[i].user_words < player[j].user_words){
                swap(player[i], player[j]);
            }
        }
    }
}
void sorting_accuracy(vector<user> &player){
    for(int i=0;i<player.size()-1;i++) {
        for(int j=i+1; j<player.size();j++){
            if(player[i].user_accuracy < player[j].user_accuracy){
                swap(player[i], player[j]);
            }
        }
    }
}
void sorting_combo(vector<user> &player){
    for(int i=0;i<player.size()-1;i++) {
        for(int j=i+1; j<player.size();j++){
            if(player[i].user_combo < player[j].user_combo){
                swap(player[i], player[j]);
            }
        }
    }
}
void tutorial(){
    ifstream read("tutorial.txt");
    vector<string> text;
    string temp;

    if(read.is_open()){
        while(!read.eof()){
            getline(read,temp);
            text.push_back(temp);
        }
        read.close();

        for(int i=0;i<25;i++){
            for(int j=0;j<50;j++){
                if(i==0 || i==24){cout<<"=";}
                else if(j==0||j==49){cout<<"|";}
                else {cout<<" ";}
            }
            cout<<endl;
        }
        gotoxy(21,25);
        colors.setfg( light_green );
        cout<<"Page 1";
        colors.setcolors( colors.initial_colors );

        for(int i=0;i<text.size();i++){
            colors.setfg( light_yellow );
            gotoxy(2,1+i);
            for(int j=0;j<text[i].size();j++){
                cout<< text[i][j];
                Sleep(10);
            }
            cout<<endl;
            if(i==3 || i==7 || i==12 || i==21){
                gotoxy(2,2+i);
                colors.setfg( white );
                cout<<"Press any key to continue...";
                getch();
                gotoxy(2,2+i);
                cout<<"                            ";
            }
        }
        colors.setcolors( colors.initial_colors );
    }
    else{
        ofstream write("tutorial.txt");
        write.close();
    }
}

int main()
{
    vector<user> player; // VECTOR UNTUK DATA PLAYER
    vector<string> words,username;
    string name,nama_file,level,temp_word;
    int menu,difficulty,temp,check;
    double secs=60;

    intro();
    read_user(player);
    do{
        system("cls");
        //MENU UTAMA
        mainmenu();
        cin>>menu;
        if(menu==1){ //PLAY GAME
            words.clear();
            system("cls");
            cout<<"================================="<<endl;
            cout<<"  INSERT NAME : ";cin>>name;
            cout<<"================================="<<endl;
            cout<<"|";colors.setfg( light_red );cout<<"1.EXPERT";colors.setcolors( colors.initial_colors );
            cout<<"|";colors.setfg( light_magenta );cout<<"2.HARD";colors.setcolors( colors.initial_colors );
            cout<<"|";colors.setfg( light_yellow );cout<<"3.NORMAL";colors.setcolors( colors.initial_colors );
            cout<<"|";colors.setfg( light_green );cout<<"4.EASY";colors.setcolors( colors.initial_colors );
            cout<<"|"<<endl;
            cout<<"================================="<<endl;
            cout<<"CHOOSE DIFFICULTY : ";cin>>difficulty;

            for(int i=3;i>0;i--){
                system("cls");
                cout<<"GAME START IN "<<i<<endl;
                cout<<"HOLD YOUR FINGER..."<<endl;
                Sleep(1000);
            }

            if(difficulty==1){
                nama_file="expert.txt";
                gameplay(difficulty,nama_file,words,name,xInput,secs,player);
            }
            else if (difficulty==2){
                nama_file="hard.txt";
                gameplay(difficulty,nama_file,words,name,xInput,secs,player);
            }
            else if (difficulty==3){
                nama_file="normal.txt";
                gameplay(difficulty,nama_file,words,name,xInput,secs,player);
            }
            else if (difficulty==4){
                nama_file="easy.txt";
                gameplay(difficulty,nama_file,words,name,xInput,secs,player);
            }
        }
        else if(menu==2){// SET TIMER
            cout<< "HOW MANY SECONDS DO YOU WANT TO PLAY ? ";cin>>secs;
            cout<< "PLAY TIME HAS BEEN SET TO ";colors.setfg( light_green );cout<<secs;colors.setcolors( colors.initial_colors );cout<<" seconds"<<endl;
            cout<< "YOUR WPM WILL BE CALCULATED AS : "<<endl;
            cout<< "============================"<<endl;
            cout<< "|";colors.setfg( light_yellow );cout<<"(SCORE/((PLAY_TIME/60)*5))";colors.setcolors( colors.initial_colors );cout<<"|"<<endl;
            cout<< "============================"<<endl;
            system("pause");
        }
        else if(menu==3){// LIBRARY
            int menu2,menu3;
            words.clear();
            system("cls");
            cout<<"==============="<<endl;
            cout<<"|   LIBRARY   |"<<endl;
            cout<<"==============="<<endl;
            cout<<"|  1. EXPERT  |"<<endl;
            cout<<"|  2. HARD    |"<<endl;
            cout<<"|  3. NORMAL  |"<<endl;
            cout<<"|  4. EASY    |"<<endl;
            cout<<"|  0. BACK    |"<<endl;
            cout<<"==============="<<endl;
            cout<<">>>";cin>>menu2;

            if(menu2==1){ //EXPERT
                string kata;
                nama_file="expert.txt";
                library(nama_file,words,menu2,menu3,check);
            }
            else if(menu2==2){ //HARD
                string kata;
                nama_file="hard.txt";
                library(nama_file,words,menu2,menu3,check);
            }
            else if(menu2==3){ //NORMAL
                string kata;
                nama_file="normal.txt";
                library(nama_file,words,menu2,menu3,check);
            }
            else if(menu2==4){ //EASY
                string kata;
                nama_file="easy.txt";
                library(nama_file,words,menu2,menu3,check);
            }
        }
        else if(menu==4){// HIGHSCORE
            int pilih=1;
            do{
                system("cls");
                if(pilih==1){sorting_speed(player);}
                else if(pilih==2){sorting_score(player);}
                else if(pilih==3){sorting_words(player);}
                else if(pilih==4){sorting_accuracy(player);}
                else if(pilih==5){sorting_combo(player);}
                highscore_list(player);
                cout<<"SORT BERDASARKAN : 1.SPEED | 2.SCORE | 3. WORDS | 4.ACCURACY | 5.COMBO | 0.BACK"<<endl;
                cout<<">>>";cin>>pilih;
            }while(pilih!=0);
        }
        else if(menu==5){// TUTORIAL
            system("cls");
            tutorial();
        }
        else if(menu==6){// EXIT
            int x=1,dx=1,y=1,random;
            system("cls");
            cout<<"=============================================="<<endl<<"|";
            gotoxy(45,1);cout<<"|"<<endl;
            gotoxy(0,2);
            cout<<"=============================================="<<endl;
            gotoxy(0,3);cout<<"Press any key to exit...";
            srand(time(0));
            do{
                gotoxy(x,y);
                cout<<"~THANKS FOR PLAYING~"<<endl;
                Sleep(100);
                gotoxy(x,y);
                cout<<"                    "<<endl;
                x+=dx;
                if(x>=25){
                    dx=-1;
                    random=rand()%4+1;
                    if(random==1){colors.setfg( light_yellow );}
                    else if(random==2){colors.setfg( light_green );}
                    else if(random==3){colors.setfg( light_magenta );}
                }
                else if(x<=1){
                    dx=1;
                    random=rand()%4+1;
                    if(random==1){colors.setfg( light_red );}
                    else if(random==2){colors.setfg( light_cyan );}
                    else if(random==3){colors.setfg( light_blue );}
                }
            }while(!kbhit());
        }
    }while(menu!=6);
    return 0;
}
