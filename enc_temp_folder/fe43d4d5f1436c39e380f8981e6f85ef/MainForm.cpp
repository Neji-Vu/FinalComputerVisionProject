#include "MainForm.h"
#include <algorithm>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading;
using namespace System::Threading::Tasks;

using namespace System::Runtime::InteropServices;

/************************************************************************************************/
/* Main function for running																	*/
/************************************************************************************************/
[STAThreadAttribute] // STAThreadAttribute is needed to be able to read files from browser
int main(array<System::String ^> ^args)
{
	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);
	FinalComputerVisionProject::MainForm nameObjectForm;
	System::Windows::Forms::Application::Run(%nameObjectForm);

	return 0;
} // End of function: main()

/************************************************************************************************/
/* Private function of class MainForm															*/
/************************************************************************************************/
namespace FinalComputerVisionProject {

/** brief Performs the dilation operation

Detail:
	Pixel:	0 - Black - background
			255 - White - foreground

@return none
*/
cv::Mat MainForm::Dilation(const cv::Mat binImg) {
	// Original image holds dilation value
	cv::Mat l_dilationImg(binImg.rows, binImg.cols, CV_8UC1);
	//// Expand the size of the image to SE size 
	cv::Mat l_tempDilationImg(binImg.rows + (2*g_SEInfo.size), binImg.cols + (2*g_SEInfo.size), CV_8UC1);

	// Set dilation image to black image
	setColorImage(l_dilationImg, 0);
	// Set temp dilation image to black image
	setColorImage(l_tempDilationImg, 0);

	// Check type of SE
	switch (g_SEInfo.type) {
	// Type of SE is Square
	case SQUARE:

		// Handle the pixels inside the image
		for (int isMatrixRow = 0; isMatrixRow < binImg.rows; ++isMatrixRow) {
			for (int isMatrixCol = 0; isMatrixCol < binImg.cols; ++isMatrixCol) {

				// If current pixel is 255 then set surrounding pixels to 255
				if (binImg.at<uint8_t>(isMatrixRow, isMatrixCol) == 255) {

					for (int SE_row = -g_SEInfo.size; SE_row <= g_SEInfo.size; ++SE_row) {
						for (int SE_col = -g_SEInfo.size; SE_col <= g_SEInfo.size; ++SE_col) {

							// Convert rows and columns to dilation image
							int rowOfDilationImg = isMatrixRow + g_SEInfo.size;
							int colOfDilationImg = isMatrixCol + g_SEInfo.size;

							// Set current pixel to 255
							l_tempDilationImg.at<uint8_t>(rowOfDilationImg + SE_row, colOfDilationImg + SE_col) = 255;

						}
					}

				}
				// Else meaning current pixel is 0 then do nothing
				else {
					// Do nothing
				}

			}
		}

		break;
	/// Cross shape size = 3:
	///			|*|
	///		  |*|*|*|
	///		|*|*|*|*|*|
	///	  |*|*|*|*|*|*|*|
	///		|*|*|*|*|*|
	///		  |*|*|*|
	/// 		|*|
	// Type of SE is Diamond
	case DIAMOND:
		// Handle the pixels inside the image
		for (int isMatrixRow = 0; isMatrixRow < binImg.rows; ++isMatrixRow) {
			for (int isMatrixCol = 0; isMatrixCol < binImg.cols; ++isMatrixCol) {

				// Convert rows and columns to dilation image
				int rowOfDilationImg = isMatrixRow + g_SEInfo.size;
				int colOfDilationImg = isMatrixCol + g_SEInfo.size;

				// If current pixel is 255 then set surrounding pixels to 255
				if (binImg.at<uint8_t>(isMatrixRow, isMatrixCol) == 255) {

					for (int SE_row = -g_SEInfo.size; SE_row <= 0; ++SE_row) {

						int space = -g_SEInfo.size - SE_row - 1;

						// Example: g_SEInfo.size = 3
						// Check for:
						// -3		  |*|
						// -2		|*|*|*|
						// -1	  |*|*|*|*|*|
						//	0	|*|*|*|*|*|*|*|
						// Set pixels in SE to 255
						for (int SE_col = space + 1; SE_col <= space + (2 * (-space) - 1); ++SE_col) {
							// Set current pixel to 255
							l_tempDilationImg.at<uint8_t>(rowOfDilationImg + SE_row, colOfDilationImg + SE_col) = 255;
						}
					}

					for (int SE_row = 1; SE_row <= g_SEInfo.size; ++SE_row) {
						int space = g_SEInfo.size - SE_row;

						// Example: g_SEInfo.size = 3
						// Check for:
						// 1	|*|*|*|*|*|
						// 2	  |*|*|*|
						// 3	    |*|
						// Set pixels in SE to 255
						for (int SE_col = -space; SE_col < -space + (2 * space + 1); ++SE_col) {
							// Set current pixel to 255
							l_tempDilationImg.at<uint8_t>(rowOfDilationImg + SE_row, colOfDilationImg + SE_col) = 255;
						}
					}

				}
				// Else meaning current pixel is 0 then do nothing
				else {
					// Do nothing
				}

			}
		}

		break;
	default:
		break;
	}

	// Save result to global of dilation image
	for (int isMatrixRow = 0; isMatrixRow < l_dilationImg.rows; ++isMatrixRow) {
		for (int isMatrixCol = 0; isMatrixCol < l_dilationImg.cols; ++isMatrixCol) {

			l_dilationImg.at<uint8_t>(isMatrixRow, isMatrixCol) = l_tempDilationImg.at<uint8_t>(isMatrixRow + g_SEInfo.size, isMatrixCol + g_SEInfo.size);

		}
	}

	return l_dilationImg;
} // Func: Dilation()


cv::Mat MainForm::Erosion(const cv::Mat binImg) {
	// Original image holds erosion value
	cv::Mat l_erosionImg(binImg.rows, binImg.cols, CV_8UC1);
	// Set erosion image to black image
	setColorImage(l_erosionImg, 0);

	// Pixels at the edges and corners of the image:
	// If the pixels at the edges and corners are foreground, 
	// then checking the surrounding pixels will ignore the pixel values outside the image.
	// To omit these values when checking the condition at this position does not raise the flag value.
	// Which means the outer pixel value will be equal to 255 (foreground).
	cv::Mat l_tempBinImg(binImg.rows + (2 * g_SEInfo.size), binImg.cols + (2 * g_SEInfo.size), CV_8UC1);
	// Set temp binary image to white image (all pixels are foreground)
	setColorImage(l_tempBinImg, 255);
	// Set value inside image.
	for (int isMatrixRow = g_SEInfo.size; isMatrixRow < l_tempBinImg.rows - g_SEInfo.size; ++isMatrixRow) {
		for (int isMatrixCol = g_SEInfo.size; isMatrixCol < l_tempBinImg.cols - g_SEInfo.size; ++isMatrixCol) {

			l_tempBinImg.at<uint8_t>(isMatrixRow, isMatrixCol) = binImg.at<uint8_t>(isMatrixRow - g_SEInfo.size, isMatrixCol - g_SEInfo.size);

		}
	}

	// Check type of SE
	switch (g_SEInfo.type) {
		// Type of SE is Square
	case SQUARE:

		// Handle the pixels inside the image
		for (int isMatrixRow = g_SEInfo.size; isMatrixRow < l_tempBinImg.rows - g_SEInfo.size; ++isMatrixRow) {
			for (int isMatrixCol = g_SEInfo.size; isMatrixCol < l_tempBinImg.cols - g_SEInfo.size; ++isMatrixCol) {

				// Convert rows and columns to erosion image
				int rowOfDilationImg = isMatrixRow - g_SEInfo.size;
				int colOfDilationImg = isMatrixCol - g_SEInfo.size;

				// If current pixel is 0 (background)
				if (l_tempBinImg.at<uint8_t>(isMatrixRow, isMatrixCol) == 0) {
					// Keep the value is 0
					l_erosionImg.at<uint8_t>(rowOfDilationImg, colOfDilationImg) = 0;
				}
				// else if current value pixel is 255, check surrounding pixels.
				else {
					uint8_t flag = 0;
					for (int SE_row = -g_SEInfo.size; SE_row <= g_SEInfo.size; ++SE_row) {
						for (int SE_col = -g_SEInfo.size; SE_col <= g_SEInfo.size; ++SE_col) {

							// Any surrounding pixel is zero (background) then set current pixel value to 0
							if (l_tempBinImg.at<uint8_t>(isMatrixRow + SE_row, isMatrixCol + SE_col) == 0){
								flag++;
							}

						}
					}

					// Set current pixel by flag.
					// flag is 0 which means all surrounding pixels are foreround
					// flag is not 0 which means any surrounding pixel is backround
					l_erosionImg.at<uint8_t>(rowOfDilationImg, colOfDilationImg) = (flag == 0 ? 255 : 0);
				}

			}
		}

		break;
	// Type of SE is Diamond
	case DIAMOND:

		// Handle the pixels inside the image
		for (int isMatrixRow = g_SEInfo.size; isMatrixRow < l_tempBinImg.rows - g_SEInfo.size; ++isMatrixRow) {
			for (int isMatrixCol = g_SEInfo.size; isMatrixCol < l_tempBinImg.cols - g_SEInfo.size; ++isMatrixCol) {

				// Convert rows and columns to erosion image
				int rowOfDilationImg = isMatrixRow - g_SEInfo.size;
				int colOfDilationImg = isMatrixCol - g_SEInfo.size;

				// If current pixel is 0 (background)
				if (l_tempBinImg.at<uint8_t>(isMatrixRow, isMatrixCol) == 0) {
					// Keep the value is 0
					l_erosionImg.at<uint8_t>(rowOfDilationImg, colOfDilationImg) = 0;
				}
				// else if current value pixel is 255, check surrounding pixels.
				else {
					uint8_t flag = 0;

					// Example: g_SEInfo.size = 3
					// Check for:
					// -3		  |*|
					// -2		|*|*|*|
					// -1	  |*|*|*|*|*|
					//	0	|*|*|*|*|*|*|*|
					for (int SE_row = -g_SEInfo.size; SE_row <= 0; ++SE_row) {

						int space = -g_SEInfo.size - SE_row - 1;

						for (int SE_col = space + 1; SE_col <= space + (2 * (-space) - 1); ++SE_col) {

							// Any surrounding pixel is zero (background) then set current pixel value to 0
							if (l_tempBinImg.at<uint8_t>(isMatrixRow + SE_row, isMatrixCol + SE_col) == 0) {
								flag++;
							}
						}
					}

					// Example: g_SEInfo.size = 3
					// Check for:
					// 1	|*|*|*|*|*|
					// 2	  |*|*|*|
					// 3	    |*|
					for (int SE_row = 1; SE_row <= g_SEInfo.size; ++SE_row) {
						int space = g_SEInfo.size - SE_row;

						for (int SE_col = -space; SE_col < -space + (2 * space + 1); ++SE_col) {

							// Any surrounding pixel is zero (background) then set current pixel value to 0
							if (l_tempBinImg.at<uint8_t>(isMatrixRow + SE_row, isMatrixCol + SE_col) == 0) {
								flag++;
							}
						}
					}

					// Set current pixel by flag.
					// flag is 0 which means all surrounding pixels are foreround
					// flag is not 0 which means any surrounding pixel is backround
					l_erosionImg.at<uint8_t>(rowOfDilationImg, colOfDilationImg) = (flag == 0 ? 255 : 0);
				}

			}
		}

		break;

	default:
		break;
	}

	return l_erosionImg;
}

cv::Mat MainForm::Opening(const cv::Mat binImg) {
	// Original image holds opening value
	cv::Mat l_openingImg(binImg.rows, binImg.cols, CV_8UC1);
	// Set opening image to black image
	setColorImage(l_openingImg, 0);

	// perform erosion 
	l_openingImg = Erosion(binImg);
	// perform dilation
	l_openingImg = Dilation(l_openingImg);

	return l_openingImg;

}

cv::Mat MainForm::Closing(const cv::Mat binImg) {
	// Original image holds closing value
	cv::Mat l_closingImg(binImg.rows, binImg.cols, CV_8UC1);
	// Set closing image to black image
	setColorImage(l_closingImg, 0);

	// perform dilation 
	l_closingImg = Dilation(binImg);
	// perform erosion
	l_closingImg = Erosion(l_closingImg);

	return l_closingImg;

}

/** brief set image to black image

@param img image need to set
@param value value of pixel
@return cv::Mat black image
*/
void MainForm::setColorImage(cv::Mat& img, const uint8_t value){

	for (int isMatrixRow = 0; isMatrixRow < img.rows; ++isMatrixRow) {
		for (int isMatrixCol = 0; isMatrixCol < img.cols; ++isMatrixCol) {

			// Set image to black image
			img.at<uint8_t>(isMatrixRow, isMatrixCol) = value;

		}
	}
	
}

/** brief Read image from browser and display in picture box

@return none
*/
System::Void MainForm::clickLoadImageButton(System::Object^  sender, System::EventArgs^  e) {

	// Read image from explorer
	OpenFileDialog^ opDialog = gcnew OpenFileDialog();
	opDialog->Filter = "Image Files (*.bmp;*.jpg;*.jpeg,*.png)|*.BMP;*.JPG;*.JPEG;*.PNG|All files (*.*)|*.*||";
	if (opDialog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel)
	{
		return;
	}

	// Resize image with aspect ratio
	g_colorImage = ToResize(cv::imread(ConvertString2Char(opDialog->FileName))); // default is color image

	// Convert to grayscale image
	cv::cvtColor(g_colorImage, g_grayImage, CV_BGR2GRAY);

	// Convert to binary image
	// pixel is greater than thresh(128) then is set to maxval(255)
	// else is set to 0
	cv::threshold(g_grayImage, g_binaryImage, 128, 255, THRESH_BINARY);

	// Display binary image
	namedWindow("Binary image", WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Binary image", g_binaryImage);         // Show our image inside it.

	showImage();

} // Func: clickBrowserButton()

/** brief Convert System::String^ to char*

@param control Point to the picture box of the form
@param str System::String^ pointer
@return strs char* pointer
*/
char * MainForm::ConvertString2Char(System::String^ str) {	// Marshal method
	char* strs = (char*)(void*)Marshal::StringToHGlobalAnsi(str);
	return strs;
}

/** brief Resize the source image

@param src Image needs to be resized
@return dst Image has been resized
*/
cv::Mat MainForm::ToResize(cv::Mat src) {
	cv::Mat dst;
	float ratio = min(((float)800 / (float)src.cols), ((float)450 / (float)src.rows));
	cv::resize(src, dst, cv::Size(src.cols*ratio, src.rows*ratio), 0, 0, INTER_LINEAR);
	return dst;

}

float min(float x, float y)
{
	return (x < y) ? x : y;
}

System::Void MainForm::clickCrossButton(System::Object^  sender, System::EventArgs^  e) {
	g_SEInfo.type = DIAMOND;

	showImage();
}

System::Void MainForm::clickSquareButton(System::Object^  sender, System::EventArgs^  e) {
	g_SEInfo.type = SQUARE;

	showImage();
}

System::Void MainForm::scrollSETrackBar(System::Object^  sender, System::EventArgs^  e) {
	this->m_isTrackBartextBox->Text = (this->m_isSESizeTrackBar->Value*2 +1).ToString();

	// read value from SE trackbar to SE size
	g_SEInfo.size = this->m_isSESizeTrackBar->Value;

	showImage();
}

void MainForm::showImage(void) {
	// Calc dilation and show to pic box
	g_dilationImage = MainForm::Dilation(g_binaryImage);

	// Calc erosion and show to pic box
	g_erosionImage = MainForm::Erosion(g_binaryImage);

	// Calc opening and show to pic box
	g_openingImage = MainForm::Opening(g_binaryImage);

	// Calc closing and show to pic box
	g_closingImage = MainForm::Closing(g_binaryImage);

	// Display dilation result
	namedWindow("Dilation image", WINDOW_AUTOSIZE);
	cv::imshow("Dilation image", g_dilationImage);

	// Display erosion result
	namedWindow("Erosion image", WINDOW_AUTOSIZE);
	cv::imshow("Erosion image", g_erosionImage);

	// Display opening result
	namedWindow("Opening image", WINDOW_AUTOSIZE);
	cv::imshow("Opening image", g_openingImage);

	// Display closing result
	namedWindow("Closing image", WINDOW_AUTOSIZE);
	cv::imshow("Closing image", g_closingImage);
}

} // Namespace: FinalComputerVisionProject