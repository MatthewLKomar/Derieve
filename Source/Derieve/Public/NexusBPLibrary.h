// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "NexusBPLibrary.generated.h"


UENUM(BlueprintType)
enum class EGestureDirection : uint8
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

UENUM(BlueprintType)
enum class ESpinDirection : uint8
{
	STILL,
	CLOCKWISE,
	COUNTERCLOCKWISE,
};

// TODO: convert to UStruct and broader Hardware UStruct
struct Port {
	int module;
	FString data;
};

UCLASS(Blueprintable, Category = "Physical Data", meta = (Keywords = "device physical data magic"))
class DERIEVE_API UNexus : public UActorComponent
{
	GENERATED_BODY()
public:

	// stream
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Stream - Connect"), Category = "Nexus: Stream", meta = (Keywords = "connect nexus stream device magic"))
		bool StreamConnect(FString portNumber, bool wireless);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Stream - Disconnect"), Category = "Nexus: Stream", meta = (Keywords = "disconnect nexus stream device magic"))
		void StreamDisconnect();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Stream - Dimension"), Category = "Nexus: Stream", meta = (Keywords = "dimension nexus stream device magic"))
		bool StreamDimension(FString& stream);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Stream - Output"), Category = "Nexus: Stream", meta = (Keywords = "output nexus stream device magic"))
		void DeviceOutput(FString value);

	// hardware
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Hardware"), Category = "Nexus: Hardware", meta = (Keywords = "hardware nexus device magic"))
		void Hardware(const FString stream, FString& port1, FString& port2, FString& port3, FString& port4, FString& port5, FString& port6, FString& port7, FString& port8);

	// modules
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Orientation"), Category = "Nexus: Modules", meta = (Keywords = "orientation module nexus magic"))
		void Orientation(const FString stream, float& xAxis, float& yAxis, float& zAxis, FRotator& orientation);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Button Module"), Category = "Nexus: Modules", meta = (Keywords = "button module nexus magic"))
		void Button(const int port, bool& found, bool& state);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Color Module"), Category = "Nexus: Modules", meta = (Keywords = "color module nexus magic"))
		 void Color(const int port, bool& found, FLinearColor& color, int& red, int& green, int& blue, int& alpha);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Dial Module"), Category = "Nexus: Modules", meta = (Keywords = "dial module nexus magic"))
		 void Dial(const int port, bool& found, int& raw, int& degree);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Digital Module"), Category = "Nexus: Modules", meta = (Keywords = "digital module nexus magic"))
		 void Digital(const int port, bool& found, bool& state);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Distance Module"), Category = "Nexus: Modules", meta = (Keywords = "distance module nexus magic"))
		 void Distance(const int port, bool& found, int& millimeters, int& inches);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Environment Module"), Category = "Nexus: Modules", meta = (Keywords = "environment module nexus magic"))
		 void Environment(const int port, bool& found, float& temperatureCelsius, float& temperatureFahrenheit, float& humidity, float& gas, float& pressure);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Flex Module"), Category = "Nexus: Modules", meta = (Keywords = "flex module nexus magic"))
		 void Flex(const int port, bool& found, int& raw, int& bend);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Force Module"), Category = "Nexus: Modules", meta = (Keywords = "force module nexus magic"))
		 void Force(const int port, bool& found, int& raw, int& strength);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Gesture Module"), Category = "Nexus: Modules", meta = (Keywords = "gesture module nexus magic"))
		 void Gesture(const int port, bool& found, EGestureDirection& direction);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Glow Module"), Category = "Nexus: Modules", meta = (Keywords = "glow module nexus magic"))
		 void Glow(const int port, bool& found, FLinearColor color);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Joystick Module"), Category = "Nexus: Modules", meta = (Keywords = "joystick module nexus magic"))
		 void Joystick(const int port, bool& found, int& rawX, int& rawY, int& x, int& y);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Light Module"), Category = "Nexus: Modules", meta = (Keywords = "light module nexus magic"))
		 void Light(const int port, bool& found, int& raw, int& brightness);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Motion Module"), Category = "Nexus: Modules", meta = (Keywords = "motion module nexus magic"))
		 void Motion(const int port, bool& found, bool& movement);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Move Module"), Category = "Nexus: Modules", meta = (Keywords = "move module nexus magic"))
		 void Move(const int port, bool& found, int degree, FString& output);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Proximity Module"), Category = "Nexus: Modules", meta = (Keywords = "proximity module nexus magic"))
		 void Proximity(const int port, bool& found, int& raw, int& amount);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Slider Module"), Category = "Nexus: Modules", meta = (Keywords = "slider module nexus magic"))
		 void Slider(const int port, bool& found, int& raw, int& position);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Sound Module"), Category = "Nexus: Modules", meta = (Keywords = "sound module nexus magic"))
		 void Sound(const int port, bool& found, int& frequency);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Spin Module"), Category = "Nexus: Modules", meta = (Keywords = "spin module nexus magic"))
		 void Spin(const int port, bool& found, int& rotation, ESpinDirection& direction);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Thermal Module"), Category = "Nexus: Modules", meta = (Keywords = "thermal module nexus magic"))
		 void Thermal(const int port, bool& found, TArray<float>& gridCelsius, TArray<float>& gridFahrenheit, float& averageTemperatureCelsius, float& averageTemperatureFahrenheit, float& centerTemperatureCelsius, float& centerTemperatureFahrenheit);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Tone Module"), Category = "Nexus: Modules", meta = (Keywords = "tone module nexus magic"))
		 void Tone(const int port, bool& found, int frequency, int time, FString& output);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Touch Module"), Category = "Nexus: Modules", meta = (Keywords = "touch module nexus magic"))
		 void Touch(const int port, bool& found, int& presence);


	/*UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Tone Module"), Category = "Physical Data - Modules", meta = (Keywords = "data serial magic"))
		static FString Tone(int port, int pitch);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Servo Module"), Category = "Physical Data - Modules", meta = (Keywords = "data serial magic"))
		static FString Servo(int port, int degree);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "LED Module"), Category = "Physical Data - Modules", meta = (Keywords = "data serial magic"))
		static FString LED(int port, ELEDColor color);*/

private: 

	Port ports[8] = { {0, "0"}, {0, "0"}, {0, "0"}, {0, "0"}, {0, "0"}, {0, "0"}, {0, "0"}, {0, "0"} };
	int portCount = 8;

	TMap<FString, TArray<float>> PortData;

};
