#include "channelh12.h"
#include "ui_channelh12.h"
#include <QDebug>




#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include "QDebug"
#include <QtGui/QApplication>
#define LEN 9000
#define LENGTH_OF_OFDM 14404
//#define TEST_SOCKET

socklen_t size_chl2;
int cnt2 = 0;
sockaddr_in addrSrv_chl2,addrrcv_chl2;
int sockser_chl2;
int pilot2[100][2] = {0};
char map1800_2[1800][16];
double data1_2[1800][2] = {0};//complex number real and image
double data2_2[1800][2] = {0};


ChannelH12::ChannelH12(QWidget *parent) :
    QGLWidget(parent),
    ui(new Ui::ChannelH12)
{
    extern int flag;
    qDebug() << "This is in the CHANNEL11 class,the flag is : "<< flag << endl;
    ui->setupUi(this);
    translate = -6.0;
    xRot = yRot = zRot = 0.0;
    xRot += 10;
    yRot += 340;


    size_chl2=sizeof(sockaddr_in);
    sockser_chl2=socket(AF_INET,SOCK_DGRAM,0);
    addrSrv_chl2.sin_addr.s_addr=htonl(INADDR_ANY);
    addrSrv_chl2.sin_family=AF_INET;
    addrSrv_chl2.sin_port=htons(7002);//server : receive port number
    bind(sockser_chl2,(sockaddr*)&addrSrv_chl2,sizeof(sockaddr));

#ifdef TEST_SOCKET
    char str[14404*3];
    recvfrom(sockser_chl1,&str,14404*3,0,(struct sockaddr *)&addrrcv_chl1,(socklen_t*)&size_chl1);
    qDebug() << str << endl;
#endif

    InputManagement();
    id1 = startTimer(50);

}

ChannelH12::~ChannelH12()
{
    delete ui;
}

void ChannelH12::initializeGL()
{
    glClearColor(0.8,0.8,0.8,0);//canvas
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);


}

void ChannelH12::resizeGL(int w, int h)
{
    glViewport(-10, 0, (GLint)w, (GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //luPerspective(40.0, (GLfloat)w/(GLfloat)h, 2, 30.0);
    //gluPerspective(40,1.33, 2, 30.0);
    glOrtho (-4 * ( GLfloat ) w / ( GLfloat ) h, 4* ( GLfloat ) w / ( GLfloat ) h, -3, 5, -5, 15.0);
     //glFrustum (-1.5 * ( GLfloat ) w / ( GLfloat ) h, 2.3* ( GLfloat ) w / ( GLfloat ) h, -2, 2, 4.3, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Ⱦ
void ChannelH12::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glTranslatef(0.0, 0.0, translate);// translate is included in <QKeyEvent>
    glTranslatef(0.0, -0.5, 0.3);// translate is included in <QKeyEvent>

    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(yRot, 0.0, 1.0, 0.0);
    glRotatef(zRot, 0.0, 0.0, 1.0);


    meshplot(2);


    edgeplot();
}
void ChannelH12::unitmeshplot(int i,int j,double xstart,double zstart,double xstep,double zstep,int dx,int dz){

    double c = *(pdata+j*300+i*dx);
    c -= -3;
    c /= 6;

   double d = (double)i/ 300.0;
   double e = j/20.0;

   if( j <= 0){
        glColor3f(1,1,1);
        glBegin(GL_LINE);
        glVertex3f(xstart-i*xstep,*(pdata+j*300+i*dx),zstart-j*zstep);
        glVertex3f(xstart-(i+1)*xstep,*(pdata+j*300+(i+1)*dx),zstart-j*zstep);
        glEnd();
        glColor3f(1,1,1);
   }
   if( i <= 1){
        glColor3f(1,1,1);
   }else{
        glColor3f(e,c/2,d);
   }
    glBegin(GL_QUADS);
    glVertex3f(xstart-i*xstep,*(pdata+j*300+i*dx),zstart-j*zstep);
    glVertex3f(xstart-(i+1)*xstep,*(pdata+j*300+(i+1)*dx),zstart-j*zstep);
    glVertex3f(xstart-(i+1)*xstep,*(pdata+(j+1)*300+(i+1)*dx),zstart-(j+1)*zstep);
    glVertex3f(xstart-i*xstep,*(pdata+(j+1)*300+(i)*dx),zstart-(j+1)*zstep);
    glEnd();

/*
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(xstart-i*xstep,*(pdata+j*300+i*dx),zstart-j*zstep);
    glVertex3f(xstart-(i+1)*xstep,*(pdata+j*300+(i+1)*dx),zstart-j*zstep);
    glVertex3f(xstart-(i+1)*xstep,*(pdata+(j+1)*300+(i+1)*dx),zstart-(j+1)*zstep);
    glVertex3f(xstart-i*xstep,*(pdata+(j+1)*300+(i)*dx),zstart-(j+1)*zstep);
    glEnd();*/
/*
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(xstart-i*xstep,*(pdata+j*300+i*dx),zstart-j*zstep);
    glVertex3f(xstart-(i+1)*xstep,*(pdata+j*300+(i+1)*dx),zstart-j*zstep);
    glVertex3f(xstart-(i+1)*xstep,*(pdata+(j+1)*300+(i+1)*dx),zstart-(j+1)*zstep);
    glVertex3f(xstart-i*xstep,*(pdata+(j+1)*300+(i)*dx),zstart-(j+1)*zstep);
    glEnd();
    */
}
void ChannelH12::meshplot(int jstart){
    int m = 300;//x
    int n = 29;//z
    double xstep = 6.0/m;
    double zstep = 8.0/n;
    double dx = 300/m;//sample point
    double xstart = 2;//decrease
    double zstart = 2;//decrease
    //pdata = pdata + jstart * 300;
    //QDebug() << zstart <<endl();
    if( pdata - pdata2 > 300*700){
        pdata = pdata2;
    }
    //qDebug()<< pdata2-pdata << endl;
    //from left to right, from front to rear  plot the mesh
    for( int i = 0 ; i < m ; i++){
        for( int j = 0 ; j < n ; j++){
            unitmeshplot(i,j,xstart,zstart,xstep,zstep,dx,1);
        }
    }
}
void ChannelH12::edgeplot(){
    double point2[8][3] = {{2,3,2},{-4,3,2},{-4,3,-6},{2,3,-6},{2,0,2},{-4,0,2},{-4,0,-6},{2,0,-6}};
    glColor4f(0, 0, 0,1);
    glBegin(GL_LINE_LOOP);
    glVertex3f(point2[0][0], point2[0][1], point2[0][2]);
    glVertex3f(point2[1][0], point2[1][1], point2[1][2]);
    glVertex3f(point2[2][0], point2[2][1], point2[2][2]);
    glVertex3f(point2[3][0], point2[3][1], point2[3][2]);
    glEnd();
    glColor4f(0, 0, 0,1);
    glBegin(GL_LINE_LOOP);
    glVertex3f(point2[4][0], point2[4][1], point2[4][2]);
    glVertex3f(point2[5][0], point2[5][1], point2[5][2]);
    glVertex3f(point2[6][0], point2[6][1], point2[6][2]);
    glVertex3f(point2[7][0], point2[7][1], point2[7][2]);
    glEnd();

    glColor4f(0, 0, 0,1);
    glBegin(GL_LINE_LOOP);
    glVertex3f(point2[0][0], point2[0][1], point2[0][2]);
    glVertex3f(point2[1][0], point2[1][1], point2[1][2]);
    glVertex3f(point2[5][0], point2[5][1], point2[5][2]);
    glVertex3f(point2[4][0], point2[4][1], point2[4][2]);
    glEnd();

    glColor4f(0, 0, 0,1);
    glBegin(GL_LINE_LOOP);
    glVertex3f(point2[2][0], point2[2][1], point2[2][2]);
    glVertex3f(point2[6][0], point2[6][1], point2[6][2]);
    glVertex3f(point2[7][0], point2[7][1], point2[7][2]);
    glVertex3f(point2[3][0], point2[3][1], point2[3][2]);
    glEnd();


    glColor4f(0, 0, 0,0.2);
    glBegin(GL_QUADS);
    glVertex3f(point2[4][0], point2[4][1], point2[4][2]);//left
    glVertex3f(point2[5][0], point2[5][1], point2[5][2]);
    glVertex3f(point2[6][0], point2[6][1], point2[6][2]);
    glVertex3f(point2[7][0], point2[7][1], point2[7][2]);
    glEnd();
}


void ChannelH12::wallplot(){
    double point2[8][3] = {{2,2,2},{-2,2,2},{-2,2,-2},{2,2,-2},{2,-1,2},{-2,-1,2},{-2,-1,-4},{2,-1,-4}};
    glColor4f(0, 0, 0,0.2);
    glVertex3f(point2[4][0], point2[4][1], point2[4][2]);
    glVertex3f(point2[5][0], point2[5][1], point2[5][2]);
    glVertex3f(point2[6][0], point2[6][1], point2[6][2]);
    glVertex3f(point2[7][0], point2[7][1], point2[7][2]);
}


void ChannelH12::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up :
        xRot += 10;
        break;
    case Qt::Key_Left :
        yRot += 10;
        break;
    case Qt::Key_Right :
        zRot += 10;
        break;
    case Qt::Key_Down :
        translate -= 1;
        if (translate <= -20)
            translate = -1;
        break;
    }
    updateGL();
    QGLWidget::keyPressEvent(event);
}

void ChannelH12::timerEvent(QTimerEvent *event){
    //meshplot(10);

    char buff[14404*3+10];

    recvfrom(sockser_chl2,&buff,14404*3+10,0,(struct sockaddr *)&addrrcv_chl2,(socklen_t*)&size_chl2);
    qDebug() << "Counter is " << cnt2++ << endl;
    // inverse
    for( int i = 0 ; i < 7203 ; i ++){
       int position = i * 6;
       char tmp;
       //swap
       tmp = buff[position];
       buff[position] = buff[position+3];
       buff[position+3] = tmp;
       //swap
       position ++;
       tmp = buff[position];
       buff[position] = buff[position+3];
       buff[position+3] = tmp;
    }//for
   // qDebug() << buff << endl;

    rendermap(buff);
    updateGL();
    //qDebug()<< "timer event in mygl2 Class!" << endl;
}


void ChannelH12::InputManagement(){
    static double h41[1000][300] = {0};
    //load pilot data
    QFile file2("./pilot/pilot_100_re.txt");
    if (!file2.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << file2.errorString() <<" NO FILE";
    QFile file3("./pilot/pilot_100_im.txt");
    if (!file3.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << file3.errorString() <<" NO FILE";

    char str[100];
    for( int i = 0 ; i < 100 ; i++){
        file2.readLine(str,100);
        int crr = char2int(str);
        pilot2[i][0] = crr;
        //qDebug() << "pilot[ "<<i<<" ][0] is :" << pilot[i][0];
        file3.readLine(str,100);
        crr = char2int(str);
        pilot2[i][1] = crr;
        //qDebug() << "pilot[ "<<i<<" ][1] is :" << pilot[i][1] << endl;
    }//for



    QFile file("h_300_1000_re.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << file.errorString();

    int length = 1000;
    for( int i = 0 ; i < length ; i++ ){
        for( int j = 0 ; j < 300 ; j++ ){
            file.readLine(str,100);
            double crr = 0;
            crr = char2double(str);
            //qDebug() << "crr is :"<<crr;
            //h41[i][j] = crr*10;
            h41[i][j] = 0;
        }
    }
    //qDebug() << "h41[40][299] one is :"<< h41[40][299] ;
    pdata = &h41[0][0];
    pdata2 = pdata;
    file.close();
}
int  ChannelH12::char2int(char *str){
    /*
      *   eg. 2896 -2897
      */
   // qDebug() << str ;
    if(str[0] == '-'){
        return -2897;
    }else{
        return 2896;
    }
}

double ChannelH12::char2double(char *str){
    /*
     *  eg. 0.000815    -0.000551
     */
    double ans = 0;
    double weight = 0.1;
    if(str[0] == '-'){
        ans = str[1]-'0';
        for(int i = 3 ; i < 9 ; i++ ){
            ans += (str[i] - '0') * weight;
            weight /= 10;
        }
        ans *= -1;
    }
    else{
        ans = str[0]-'0';
        for(int i= 2 ; i < 8 ; i++ ){
            ans += (str[i] - '0') * weight;
            weight /= 10;
        }
    }
    return ans;
}

int ChannelH12::hex2int(char a,char b,char c,char d){
    /*
      *   eg. ff to 1111 1111
      */

    //qDebug() << "abcd is : " << a << b << c << d << endl;
    int deca = 0,decb = 0,decc = 0,decd = 0;
    if( a >= 'a' && a <= 'f' ){
        deca  = 10 + a -'a';
    }else{
        deca = a - '0';
    }//else

    if( b >= 'a' && b <= 'f' ){
       decb  = 10 + b -'a';
    }else{
        decb = b - '0';
    }//else

    if( c >= 'a' && c <= 'f' ){
        decc  = 10 + c -'a';
    }else{
        decc = c - '0';
    }//else

    if( d >= 'a' && d <= 'f' ){
       decd  = 10 + d -'a';
    }else{
        decd = d - '0';
    }//else

  //qDebug() << "dec abcd is : " << deca << decb << decc << decd << endl;

   int num[16] = {0};
   num[12] = deca % 2;
   deca /= 2;
   num[13] = deca % 2;
   deca /= 2;
   num[14] = deca % 2;
   deca /= 2;
   num[15] = deca % 2;
   deca /= 2;

   num[8] = decb % 2;
   decb /= 2;
   num[9] = decb % 2;
   decb /= 2;
   num[10] = decb % 2;
   decb /= 2;
   num[11] = decb % 2;
   decb /= 2;

   num[4] = decc % 2;
   decc /= 2;
   num[5] = decc % 2;
   decc /= 2;
   num[6] = decc % 2;
   decc /= 2;
   num[7] = decc % 2;
   decc /= 2;

   num[0] = decd % 2;
   decd /= 2;
   num[1] = decd % 2;
   decd /= 2;
   num[2] = decd % 2;
   decd /= 2;
   num[3] = decd % 2;
   decd /= 2;

   int flag = 1;
   if(num[15] == 1){
        flag = -1;
        for( int i = 0 ; i < 15 ; i++ ){
            num[i] = 1 - num[i];
        }
        num[0] ++;
        for( int i = 0 ; i <15 ; i++ ){
            if( num[i] > 1){
                num[i] = 0;
                num[i+1]++;
            }//if
        }
   }
   int res = 0;
   int base = 1;
   for( int i = 0 ; i < 15 ; i++ ){
       res += num[i] *base;
       base *= 2;
   }


   //for i
  // qDebug() << "16 bit is : "  << num[15] << num[14] << num[13] << num[12] << num[11] << num[10] << num[9] << num[8]<< num[7] << num[6]<< num[5] << num[4]<< num[3] << num[2]<< num[1] << num[0] << endl;
   res *= flag;
   //qDebug() << res << endl;
    return res;
}


void ChannelH12::rendermap(char *buff){
    int position = 12; // avoid the header a0aa 3c20
    for( int i = 0 ; i < 1800 ; i++){
        for( int j = 0 ; j <16 ;) {
            map1800_2[i][j++] = buff[position++];
            map1800_2[i][j++] = buff[position++];
            position++;//avoid the comma
       }//for j
       // position += 24;
    }//for i
   //divide the data into 4 road
    for( int i = 0 ; i < 1800 ; i++){
        data1_2[i][0]=hex2int(map1800_2[i][0],map1800_2[i][1],map1800_2[i][2],map1800_2[i][3]);
        data1_2[i][1]=hex2int(map1800_2[i][4],map1800_2[i][5],map1800_2[i][6],map1800_2[i][7]);
        data2_2[i][0]=hex2int(map1800_2[i][8],map1800_2[i][9],map1800_2[i][10],map1800_2[i][11]);
        data2_2[i][1]=hex2int(map1800_2[i][12],map1800_2[i][13],map1800_2[i][14],map1800_2[i][15]);
        //qDebug() << data1[i][0] << data1[i][1] <<data2[i][0] <<data2[i][1] <<endl;
    }//for i
/*
    if(first1 == 1){
         first1 = 0;
         FILE *fp3 = fopen("./pilot/ofdmdata.txt","w");

        for( int i = 0 ; i <1800 ; i++){
            fprintf(fp3,"%.0lf\t\t%.0lf\t\t%.0lf\t\t%.0lf\n", data1[i][0], data1[i][1], data2[i][0], data2[i][1]);
        }
        fclose(fp3);
     }
*/
    // extract the pilot , 4 in sum
    double pilotH12[100][2] = {0};
    int cntH12 = 0;
    for( int i = 0 ; i < 300 ; i++){
        if(i % 6 == 3){//position for the pilot H12
            pilotH12[cntH12][0] = data1_2[i][0];
            pilotH12[cntH12][1] = data1_2[i][1];
            cntH12 += 2;
        }//if
    }

    cntH12 = 1;
    for( int i = 1200 ; i < 1500 ; i++){
        if( i % 6 == 0){//position for the pilot H12
            pilotH12[cntH12][0] = data1_2[i][0];
            pilotH12[cntH12][1] = data1_2[i][1];
            cntH12 += 2;
        }//if
    }

    //print the pilot H12
    /*
    if(first1 == 1){
         first1 = 0;
         FILE *fp3 = fopen("./pilot/pilotH12.txt","w");

        for( int i = 0 ; i <100 ; i++){
            fprintf(fp3,"%.0lf\t\t%.0lf\n",pilotH12[i][0],pilotH12[i][1]);
        }
        fclose(fp3);
     }
    */
    double H12[100][2] = {0};
    double absH12[100] = {0};
    double absH12_3[300] = {0};

    for( int i = 0 ; i < 100 ; i++ ){
        H12[i][0] = (pilotH12[i][0]*pilot2[i][0]+pilotH12[i][1]*pilot2[i][1])/(pilot2[i][0]*pilot2[i][0]+pilot2[i][1]*pilot2[i][1]);
        H12[i][1] = (pilotH12[i][1]*pilot2[i][0]-pilotH12[i][0]*pilot2[i][1])/(pilot2[i][0]*pilot2[i][0]+pilot2[i][1]*pilot2[i][1]);
        absH12[i] = sqrt(H12[i][0]*H12[i][0] + H12[i][1]*H12[i][1]);
        absH12_3[i*3] = absH12[i];
         absH12_3[i*3 + 1 ] = absH12[i];
          absH12_3[i*3 + 2 ] = absH12[i];
    }//for i
/*
    if(first1 == 1){
         first1 = 0;
         FILE *fp3 = fopen("./pilot/H12.txt","w");

        for( int i = 0 ; i <300 ; i++){
            fprintf(fp3,"%.3lf\t\t%.3lf\t\t\t%.3lf\n",absH12_3[i],absH12_3[i],absH12_3[i]);
        }
        fclose(fp3);
     }
     */
     // 300 * 30   wedth * length
    for( int i = 30 ; i >= 1 ; i-- ){
        for( int j = 0 ; j < 300 ; j++ ){
            *(pdata+i*300 + j) = *( pdata + (i-1)*300 + j);
        }//for j
    }
    for( int j = 0 ; j < 300 ; j++){
        *(pdata + j) = absH12_3[j] *5 ;
    }

}
