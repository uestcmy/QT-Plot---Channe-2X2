#include <QApplication>


#include <channelh11.h>
#include <qfile.h>
#include <subwindow.h>
#include <QDebug>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <GL/glut.h>
//#define SHOW
#define DEBUG84
#define CHANNEL
#define LEN 9000
#define LENGTH_OF_CORE 84
//#define SOCKET
int flag = 199;

void search_core_data(char *buff);
void search_core_data2(char *buff);
int continueflag = 0;
int rest_num_comma = 0;
char out[LEN*5];
int outposition = 0;
int core_data_state = 0;//initail

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    qDebug() << "This is in the MAIN class,the flag is : "<< flag << endl;

    char buff[2000];
    socklen_t size;

    sockaddr_in addrSrv,addrrcv;
    int sockser;
    long int num_frame = 0;

     size=sizeof(sockaddr_in);
     sockser=socket(AF_INET,SOCK_DGRAM,0);
     addrSrv.sin_addr.s_addr=htonl(INADDR_ANY);
     addrSrv.sin_family=AF_INET;
     addrSrv.sin_port=htons(6000);//server : receive port number
     bind(sockser,(sockaddr*)&addrSrv,sizeof(sockaddr));

     QFile file("./dataout/data2.txt");
     if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
         qDebug() << file.errorString();
     //char str[] = "Hello world!\n";


#ifdef CORE
/*
     mygl2 w1;
     w1.resize(800,600);
     w1.show();
     w1.setWindowTitle("CORE STATE");
     */
/*
     NetworkLink w5;
     w5.resize(600,400);
     w5.show();
*/
#endif

     SubWindow w1;
     w1.resize(1280,1080);
     w1.show();
     w1.setWindowTitle("Channel Gain");




#ifdef SHOW
   //2*3 windows

    MultiWindow w;
    w.resize(1300,768);
    w.show();
    w.setWindowTitle("State");


/*
    QApplication app(argc,argv);
    mygl2 w;
    w.resize(600,450);
    w.show();
    w.setWindowTitle("FPGA Resource State");

    ChannelCapacity w2;
    w2.resize(600,450);
    w2.show();
    w2.setWindowTitle("Channel Capacity");

    QPSK w3;
    w3.resize(600,450);
    w3.show();
    w3.setWindowTitle("QPSK Star");

*/
    ChannelH11 w41;
    w41.resize(600,450);
    w41.show();
    w41.setWindowTitle("Channel State H11");
    ChannelH12 w42;
    w42.resize(600,450);
    w42.show();
    w42.setWindowTitle("Channel State H12");
    ChannelH21 w43;
    w43.resize(600,450);
    w43.show();
    w43.setWindowTitle("Channel State H21");
    ChannelH22 w44;
    w44.resize(600,450);
    w44.show();
    w44.setWindowTitle("Channel State H22");

    NetworkLink w5;
    w5.resize(600,400);
    w5.show();


    ChCap w6;
    w6.resize(600,450);
    w6.show();
    w6.setWindowTitle("Channel Capacity");
#endif
    return app.exec();

}



void search_core_data(  char* buff ){
    int i;
    int find = 0;//consider that there is no header?
    for( i = 0 ; i < LEN - 10 ; i++ ){
        //search the header of the core-frame
        //if( buff[i] == 'a' && buff[i+1] == 'a' && buff[i+2] == ',' && buff[i+3] == 'a' && buff[i+4] == '0'&& buff[i+5] == ','  && buff[i+6] == '2' && buff[i+7] == '0' && buff[i+8] == ',' && buff[i+9] == '3' && buff[i+10] == 'c'){ // && buff[i+6] == '2' && buff[i+7] == '0' && buff[i+8] == ',' && buff[i+9] == '3' && buff[i+10] == 'c'

#ifdef DEBUG84
        if(buff[i] == 'a' && buff[i+1] == 'a' && buff[i+2] == ',' && buff[i+3] == 'a' && buff[i+4] == '0' && buff[i+5] == ',' && buff[i+6] == '2' && buff[i+7] == '8' && buff[i+8] == ',' && buff[i+9] == '4' && buff[i+10] == '0'){
#endif

#ifdef DEBUG14404

        if(buff[i] == 'a' && buff[i+1] == 'a' && buff[i+2] == ',' && buff[i+3] == 'a' && buff[i+4] == '0' && buff[i+5] == ',' && buff[i+6] == '2' && buff[i+7] == '0' && buff[i+8] == ',' && buff[i+9] == '3' && buff[i+10] == 'c'){
#endif
            //qDebug() << buff <<endl;//the length is LEN ,  very long ...
            //qDebug() << "-------------------------------------------------------------------------------------------------" << endl;
            find = 1;
            break;
        }
    }//for
    if( find == 0 ){
        return ;
    }
    char bottle[LEN] = {0};
    int cnt = 0;
    int cnt_comma = 0;
    outposition = 0;
    for( int j = i ;  ; j++){
        if( buff[j] != '-' && buff[j] != '\n'){
            bottle[cnt++]  = buff[j];
            //out[outposition++] = buff[j];
        }//if
        if( buff[j] ==',' && buff[j+2] == ',' ){

        }//if
        if( buff[j] == ',' ){
            cnt_comma ++;
        }

        if( cnt_comma == LENGTH_OF_CORE ){
           rest_num_comma = 0;
           continueflag = 0;
           break;
        }
        else if( j == LEN){
            rest_num_comma = LENGTH_OF_CORE - cnt_comma;
            continueflag = 1; //there is still some frames in the later buffer
            break;
        }
    }//for j
    //qDebug() <<"FIRST :" <<bottle <<endl;
    strcat(out,bottle);
}

void search_core_data2(  char* buff ){ // continue to send the data

    char bottle[LEN] = {0};
    int cnt = 0;
    int cnt_comma = 0;

    for( int j = 0 ;  ; j++){
        if( buff[j] != '-' && buff[j] != '\n'){
            bottle[cnt++]  = buff[j];
            //out[outposition++] = buff[j];
        }//if
        if( buff[j] == ',' ){
            cnt_comma ++;
            rest_num_comma --;
        }
        if( rest_num_comma == 0){// the end of the later frame
            continueflag = 0;
            //qDebug() << "rest is zero."<<endl;
            break;
        }
        else if( rest_num_comma > 0 && j == LEN){
            continueflag = 1; //there is still some frames in the later buffer
            break;
        }
    }//for j
    //qDebug() <<" (next)" <<bottle ;
    strcat(out,bottle);
}

