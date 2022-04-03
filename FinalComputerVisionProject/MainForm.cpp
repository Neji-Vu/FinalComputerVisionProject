#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading;
using namespace System::Threading::Tasks;

using namespace System::Runtime::InteropServices;

// Main function for running  
int main(array<System::String ^> ^args)
{
	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);
	FinalComputerVisionProject::MainForm nameObjectForm;
	System::Windows::Forms::Application::Run(%nameObjectForm);

	return 0;
}

// Private function of class MainForm
namespace FinalComputerVisionProject {

System::Void MainForm::clickBrowserButton(System::Object^  sender, System::EventArgs^  e) {

	Mat img;
	// read image from explorer
	System::Windows::Forms::OpenFileDialog^ opDialog = gcnew OpenFileDialog();
	opDialog->Filter = "Image(*.bmp; *.jpg)|*.bmp;*.jpg|All files (*.*)|*.*||";

	if (opDialog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel)
	{
		return;
	}

	// display image
	img = cv::imread(ConvertString2Char(opDialog->FileName));
	//img = cv::imread("Lenna.png", CV_LOAD_IMAGE_COLOR);
	DrawCVImage(MainForm::m_isPictureBoxForSourceImage, img);

} // end of function clickBrowserButton()

/** brief Display the image in the picture box

@param control point to the picture box of the form
@param colorImage image to be display
@return none
*/
void MainForm::DrawCVImage(System::Windows::Forms::Control^ control, cv::Mat& colorImage)
{
	System::Drawing::Graphics^ graphics = control->CreateGraphics();
	System::IntPtr ptr(colorImage.ptr());
	System::Drawing::Bitmap^ b;

	// check type of image
	switch (colorImage.type())
	{
	case CV_8UC3: // non-grayscale images are correctly displayed here
		b = gcnew System::Drawing::Bitmap(colorImage.cols, colorImage.rows, colorImage.step,
			System::Drawing::Imaging::PixelFormat::Format24bppRgb, ptr);
		break;
	case CV_8UC1: // grayscale images are incorrectly displayed here 
		b = gcnew System::Drawing::Bitmap(colorImage.cols, colorImage.rows, colorImage.step,
			System::Drawing::Imaging::PixelFormat::Format8bppIndexed, ptr);
		break;
	default:
		// error message
		break;
	}

	// display image to picture box
	System::Drawing::RectangleF rect(0, 0, (float)control->Width, (float)control->Height);
	graphics->DrawImage(b, rect);

} // end of function DrawCVImage()

/** brief Convert System::String^ to char*

@param control point to the picture box of the form
@param str System::String^ pointer
@return strs char* pointer
*/
char * MainForm::ConvertString2Char(System::String^ str) {	// Marshal method
	char* strs = (char*)(void*)Marshal::StringToHGlobalAnsi(str);
	return strs;
} // end of function ConvertString2Char()

} // end of namespace FinalComputerVisionProject