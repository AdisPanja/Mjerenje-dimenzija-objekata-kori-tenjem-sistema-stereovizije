#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <vector>
using namespace std;
using namespace cv;

// Definiranje osnovnih parametara
int board_width = 5;
int board_height = 9;
int broj_slika = 97;
double dimenzija_kvadratica = 15.0;

int main()
{
  int val_slika = 0;
  //zbog ucitavanja slika
  vector<string> slika{};
  for (int i = 0; i < 97; i++){
      slika.push_back(to_string(i));
    }

  //3D tacke na kalibracionoj ploci
  vector<vector<Point3f>> tacke_na_ploci;

  //2D tacke kalibracione ploce na slici
  vector<vector<Point2f>> tacke_na_slici_desno;
  vector<vector<Point2f>> tacke_na_slici_lijevo;
  vector<vector<Point2f>> tacke_desno;
  vector<vector<Point2f>> tacke_lijevo;

  Size dimenzije_ploce = Size(board_width, board_height);

    for (int i = 0; i < broj_slika; i++) {
      Mat slika_lijevo = imread("ImagesLD\\imagesL\\slika" + slika[i] + ".bmp", IMREAD_COLOR);
      Mat slika_desno = imread("ImagesLD\\imagesD\\slika" + slika[i] + ".bmp", IMREAD_COLOR);
      Mat gray_lijevo, gray_desno;
      cvtColor(slika_lijevo, gray_lijevo, COLOR_BGR2GRAY);
      cvtColor(slika_desno, gray_desno, COLOR_BGR2GRAY);

      bool found1 = false, found2 = false;

      vector<Point2f> coskovi_lijevo, coskovi_desno;
      found1 = cv::findChessboardCorners(gray_lijevo, dimenzije_ploce, coskovi_lijevo, CALIB_CB_ADAPTIVE_THRESH +  CALIB_CB_FAST_CHECK + CALIB_CB_NORMALIZE_IMAGE);
      found2 = cv::findChessboardCorners(gray_desno, dimenzije_ploce, coskovi_desno, CALIB_CB_ADAPTIVE_THRESH +  CALIB_CB_FAST_CHECK + CALIB_CB_NORMALIZE_IMAGE);


      if(!found1 || !found2){
        cout << i << ". Coskovi nisu pronadjeni!" << endl;
        continue;
      }

      val_slika++;
      if (found1) {
        cv::cornerSubPix(gray_lijevo, coskovi_lijevo, cv::Size(11, 11), cv::Size(-1, -1), TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.001));
        cv::drawChessboardCorners(slika_lijevo, dimenzije_ploce, coskovi_lijevo, found1);
      }

      if (found2) {
          cv::cornerSubPix(gray_desno, coskovi_desno, cv::Size(11, 11), cv::Size(-1, -1), TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.001));
          cv::drawChessboardCorners(slika_desno, dimenzije_ploce, coskovi_desno, found2);;
      }

      if (i == 13){
          imshow("Lijeva", slika_lijevo);
          imshow("Desna", slika_desno);
          cout << slika_lijevo.size() << endl;
      }
      vector<Point3f> obj;
      for (int i = 0; i < board_height; i++)
        for (int j = 0; j < board_width; j++)
          obj.push_back(Point3f((float)j * dimenzija_kvadratica, (float)i * dimenzija_kvadratica, 0));

      if (found1 && found2) {
        cout << i << ". Coskovi pronadjeni!" << endl;
        tacke_na_slici_lijevo.push_back(coskovi_lijevo);
        tacke_na_slici_desno.push_back(coskovi_desno);
        tacke_na_ploci.push_back(obj);
      }
    }

   Mat cameraMatrix1, distCoeffs1, cameraMatrix2, distCoeffs2, R, E, F;
   Vec3d T;
   stereoCalibrate(tacke_na_ploci, tacke_na_slici_lijevo, tacke_na_slici_desno, cameraMatrix1, distCoeffs1,
                   cameraMatrix2, distCoeffs2, Size(1280,720), R, T, E, F, CALIB_SAME_FOCAL_LENGTH | CALIB_FIX_PRINCIPAL_POINT,
                   cv::TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 500,1e-5));

  cout << "Ukupno je pronadjeno " << val_slika << " validnih i " << broj_slika - val_slika << " nevalidnih slika." << endl;
  cout << "M_intL : " << endl << cameraMatrix1 <<endl;
  cout << "M_intD : " << endl << cameraMatrix2 <<endl;
  cout << "D_L : " << distCoeffs1 << endl;
  cout << "D_D : " << distCoeffs2 << endl;
  cout << "R : " << R << endl;
  cout << "T : " << T << endl;
  cout << "F : " << F << endl;
  cout << "E : " << E << endl;

    char keyPress;
    while(1){
        keyPress = waitKey(10);
        if (keyPress == 'q'){
        destroyAllWindows();
        break;
       }
    }

return 0;
}
