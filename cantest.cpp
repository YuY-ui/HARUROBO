#include "mbed.h"
//2.ピン宣言
//CAN
CAN can1(PA_11,PA_12);//rd,td
DigitalOut LED(LED1);

Ticker ticker;


//グローバル関数の宣言
char can_data[4]={1,0,0,0};//CAN送信用の配列4byte
//int data_from0;//CAN送信用の十進法
char msg_from1,msg_from2;//CAN受信内容1byte

//4.関数
void can_send();
void can_read();
void can_both();


//5.メイン関数
int main(){
    wait(10);
    ticker.attach(&can_both,0.001);//CAN通信の受信処理
    
    while(1){
        LED = msg_from1;
        wait(0.1);
    }
}




void can_read(){  //CAN受信設定4byte
    CANMessage msg;
    if(can1.read(msg)){
        /*if(msg.id==0){
            自分からの送信なので受け取らない
        }*/
        if(msg.id==1){
            //ID=1 RunBoardからの受信だった場合
            msg_from1 = msg.data[0];
            can_data[0] = !can_data[0];
            //msg.data[1]
            //msg.data[2]
            //msg.data[3]
        }
        /*if(msg.id==2){
            //ID=2 ArmBoardからの受信だった場合
            if(msg.data[0]==1){//まだアームが動いてるとき
                can_data[0] = 0;//足回りとのCAN通信停止
            }else{
                can_data[0] = 1;//足回りとのCAN通信再開
            }
            //msg.data[1]
            //msg.data[2]
            //msg.data[3]
        }*/
    }
}

void can_send(){  //CAN送信設定4byte
    if(can1.write(CANMessage(0,can_data,4))){//ID=0で送信
        //送信成功
    }
}

void can_both(){
    can_read();
    can_send();
}