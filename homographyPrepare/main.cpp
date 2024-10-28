#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "../INIReader.h"

using namespace cv;
using namespace std;

Mat image;
Mat displayImage;
Mat overlay;
vector<Point> points;
bool zoomMode = false;
int windowSize;
bool mouseMoved = false;
double scaleFactorX = 1.0, scaleFactorY = 1.0;
Rect zoomROI;
string imagePath;

Point currentMousePos(-1, -1);
Point lastMousePos(-1, -1);


// INI-file read function
bool get_ini_params(const string& config)
{
    cout << "BEGIN get_ini_params" << endl;

    INIReader reader(config);

    if (reader.ParseError() < 0)
    {
        cout << "Не удалось загрузить '" << config << "'" << endl;
        return false;
    }

    imagePath = reader.Get("Settings", "image_path", "");
    windowSize = reader.GetInteger("Settings", "window_size", 50);

    if (imagePath.empty())
    {
        cout << "Путь к изображению не указан в '" << config << "'" << endl;
        return false;
    }

    cout << "END get_ini_params" << endl;
    return true;
}

// Function to draw points on image
void drawPoints(Mat& img, const vector<Point>& points, bool isZoomed)
{
    for (size_t i = 0; i < points.size(); ++i)
    {
        Point pt = points[i];
        if (isZoomed)
        {
            pt.x = static_cast<int>((pt.x - zoomROI.x) * scaleFactorX);
            pt.y = static_cast<int>((pt.y - zoomROI.y) * scaleFactorY);
        }
        circle(img, pt, 3, Scalar(0, 0, 255), -1);
    }
}


// Function to draw frame near cursor
void drawFrame()
{

    overlay.setTo(Scalar(0, 0, 0, 0));

    rectangle(overlay,
              Point(currentMousePos.x - windowSize / 2, currentMousePos.y - windowSize / 2),
              Point(currentMousePos.x + windowSize / 2, currentMousePos.y + windowSize / 2),
              Scalar(0, 0, 255), 2);

    Mat combined;
    addWeighted(displayImage, 1.0, overlay, 1.0, 0, combined);

    imshow("Image", combined);
}


// Main callback mouse function
void onMouseMain(int event, int x, int y, int flags, void* userdata)
{
    switch (event)
    {
        case EVENT_MOUSEMOVE:
            mouseMoved = true;
            lastMousePos = currentMousePos;
            currentMousePos = Point(x, y);

            drawFrame();
            break;

        case EVENT_LBUTTONDOWN:
            if (!zoomMode && points.size() < 4)
            {
                points.push_back(Point(x, y));
                circle(image, Point(x, y), 3, Scalar(0, 0, 255), -1);
            }
            break;

        default:
            break;
    }
}

// Function to draw/display points on zoomed image
void onMouseZoom(int event, int x, int y, int flags, void* userdata)
{
    if (zoomMode && event == EVENT_LBUTTONDOWN)
    {
        int origX = zoomROI.x + static_cast<int>(x / scaleFactorX);
        int origY = zoomROI.y + static_cast<int>(y / scaleFactorY);

        origX = max(0, min(origX, image.cols - 1));
        origY = max(0, min(origY, image.rows - 1));

        if (points.size() < 4)
        {
            points.push_back(Point(origX, origY));
        }
    }
}

int main(int argc, char** argv)
{
    if (!get_ini_params("../config.ini"))
    {
        return -1;
    }

    image = imread(imagePath);
    if (image.empty())
    {
        cout << "Can not open the image: " << imagePath << endl;
        return -1;
    }

    displayImage = image.clone();
    overlay = Mat::zeros(image.size(), CV_8UC3);
    drawFrame();

    namedWindow("Image", WINDOW_AUTOSIZE);
    setMouseCallback("Image", onMouseMain, NULL);


    while (true)
    {
        if (!zoomMode)
        {
            displayImage = image.clone();

            // Display points on original image
            drawPoints(displayImage, points, false);

            imshow("Image", displayImage);

            int key = waitKey(30);
            if (key == 27) // ESC
            {
                break;
            }
            else if (key == 32) // Space
            {
                zoomMode = true;
                int x = currentMousePos.x;
                int y = currentMousePos.y;

                int x_start = max(x - windowSize / 2, 0);
                int y_start = max(y - windowSize / 2, 0);
                int x_end = min(x + windowSize / 2, image.cols);
                int y_end = min(y + windowSize / 2, image.rows);

                zoomROI = Rect(x_start, y_start, x_end - x_start, y_end - y_start);

                Mat roi = image(zoomROI);

                scaleFactorX = (double)image.cols / roi.cols;
                scaleFactorY = (double)image.rows / roi.rows;

                resize(roi, displayImage, Size(image.cols, image.rows), 0, 0, INTER_LINEAR);

                setMouseCallback("Image", onMouseZoom, NULL);
            }

        }
        else
        {
            // Display points on zoomed image
            drawPoints(displayImage, points, true);

            imshow("Image", displayImage);

            int key = waitKey(30);
            if (key == 27) // ESC
            {
                break;
            }
            else if (key == 32) // Пробел
            {
                zoomMode = false;
                setMouseCallback("Image", onMouseMain, NULL);
            }
        }

        mouseMoved = false;
    }

    cout << "Координаты точек:" << endl;
    for (size_t i = 0; i < points.size(); ++i)
    {
        cout << i + 1 << ": (" << points[i].x << ", " << points[i].y << ")" << endl;
    }

    return 0;
}
