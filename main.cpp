#include <iostream>
#include </usr/local/include/tesseract/baseapi.h>
#include </usr/local/include/leptonica/allheaders.h>
#include <fstream>
#include <sys/stat.h>
#include <string>
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "please provide the image name!" << std::endl;
        return 0;
    }
    else {
        //std::string inputFileName("/home/i59034/Documents/OCR/code/verisk/Tesseract-related/p1-16-medical.png");
        //argv[1]  = (char *)inputFileName.c_str();
        Pix *image = pixRead (argv[1]);
        tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
        std::string folderPrefix("/home/ankurmali/OCR-Verisk-master/data/tmp");
        std::string inputImage(argv[1]);
        // these two lines find t
        const int startLocation = inputImage.find_last_of("/");
        const int endLocation = inputImage.find_last_of(".");
        std::string prefixName = inputImage.substr(startLocation,endLocation-startLocation);

        std::string fullPrefix = folderPrefix +prefixName+"/";
        mkdir(fullPrefix.c_str(),S_IRUSR | S_IWUSR | S_IXUSR);
        api->Init(NULL, "eng");
        api->SetImage(image);
        api->SetPageSegMode(tesseract::PSM_AUTO_OSD);
        api->Recognize(0);
        tesseract::ResultIterator* ri = api->GetIterator();
        tesseract::PageIteratorLevel level = tesseract::RIL_TEXTLINE;//generate box level information
        int count = 0;
        if (ri != 0) {
            do {
                int x1, y1, x2, y2;//x1:left, y1:top, x2:right, y2:bottom
                ri->BoundingBox(level,&x1, &y1,&x2,&y2);
                int x = x1; // x coordinate
                int y = y1; // y coordniate
                int w = x2 - x1;//w = right-left
                int h = y2 - y1;//y = bottom - top
                BOX* cropWindow = boxCreate(x, y, w, h);
                PIX* croppedImage = pixClipRectangle(image, cropWindow, NULL);
                const std::string fullPath = fullPrefix+std::to_string(count)+".png";
                const char *fileName = fullPath.c_str();
                pixWritePng(fileName,croppedImage,1.0);
                //printf("%d, %d, %d, %d;\n", x1, y1, x2, y2);
                count = count + 1;
            } while(ri->Next(level));
        }
        std::cout<<fullPrefix;
        api->End();
        pixDestroy(&image);
        return 0;
    }
}
