#include <iostream>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdarg.h>
using namespace std;
int main(int argc, char** argv) {
    string obj="",freq="",proc="",bsize="";
    proc=argv[1];
    bsize=argv[2];
    freq=argv[3];
    obj=argv[4];
    pid_t pid = fork();
    //while(1) {
        if (pid == 0)
        {
            //char *args[]={argv[1],argv[2]}; 
            //execvp("./bcast",args); 
            clock_t start = clock();
            string s = "./bcast";
            //string cmd= "mpiexec -n " + proc + " ./" + obj + " " + bsize + " " + freq;
            //string cmd1= "mpiexec -n " + proc + " ./" + obj + " ";
            /*const char *command = cmd.c_str(); 
            char *c = (char*)(cmd1.c_str()); */
            char *b = (char*)(bsize.c_str()); 
            char *f = (char*)(freq.c_str()); 
            char *str = (char*)(s.c_str()); 
            char *args[]= {"./bcast",b,f,NULL};
            //system(command);
            cout<<"Child"<<endl;  
            int r = execvp(args[0], args);
            cout<<"r "<<r<<endl;
            //usleep((1000/stoi(freq) - ((clock()-start)/ (double)(CLOCKS_PER_SEC / 1000)))*1000);
            cout<<"Exit"<<endl;
            //exit(-1);
        }
        else if (pid > 0)
        {
            //cout<<"Parent process"<<endl;
        }
        else
        {
            cout<<"Fork failed"<<endl;
            return 1;
        }
    //}
    
    return 0;
}