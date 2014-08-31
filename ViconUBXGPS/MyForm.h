#pragma once

#include "ViconToGeo.h"
#include "SerialWriter.h"
#include <msclr\marshal_cppstd.h>
namespace ViconUBXGPS {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
//..
	using namespace msclr::interop;
	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			terminateLoop = false;
			working = false;
			viconOn = false;
			//UpdateLabel("Nothing");
			//
			//TODO: Add the constructor code here
			//
			//convertedData = gcnew ViconToGeo();
			convertedData = gcnew ViconToGeo();//default to 1
			//initial code for the comboboxes
			for each (System::String^ s in SerialPort::GetPortNames())
			{
				comboBox2->Items->Add(s);
			}

			textBox1->Text = "131.181.87.177:801";

			button1->Enabled = false;
			button2->Enabled = false;
			Thread^ testThread = gcnew Thread(gcnew ThreadStart(this, &MyForm::MainLoop));
			testThread->Start();
		}

		System::String^ s;
		System::String^ textBOne;
		System::String^ textBTwo;
		System::String^ textBThree;
		bool terminateLoop;
		bool working;
		SerialWriter^ sw;
		bool viconOn;

	private: System::Windows::Forms::TextBox^  textBox1;
	public: 
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Button^  button6;

		int uavNum;

		void MainLoop(){
			
			System::String^ message = "";

			textBOne = gcnew System::String("");
			textBTwo = gcnew System::String("");
			textBThree = gcnew System::String("");

			//need this to update the text boxes
			while(!terminateLoop){
				
				if(working){
					
					if(viconOn == true){

						convertedData->GetandAssignViconData();
						//convertedData->ConvertViconToGeo();
					}

					//reset strings
					textBOne = "";
					textBTwo = "";
					textBThree = "";

					for (int i = 0; i < uavNum; i++){
						//sw->Send(convertedData->ToPayload(i));
					
						convertedData->SendPayload(i, sw);
						//sw->Send(convertedData->ToPayload());

						s = gcnew System::String("");
					
						//message = sw->Read();
						
						//if ((message != nullptr) && (message->Length > 0)){
							//Console::WriteLine("Message:" + message);
						//}

						//Update textboxes with new data
						if (this->IsHandleCreated && this->InvokeRequired){
							textBOne += convertedData->GetSentMessage();
							Invoke(gcnew MethodInvoker(this, &MyForm::UpdateTextBox1));
							//textBTwo += message;
							//Invoke(gcnew MethodInvoker(this, &MyForm::UpdateTextBox2));
							textBThree += convertedData->GetStringRep();
							Invoke(gcnew MethodInvoker(this, &MyForm::UpdateTextBox3));
						}
					}
				}
				//Sleep(200);

			}
			std::terminate();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button5;
	protected: 
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::RichTextBox^  richTextBox2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::RichTextBox^  richTextBox3;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::Label^  label6;


	private:
		void UpdateLabel(){
			label7->Text += s;
		}
		void UpdateTextBox1(){
			richTextBox1->Text = textBOne;

		}
		void UpdateTextBox2(){		
			richTextBox2->Text = textBTwo;
			
		}
		void UpdateTextBox3(){
			
			richTextBox3->Text = textBThree;
		
		}
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->richTextBox2 = (gcnew System::Windows::Forms::RichTextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->richTextBox3 = (gcnew System::Windows::Forms::RichTextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->button6 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(26, 59);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(107, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Start Transmission";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(26, 162);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(107, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Stop Transmission";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(156, 40);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->ReadOnly = true;
			this->richTextBox1->Size = System::Drawing::Size(322, 66);
			this->richTextBox1->TabIndex = 2;
			this->richTextBox1->Text = L"";
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(26, 241);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 3;
			this->button3->Text = L"Connect";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// comboBox1
			// 
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(11) {L"1200", L"2400", L"4800", L"9600", L"14400", L"19200", 
				L"28800", L"38400", L"56000", L"57600", L"115200"});
			this->comboBox1->Location = System::Drawing::Point(302, 243);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 21);
			this->comboBox1->TabIndex = 4;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged);
			// 
			// comboBox2
			// 
			this->comboBox2->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Location = System::Drawing::Point(156, 243);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(121, 21);
			this->comboBox2->TabIndex = 5;
			this->comboBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox2_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(189, 227);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(50, 13);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Com Port";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(335, 227);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(58, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"Baud Rate";
			// 
			// richTextBox2
			// 
			this->richTextBox2->Location = System::Drawing::Point(156, 141);
			this->richTextBox2->Name = L"richTextBox2";
			this->richTextBox2->ReadOnly = true;
			this->richTextBox2->Size = System::Drawing::Size(322, 66);
			this->richTextBox2->TabIndex = 8;
			this->richTextBox2->Text = L"";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(254, 24);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(101, 13);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Out Going Message";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(254, 125);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(96, 13);
			this->label4->TabIndex = 10;
			this->label4->Text = L"Incoming Message";
			// 
			// richTextBox3
			// 
			this->richTextBox3->Location = System::Drawing::Point(526, 40);
			this->richTextBox3->Name = L"richTextBox3";
			this->richTextBox3->ReadOnly = true;
			this->richTextBox3->Size = System::Drawing::Size(285, 285);
			this->richTextBox3->TabIndex = 11;
			this->richTextBox3->Text = L"";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(649, 24);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(55, 13);
			this->label5->TabIndex = 12;
			this->label5->Text = L"Raw Data";
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(26, 302);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(107, 23);
			this->button4->TabIndex = 13;
			this->button4->Text = L"Vicon Connect";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(373, 303);
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(120, 20);
			this->numericUpDown1->TabIndex = 14;
			this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(410, 287);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(34, 13);
			this->label6->TabIndex = 15;
			this->label6->Text = L"UAVs";
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(761, 2);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 23);
			this->button5->TabIndex = 17;
			this->button5->Text = L"Close";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(156, 304);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(194, 20);
			this->textBox1->TabIndex = 18;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(153, 287);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(75, 13);
			this->label7->TabIndex = 19;
			this->label7->Text = L"Vicon Address";
			// 
			// button6
			// 
			this->button6->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"button6.Image")));
			this->button6->Location = System::Drawing::Point(126, 241);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(24, 23);
			this->button6->TabIndex = 20;
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &MyForm::button6_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(838, 349);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->numericUpDown1);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->richTextBox3);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->richTextBox2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboBox2);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->Text = L" ";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MyForm::MyForm_FormClosed);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		
	private: 

		ViconToGeo^ convertedData;// = new ViconToGeo();
			
		int baudRate;
		System::String^ comPort;

		//stop transmission button
		System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {

			//convertedData->SetCommunicationContinue(false);
			button1->Enabled = true;
			//button3->Enabled = true;
			button2->Enabled = false;
			//terminateLoop = true;
			working = false;
		}
		//Baud rate combo box
		System::Void comboBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			
			comPort = comboBox2->Text;
		}
		//Start transmission button
		System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			
			uavNum = (int)numericUpDown1->Value;
			
			//convertedData = gcnew ViconToGeo(uavNum);
			convertedData->SetUavs(uavNum);//should work, will need some tweaking
			//gray out all other buttons
			button1->Enabled = false;
			button2->Enabled = true;
			//button3->Enabled = false;
			//convertedData->StartTransmissions();
			working = true;

		}
		//Connect to comport
		System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {

			if(button3->Text == "Connect"){
				sw = gcnew SerialWriter(comPort, baudRate);
				if (sw->IsOpen()){
					button3->Text = "Disconnect";
					button1->Enabled = true;
				}
				
			} else {
				sw->Close();
				button3->Text = "Connect";
				button1->Enabled = false;
			}

		 }
		//Com port selection
		System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			baudRate = Int32::Parse(comboBox1->Text);
			
		 }
		
		//Vicon connect button - needs a setting function in the Vicon to Geo Class
		System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {

			//on/off code for it changes the text of the button
			if(button4->Text == "Vicon Connect"){
				
				std::string hostName = marshal_as<std::string>(textBox1->Text); 

				convertedData->ConnectToVicon(hostName);
				button4->Text = "Vicon Disconnect";
				viconOn = true;
				
			} else {
				convertedData->DisconnectFromVicon();
				button4->Text = "Vicon Connect";
				viconOn = false;
			}

		}

		System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
			terminateLoop = true;
			//std::terminate();
			
			//this->Close();
			Application::Exit();
			
			
			
		}
		
		System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
			
			comboBox2->Items->Clear();
			
			for each (System::String^ s in SerialPort::GetPortNames())
			{

				

				comboBox2->Items->Add(s);
			}
		 }
private: System::Void MyForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
		
			 Application::Exit();
			exit(0);
		 }
};
}

//void test(){
//			s = gcnew String("");
//			for(int i = 0; i < 100; i++){
//				
//				if(i == 99){
//					i = 0;
//				}
//				s = ""+i;
//				//label7->Text = ;
//				if (this->IsHandleCreated && this->InvokeRequired){
//					Invoke(gcnew MethodInvoker(this, &MyForm::UpdateLabel));
//				}
//				
//				//Console::WriteLine("" + i);
//				Sleep(500);
//			}
//
//		}