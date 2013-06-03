//
//  main.cpp
//  ImageProcessor
//
//  Created by 馬場 惇 on 13/06/04.
//  Copyright (c) 2013年 馬場　惇. All rights reserved.
//

#include <iostream>
#include <GLUT/GLUT.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

IplImage *inputImage;

/* 表示用 */
IplImage *image;
IplImage *rgbImage;

void initializeImageProcessing()
{
    /*	ここにプログラム起動時の行う画像処理を書く  */
    //inputImage    = cvLoadImage("img/bmp/Mandrill.bmp", CV_LOAD_IMAGE_GRAYSCALE);  // グレースケール
    inputImage    = cvLoadImage("testset/102.jpg", CV_LOAD_IMAGE_ANYCOLOR);
    /*  ここまで */
}

void imageProcessing()
{
    /*  ここに適当な画像処理を書く．imageに最終的な出力を行う． */
    //cvErode(inputImage, image, 0, erosions); // 例えば縮退処理を行なう
    /*  ここまで  */
}


void display()
{
    imageProcessing();
    
    glClear(GL_COLOR_BUFFER_BIT);
    glPixelZoom(1, -1);
    glRasterPos2i(0 , 0);
    
    rgbImage = cvCloneImage(image);
    //IPL形式の画像（BGR）をOpenGL形式（RGB）に変換
    if (image->nChannels >= 2)
    {
        // BGR(A) -> RGB(A)
        for (int i = 0; i < image->width * image->height * image->nChannels; i = i + image->nChannels) {
            rgbImage->imageData[i]     = image->imageData[i + 2];
            rgbImage->imageData[i + 2] = image->imageData[i];
        }
    }
	
    //チャンネルに応じて書き込み方を変更
    switch (image->nChannels)
    {
        case 1:
            glDrawPixels(image->width, image->height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image->imageData);
            break;
        case 3:
            glDrawPixels(image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, rgbImage->imageData);
            break;
        case 4:
            glDrawPixels(image->width, image->height, GL_RGBA, GL_UNSIGNED_BYTE, rgbImage->imageData);
            break;
        default:
            break;
    }
    //出力
    glFlush();
}


void glInit()
{
    glClearColor(0.0, 0.0, 1.0, 1.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, image->width - 1.0, image->height - 1.0, 0.0, -1.0, 1.0);
}

void cvInit()
{
    initializeImageProcessing();
    
    if (inputImage == NULL) {
        fprintf(stderr, "読込みに失敗しました.");
    }
    
    /*  表示する画像のサイズやチャネル数を設定する（とりあえずIPL_DPETH_8Uは固定）*/
    //image = cvCreateImage(cvSize(inputImage->width, inputImage->height), IPL_DEPTH_8U, 3);
    image = cvCloneImage(inputImage);
    //image = cvCreateImage(cvSize(inputImage->width, inputImage->height), IPL_DEPTH_8U, 1);グレースケール
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'q':
        case 'Q':
        case '\033':  /* '\033' は ESC の ASCII コード */
            exit(0);
        default:
            break;
    }
}

int main(int argc, char *argv[])
{
    //OpenCVの初期化
    cvInit();
	
    //OpenGLとGLUTの初期化
    //(OpenCVみたいにまとめないのはargcとargvが必要だから？)
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(image->width, image->height);
    glutCreateWindow("IplImage");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glInit();
	
    //メインループ突入（displayを繰り返し実行）
    glutMainLoop();
    
    return 0;
}