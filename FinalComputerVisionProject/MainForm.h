#pragma once
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/opencv.hpp>

namespace FinalComputerVisionProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace System::Runtime::InteropServices;
	using namespace cv;
	using namespace std;

	enum SEType {
		SQUARE,
		DIAMOND
	};

	struct StructuringElement {
		SEType type;
		uint8_t size;
	};

	// Global variables store image values
	cv::Mat g_colorImage;
	cv::Mat g_grayImage;
	cv::Mat g_binaryImage;

	cv::Mat g_dilationImage;
	cv::Mat g_erosionImage;
	cv::Mat g_openingImage;
	cv::Mat g_closingImage;

	StructuringElement g_SEInfo { SQUARE, 1 };

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  m_isLoadImgButton;
	protected:

	protected:



	private: System::Windows::Forms::Label^  m_isSETypeLabel;
	private: System::Windows::Forms::Button^  m_isSquareButton;
	private: System::Windows::Forms::Button^  m_isCrossButton;
	private: System::Windows::Forms::Label^  m_isSESizeLabel;









	private: System::Windows::Forms::TrackBar^  m_isSESizeTrackBar;
	private: System::Windows::Forms::TextBox^  m_isTrackBartextBox;

	private: System::ComponentModel::IContainer^  components;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->m_isLoadImgButton = (gcnew System::Windows::Forms::Button());
			this->m_isSETypeLabel = (gcnew System::Windows::Forms::Label());
			this->m_isSquareButton = (gcnew System::Windows::Forms::Button());
			this->m_isCrossButton = (gcnew System::Windows::Forms::Button());
			this->m_isSESizeLabel = (gcnew System::Windows::Forms::Label());
			this->m_isSESizeTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->m_isTrackBartextBox = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->m_isSESizeTrackBar))->BeginInit();
			this->SuspendLayout();
			// 
			// m_isLoadImgButton
			// 
			this->m_isLoadImgButton->Location = System::Drawing::Point(50, 40);
			this->m_isLoadImgButton->Name = L"m_isLoadImgButton";
			this->m_isLoadImgButton->Size = System::Drawing::Size(150, 40);
			this->m_isLoadImgButton->TabIndex = 0;
			this->m_isLoadImgButton->Text = L"Load Image";
			this->m_isLoadImgButton->UseVisualStyleBackColor = true;
			this->m_isLoadImgButton->Click += gcnew System::EventHandler(this, &MainForm::clickLoadImageButton);
			// 
			// m_isSETypeLabel
			// 
			this->m_isSETypeLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->m_isSETypeLabel->Location = System::Drawing::Point(250, 20);
			this->m_isSETypeLabel->Name = L"m_isSETypeLabel";
			this->m_isSETypeLabel->Size = System::Drawing::Size(220, 20);
			this->m_isSETypeLabel->TabIndex = 4;
			this->m_isSETypeLabel->Text = L"Structuring Element Type:";
			// 
			// m_isSquareButton
			// 
			this->m_isSquareButton->Location = System::Drawing::Point(500, 10);
			this->m_isSquareButton->Name = L"m_isSquareButton";
			this->m_isSquareButton->Size = System::Drawing::Size(120, 40);
			this->m_isSquareButton->TabIndex = 5;
			this->m_isSquareButton->Text = L"Square";
			this->m_isSquareButton->UseVisualStyleBackColor = true;
			this->m_isSquareButton->Click += gcnew System::EventHandler(this, &MainForm::clickSquareButton);
			// 
			// m_isCrossButton
			// 
			this->m_isCrossButton->Location = System::Drawing::Point(650, 10);
			this->m_isCrossButton->Name = L"m_isCrossButton";
			this->m_isCrossButton->Size = System::Drawing::Size(120, 40);
			this->m_isCrossButton->TabIndex = 5;
			this->m_isCrossButton->Text = L"Diamond";
			this->m_isCrossButton->UseVisualStyleBackColor = true;
			this->m_isCrossButton->Click += gcnew System::EventHandler(this, &MainForm::clickCrossButton);
			// 
			// m_isSESizeLabel
			// 
			this->m_isSESizeLabel->AutoSize = true;
			this->m_isSESizeLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->m_isSESizeLabel->Location = System::Drawing::Point(250, 70);
			this->m_isSESizeLabel->Name = L"m_isSESizeLabel";
			this->m_isSESizeLabel->Size = System::Drawing::Size(214, 20);
			this->m_isSESizeLabel->TabIndex = 6;
			this->m_isSESizeLabel->Text = L"Structuring Element Size:";
			// 
			// m_isSESizeTrackBar
			// 
			this->m_isSESizeTrackBar->AutoSize = false;
			this->m_isSESizeTrackBar->Location = System::Drawing::Point(500, 60);
			this->m_isSESizeTrackBar->Maximum = 7;
			this->m_isSESizeTrackBar->Minimum = 1;
			this->m_isSESizeTrackBar->Name = L"m_isSESizeTrackBar";
			this->m_isSESizeTrackBar->Size = System::Drawing::Size(270, 50);
			this->m_isSESizeTrackBar->TabIndex = 9;
			this->m_isSESizeTrackBar->Value = 1;
			this->m_isSESizeTrackBar->Scroll += gcnew System::EventHandler(this, &MainForm::scrollSETrackBar);
			// 
			// m_isTrackBartextBox
			// 
			this->m_isTrackBartextBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->m_isTrackBartextBox->Location = System::Drawing::Point(780, 70);
			this->m_isTrackBartextBox->Name = L"m_isTrackBartextBox";
			this->m_isTrackBartextBox->Size = System::Drawing::Size(45, 19);
			this->m_isTrackBartextBox->TabIndex = 10;
			this->m_isTrackBartextBox->Text = L"3";
			this->m_isTrackBartextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(860, 120);
			this->Controls->Add(this->m_isTrackBartextBox);
			this->Controls->Add(this->m_isSESizeTrackBar);
			this->Controls->Add(this->m_isSESizeLabel);
			this->Controls->Add(this->m_isCrossButton);
			this->Controls->Add(this->m_isSquareButton);
			this->Controls->Add(this->m_isSETypeLabel);
			this->Controls->Add(this->m_isLoadImgButton);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->m_isSESizeTrackBar))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		// private function for handling image
	private: System::Void clickLoadImageButton(System::Object^  sender, System::EventArgs^  e);
	private: void setColorImage(cv::Mat&, const uint8_t);
	private: cv::Mat Dilation(const cv::Mat);
	private: cv::Mat Erosion(const cv::Mat);
	private: cv::Mat Opening(const cv::Mat);
	private: cv::Mat Closing(const cv::Mat);
	private: void showImage(void);
	private: System::Void clickCrossButton(System::Object^  sender, System::EventArgs^  e);
	private: System::Void clickSquareButton(System::Object^  sender, System::EventArgs^  e);
	private: System::Void scrollSETrackBar(System::Object^  sender, System::EventArgs^  e);

	private: char * ConvertString2Char(System::String^ str);
	private: cv::Mat ToResize(cv::Mat src);
	
}; // Class: MainForm

} // Namespace: FinalComputerVisionProject
