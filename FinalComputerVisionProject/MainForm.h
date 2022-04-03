#pragma once
#include "opencv2/opencv.hpp"

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
	private: System::Windows::Forms::Button^  m_isBrowserButton;
	protected:

	private: System::Windows::Forms::Button^  m_isProcessButton;
	private: System::Windows::Forms::PictureBox^  m_isPictureBoxForSourceImage;


	private: System::Windows::Forms::Label^  m_isLabelSourceImage;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->m_isBrowserButton = (gcnew System::Windows::Forms::Button());
			this->m_isProcessButton = (gcnew System::Windows::Forms::Button());
			this->m_isPictureBoxForSourceImage = (gcnew System::Windows::Forms::PictureBox());
			this->m_isLabelSourceImage = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->m_isPictureBoxForSourceImage))->BeginInit();
			this->SuspendLayout();
			// 
			// m_isBrowserButton
			// 
			this->m_isBrowserButton->Location = System::Drawing::Point(10, 10);
			this->m_isBrowserButton->Name = L"m_isBrowserButton";
			this->m_isBrowserButton->Size = System::Drawing::Size(75, 35);
			this->m_isBrowserButton->TabIndex = 0;
			this->m_isBrowserButton->Text = L"Browser";
			this->m_isBrowserButton->UseVisualStyleBackColor = true;
			this->m_isBrowserButton->Click += gcnew System::EventHandler(this, &MainForm::clickBrowserButton);
			// 
			// m_isProcessButton
			// 
			this->m_isProcessButton->Location = System::Drawing::Point(100, 10);
			this->m_isProcessButton->Name = L"m_isProcessButton";
			this->m_isProcessButton->Size = System::Drawing::Size(75, 35);
			this->m_isProcessButton->TabIndex = 1;
			this->m_isProcessButton->Text = L"Process";
			this->m_isProcessButton->UseVisualStyleBackColor = true;
			// 
			// m_isPictureBoxForSourceImage
			// 
			this->m_isPictureBoxForSourceImage->Location = System::Drawing::Point(10, 100);
			this->m_isPictureBoxForSourceImage->Name = L"m_isPictureBoxForSourceImage";
			this->m_isPictureBoxForSourceImage->Size = System::Drawing::Size(1000, 1000);
			this->m_isPictureBoxForSourceImage->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->m_isPictureBoxForSourceImage->TabIndex = 2;
			this->m_isPictureBoxForSourceImage->TabStop = false;
			// 
			// m_isLabelSourceImage
			// 
			this->m_isLabelSourceImage->AutoSize = true;
			this->m_isLabelSourceImage->Location = System::Drawing::Point(10, 60);
			this->m_isLabelSourceImage->Name = L"m_isLabelSourceImage";
			this->m_isLabelSourceImage->Size = System::Drawing::Size(109, 20);
			this->m_isLabelSourceImage->TabIndex = 3;
			this->m_isLabelSourceImage->Text = L"Source Image";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1100, 1110);
			this->Controls->Add(this->m_isLabelSourceImage);
			this->Controls->Add(this->m_isPictureBoxForSourceImage);
			this->Controls->Add(this->m_isProcessButton);
			this->Controls->Add(this->m_isBrowserButton);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->m_isPictureBoxForSourceImage))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	// private function for handling image
	private: System::Void clickBrowserButton(System::Object^  sender, System::EventArgs^  e);
	private: void DrawCVImage(System::Windows::Forms::Control^ control, cv::Mat& colorImage);

	private: char * ConvertString2Char(System::String^ str);

	};
}
