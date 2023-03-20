// Fill out your copyright notice in the Description page of Project Settings.


#include "NexusBPLibrary.h"


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <tchar.h>
//#include "Logging/LogMacros.h"


// TODO: create multiple dynamic streams
HANDLE serialPortH; // global reference to serial port



// TODO: convert to UStruct and broader Hardware UStruct
struct Port {
	int module;
	FString data;
};
Port ports[8] = { {0, "0"}, {0, "0"}, {0, "0"}, {0, "0"}, {0, "0"}, {0, "0"}, {0, "0"}, {0, "0"} };
int portCount = 8;

// stream
// connect to a stream
bool UNexus::StreamConnect(FString portNumber, bool wireless)
{
	// create path name
	FString portCombo = "COM" + portNumber;
	std::string portS = TCHAR_TO_UTF8(*portCombo);

	// create file IO and set characteristics
	std::wstring portWS;
	portWS.assign(portS.begin(), portS.end());
	serialPortH = CreateFile(portWS.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	// try to open port
	if (serialPortH == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Device Not Found"));
			return false;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error"));
		return false;
	}

	// define serial properties
	DCB dcbSP = { 0 };
	dcbSP.DCBlength = sizeof(dcbSP);
	if (!GetCommState(serialPortH, &dcbSP)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("State Error"));
		return false;
	}
	dcbSP.BaudRate = CBR_115200;
	dcbSP.ByteSize = 8;
	dcbSP.StopBits = ONESTOPBIT;
	dcbSP.Parity = NOPARITY;

	if (!SetCommState(serialPortH, &dcbSP)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Serial Port Set Error"));
		return false;
	}

	// create timeout detection
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (!SetCommTimeouts(serialPortH, &timeouts)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Timeout Error"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Stream successfuly started"));

	return true;
}

// disconnect from a stream
void UNexus::StreamDisconnect()
{
	CloseHandle(serialPortH);
	UE_LOG(LogTemp, Warning, TEXT("Stream Ended"));
}

// parse stream
bool UNexus::StreamDimension(FString& stream) {

	// file buffer variables
	char szBuff[256] = { 0 };
	DWORD dwBytesRead = 0;

	// string conversion variables
	std::string s = "";
	std::string fData = "";

	// read from serial port
	if (!ReadFile(serialPortH, szBuff, 256, &dwBytesRead, NULL)) {
		UE_LOG(LogTemp, Error, TEXT("Stream is closed"));
		return false;
	}
	else {

		// add characters from buffer to string
		for (int i = 0; i < sizeof(szBuff); i++) {
			s = s + szBuff[i];
			//what to do instead...
			//....
			//populate a dictionary
			//data structure will be composed of
			//int Key
			//int Value []
			/*
				if (szBuff[i] is newline) 
					dump what we have into the queue
					isLastColon = false
				else if (szBuff[i] is semicolon) 
					data.key = stoi(keyBuffer)
					data.value = stoi(valueBuffer)
					valueBuffer = ""
					isLastColon = false
				else if (szBuff[i] is colon)
					isLastColon = true
					if valueBuffer is not empty {
						append(stoi(valueBuffer)) to data.Value array.
						valueBuffer = ""
					}
				else //we are now a number... 
					if (lastIsColon) // then we're now a value
					{
						valueBuffer += szBuff[i]
					}
					else keyBuffer += szBuff[i]
					isLastColon = false
			
			this new algorithm will replace everything below and it will help speed up
			all the other functions
			*/
		}

		// split string stream to each new line string
		std::istringstream iss(s);
		getline(iss, fData);

		// convert string to FString
		stream = UTF8_TO_TCHAR(fData.c_str());

		// split stream into ports 
		// MLKomar: The for loop sorts values in Ports[PORT VALUES] to PORT DATA
			//So why not store it immedietally while parsing and discard all the string parsing? 
			//In fact, why can't we just do this internally when ever stream is called?
		//UE_LOG(LogTemp, Warning, TEXT("%s"), UTF8_TO_TCHAR(fData.c_str()));
		for (int i = 0; i < 1; i++) {

			auto string1 = fData.substr(0, fData.find(":"));
			auto string2 = "20";
			if (string1.compare(string2) != 0)
				break;
			ports[i].module = std::stoi(fData.substr(0, fData.find(":")).c_str());
			ports[i].data = UTF8_TO_TCHAR(fData.substr(s.find(":") + 1, fData.find(";") - 2).c_str());

			fData = fData.erase(0, (fData.find(";") + 1));
		}

		// debug
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, (TEXT("%s"), UTF8_TO_TCHAR(fData.c_str())));
	}
	return true;
}

// send output to stream
void UNexus::DeviceOutput(FString value) {
	// file buffer variables
	char szBuff[256] = { 0 };
	DWORD dwBytesRead = 0;

	// convert from FString to string
	std::string val = std::string(TCHAR_TO_UTF8(*value));

	// pass string characters to buffer
	for (int i = 0; i < 40; i++) {
		szBuff[i] = val[i];
	}

	// send data
	WriteFile(serialPortH, szBuff, 256, &dwBytesRead, NULL);
}

// hardware
// abstract hardware data
void UNexus::Hardware(const FString stream, FString& port1, FString& port2, FString& port3, FString& port4, FString& port5, FString& port6, FString& port7, FString& port8) {

	// convert FString to string
	std::string s = std::string(TCHAR_TO_UTF8(*stream));

	// give access to individual port data
	port1 = ports[0].data;
	port2 = ports[1].data;
	port3 = ports[2].data;
	port4 = ports[3].data;
	port5 = ports[4].data;
	port6 = ports[5].data;
	port7 = ports[6].data;
	port8 = ports[7].data;
}

// modules
// TODO: create base module class to inherit from

// orientation
void UNexus::Orientation(const FString stream, float& xAxis, float& yAxis, float& zAxis, FRotator& orientation) {
	std::string _data = std::string(TCHAR_TO_UTF8(*stream));

	if (_data.length() > 9) {
		std::string o = _data.substr(0, _data.find(";"));

		o = o.erase(0, (o.find(":") + 1)); // remove orientation mode

		// set xAxis
		std::string _xAxis = o.substr(0, o.find(":"));
		if (_xAxis.length() > 0) {
			xAxis = std::stof(_xAxis);
			orientation.Yaw = xAxis;
		}

		// set yAxis
		o = o.erase(0, (o.find(":") + 1));
		std::string _yAxis = o.substr(0, o.find(":"));
		if (_yAxis.length() > 0) {
			yAxis = std::stof(_yAxis);
			orientation.Roll = yAxis;
		}

		// set zAxis
		o = o.erase(0, (o.find(":") + 1));
		std::string _zAxis = o.substr(0, o.find(":"));
		if (_zAxis.length() > 0) {
			zAxis = std::stof(_zAxis);
			orientation.Pitch = zAxis;
		}
	}
}

// button
void UNexus::Button(const int port, bool& found, bool& state) {
	//std::string val = std::string(TCHAR_TO_UTF8(*stream)); // convert FString to string

	//TODO: select stream index

	int mN = 1; // module number
	int mI = 0; // module index number
	bool f = false; // check if found
	bool search = false; // TODO: optimize performing search on start

	// automatically or manually find port
	if (search == false) {
		if (port == 0) {
			// automatic
			for (int i = 0; i < portCount; i++) {
				if (ports[i].module == mN) {
					f = true;
					search = true;

					mI = i; // set port index
					break;

				}
				else {
					f = false;
					search = true;
				}
			}
		}
		// manual
		else {
			if (ports[port - 1].module == mN) {
				f = true;
				search = true;

				mI = port - 1; // set port index

			}
			else {
				f = false;
				search = true;
			}
		}
	}

	if (f) {
		state = (bool)std::stoi(std::string(TCHAR_TO_UTF8(*ports[mI].data)));
	}
}

// color
void UNexus::Color(const int port, bool& found, FLinearColor& color, int& red, int& green, int& blue, int& alpha) {
	int mN = 4;
	int mI = 0;
	bool f = false;
	bool search = false;

	if (search == false) {
		if (port == 0) {
			for (int i = 0; i < portCount; i++) {
				if (ports[i].module == mN) {
					f = true;
					search = true;

					mI = i;
					break;

				}
				else {
					f = false;
					search = true;
				}
			}
		}
		else {
			if (ports[port - 1].module == mN) {
				f = true;
				search = true;

				mI = port - 1;

			}
			else {
				f = false;
				search = true;
			}
		}
	}

	if (f) {
		//TODO: generic parse data function
		std::string s = std::string(TCHAR_TO_UTF8(*ports[mI].data));
		std::string a = (s.substr(0, s.find(",")).c_str());
		s = s.erase(0, (s.find(",") + 1));
		std::string r = (s.substr(0, s.find(",")).c_str());
		s = s.erase(0, (s.find(",") + 1));
		std::string g = (s.substr(0, s.find(",")).c_str());
		s = s.erase(0, (s.find(",") + 1));
		std::string b = (s.substr(0, s.find(";")).c_str());

		alpha = std::stoi(a);
		red = std::stoi(r);
		green = std::stoi(g);
		blue = std::stoi(b);

		//TODO: raw color data to FLinearColor conversion
	}

}

// dial
void UNexus::Dial(const int port, bool& found, int& raw, int& degree) {
	int mN = 2;
	int mI = 0;
	bool f = false;
	bool search = false;

	if (search == false) {
		if (port == 0) {
			for (int i = 0; i < portCount; i++) {
				if (ports[i].module == mN) {
					f = true;
					search = true;

					mI = i;
					break;

				}
				else {
					f = false;
					search = true;
				}
			}
		}
		else {
			if (ports[port - 1].module == mN) {
				f = true;
				search = true;

				mI = port - 1;

			}
			else {
				f = false;
				search = true;
			}
		}
	}

	if (f) {
		//TODO: generic parse data function
		std::string s = std::string(TCHAR_TO_UTF8(*ports[mI].data));

		float _raw = std::stof(s);


		float _degree = ((_raw) / 4095) * (290) + 40;

		raw = std::stoi(s);
		degree = static_cast<int>(_degree);
	}
}

// digital
void UNexus::Digital(const int port, bool& found, bool& state) {
	int mN = 10;
	int mI = 0;
	bool f = false;
	bool search = false;

	// automatically or manually find port
	if (search == false) {
		if (port == 0) {
			// automatic
			for (int i = 0; i < portCount; i++) {
				if (ports[i].module == mN) {
					f = true;
					search = true;

					mI = i; // set port index
					break;

				}
				else {
					f = false;
					search = true;
				}
			}
		}
		// manual
		else {
			if (ports[port - 1].module == mN) {
				f = true;
				search = true;

				mI = port - 1; // set port index

			}
			else {
				f = false;
				search = true;
			}
		}
	}

	if (f) {
		state = (bool)std::stoi(std::string(TCHAR_TO_UTF8(*ports[mI].data)));
	}
}

// distance
void UNexus::Distance(const int port, bool& found, int& millimeters, int& inches) {
	int mN = 8;
	int mI = 0;
	bool f = false;
	bool search = false;

	if (search == false) {
		if (port == 0) {
			for (int i = 0; i < portCount; i++) {
				if (ports[i].module == mN) {
					f = true;
					search = true;

					mI = i;
					break;

				}
				else {
					f = false;
					search = true;
				}
			}
		}
		else {
			if (ports[port - 1].module == mN) {
				f = true;
				search = true;

				mI = port - 1;

			}
			else {
				f = false;
				search = true;
			}
		}
	}

	if (f) {
		//TODO: generic parse data function
		std::string s = std::string(TCHAR_TO_UTF8(*ports[mI].data));

		float _millimeters = std::stof(s);


		float _inches = (((_millimeters) / 25.4) * (10))/10;

		millimeters = std::stoi(s);
		inches = static_cast<int>(_inches);
	}
}

// environment
void UNexus::Environment(const int port, bool& found, float& temperatureCelsius, float& temperatureFahrenheit, float& humidity, float& gas, float& pressure) {
	int mN = 21;
	int mI = 0;
	bool f = false;
	bool search = false;

	if (search == false) {
		if (port == 0) {
			for (int i = 0; i < portCount; i++) {
				if (ports[i].module == mN) {
					f = true;
					search = true;

					mI = i;
					break;

				}
				else {
					f = false;
					search = true;
				}
			}
		}
		else {
			if (ports[port - 1].module == mN) {
				f = true;
				search = true;

				mI = port - 1;

			}
			else {
				f = false;
				search = true;
			}
		}
	}

	if (f) {
		std::string s = std::string(TCHAR_TO_UTF8(*ports[mI].data));
		std::string tC = (s.substr(0, s.find(",")).c_str());
		s = s.erase(0, (s.find(",") + 1));
		std::string p = (s.substr(0, s.find(",")).c_str());
		s = s.erase(0, (s.find(",") + 1));
		std::string h = (s.substr(0, s.find(",")).c_str());
		s = s.erase(0, (s.find(",") + 1));
		std::string g = (s.substr(0, s.find(";")).c_str());
		s = s.erase(0, (s.find(",") + 1));

		temperatureCelsius = std::stof(tC);
		temperatureFahrenheit = (((9 / 5) * temperatureCelsius * 10) / 10) + 32;
		humidity = std::stof(h);
		gas = std::stof(g);
		pressure = std::stof(p);
	}
}

// flex
void UNexus::Flex(const int port, bool& found, int& raw, int& bend) {
	int mN = 19;
	int mI = 0;
	bool f = false;
	bool search = false;

	if (search == false) {
		if (port == 0) {
			for (int i = 0; i < portCount; i++) {
				if (ports[i].module == mN) {
					f = true;
					search = true;

					mI = i;
					break;

				}
				else {
					f = false;
					search = true;
				}
			}
		}
		else {
			if (ports[port - 1].module == mN) {
				f = true;
				search = true;

				mI = port - 1;

			}
			else {
				f = false;
				search = true;
			}
		}
	}

	if (f) {
		//TODO: generic parse data function
		std::string s = std::string(TCHAR_TO_UTF8(*ports[mI].data));

		float _raw = std::stof(s);


		float _bend = ((_raw) / 4095) * (100);

		raw = std::stoi(s);
		bend = static_cast<int>(_bend);
	}
}

// force
void UNexus::Force(const int port, bool& found, int& raw, int& strength) {
	int mN = 20;
	int mI = 0;
	bool f = false;
	bool search = false;

	if (search == false) {
		if (port == 0) {
			for (int i = 0; i < portCount; i++) {
				if (ports[i].module == mN) {
					f = true;
					search = true;

					mI = i;
					break;

				}
				else {
					f = false;
					search = true;
				}
			}
		}
		else {
			if (ports[port - 1].module == mN) {
				f = true;
				search = true;

				mI = port - 1;

			}
			else {
				f = false;
				search = true;
			}
		}
	}

	if (f) {
		//TODO: generic parse data function
		std::string s = std::string(TCHAR_TO_UTF8(*ports[mI].data));

		float _raw = std::stof(s);


		float _strength = ((_raw)/4095)*(100);

		raw = std::stoi(s);
		strength = static_cast<int>(_strength);
	}
}

// gesture
void UNexus::Gesture(const int port, bool& found, EGestureDirection& direction) {

}

// glow
void UNexus::Glow(const int port, bool& found, FLinearColor color) {

}

// joystick
void UNexus::Joystick(const int port, bool& found, int& rawX, int& rawY, int& x, int& y) {

}

// light
void UNexus::Light(const int port, bool& found, int& raw, int& brightness) {
	int mN = 11;
	int mI = 0;
	bool f = false;
	bool search = false;

	if (search == false) {
		if (port == 0) {
			for (int i = 0; i < portCount; i++) {
				if (ports[i].module == mN) {
					f = true;
					search = true;

					mI = i;
					break;

				}
				else {
					f = false;
					search = true;
				}
			}
		}
		else {
			if (ports[port - 1].module == mN) {
				f = true;
				search = true;

				mI = port - 1;

			}
			else {
				f = false;
				search = true;
			}
		}
	}

	if (f) {
		//TODO: generic parse data function
		std::string s = std::string(TCHAR_TO_UTF8(*ports[mI].data));

		float _raw = std::stof(s);


		float _brightness = ((_raw) / 4095) * (100);

		raw = std::stoi(s);
		brightness = static_cast<int>(_brightness);
	}
}

// motion
void UNexus::Motion(const int port, bool& found, bool& movement) {

}

// move
void UNexus::Move(const int port, bool& found, int degree, FString& output) {
	int mN = 14;
	int mI = 0;
	bool f = false;
	bool search = false;

	if (search == false) {
		if (port == 0) {
			for (int i = 0; i < portCount; i++) {
				if (ports[i].module == mN) {
					f = true;
					search = true;

					mI = i;
					break;

				}
				else {
					f = false;
					search = true;
				}
			}
		}
		else {
			if (ports[port - 1].module == mN) {
				f = true;
				search = true;

				mI = port - 1;

			}
			else {
				f = false;
				search = true;
			}
		}
	}

	if (f) {
		std::string _port = std::to_string(mI + 1);
		std::string _degree = std::to_string(degree);

		std::string val = "3," + _port + "," + _degree;

		output = UTF8_TO_TCHAR(val.c_str());
	}
}

// proximity
void UNexus::Proximity(const int port, bool& found, int& raw, int& amount) {

}

// slider
void UNexus::Slider(const int port, bool& found, int& raw, int& position) {

}

// sound
void UNexus::Sound(const int port, bool& found, int& frequency) {


}

// spin
void UNexus::Spin(const int port, bool& found, int& rotation, ESpinDirection& direction) {

}

// thermal
void UNexus::Thermal(const int port, bool& found, TArray<float>& gridCelsius, TArray<float>& gridFahrenheit, float& averageTemperatureCelsius, float& averageTemperatureFahrenheit, float& centerTemperatureCelsius, float& centerTemperatureFahrenheit) {

}

// tone
void UNexus::Tone(const int port, bool& found, int frequency, int time, FString& output) {
	int mN = 17;
	int mI = 0;
	bool f = false;
	bool search = false;

	if (search == false) {
		if (port == 0) {
			for (int i = 0; i < portCount; i++) {
				if (ports[i].module == mN) {
					f = true;
					search = true;

					mI = i;
					break;

				}
				else {
					f = false;
					search = true;
				}
			}
		}
		else {
			if (ports[port - 1].module == mN) {
				f = true;
				search = true;

				mI = port - 1;

			}
			else {
				f = false;
				search = true;
			}
		}
	}

	if (f) {
		std::string _port = std::to_string(mI + 1);
		std::string _frequency = std::to_string(frequency);
		std::string _time = std::to_string(time);

		std::string val = "3," + _port + "," + _frequency + "," + _time;

		output = UTF8_TO_TCHAR(val.c_str());
	}
}

// touch
void UNexus::Touch(const int port, bool& found, int& presence) {

}

