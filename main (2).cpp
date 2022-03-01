//Board1:足回り テスト用
//走行前、エンコーダの数字をpcに書き出すプログラム

#include "mbed.h"
#include "EC.h"
#include "CalPID.h"
#include "MotorController.h"

Serial pc(USBTX,USBRX);     //TeraTerm利用設定

Ticker ticker;

//*****************************************************************************
//足回りモータに関する設定　suzaku775/24V
#define RESOLUTION 2048     //エンコーダの分解能　データシート参照 >>48でいいかどうかは確認する//Kenshinです
#define DELTA_T 0.001     //pidの幅Δt
#define DUTY_MAX 0.5      //duty上限
#define OMEGA_MAX 6       //ω上限
//#define TIME_STOP 10.0
#define NUM_DATA 500    //角度保存の際に使う

//CalPID設定×4 インスタンス設定
CalPID M1_speed_pid(0.003,0,0.000013,DELTA_T,DUTY_MAX);    //速度制御のPID,係数はモーターごとに調整の必要あり
//CalPID M1_duty_pid(0.003,0,0.000013,DELTA_T,DUTY_MAX);   //角度制御のPID,モーターに出力されたduty比からフィードバックの場合
    //↑↓dutyかomegaかどちらかで角速度のフィードバックを行う
CalPID M1_omega_pid(10.0,0,0.0100,DELTA_T,OMEGA_MAX);      //角度制御のPID,SpeedControllerのωからフィードバックの場合

CalPID M2_speed_pid(0.003,0,0.000013,DELTA_T,DUTY_MAX);
//CalPID M2_duty_pid(0.003,0,0.000013,DELTA_T,DUTY_MAX);
CalPID M2_omega_pid(10.0,0,0.0100,DELTA_T,OMEGA_MAX);

CalPID M3_speed_pid(0.003,0,0.000013,DELTA_T,DUTY_MAX);
//CalPID M3_duty_pid(0.003,0,0.000013,DELTA_T,DUTY_MAX);
CalPID M3_omega_pid(10.0,0,0.0100,DELTA_T,OMEGA_MAX);

CalPID M4_speed_pid(0.003,0,0.000013,DELTA_T,DUTY_MAX);
//CalPID M4_duty_pid(0.003,0,0.000013,DELTA_T,DUTY_MAX);
CalPID M4_omega_pid(10.0,0,0.0100,DELTA_T,OMEGA_MAX);

//今回は関係ないけどただのメモ
//角度制御のPIDに複数種類あり
//1.ωの最大値を指定して制御するパターン
//2.角度だけ設定して急いで動かすパターン
//PIDで角度設定するとストールトルクで角度保持可能だけど、PWM完全に0に落とすとトルクゼロにできそう。＜＜初期状態、負荷が減る（？）

//エンコーダ設定 インスタンス設定
Ec1multi M1_EC(PA_12,PC_5,RESOLUTION);
            //A相,B相,RESOLUTION:分解能
Ec1multi M2_EC(PB_13,PB_15,RESOLUTION);
Ec1multi M3_EC(PC_12,PC_10,RESOLUTION);
Ec1multi M4_EC(PC_3,PC_2,RESOLUTION);

//モーター設定 インスタンス設定
MotorController M1_MC(PC_6,PC_8,DELTA_T,M1_EC,M1_speed_pid,M1_omega_pid); //引数は下行
                    //モーター正転PWMpin、逆転PWMpin、Δh周期[s]、エンコーダインスタンス、速度制御用のPIDインスタンス、角度制御のPIDインスタンス
MotorController M2_MC(PA_9,PA_8,DELTA_T,M2_EC,M2_speed_pid,M2_omega_pid);
MotorController M3_MC(PB_8,PB_9,DELTA_T,M3_EC,M3_speed_pid,M3_omega_pid);
MotorController M4_MC(PA_1,PA_0,DELTA_T,M4_EC,M4_speed_pid,M4_omega_pid);

//*************************************************************************

//****************************************************ここまでは理解した    

/*これが何に使われるか分からない
double convertRad(double degree)//単位の変換deg→rad
{
    return degree*M_PI/180.0;
}
double convertDeg(double rad)//単位の変換rad→deg
{
    return rad*180.0/M_PI;
}
*/

void calOmega4();   //プロトタイプ宣言

int countM1,countM2,countM3,countM4 = 0;
double omegaM1,omegaM2,omegaM3,omegaM4;


//角度を保存する変数と関数
float M1_angle_saved[NUM_DATA]= {};
float M2_angle_saved[NUM_DATA]= {};
float M3_angle_saved[NUM_DATA]= {};
float M4_angle_saved[NUM_DATA]= {};

int M1_angle_count=0;
int M2_angle_count=0;
int M3_angle_count=0;
int M4_angle_count=0;


void saveAngle()
{
    if(M1_angle_count<NUM_DATA) {
        M1_angle_saved[angle_count] = M1_MC.getAngle();
        M1_angle_count++;
    }
}

//角速度を保存する変数と関数
float M1_omega_saved[NUM_DATA]= {};
int M1_omega_count=0;
void saveOmega()//
{
    if(M1_omega_count<NUM_DATA) {
        M1_omega_saved[omega_count]=ec.getOmega();
        M1_omega_count++;
    }
}
//
float target_speed = 0;
int save_count;//データ保存の周期調整用1kHzで保存すると1秒で1000個もデータが溜まってしまう
#define SAVE_COUNT_THRESHOLD 10
void speedControll()
{
    motor.Sc(target_speed);
    save_count++;
    if(save_count>SAVE_COUNT_THRESHOLD) {
        saveAngle();
        saveOmega();
        save_count=0;
    }
}
    
    
}


//********************************ここの後ろは理解した
int main(){
    ticker.attach(&calcOmega,0.01);     //割り込み
    
    
    while(1){
        countM1 = M1_EC.getCount();
        omegaM1 = M1_EC.getOmega();
        pc.printf("%d, ",count);
        pc.printf("%lf\n",omega);
        wait(1);
    }
    



void calOmega4(){   //プロトタイプ宣言したところ
    M1_EC.calOmega();
    M2_EC.calOmega();
    M3_EC.calOmega();
    M4_EC.calOmega();
}
