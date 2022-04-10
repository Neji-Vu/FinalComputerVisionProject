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

/** brief Performs the erosion operation

Detail:
	Pixel:	0 - Black - background
			255 - White - foreground

@return none
*/
cv::Mat MainForm::Dilation(const cv::Mat binImg) {
	cv::Mat l_dilationImg(binImg.rows, binImg.cols, CV_8UC1);

	for (int row = 0; row < binImg.rows; row++) {
		for (int col = 0; col < binImg.cols; col++) {
			// Set dilation image to black image
			l_dilationImg.at<uint8_t>(row, col) = 0;
		}
	}
	// Check type of SE
	switch (g_SEInfo.type) {
	/// Cross shape size = 5:
	///			|*|
	///		  |*|*|*|
	///		|*|*|*|*|*|
	///		  |*|*|*|
	/// 		|*|
	// Type of SE is Cross
	case CROSS:
		for (int row = g_SEInfo.size; row < binImg.rows - g_SEInfo.size; row++) {
			for (int col = g_SEInfo.size; col < binImg.cols - g_SEInfo.size; col++) {

				if (binImg.at<uint8_t>(row, col) == 255) { //check current pixel == 255

					for (int SE_row = -1; SE_row <= 1; SE_row++) {
						for (int SE_col = -1; SE_col <= 1; SE_col++) {
							l_dilationImg.at<uint8_t>(row + SE_row, col + SE_col) = 255; //set current pixel to 0
						}
					}

				}
				else {
					//do nothing
				}

			}
		}
		break;
	// Type of SE is Square
	case SQUARE:
		// Handle the pixels inside the image
		for (int row = g_SEInfo.size; row < binImg.rows - g_SEInfo.size; ++row) {
			for (int col = g_SEInfo.size; col < binImg.cols - g_SEInfo.size; ++col) {
				// If current pixel is 255 then set surrounding pixels to 255
				if (binImg.at<uint8_t>(row, col) == 255) {

					for (int SE_row = -g_SEInfo.size; SE_row <= g_SEInfo.size; ++SE_row) {
						for (int SE_col = -g_SEInfo.size; SE_col <= g_SEInfo.size; ++SE_col) {
							// Set current pixel to 255
							l_dilationImg.at<uint8_t>(row + SE_row, col + SE_col) = 255;
						}
					}

				}
				// Else meaning current pixel is 0 then do nothing
				else {
					// Do nothing
				}

			}
		}
		
		// Handle the pixels at the border of the image
		// Upper border
		for (int row = 0; row < g_SEInfo.size; ++row) {
			for (int col = g_SEInfo.size; col < binImg.cols - g_SEInfo.size; ++col) {
				// If current pixel is 255 then set surrounding pixels to 255
				if (binImg.at<uint8_t>(row, col) == 255) {

					for (int SE_row = -row; SE_row <= g_SEInfo.size; ++SE_row) {
						for (int SE_col = -g_SEInfo.size; SE_col <= g_SEInfo.size; ++SE_col) {
							// Set current pixel to 255
							l_dilationImg.at<uint8_t>(row + SE_row, col + SE_col) = 255;
						}
					}

				}
				// Else meaning current pixel is 0 then do nothing
				else {
					// Do nothing
				}

			}
		}

		// Bottom border
		for (int row = (binImg.rows - g_SEInfo.size); row < binImg.rows; ++row) {
			for (int col = g_SEInfo.size; col < binImg.cols - g_SEInfo.size; ++col) {
				// If current pixel is 255 then set surrounding pixels to 255
				if (binImg.at<uint8_t>(row, col) == 255) {

					for (int SE_row = -g_SEInfo.size; SE_row <= ((binImg.rows - 1U) - row); ++SE_row) { // (binImg - 1U) is size of image
						for (int SE_col = -g_SEInfo.size; SE_col <= g_SEInfo.size; ++SE_col) {
							// Set current pixel to 255
							l_dilationImg.at<uint8_t>(row + SE_row, col + SE_col) = 255;
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

	namedWindow("Dilation image", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Dilation image", l_dilationImg);         // Show our image inside it.

	return l_dilationImg;

} // Func: Dilation()

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
	imshow("Binary image", g_binaryImage);         // Show our image inside it.

	// Calc dilation and show to pic box
	MainForm::Dilation(g_binaryImage);

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
	g_SEInfo.type = CROSS;
}

System::Void MainForm::clickSquareButton(System::Object^  sender, System::EventArgs^  e) {
	g_SEInfo.type = SQUARE;
}

System::Void MainForm::scrollSETrackBar(System::Object^  sender, System::EventArgs^  e) {
	this->m_isTrackBartextBox->Text = (this->m_isSESizeTrackBar->Value*2 +1).ToString();

	// read value from SE trackbar to SE size
	g_SEInfo.size = this->m_isSESizeTrackBar->Value;

	// Calc dilation and show to pic box
	MainForm::Dilation(g_binaryImage);
}

} // Namespace: FinalComputerVisionProject