#include "stdafx.h"
#include "stdio.h"
#include <string>
#include <windows.h>
#include "includes\injector\injector.hpp"
#include "includes\IniReader.h"

int CarCount = 46;
char CarININame[MAX_PATH] = "";

#define MAX_FILES 100

#define SIZE_OF_g_SND_ENGINE_DATA 0x7ED8F8
#define SIZE_OF_g_SND_DUAL_GINSU 0x7ED8FC
#define SIZE_OF_g_SND_CARDATAMAPPING 0x7ED900
#define SIZE_OF_g_SND_SHIFTPATTERNS 0x7ED904
#define SIZE_OF_g_SND_SWEETNERDATASET 0x7ED90C
#define SIZE_OF_g_SND_ACCELFROMIDLE 0x7ED910
#define SIZE_OF_g_SND_TURBODATASET 0x7ED914

#define g_SND_ENGINE_DATA 0x7F0DE0
#define g_SND_DUAL_GINSU 0x7F1AA8
#define g_SND_CARDATAMAPPING 0x7F2230
#define g_SND_SHIFTPATTERNS 0x7F2860
#define g_SND_SWEETNERDATASET 0x7F3040
#define g_SND_ACCELFROMIDLE 0x7F3248
#define g_SND_TURBODATASET 0x7F3338

#define CarTypeMappingPath "CarSoundData\\CarTypeMapping"
#define EngineDataPath "CarSoundData\\EngineData"
#define DualGinsuPath "CarSoundData\\DualGinsu"
#define CarDataMappingPath "CarSoundData\\CarDataMapping"
#define ShiftPatternsPath "CarSoundData\\ShiftPatterns"
#define SweetnerDataSetPath "CarSoundData\\SweetnerDataSet"
#define AccelFromIdlePath "CarSoundData\\AccelFromIdle"
#define TurboDataSetPath "CarSoundData\\TurboDataSet"

#define CarTypeMappingFilePath "CarSoundData\\CarTypeMapping\\%s.ini"
#define EngineDataFilePath "CarSoundData\\EngineData\\%02d.ini"
#define DualGinsuFilePath "CarSoundData\\DualGinsu\\%02d.ini"
#define CarDataMappingFilePath "CarSoundData\\CarDataMapping\\%02d.ini"
#define ShiftPatternsFilePath "CarSoundData\\ShiftPatterns\\%02d.ini"
#define SweetnerDataSetFilePath "CarSoundData\\SweetnerDataSet\\%02d.ini"
#define AccelFromIdleFilePath "CarSoundData\\AccelFromIdle\\%02d.ini"
#define TurboDataSetFilePath "CarSoundData\\TurboDataSet\\%02d.ini"

//int(*GetCarTypeMapping_Game)(int CarTypeID) = (int(*)(int))0x459240;
char* (*GetCarTypeName)(int CarTypeID) = (char* (*)(int))0x610110;

bool DoesFileExist(char const* _path) {

	std::ifstream ifile(_path);
	return (bool)ifile;
}

struct CarDataMapping
{
	int StockEngineData;
	int StockShiftPatternData;
	int StockTurboData;

	int StreetEngineData;
	int StreetShiftPatternData;
	int StreetTurboData;

	int ProEngineData;
	int ProShiftPatternData;
	int ProTurboData;

	int ExtremeEngineData;
	int ExtremeShiftPatternData;
	int ExtremeTurboData;
};

// No need to define a structure for CarTypeMapping, it has only 1 integer.

struct EngineData
{
	char* MainRAMBankName = " ";
	char* AuxRAMBankName = " ";
	int CarID;
	char* GinsuAccel = " ";
	int UseDualGinsu; // 0 on Hummer and Skyline. 1 on others
	int GinsuDecelID;
	int AccelFromIdleID;
	float MaxRPM;
	float MinRPM;
	int AEMSVol;
	int GinsuAccelVol;
	float AccelDeltaRPMThreshold;
	float AEMSMixLRPM;
	float GinsuMixLRPM;
	float AEMSMixSRPM;
	float GinsuMixSRPM;
	float UnkFloat1;
	float UnkFloat2;
	int Unk2;
};

struct DualGinsu
{
	char* GinsuDecelFileName = " ";
	int AEMSDecelVol;
	int GinsuDecelVol;
	int MinRPM;
	int MaxRPM;
	float FadeIn;
	float FadeOut;
	float DecelDeltaRPMThreshold;
	int Unk0;
	float DecelAEMSMixLRPM;
	float DecelGinsuMixLRPM;
	float DecelAEMSMixSRPM;
	float DecelGinsuMixSRPM;
};		 

struct ShiftPattern
{
	//most of it is unknown atm
	char* BankName = " ";
	int Unk1;
	float UnkFloat1;
	int Unk2;
	int Unk3;
	int Unk4;
	int Unk5;
	int Unk6;
	float UnkFloat2;
	int Unk7;
	int Unk8;
	int Unk9;
	int Unk10;
	int Unk11;
	int Unk12;
	int Unk13;
	float UnkFloat3;
	int Unk14;
	int Unk15;
	int Unk16;
	int Unk17;
	int Unk18;
	int Unk19;
	float UnkFloat4;
};

struct SweetnerDataSet
{
	char* SweetBank = " ";
	int SputterVol;
	int ShiftSweetsVol;
};

struct TurboDataSet
{
	char* TurboBank = " ";
	int Unk1;
	int Unk2;
	int Unk3;
	float UnkFloat1;
	float ChargeTime;
};

struct AccelFromIdle
{
	float UnkFloat1;
	int Unk1;
	int Unk2;
	int Unk3;
	int Unk4;
};

// Allocate memory for our stuff
CarDataMapping g_CDM[MAX_FILES];
EngineData g_ED[MAX_FILES];
DualGinsu g_DG[MAX_FILES];
ShiftPattern g_SH[MAX_FILES];
SweetnerDataSet g_SDS[MAX_FILES];
TurboDataSet g_TDS[MAX_FILES];
AccelFromIdle g_AFI[MAX_FILES];

int GetCarTypeMapping_Game(int CarTypeID)
{
	int CarTypeMappingID; // eax

	switch (CarTypeID)
	{
	case 0:
		CarTypeMappingID = 22;
		break;
	case 1:
		CarTypeMappingID = 10;
		break;
	case 2:
		CarTypeMappingID = 6;
		break;
	case 3:
		CarTypeMappingID = 0;
		break;
	case 4:
		CarTypeMappingID = 19;
		break;
	case 5:
		CarTypeMappingID = 5;
		break;
	case 6:
		CarTypeMappingID = 23;
		break;
	case 7:
		CarTypeMappingID = 7;
		break;
	case 8:
		CarTypeMappingID = 14;
		break;
	case 9:
		CarTypeMappingID = 21;
		break;
	case 10:
		CarTypeMappingID = 9;
		break;
	case 11:
		CarTypeMappingID = 31;
		break;
	case 12:
		CarTypeMappingID = 28;
		break;
	case 13:
		CarTypeMappingID = 4;
		break;
	case 14:
		CarTypeMappingID = 17;
		break;
	case 15:
		CarTypeMappingID = 30;
		break;
	case 16:
		CarTypeMappingID = 12;
		break;
	case 17:
		CarTypeMappingID = 3;
		break;
	case 18:
		CarTypeMappingID = 24;
		break;
	case 19:
		CarTypeMappingID = 8;
		break;
	case 20:
		CarTypeMappingID = 32;
		break;
	case 21:
		CarTypeMappingID = 26;
		break;
	case 22:
		CarTypeMappingID = 2;
		break;
	case 23:
		CarTypeMappingID = 11;
		break;
	case 24:
		CarTypeMappingID = 1;
		break;
	case 25:
		CarTypeMappingID = 13;
		break;
	case 26:
		CarTypeMappingID = 20;
		break;
	case 27:
		CarTypeMappingID = 29;
		break;
	case 29:
		CarTypeMappingID = 25;
		break;
	case 30:
		CarTypeMappingID = 15;
		break;
	default:
		CarTypeMappingID = 18;
		break;
	}
	return CarTypeMappingID;
}

int GetCarTypeMapping(int CarTypeID)
{
	sprintf(CarININame, CarTypeMappingFilePath, GetCarTypeName(CarTypeID));

	CIniReader CarTypeMappingReader(CarININame);
	return CarTypeMappingReader.ReadInteger("CarTypeMapping", "CarDataMapping", GetCarTypeMapping_Game(CarTypeID));
}

int CTM_Temp;

void __declspec(naked) CarTypeMappingCodeCave()
{
	_asm mov CTM_Temp, eax;
	_asm pushad;

	CTM_Temp = GetCarTypeMapping(CTM_Temp);

	_asm popad;
	_asm mov eax, CTM_Temp;
	_asm retn;
}

void ExportConfigFiles()
{
	//CarTypeMapping
	CreateDirectory(CarTypeMappingPath, NULL);

	for (int i = 0; i < CarCount; i++)
	{
		sprintf(CarININame, CarTypeMappingFilePath, GetCarTypeName(i));
		CIniReader CarTypeMappingDataWriter(CarININame);

		CarTypeMappingDataWriter.WriteInteger("CarTypeMapping", "CarDataMapping", GetCarTypeMapping_Game(i));
	}

	// CarDataMapping
	CarDataMapping _CDM;
	int CarDataMappingCount = injector::ReadMemory<int>(SIZE_OF_g_SND_CARDATAMAPPING, true) / sizeof(CarDataMapping);

	CreateDirectory(CarDataMappingPath, NULL);

	for (int i = 0; i < CarDataMappingCount; i++)
	{
		_CDM = injector::ReadMemory<CarDataMapping>(g_SND_CARDATAMAPPING + i * sizeof(CarDataMapping), true);

		sprintf(CarININame, CarDataMappingFilePath, i);
		CIniReader CarTypeMappingDataWriter(CarININame);

		CarTypeMappingDataWriter.WriteInteger("Stock", "EngineData", _CDM.StockEngineData);
		CarTypeMappingDataWriter.WriteInteger("Stock", "ShiftPattern", _CDM.StockShiftPatternData);
		CarTypeMappingDataWriter.WriteInteger("Stock", "TurboDataSet", _CDM.StockTurboData);

		CarTypeMappingDataWriter.WriteInteger("Street", "EngineData", _CDM.StreetEngineData);
		CarTypeMappingDataWriter.WriteInteger("Street", "ShiftPattern", _CDM.StreetShiftPatternData);
		CarTypeMappingDataWriter.WriteInteger("Street", "TurboDataSet", _CDM.StreetTurboData);

		CarTypeMappingDataWriter.WriteInteger("Pro", "EngineData", _CDM.ProEngineData);
		CarTypeMappingDataWriter.WriteInteger("Pro", "ShiftPattern", _CDM.ProShiftPatternData);
		CarTypeMappingDataWriter.WriteInteger("Pro", "TurboDataSet", _CDM.ProTurboData);

		CarTypeMappingDataWriter.WriteInteger("Extreme", "EngineData", _CDM.ExtremeEngineData);
		CarTypeMappingDataWriter.WriteInteger("Extreme", "ShiftPattern", _CDM.ExtremeShiftPatternData);
		CarTypeMappingDataWriter.WriteInteger("Extreme", "TurboDataSet", _CDM.ExtremeTurboData);
	}

	// Engine Data
	EngineData _ED;
	int EngineDataCount = injector::ReadMemory<int>(SIZE_OF_g_SND_ENGINE_DATA, true) / sizeof(EngineData);

	CreateDirectory(EngineDataPath, NULL);

	for (int i = 0; i < EngineDataCount; i++)
	{
		_ED = injector::ReadMemory<EngineData>(g_SND_ENGINE_DATA + i * sizeof(EngineData), true);

		sprintf(CarININame, EngineDataFilePath, i);
		CIniReader EngineDataWriter(CarININame);

		EngineDataWriter.WriteString("EngineData", "MainRAMBankName", _ED.MainRAMBankName);
		EngineDataWriter.WriteString("EngineData", "AuxRAMBankName", _ED.AuxRAMBankName);
		EngineDataWriter.WriteInteger("EngineData", "CarID", _ED.CarID);
		EngineDataWriter.WriteString("EngineData", "GinsuAccel", _ED.GinsuAccel);
		EngineDataWriter.WriteInteger("EngineData", "UseDualGinsu", _ED.UseDualGinsu);
		EngineDataWriter.WriteInteger("EngineData", "GinsuDecelID", _ED.GinsuDecelID);
		EngineDataWriter.WriteInteger("EngineData", "AccelFromIdleID", _ED.AccelFromIdleID);
		EngineDataWriter.WriteFloat("EngineData", "MaxRPM", _ED.MaxRPM);
		EngineDataWriter.WriteFloat("EngineData", "MinRPM", _ED.MinRPM);
		EngineDataWriter.WriteInteger("EngineData", "AEMSVol", _ED.AEMSVol);
		EngineDataWriter.WriteInteger("EngineData", "GinsuAccelVol", _ED.GinsuAccelVol);
		EngineDataWriter.WriteFloat("EngineData", "AccelDeltaRPMThreshold", _ED.AccelDeltaRPMThreshold);
		EngineDataWriter.WriteFloat("EngineData", "AEMSMixLRPM", _ED.AEMSMixLRPM);
		EngineDataWriter.WriteFloat("EngineData", "GinsuMixLRPM", _ED.GinsuMixLRPM);
		EngineDataWriter.WriteFloat("EngineData", "AEMSMixSRPM", _ED.AEMSMixSRPM);
		EngineDataWriter.WriteFloat("EngineData", "GinsuMixSRPM", _ED.GinsuMixSRPM);
		EngineDataWriter.WriteFloat("EngineData", "UnkFloat1", _ED.UnkFloat1);
		EngineDataWriter.WriteFloat("EngineData", "UnkFloat2", _ED.UnkFloat2);
		EngineDataWriter.WriteInteger("EngineData", "Unk2", _ED.Unk2);
	}

	// Dual Ginsu
	DualGinsu _DG;
	int DualGinsuCount = injector::ReadMemory<int>(SIZE_OF_g_SND_DUAL_GINSU, true) / sizeof(DualGinsu);

	CreateDirectory(DualGinsuPath, NULL);

	for (int i = 0; i < DualGinsuCount; i++)
	{
		_DG = injector::ReadMemory<DualGinsu>(g_SND_DUAL_GINSU + i * sizeof(DualGinsu), true);

		sprintf(CarININame, DualGinsuFilePath, i);
		CIniReader DualGinsuWriter(CarININame);

		DualGinsuWriter.WriteString("DualGinsu", "GinsuDecelFileName", _DG.GinsuDecelFileName);
		DualGinsuWriter.WriteInteger("DualGinsu", "AEMSDecelVol", _DG.AEMSDecelVol);
		DualGinsuWriter.WriteInteger("DualGinsu", "GinsuDecelVol", _DG.GinsuDecelVol);
		DualGinsuWriter.WriteInteger("DualGinsu", "MinRPM", _DG.MinRPM);
		DualGinsuWriter.WriteInteger("DualGinsu", "MaxRPM", _DG.MaxRPM);
		DualGinsuWriter.WriteFloat("DualGinsu", "FadeIn", _DG.FadeIn);
		DualGinsuWriter.WriteFloat("DualGinsu", "FadeOut", _DG.FadeOut);
		DualGinsuWriter.WriteFloat("DualGinsu", "DecelDeltaRPMThreshold", _DG.DecelDeltaRPMThreshold);
		DualGinsuWriter.WriteInteger("DualGinsu", "Unk0", _DG.Unk0);
		DualGinsuWriter.WriteFloat("DualGinsu", "DecelAEMSMixLRPM", _DG.DecelAEMSMixLRPM);
		DualGinsuWriter.WriteFloat("DualGinsu", "DecelGinsuMixLRPM", _DG.DecelGinsuMixLRPM);
		DualGinsuWriter.WriteFloat("DualGinsu", "DecelAEMSMixSRPM", _DG.DecelAEMSMixSRPM);
		DualGinsuWriter.WriteFloat("DualGinsu", "DecelGinsuMixSRPM", _DG.DecelGinsuMixSRPM);
	}

	// Shift Patterns
	ShiftPattern _SH;
	int ShiftPatternCount = injector::ReadMemory<int>(SIZE_OF_g_SND_SHIFTPATTERNS, true) / sizeof(ShiftPattern);

	CreateDirectory(ShiftPatternsPath, NULL);

	for (int i = 0; i < ShiftPatternCount; i++)
	{
		_SH = injector::ReadMemory<ShiftPattern>(g_SND_SHIFTPATTERNS + i * sizeof(ShiftPattern), true);

		sprintf(CarININame, ShiftPatternsFilePath, i);
		CIniReader ShiftPatternWriter(CarININame);

		ShiftPatternWriter.WriteString("ShiftPattern", "BankName", _SH.BankName);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk1", _SH.Unk1);
		ShiftPatternWriter.WriteFloat("ShiftPattern", "UnkFloat1", _SH.UnkFloat1);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk2", _SH.Unk2);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk3", _SH.Unk3);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk4", _SH.Unk4);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk5", _SH.Unk5);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk6", _SH.Unk6);
		ShiftPatternWriter.WriteFloat("ShiftPattern", "UnkFloat2", _SH.UnkFloat2);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk7", _SH.Unk7);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk8", _SH.Unk8);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk9", _SH.Unk9);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk10", _SH.Unk10);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk11", _SH.Unk11);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk12", _SH.Unk12);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk13", _SH.Unk13);
		ShiftPatternWriter.WriteFloat("ShiftPattern", "UnkFloat3", _SH.UnkFloat3);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk14", _SH.Unk14);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk15", _SH.Unk15);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk16", _SH.Unk16);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk17", _SH.Unk17);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk18", _SH.Unk18);
		ShiftPatternWriter.WriteInteger("ShiftPattern", "Unk19", _SH.Unk19);
		ShiftPatternWriter.WriteFloat("ShiftPattern", "UnkFloat4", _SH.UnkFloat4);
	}

	// Sweetner
	SweetnerDataSet _SDS;
	int SweetnerDataCount = injector::ReadMemory<int>(SIZE_OF_g_SND_SWEETNERDATASET, true) / sizeof(SweetnerDataSet);

	CreateDirectory(SweetnerDataSetPath, NULL);

	for (int i = 0; i < SweetnerDataCount; i++)
	{
		_SDS = injector::ReadMemory<SweetnerDataSet>(g_SND_SWEETNERDATASET + i * sizeof(SweetnerDataSet), true);

		sprintf(CarININame, SweetnerDataSetFilePath, i);
		CIniReader SweetnerWriter(CarININame);

		SweetnerWriter.WriteString("Sweetner", "SweetBank", _SDS.SweetBank);
		SweetnerWriter.WriteInteger("Sweetner", "SputterVol", _SDS.SputterVol);
		SweetnerWriter.WriteInteger("Sweetner", "ShiftSweetsVol", _SDS.ShiftSweetsVol);
	}

	// Turbo
	TurboDataSet _TDS;
	int TurboDataCount = injector::ReadMemory<int>(SIZE_OF_g_SND_TURBODATASET, true) / sizeof(TurboDataSet);

	CreateDirectory(TurboDataSetPath, NULL);

	for (int i = 0; i < TurboDataCount; i++)
	{
		_TDS = injector::ReadMemory<TurboDataSet>(g_SND_TURBODATASET + i * sizeof(TurboDataSet), true);

		sprintf(CarININame, TurboDataSetFilePath, i);
		CIniReader TurboWriter(CarININame);

		TurboWriter.WriteString("Turbo", "TurboBank", _TDS.TurboBank);
		TurboWriter.WriteInteger("Turbo", "TurboChargeVol", _TDS.Unk1);
		TurboWriter.WriteInteger("Turbo", "LowBoostBovVol", _TDS.Unk2);
		TurboWriter.WriteInteger("Turbo", "HighBoostBovVol", _TDS.Unk3);
		TurboWriter.WriteFloat("Turbo", "UnkFloat1", _TDS.UnkFloat1);
		TurboWriter.WriteFloat("Turbo", "ChargeTime", _TDS.ChargeTime);
	}

	// Acceltrans (AccelFromIdle)
	AccelFromIdle _AFI;
	int AccelFromIdleCount = injector::ReadMemory<int>(SIZE_OF_g_SND_ACCELFROMIDLE, true) / sizeof(AccelFromIdle);

	CreateDirectory(AccelFromIdlePath, NULL);

	for (int i = 0; i < AccelFromIdleCount; i++)
	{
		_AFI = injector::ReadMemory<AccelFromIdle>(g_SND_ACCELFROMIDLE + i * sizeof(AccelFromIdle), true);

		sprintf(CarININame, AccelFromIdleFilePath, i);
		CIniReader AccelTransWriter(CarININame);

		AccelTransWriter.WriteFloat("AccelTrans", "UnkFloat1", _AFI.UnkFloat1);
		AccelTransWriter.WriteInteger("AccelTrans", "Unk1", _AFI.Unk1);
		AccelTransWriter.WriteInteger("AccelTrans", "Unk2", _AFI.Unk2);
		AccelTransWriter.WriteInteger("AccelTrans", "Unk3", _AFI.Unk3);
		AccelTransWriter.WriteInteger("AccelTrans", "Unk4", _AFI.Unk4);
	}

	MessageBoxA(NULL, "Done!\nThe game will now close.", "NFSU2 Car Sound Tuner", MB_ICONINFORMATION);

	injector::WriteMemory<int>(0x864F4C, 1, true); // exit the game
}

void ImportConfigFiles()
{
	// Copy vanilla data and wrap them around to populate our new arrays in case of a tornado

	// CarDataMapping
	int CarDataMappingCount = injector::ReadMemory<int>(SIZE_OF_g_SND_CARDATAMAPPING, true) / sizeof(CarDataMapping);
	for (int i = 0; i < MAX_FILES; i++) g_CDM[i] = injector::ReadMemory<CarDataMapping>(g_SND_CARDATAMAPPING + (i % CarDataMappingCount) * sizeof(CarDataMapping), true);

	// Engine Data
	int EngineDataCount = injector::ReadMemory<int>(SIZE_OF_g_SND_ENGINE_DATA, true) / sizeof(EngineData);
	for (int i = 0; i < MAX_FILES; i++) g_ED[i] = injector::ReadMemory<EngineData>(g_SND_ENGINE_DATA + (i % EngineDataCount) * sizeof(EngineData), true);

	// Dual Ginsu
	int DualGinsuCount = injector::ReadMemory<int>(SIZE_OF_g_SND_DUAL_GINSU, true) / sizeof(DualGinsu);
	for (int i = 0; i < MAX_FILES; i++) g_DG[i] = injector::ReadMemory<DualGinsu>(g_SND_DUAL_GINSU + (i % DualGinsuCount) * sizeof(DualGinsu), true);

	// Shift Patterns
	int ShiftPatternCount = injector::ReadMemory<int>(SIZE_OF_g_SND_SHIFTPATTERNS, true) / sizeof(ShiftPattern);
	for (int i = 0; i < MAX_FILES; i++) g_SH[i] = injector::ReadMemory<ShiftPattern>(g_SND_SHIFTPATTERNS + (i % ShiftPatternCount) * sizeof(ShiftPattern), true);

	// Sweetner
	int SweetnerDataCount = injector::ReadMemory<int>(SIZE_OF_g_SND_SWEETNERDATASET, true) / sizeof(SweetnerDataSet);
	for (int i = 0; i < MAX_FILES; i++) g_SDS[i] = injector::ReadMemory<SweetnerDataSet>(g_SND_SWEETNERDATASET + (i % SweetnerDataCount) * sizeof(SweetnerDataSet), true);

	// Turbo
	int TurboDataCount = injector::ReadMemory<int>(SIZE_OF_g_SND_TURBODATASET, true) / sizeof(TurboDataSet);
	for (int i = 0; i < MAX_FILES; i++) g_TDS[i] = injector::ReadMemory<TurboDataSet>(g_SND_TURBODATASET + (i % TurboDataCount) * sizeof(TurboDataSet), true);

	// Acceltrans (AccelFromIdle)
	int AccelFromIdleCount = injector::ReadMemory<int>(SIZE_OF_g_SND_ACCELFROMIDLE, true) / sizeof(AccelFromIdle);
	for (int i = 0; i < MAX_FILES; i++) g_AFI[i] = injector::ReadMemory<AccelFromIdle>(g_SND_ACCELFROMIDLE + (i % AccelFromIdleCount) * sizeof(AccelFromIdle), true);


	// Now start to read from config files
	for (int i = 0; i < MAX_FILES; i++)
	{
		// CarDataMapping
		sprintf(CarININame, CarDataMappingFilePath, i);
		CIniReader CarTypeMappingDataReader(CarININame);
		
		if (DoesFileExist(CarTypeMappingDataReader.GetIniPath().c_str()))
		{
			g_CDM[i].StockEngineData = CarTypeMappingDataReader.ReadInteger("Stock", "EngineData", g_CDM[i].StockEngineData);
			g_CDM[i].StockShiftPatternData = CarTypeMappingDataReader.ReadInteger("Stock", "ShiftPattern", g_CDM[i].StockShiftPatternData);
			g_CDM[i].StockTurboData = CarTypeMappingDataReader.ReadInteger("Stock", "TurboDataSet", g_CDM[i].StockTurboData);

			g_CDM[i].StreetEngineData = CarTypeMappingDataReader.ReadInteger("Street", "EngineData", g_CDM[i].StreetEngineData);
			g_CDM[i].StreetShiftPatternData = CarTypeMappingDataReader.ReadInteger("Street", "ShiftPattern", g_CDM[i].StreetShiftPatternData);
			g_CDM[i].StreetTurboData = CarTypeMappingDataReader.ReadInteger("Street", "TurboDataSet", g_CDM[i].StreetTurboData);

			g_CDM[i].ProEngineData = CarTypeMappingDataReader.ReadInteger("Pro", "EngineData", g_CDM[i].ProEngineData);
			g_CDM[i].ProShiftPatternData = CarTypeMappingDataReader.ReadInteger("Pro", "ShiftPattern", g_CDM[i].ProShiftPatternData);
			g_CDM[i].ProTurboData = CarTypeMappingDataReader.ReadInteger("Pro", "TurboDataSet", g_CDM[i].ProTurboData);

			g_CDM[i].ExtremeEngineData = CarTypeMappingDataReader.ReadInteger("Extreme", "EngineData", g_CDM[i].ExtremeEngineData);
			g_CDM[i].ExtremeShiftPatternData = CarTypeMappingDataReader.ReadInteger("Extreme", "ShiftPattern", g_CDM[i].ExtremeShiftPatternData);
			g_CDM[i].ExtremeTurboData = CarTypeMappingDataReader.ReadInteger("Extreme", "TurboDataSet", g_CDM[i].ExtremeTurboData);
		}
		
		// EngineData
		sprintf(CarININame, EngineDataFilePath, i);
		CIniReader EngineDataReader(CarININame);

		if (DoesFileExist(EngineDataReader.GetIniPath().c_str()))
		{
			g_ED[i].MainRAMBankName = EngineDataReader.ReadString("EngineData", "MainRAMBankName", g_ED[i].MainRAMBankName);
			g_ED[i].AuxRAMBankName = EngineDataReader.ReadString("EngineData", "AuxRAMBankName", g_ED[i].AuxRAMBankName);
			g_ED[i].CarID = EngineDataReader.ReadInteger("EngineData", "CarID", g_ED[i].CarID);
			g_ED[i].GinsuAccel = EngineDataReader.ReadString("EngineData", "GinsuAccel", g_ED[i].GinsuAccel);
			g_ED[i].UseDualGinsu = EngineDataReader.ReadInteger("EngineData", "UseDualGinsu", EngineDataReader.ReadInteger("EngineData", "Unk1", g_ED[i].UseDualGinsu));
			g_ED[i].GinsuDecelID = EngineDataReader.ReadInteger("EngineData", "GinsuDecelID", g_ED[i].GinsuDecelID);
			g_ED[i].AccelFromIdleID = EngineDataReader.ReadInteger("EngineData", "AccelFromIdleID", g_ED[i].AccelFromIdleID);
			g_ED[i].MaxRPM = EngineDataReader.ReadFloat("EngineData", "MaxRPM", g_ED[i].MaxRPM);
			g_ED[i].MinRPM = EngineDataReader.ReadFloat("EngineData", "MinRPM", g_ED[i].MinRPM);
			g_ED[i].AEMSVol = EngineDataReader.ReadInteger("EngineData", "AEMSVol", g_ED[i].AEMSVol);
			g_ED[i].GinsuAccelVol = EngineDataReader.ReadInteger("EngineData", "GinsuAccelVol", g_ED[i].GinsuAccelVol);
			g_ED[i].AccelDeltaRPMThreshold = EngineDataReader.ReadFloat("EngineData", "AccelDeltaRPMThreshold", g_ED[i].AccelDeltaRPMThreshold);
			g_ED[i].AEMSMixLRPM = EngineDataReader.ReadFloat("EngineData", "AEMSMixLRPM", g_ED[i].AEMSMixLRPM);
			g_ED[i].GinsuMixLRPM = EngineDataReader.ReadFloat("EngineData", "GinsuMixLRPM", g_ED[i].GinsuMixLRPM);
			g_ED[i].AEMSMixSRPM = EngineDataReader.ReadFloat("EngineData", "AEMSMixSRPM", g_ED[i].AEMSMixSRPM);
			g_ED[i].GinsuMixSRPM = EngineDataReader.ReadFloat("EngineData", "GinsuMixSRPM", g_ED[i].GinsuMixSRPM);
			g_ED[i].UnkFloat1 = EngineDataReader.ReadFloat("EngineData", "UnkFloat1", g_ED[i].UnkFloat1);
			g_ED[i].UnkFloat2 = EngineDataReader.ReadFloat("EngineData", "UnkFloat2", g_ED[i].UnkFloat2);
			g_ED[i].Unk2 = EngineDataReader.ReadInteger("EngineData", "Unk2", g_ED[i].Unk2);
		}

		// Dual Ginsu
		sprintf(CarININame, DualGinsuFilePath, i);
		CIniReader DualGinsuReader(CarININame);

		if (DoesFileExist(DualGinsuReader.GetIniPath().c_str()))
		{
			g_DG[i].GinsuDecelFileName = DualGinsuReader.ReadString("DualGinsu", "GinsuDecelFileName", g_DG[i].GinsuDecelFileName);
			g_DG[i].AEMSDecelVol = DualGinsuReader.ReadInteger("DualGinsu", "AEMSDecelVol", g_DG[i].AEMSDecelVol);
			g_DG[i].GinsuDecelVol = DualGinsuReader.ReadInteger("DualGinsu", "GinsuDecelVol", g_DG[i].GinsuDecelVol);
			g_DG[i].MinRPM = DualGinsuReader.ReadInteger("DualGinsu", "MinRPM", g_DG[i].MinRPM);
			g_DG[i].MaxRPM = DualGinsuReader.ReadInteger("DualGinsu", "MaxRPM", g_DG[i].MaxRPM);
			g_DG[i].FadeIn = DualGinsuReader.ReadFloat("DualGinsu", "FadeIn", g_DG[i].FadeIn);
			g_DG[i].FadeOut = DualGinsuReader.ReadFloat("DualGinsu", "FadeOut", g_DG[i].FadeOut);
			g_DG[i].DecelDeltaRPMThreshold = DualGinsuReader.ReadFloat("DualGinsu", "DecelDeltaRPMThreshold", g_DG[i].DecelDeltaRPMThreshold);
			g_DG[i].Unk0 = DualGinsuReader.ReadInteger("DualGinsu", "Unk0", g_DG[i].Unk0);
			g_DG[i].DecelAEMSMixLRPM = DualGinsuReader.ReadFloat("DualGinsu", "DecelAEMSMixLRPM", g_DG[i].DecelAEMSMixLRPM);
			g_DG[i].DecelGinsuMixLRPM = DualGinsuReader.ReadFloat("DualGinsu", "DecelGinsuMixLRPM", g_DG[i].DecelGinsuMixLRPM);
			g_DG[i].DecelAEMSMixSRPM = DualGinsuReader.ReadFloat("DualGinsu", "DecelAEMSMixSRPM", g_DG[i].DecelAEMSMixSRPM);
			g_DG[i].DecelGinsuMixSRPM = DualGinsuReader.ReadFloat("DualGinsu", "DecelGinsuMixSRPM", g_DG[i].DecelGinsuMixSRPM);
		}

		// Shift Patterns
		sprintf(CarININame, ShiftPatternsFilePath, i);
		CIniReader ShiftPatternReader(CarININame);

		if (DoesFileExist(ShiftPatternReader.GetIniPath().c_str()))
		{
			g_SH[i].BankName = ShiftPatternReader.ReadString("ShiftPattern", "BankName", g_SH[i].BankName);
			g_SH[i].Unk1 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk1", g_SH[i].Unk1);
			g_SH[i].UnkFloat1 = ShiftPatternReader.ReadFloat("ShiftPattern", "UnkFloat1", g_SH[i].UnkFloat1);
			g_SH[i].Unk2 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk2", g_SH[i].Unk2);
			g_SH[i].Unk3 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk3", g_SH[i].Unk3);
			g_SH[i].Unk4 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk4", g_SH[i].Unk4);
			g_SH[i].Unk5 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk5", g_SH[i].Unk5);
			g_SH[i].Unk6 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk6", g_SH[i].Unk6);
			g_SH[i].UnkFloat2 = ShiftPatternReader.ReadFloat("ShiftPattern", "UnkFloat2", g_SH[i].UnkFloat2);
			g_SH[i].Unk7 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk7", g_SH[i].Unk7);
			g_SH[i].Unk8 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk8", g_SH[i].Unk8);
			g_SH[i].Unk9 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk9", g_SH[i].Unk9);
			g_SH[i].Unk10 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk10", g_SH[i].Unk10);
			g_SH[i].Unk11 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk11", g_SH[i].Unk11);
			g_SH[i].Unk12 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk12", g_SH[i].Unk12);
			g_SH[i].Unk13 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk13", g_SH[i].Unk13);
			g_SH[i].UnkFloat3 = ShiftPatternReader.ReadFloat("ShiftPattern", "UnkFloat3", g_SH[i].UnkFloat3);
			g_SH[i].Unk14 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk14", g_SH[i].Unk14);
			g_SH[i].Unk15 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk15", g_SH[i].Unk15);
			g_SH[i].Unk16 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk16", g_SH[i].Unk16);
			g_SH[i].Unk17 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk17", g_SH[i].Unk17);
			g_SH[i].Unk18 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk18", g_SH[i].Unk18);
			g_SH[i].Unk19 = ShiftPatternReader.ReadInteger("ShiftPattern", "Unk19", g_SH[i].Unk19);
			g_SH[i].UnkFloat4 = ShiftPatternReader.ReadFloat("ShiftPattern", "UnkFloat4", g_SH[i].UnkFloat4);
		}

		// Sweetner
		sprintf(CarININame, SweetnerDataSetFilePath, i);
		CIniReader SweetnerReader(CarININame);

		if (DoesFileExist(SweetnerReader.GetIniPath().c_str()))
		{
			g_SDS[i].SweetBank = SweetnerReader.ReadString("Sweetner", "SweetBank", g_SDS[i].SweetBank);
			g_SDS[i].SputterVol = SweetnerReader.ReadInteger("Sweetner", "SputterVol", g_SDS[i].SputterVol);
			g_SDS[i].ShiftSweetsVol = SweetnerReader.ReadInteger("Sweetner", "ShiftSweetsVol", g_SDS[i].ShiftSweetsVol);
		}

		// Turbo
		sprintf(CarININame, TurboDataSetFilePath, i);
		CIniReader TurboReader(CarININame);

		if (DoesFileExist(TurboReader.GetIniPath().c_str()))
		{
			g_TDS[i].TurboBank = TurboReader.ReadString("Turbo", "TurboBank", g_TDS[i].TurboBank);
			g_TDS[i].Unk1 = TurboReader.ReadInteger("Turbo", "TurboChargeVol", TurboReader.ReadInteger("Turbo", "Unk1", g_TDS[i].Unk1));
			g_TDS[i].Unk2 = TurboReader.ReadInteger("Turbo", "LowBoostBovVol", TurboReader.ReadInteger("Turbo", "Unk2", g_TDS[i].Unk2));
			g_TDS[i].Unk3 = TurboReader.ReadInteger("Turbo", "HighBoostBovVol", TurboReader.ReadInteger("Turbo", "Unk3", g_TDS[i].Unk3));
			g_TDS[i].UnkFloat1 = TurboReader.ReadFloat("Turbo", "UnkFloat1", g_TDS[i].UnkFloat1);
			g_TDS[i].ChargeTime = TurboReader.ReadFloat("Turbo", "ChargeTime", g_TDS[i].ChargeTime);
		}

		// Accel Trans
		sprintf(CarININame, AccelFromIdleFilePath, i);
		CIniReader AccelTransReader(CarININame);

		if (DoesFileExist(AccelTransReader.GetIniPath().c_str()))
		{
			g_AFI[i].UnkFloat1 = AccelTransReader.ReadFloat("AccelTrans", "UnkFloat1", g_AFI[i].UnkFloat1);
			g_AFI[i].Unk1 = AccelTransReader.ReadInteger("AccelTrans", "Unk1", g_AFI[i].Unk1);
			g_AFI[i].Unk2 = AccelTransReader.ReadInteger("AccelTrans", "Unk2", g_AFI[i].Unk2);
			g_AFI[i].Unk3 = AccelTransReader.ReadInteger("AccelTrans", "Unk3", g_AFI[i].Unk3);
			g_AFI[i].Unk4 = AccelTransReader.ReadInteger("AccelTrans", "Unk4", g_AFI[i].Unk4);
		}
	}

	// Replace references in game
	injector::WriteMemory(0x4594C7, g_CDM, true); // EAXCar::CarPreSetup
	injector::WriteMemory(0x4796AD, g_CDM, true); // EAXCar::EAXCar

	injector::WriteMemory(0x4594DB, g_ED, true); // EAXCar::CarPreSetup
	injector::WriteMemory(0x4642C3, g_ED, true); // CSTATEMGR_AICar::ResolveEngineBankLoading
	injector::WriteMemory(0x46431A, g_ED, true); // CSTATEMGR_AICar::ResolveEngineBankLoading
	injector::WriteMemory(0x469939, g_ED, true); // EAXAITunerCar::ConnectCar
	injector::WriteMemory(0x469959, g_ED, true); // EAXAITunerCar::ConnectCar
	injector::WriteMemory(0x476CD4, g_ED, true); // CSTATEMGR_AICar::LoadData
	injector::WriteMemory(0x4796A3, g_ED, true); // EAXCar::EAXCar

	injector::WriteMemory(0x459509, g_DG, true); // EAXCar::CarPreSetup
	injector::WriteMemory(0x459526, g_DG, true); // EAXCar::CarPreSetup

	injector::WriteMemory(0x45C384, g_SH, true); // SFXCTL_Shifting::SetupCTL
	injector::WriteMemory(0x45C379, (int*)g_CDM + 1, true); // SFXCTL_Shifting::SetupCTL
	injector::WriteMemory(0x45E6CB, g_SH, true); // CARSFX_Shift::SetupSFX
	injector::WriteMemory(0x45E6BF, (int*)g_CDM + 1, true); // CARSFX_Shift::SetupSFX

	injector::WriteMemory(0x45E6E4, g_SDS, true); // CARSFX_Shift::SetupSFX
	injector::WriteMemory(0x45E6F1, g_SDS, true); // CARSFX_Shift::SetupSFX
	injector::WriteMemory(0x45F735, g_SDS, true); // CARSFX_SparkChatter::SetupSFX
	injector::WriteMemory(0x45F743, g_SDS, true); // CARSFX_SparkChatter::SetupSFX

	injector::WriteMemory(0x45EBA7, g_TDS, true); // CARSFX_Turbo::SetupSFX
	injector::WriteMemory(0x45EB9D, (int*)g_CDM + 2, true); // CARSFX_Turbo::SetupSFX
}

int Init()
{
	CIniReader Settings("NFSU2CarSoundTunerSettings.ini");

	// Main
	bool EnableCarSoundTuner = Settings.ReadInteger("Main", "EnableCarSoundTuner", 1) == 1;

	// Debug
	CarCount = Settings.ReadInteger("Debug", "CarCount", 46);
	bool DumpSoundData = Settings.ReadInteger("Debug", "DumpSoundData", 0) == 1;

	if (DumpSoundData) // Create config files from static in-game variables.
	{
		injector::MakeCALL(0x57EDA3, ExportConfigFiles, true);
		return 0;
	}

	if (EnableCarSoundTuner) // If the mod is enabled
	{
		injector::MakeJMP(0x459240, CarTypeMappingCodeCave, true); // Hook CarTypeMapping call.
		injector::MakeCALL(0x57EDA3, ImportConfigFiles, true); // InitializeEverything, Read config files in CarSoundData.

		// Fix sweetner data limit (43)
		injector::WriteMemory<BYTE>(0x45F72C, MAX_FILES - 1, true); // CARSFX_SparkChatter::SetupSFX
		injector::WriteMemory<BYTE>(0x45E6DA, MAX_FILES - 1, true); // CARSFX_Shift::SetupSFX

		// Fix engine data limits (60)
		injector::WriteMemory<BYTE>(0x46137C, MAX_FILES - 1, true); // CARSFX_AIEngine::InitializeEngine
		injector::WriteMemory<int>(0x461380, MAX_FILES - 1, true);
		injector::WriteMemory<BYTE>(0x4721BB, MAX_FILES - 1, true); // CARSFX_Shift::PlayDisengageSnd
		injector::WriteMemory<int>(0x4721BF, MAX_FILES - 1, true);
		injector::WriteMemory<BYTE>(0x47239E, MAX_FILES - 1, true); // CARSFX_Shift::PlayEngageSnd
		injector::WriteMemory<int>(0x4723A2, MAX_FILES - 1, true);
		injector::WriteMemory<BYTE>(0x47D9E8, MAX_FILES - 1, true); // CARSFX_PlayerEngine::InitializeEngine
		injector::WriteMemory<int>(0x47D9EC, MAX_FILES - 1, true);
		injector::WriteMemory<BYTE>(0x4851D7, MAX_FILES - 1, true); // CARSFX_SparkChatter::InitSFX
		injector::WriteMemory<int>(0x4851DB, MAX_FILES - 1, true);
	}
	
	return 0;
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x75BCC7) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
			Init();

		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.2 NTSC speed2.exe (4,57 MB (4.800.512 bytes)).", "NFSU2 Car Sound Tuner", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;

}
