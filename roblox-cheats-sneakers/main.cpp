#include <iostream>
#include <opencv2.4/opencv2/opencv.hpp>
#include <Windows.h>

using namespace std;
using namespace cv;

Mat screenregion(int x1,int y1,int x2,int y2) {
    // Take a screenshot of the specified region
    HDC hScreen = GetDC(NULL);
    HDC hDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen,x2 - x1,y2 - y1);
    HGDIOBJ old_obj = SelectObject(hDC,hBitmap);
    BitBlt(hDC,0,0,x2 - x1,y2 - y1,hScreen,x1,y1,SRCCOPY);
    SelectObject(hDC,old_obj);
    DeleteDC(hDC);
    ReleaseDC(NULL,hScreen);

    // Convert bitmap to Mat
    BITMAPINFOHEADER   bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = x2 - x1;
    bi.biHeight = y2 - y1;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    Mat img = Mat(y2 - y1,x2 - x1,CV_8UC3);
    GetDIBits(CreateCompatibleDC(NULL),hBitmap,0,y2 - y1,img.data,(BITMAPINFO*)&bi,DIB_RGB_COLORS);

    DeleteObject(hBitmap);

    return img;
}

int count_green_pixels(const Mat& image) {
    // Count the number of green pixels in the image
    Mat green_channel;
    extractChannel(image,green_channel,1);
    return countNonZero(green_channel > 229);
}

void mouse_click(int x,int y) {
    // Perform a mouse click at the specified coordinates
    SetCursorPos(x,y);
    mouse_event(MOUSEEVENTF_LEFTDOWN,x,y,0,0);
    mouse_event(MOUSEEVENTF_LEFTUP,x,y,0,0);
}

bool is_middle_button_pressed() {
    // Check if the middle mouse button is pressed
    return (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
}

int main() {
    // Capture the screen region only once
    cout << "MIDDLE MOUSE BUTTON TO CLOSE THE SCRIPT AUTOMATICLY" << endl;
    int i = 0;
    while(true) {
        if(is_middle_button_pressed()) {
            return 0;
        }
        else if(count_green_pixels(screenregion(380,1220,2180,1224)) < 350) {
            mouse_click(1900,1000);
        }
    }
    return 0;
}