# NFSU2 Car Sound Tuner
NFSU2 Car Sound Tuner is a script mod which aims to make car sound modding easier in NFS Underground 2.

## Explanation:
The game has a function to map sound data from an array according to the car's ID (CarTypeID) in GlobalB.bun/lzc. The sound data is pulled from some arrays which are hardcoded in the executable.

```cpp
int(*GetCarTypeMapping)(int CarTypeID) = (int(*)(int))0x459240;
#define g_SND_ENGINE_DATA 0x7F0DE0
#define g_SND_DUAL_GINSU 0x7F1AA8
#define g_SND_CARDATAMAPPING 0x7F2230
#define g_SND_SHIFTPATTERNS 0x7F2860
#define g_SND_SWEETNERDATASET 0x7F3040
#define g_SND_ACCELFROMIDLE 0x7F3248
#define g_SND_TURBODATASET 0x7F3338
```

The script makes it simple to change the hardcoded data. For this, it reads an .ini file with the car's **name** from **CarSoundData\CarTypeMapping** directory next to it.

Here is an example file, **CarSoundData\CarTypeMapping\MUSTANGGT.ini**:

```ini
[CarTypeMapping]
CarDataMapping= 20
```

It means that **MUSTANGGT** uses the **20th** sound in the **Car Data Mapping** array. The array consist of 3 elements *(Engine Sound, Sweetner and Turbo data)* for each performance upgrade.

So, let's check the one used for **MUSTANGGT**, which is at **CarSoundData\CarDataMapping\20.ini**:
```ini
[Stock]
EngineData= 14
ShiftPattern= 8
TurboDataSet= 12
[Street]
EngineData= 12
ShiftPattern= 9
TurboDataSet= 12
[Pro]
EngineData= 12
ShiftPattern= 10
TurboDataSet= 13
[Extreme]
EngineData= 35
ShiftPattern= 11
TurboDataSet= 14
```
Here you can see all the data from that array. I will choose the data from **Street** Package to take a look.

Let's start with **EngineData**, which can be found at **CarSoundData\EngineData\12.ini**:
```ini
[EngineData]
MainRAMBankName=CAR_12_ENG_MB_EE.abk
AuxRAMBankName=CAR_12_ENG_MB_SPU.abk
CarID= 12
GinsuAccel=GIN_Mustang_Drag.gin
Unk1= 1
GinsuDecelID= 0
AccelFromIdleID= 6
MaxRPM= 6509.000000
MinRPM= 750.000000
AEMSVol= 18000
GinsuAccelVol= 28000
AccelDeltaRPMThreshold= 135.000000
AEMSMixLRPM= 0.200000
GinsuMixLRPM= 0.850000
AEMSMixSRPM= 0.850000
GinsuMixSRPM= 0.300000
UnkFloat1= 0.250000
UnkFloat2= 0.750000
Unk2= 300
```
This is how it looks. **CarID**, **GinsuDecelID** and **AccelFromIdleID** values also refer to some other stuff. Let's check them out, too!

CarID is used to reference to the Sweetner data. It's at **CarSoundData\SweetnerDataSet\12.ini**:
```ini
[Sweetner]
SweetBank=SWTN_CAR_12_MB.abk
SputterVol= 20000
ShiftSweetsVol= 18000
```
 **GinsuDecelID** is used to reference the DualGinsu data. 
 The file for it is **CarSoundData\DualGinsu\00.ini**:
```ini
[DualGinsu]
GinsuDecelFileName=GIN_Mustang_Drag_DCL.gin
AEMSDecelVol= 14000
GinsuDecelVol= 18000
MinRPM= 2779
MaxRPM= 6381
FadeIn= 0.250000
FadeOut= 0.250000
DecelDeltaRPMThreshold= 120.000000
Unk0= 0
DecelAEMSMixLRPM= 0.200000
DecelGinsuMixLRPM= 0.800000
DecelAEMSMixSRPM= 0.700000
DecelGinsuMixSRPM= 0.200000
```
**AccelFromIdleID** is used to reference the acceleration trans for the car. We can find it at **CarSoundData\AccelFromIdle\06.ini**:
```ini
[AccelTrans]
UnkFloat1= 0.500000
Unk1= 6500
Unk2= 400
Unk3= 2500
Unk4= 1500
```
Confusing, right? I thought so while coding this script. And it's just the beginning. Let's turn back to our **CarDataMapping** to check the IDs for **ShiftPattern** and **Turbo** data for the **Street** package: (**CarSoundData\CarDataMapping\20.ini**)
```ini
[Street]
EngineData= 12
ShiftPattern= 9
TurboDataSet= 12
```
Let's see the Shift Pattern for our car when it's upgraded with the Street Package. It should be at **CarSoundData\ShiftPatterns\09.ini**:
```ini
[ShiftPattern]
BankName=GEAR_LRG_Lev1.abk
Unk1= 14000
UnkFloat1= 0.150000
Unk2= 1200
Unk3= 500
Unk4= 1500
Unk5= 550
Unk6= 500
UnkFloat2= 0.100000
Unk7= 2000
Unk8= 300
Unk9= 1200
Unk10= 400
Unk11= 250
Unk12= 800
Unk13= 12000
UnkFloat3= 0.100000
Unk14= 800
Unk15= 250
Unk16= 1200
Unk17= 400
Unk18= 500
Unk19= 350
UnkFloat4= 0.500000
```
And finally, our Turbo data for the street package is at **CarSoundData\TurboDataSet\12.ini**:
```ini
[Turbo]
TurboBank=TURBO_TRUCK_0_MB.abk
Unk1= 8500
Unk2= 16500
Unk3= 12500
UnkFloat1= 40.000000
ChargeTime= 0.500000
```

You can play around with these until you get the desired result. You can even create new ones up to 99! But you will need to understand what's going on here first. Why didn't they make it file-based at the first place and chose hardcoding all these stuff into the executable instead?

Also check out: https://docs.google.com/spreadsheets/d/18PUeiZC9SmykspJU8SM2sgKe71cLQ9cM3sbLQuzNAJ4/edit?usp=sharing

# Download
You can [download Car Sound Tuner](https://github.com/nlgzrgn/NFSU2CarSoundTuner/releases) from Releases page, or from [NFSMods.xyz](https://www.nfsmods.xyz).
If you want to compile it yourself, you can download the source code from the green Clone or Download button up there.
