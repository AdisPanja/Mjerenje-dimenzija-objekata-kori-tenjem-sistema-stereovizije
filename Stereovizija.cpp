#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include "IDSVideoCapture.h"
#include <opencv2/core.hpp>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include<string>
#include <vector>
using namespace std;
using namespace cv;

#define fx 1242.455
#define fy 1235.196
#define ox 639.5
#define oy 359.5
#define b 156.612

//Pomocni objekti za UEYE cameru
    HIDS hCam1, hCam2;
    SENSORINFO sensor_info1, sensor_info2 ;
    CAMINFO camera_info1, camera_info2 ;
    int memID1, memID2;
    int img_width = 1280;
    int img_height = 720;

//globalne varijable
int broj_klikovaL = 0, xpL, ypL, xdL, ydL;
int broj_klikovaD = 0, xpD, ypD, xdD, ydD;
int zastava_tacka, zastava_linija;
Point3i prva_tacka, druga_tacka;
int broj_pozivanja = 0;
Mat prava(720,1280, CV_8UC1);
int crvena, zelena, plava;
Scalar color;
bool ulaz;
Mat frameL, frameD;



int InicijalizacijaUEyeKamere(int adresa)
{
    //initialize camera
    hCam1 = static_cast<HIDS>(adresa);
    int nRet = is_InitCamera(&(hCam1), nullptr);
    std::cout << "Status Init: " << nRet << std::endl;

    //Camera Id
    nRet = is_SetCameraID(hCam1,IS_GET_CAMERA_ID);
    std::cout << "CameraID: " << nRet << std::endl;

    //get sensor info
    nRet = is_GetSensorInfo(hCam1, &sensor_info1);
    std::cout << "Sensor Color Mode: " << sensor_info1.nColorMode << std::endl;
    std::cout << "Camera Model: " << sensor_info1.strSensorName << std::endl;

    //get camera info
    // nRet = is_GetCameraInfo(hCam1, &camera_info);
    //  cout << "Camera ID: " << camera_info.ID << endl;
    //cout << "Camera SerNum: " << camera_info.SerNo << endl;
    //cout << "Camera Version: " << camera_info.Version << endl;
    //cout << "Camera Type: " << camera_info.Type << endl;

    // color mode
    int Mode = IS_CM_RGB8_PACKED;
    nRet = is_SetColorMode(hCam1, Mode);
    // cout << "Color Mode: " << nRet << endl;

    //UINT formatID = 13;
    //nRet = is_ImageFormat(hCam1, IMGFRMT_CMD_SET_FORMAT, &formatID, 4);
    // cout << "Status Image Format: " << nRet << endl;

    char* pMem = nullptr;
    memID1 = 0;

    // Allocate image mem for current format, set format
    nRet = is_AllocImageMem(hCam1, img_width, img_height, 24, &pMem, &(memID1));
    nRet = is_SetImageMem(hCam1, pMem, memID1);
    //nRet = is_ImageFormat(hCam1, IMGFRMT_CMD_SET_FORMAT, &formatInfo.nFormatID, sizeof(formatInfo.nFormatID));

    // postavi display mode
    Mode = IS_SET_DM_DIB;
    nRet = is_SetDisplayMode(hCam1, Mode);


    //Iskljucenje autofocusa
    Mode = FOC_CMD_SET_DISABLE_AUTOFOCUS;
    nRet = is_Focus(hCam1, Mode, nullptr, 0);

     //double enable = 1;
     double disable = 0;
     //is_SetAutoParameter (hCam1, IS_SET_ENABLE_AUTO_GAIN, &disable, 0);
     //is_SetAutoParameter (hCam1, IS_SET_ENABLE_AUTO_WHITEBALANCE, &disable, 0);
     //is_SetAutoParameter (hCam1, IS_SET_ENABLE_AUTO_FRAMERATE, &disable, 0);
     //is_SetAutoParameter (hCam1, IS_SET_ENABLE_AUTO_SHUTTER, &disable, 0);
     //is_SetAutoParameter (hCam1, IS_SET_ENABLE_AUTO_SENSOR_GAIN, &disable, 0);
     //is_SetAutoParameter (hCam1, IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE,&disable,0);
     //is_SetAutoParameter (hCam1, IS_SET_ENABLE_AUTO_SENSOR_SHUTTER, &disable, 0);

     //double* pFPS(nullptr);
     //nRet = is_SetFrameRate(hCam1,30,pFPS);
     //double parameter = 250;
     //nRet = is_Exposure(hCam1,IS_EXPOSURE_CMD_SET_EXPOSURE,(void*) &parameter, sizeof(parameter));


     //Provjeri je li sve OK
     return 0;
}

int InicijalizacijaUEyeKamere2(int adresa)
{
    //initialize camera
    hCam2 = static_cast<HIDS>(adresa);
    int nRet = is_InitCamera(&(hCam2), nullptr);
    std::cout << "Status Init: " << nRet << std::endl;

    //Camera Id
    nRet = is_SetCameraID(hCam2 ,IS_GET_CAMERA_ID);
    std::cout << "CameraID: " << nRet << std::endl;

    //get sensor info
    nRet = is_GetSensorInfo(hCam2 , &sensor_info2);
    std::cout << "Sensor Color Mode: " << sensor_info2.nColorMode << std::endl;
    std::cout << "Camera Model: " << sensor_info2.strSensorName << std::endl;

    //get camera info
    // nRet = is_GetCameraInfo(hCam2 , &camera_info);
    //  cout << "Camera ID: " << camera_info.ID << endl;
    //cout << "Camera SerNum: " << camera_info.SerNo << endl;
    //cout << "Camera Version: " << camera_info.Version << endl;
    //cout << "Camera Type: " << camera_info.Type << endl;

    // color mode
    int Mode = IS_CM_RGB8_PACKED;
    nRet = is_SetColorMode(hCam2 , Mode);
    // cout << "Color Mode: " << nRet << endl;

    //UINT formatID = 13;
    //nRet = is_ImageFormat(hCam2 , IMGFRMT_CMD_SET_FORMAT, &formatID, 4);
    // cout << "Status Image Format: " << nRet << endl;

    char* pMem = nullptr;
    memID2 = 0;

    // Allocate image mem for current format, set format
    nRet = is_AllocImageMem(hCam2 , img_width, img_height, 24, &pMem, &(memID2));
    nRet = is_SetImageMem(hCam2 , pMem, memID2);
    //nRet = is_ImageFormat(hCam2 , IMGFRMT_CMD_SET_FORMAT, &formatInfo.nFormatID, sizeof(formatInfo.nFormatID));

    // postavi display mode
    Mode = IS_SET_DM_DIB;
    nRet = is_SetDisplayMode(hCam2 , Mode);

    //Iskljucenje autofocusa
    Mode = FOC_CMD_SET_DISABLE_AUTOFOCUS;
    nRet = is_Focus(hCam2 , Mode, nullptr, 0);

    //double enable = 1;
//     double disable = 0;
//     is_SetAutoParameter (hCam2 , IS_SET_ENABLE_AUTO_GAIN, &disable, 0);
//     is_SetAutoParameter (hCam2 , IS_SET_ENABLE_AUTO_WHITEBALANCE, &disable, 0);
//     is_SetAutoParameter (hCam2 , IS_SET_ENABLE_AUTO_FRAMERATE, &disable, 0);
//     is_SetAutoParameter (hCam2 , IS_SET_ENABLE_AUTO_SHUTTER, &disable, 0);
//     is_SetAutoParameter (hCam2 , IS_SET_ENABLE_AUTO_SENSOR_GAIN, &disable, 0);
//     is_SetAutoParameter (hCam2 , IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE,&disable,0);
//     is_SetAutoParameter (hCam2 , IS_SET_ENABLE_AUTO_SENSOR_SHUTTER, &disable, 0);

//     double* pFPS(nullptr);
//     nRet = is_SetFrameRate(hCam2 ,30,pFPS);
//     double parameter = 250;
//     nRet = is_Exposure(hCam2 ,IS_EXPOSURE_CMD_SET_EXPOSURE,(void*) &parameter, sizeof(parameter));

     //Provjeri je li sve OK
     return 0;
}

Point3i NacrtajKoordinatu(int xL, int yL, int xD, int yD, Mat frameL){

    // odabir boje
    if (broj_pozivanja % 10 == 0) color = Scalar(255,0,0);
    else if (broj_pozivanja % 10 == 1) color = Scalar(0,255,0);
    else if (broj_pozivanja % 10 == 2) color = Scalar(0,0,255);
    else if (broj_pozivanja % 10 == 3) color = Scalar(0,255,255);
    else if (broj_pozivanja % 10 == 4) color = Scalar(255,0,255);
    else if (broj_pozivanja % 10 == 5) color = Scalar(255,255,0);
    else if (broj_pozivanja % 10 == 6) color = Scalar(0,0,128);
    else if (broj_pozivanja % 10 == 7) color = Scalar(0,128,0);
    else if (broj_pozivanja % 10 == 8) color = Scalar(128,0,0);
    else color = Scalar(0,128,128);

    // racunanje koordinate tacke u prostoru svijeta
    double x = b*(xL-ox)/(abs(xD-xL));
    double y = b*fx*(yL-oy)/(fy*abs(xD-xL));
    double z = b*fx/(abs(xD-xL));

    char koordinata[50];
    sprintf(koordinata,"(%.2f,%.2f,%.2f)", x/10, y/10,z/10);
    cout << "Koordinata u prostoru svijeta [cm]: (" << ceil(x/10 * 1000.0) / 1000.0 << "," << ceil(y/10 * 1000.0) / 1000.0 << "," << ceil(z/10 * 1000.0) / 1000.0 << ")" << endl;

    circle(frameL, Point(xL,yL),2,color,5);
    putText(frameL, koordinata, Point(xL,yL),FONT_HERSHEY_DUPLEX,0.5,color,1,false);

    return Point3i(x,y,z);
}

void OdrediRazmak(Point3i prva, Point3i druga, Mat frameL){

    broj_pozivanja++; //prati redni broj mjerenja
    double dx = abs(druga.x - prva.x);
    double dy = abs(druga.y - prva.y);
    double dz = abs(druga.z - prva.z);

    double razmak = sqrt(pow(dx,2)+pow(dy,2)+pow(dz,2)); //udaljenost izmedju dvije tacke

    char d[50];
    sprintf(d, "%d. mjerenje = %.3f cm", broj_pozivanja, razmak/10);
    cout << broj_pozivanja << " . d = " << ceil(razmak/10 * 1000.0) / 1000.0 << " cm" << endl;

    putText(frameL, d, Point(10, broj_pozivanja*50),FONT_HERSHEY_DUPLEX,0.5,color,1,false);

}

void CallBackFuncL(int event, int x, int y, int flags, void* userdata) {

     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left picture - position (" << x << ", " << y << ")" << endl;
          if (broj_klikovaL == 0){
              xpL = x;
              ypL = y;
              broj_klikovaL++;
              ulaz = true;
          }
          else if (broj_klikovaL == 1){
              xdL = x;
              ydL = y;
              broj_klikovaL = 0;
              ulaz = false;
          }
     }
}

void CallBackFuncD(int event, int x, int y, int flags, void* userdata) {

     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Right picture - position (" << x << ", " << y << ")" << endl;
          if (broj_klikovaD == 0){
              xpD = x;
              ypD = y;
              broj_klikovaD++;
              zastava_tacka = 1;
          }
          else if (broj_klikovaD == 1){
              xdD = x;
              ydD = y;
              zastava_linija = 1;
              broj_klikovaD = 0;
          }
     }
}

int main()
{

    InicijalizacijaUEyeKamere(200);
    InicijalizacijaUEyeKamere2(250);

    // ucitavanje slika
    // ukoliko hocemo slike direktno sa kamera potrebno je koristiti kod ispod
    //frameL = imread("lijeva_slika.png", IMREAD_COLOR);
    //frameD = imread("desna_slika.png", IMREAD_COLOR);

    int status;
    char key;
    int br = 0;

   // ukoliko hocemo slike direktno sa kamera
   // while(true){
    if (is_FreezeVideo(hCam1, IS_WAIT) == IS_SUCCESS)
                        {
                              void* pMemVoid; //pointer to where the image is stored
                              is_GetImageMem(hCam1, &pMemVoid);
                              frameL = cv::Mat(cv::Size(img_width, img_height), CV_8UC3, pMemVoid);
                              //fail = 0;
                              status = 0;
                        } else
                        {
                              //status = -1; //Blokira ponovno uzimanje frejma
                              frameL = cv::Mat::zeros(cv::Size(img_width,img_height), CV_8UC3);
                        }


                       //vector<Rect> kvadratL = NadjiObjekte(frame,1);

    if (is_FreezeVideo(hCam2, IS_WAIT) == IS_SUCCESS)
                        {
                              void* pMemVoid2; //pointer to where the image is stored
                              is_GetImageMem(hCam2, &pMemVoid2);
                              frameD = cv::Mat(cv::Size(img_width, img_height), CV_8UC3, pMemVoid2);
                              //fail = 0;
                              status = 0;
                        } else
                        {
                              //status = -1; //Blokira ponovno uzimanje frejma
                              frameD = cv::Mat::zeros(cv::Size(img_width,img_height), CV_8UC3);
                        }

        imshow("FrameL", frameL);
        setMouseCallback("FrameL", CallBackFuncL, NULL);
        imshow("FrameR", frameD);
        setMouseCallback("FrameR", CallBackFuncD, NULL);


        waitKey(10);
    //}


    char keyPress;
    while(1){
        if (zastava_tacka == 1){
            prva_tacka = NacrtajKoordinatu(xpL, ypL, xpD, ypD, frameL);
            imshow("FrameL", frameL);
            zastava_tacka = 0;
        }
        if (zastava_linija == 1){
            druga_tacka = NacrtajKoordinatu(xdL, ydL, xdD, ydD, frameL);
            line(frameL, Point(xpL, ypL), Point(xdL, ydL), color, 3, LINE_4);
            OdrediRazmak(prva_tacka, druga_tacka, frameL);
            imshow("FrameL", frameL);
            zastava_linija = 0;
        }
        imshow("FrameL", frameL);
        keyPress = waitKey(25);
        if (keyPress == 'q'){
        destroyAllWindows();
        break;
       }
    }

    return 0;
}
