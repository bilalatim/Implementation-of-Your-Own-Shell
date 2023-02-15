#include <iostream>
#include <fstream>      
#include <sys/wait.h>  
#include <regex>        
#include <fcntl.h>      

using namespace std;    


// run ls command
void listdir_com(){

    execlp("/bin/ls", "ls ", NULL);                 

}

// run hostname command
void mycomputername_com(){
    
    execlp("/bin/hostname", "hostname", NULL);      

}

// run current id command
void whatsmyip_com(){

    execlp("/bin/hostname", "hostname ", "-I", NULL); 

}

// print file with cat command
void printfile_com(string command, regex printfile) {

    ifstream file;
    string text;

    smatch sm;
    regex_search(command, sm, printfile);   //parse command

    file.open(sm.str(1).c_str());           //open file
    while(getline(file, text)){             //read file line by line

        cout<<text;
        getline(cin, command);              //wait for press enter

    }

}

// print other file directly with cat command
void printtofile_com(string command, regex printtofile) {

    smatch sm;
    regex_search(command, sm, printtofile);                                         //parse command

    int second_file = open(sm.str(2).c_str(), O_CREAT| O_WRONLY | O_APPEND , S_IWGRP | S_IWUSR | S_IRGRP | S_IRUSR);       // create or open file and can write file
    dup2(second_file, 1);                                                           // duplicate
    execlp("/bin/cat", "cat ", sm.str(1).c_str() , NULL);                           // run cat command to write first file to second file

}

//check if there any same command in history 
void dididothat_com(string command, regex dididothat, string history[]){

    smatch sm;
    regex_search(command, sm, dididothat);
    for(int i = 0; i < 15; i++){
        if (sm.str(1).c_str() == history[i]){
            cout<<"Yes"<<endl;                  // prints yes if there is a same command in history
            return;
        }
    

    }
    cout<<"No"<<endl; // prints no if there is not same command in history
}

//open default txt aplication
void hellotext_com(){
    system("gedit");
}

//save command to history
void save_hist(string command, string history[]){

    for(int i = 0; i < 15 ; i++){               //put command if there is an empty place
        if(history[i] == ""){
            history[i] = command;
            break;
        }
    }
    if(history[14] != ""){                      //check if history is full
        for (int i = 0; i <14 ; i++){
            history[i] = history[i+1];
        }
        history[14] = command;
    }
}
 

int main (){

    string history[15];     // keeps commands history

    smatch sm;                          // regex match value

    regex listdir("\\s*listdir\\s*");                   // listdir command
    regex mycomputername("\\s*mycomputername\\s*");     // mycomputername command
    regex whatsmyip("\\s*whatsmyip\\s*");               // whatsmyip command
    regex printfile("\\s*printfile\\s+(\\S+)\\s*");     // printfile command
    regex printtofile("\\s*printfile\\s+(\\S+)\\s+>\\s+(\\S+)\\s*"); // printfile > command
    regex dididothat("\\s*dididothat\\s+([\\S\\s]+)");   // dididothat command   
    regex hellotext("\\s*hellotext\\s*");               // hellotext command
    regex exit("\\s*exit\\s*");                         // exit command

    while(true){

        cout<<getenv("LOGNAME")<<" >>> ";   // print bilal >>>

        string command;                     // store input
        
        getline(cin, command);              // get input

        

        pid_t pid;
        pid = fork(); // creates child process

        if (pid < 0){

            printf("Fork Failed");      //if process fail

        }

        // if child process
        else if(pid == 0){ 

            if ( regex_match( command, listdir)){               //check command if listdir

                listdir_com();
            }

            else if ( regex_match( command, mycomputername)){   //check command if mycomputername

                mycomputername_com();
            }

            else if ( regex_match( command, whatsmyip)){        //check command if whatsmyip

                whatsmyip_com();
            }

            else if ( regex_match( command, printfile)){        //check command if printfile

                printfile_com(command, printfile);
                return 0;
                
            }
            else if ( regex_match( command, printtofile)){      //check command if printfile file1 > file2
                
                printtofile_com(command, printtofile);

            }
            else if ( regex_match( command, dididothat)){       //check command if dididothat
                
                dididothat_com(command, dididothat, history);
                return 0;
            }   
            else if (regex_match( command, hellotext)){         //check command if hellotext

                hellotext_com();
                return 0;
            }   
            
            else if ( regex_match( command, exit)){              // check command if exit
                return 0;
            }

            else{                                               // if it is not a valid command

                cout<<command<<">>> invalid command"<<endl;
                return 0;
            }


        }
        //parent process
        else{

            wait(NULL);                             // waits for its child
            if ( regex_match( command, exit)){      // if command is exit
                return 0;
            }
        }
        save_hist(command, history);                //call save history function


    }
    return 0;
}
