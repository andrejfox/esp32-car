# Pozor roboti na cesti!

## Opis projekta
Ta projekt predstavlja nadzor avtomobila, ki temelji na ESP32. Uporablja se za učenje in eksperimentiranje z IoT tehnologijami ter programiranjem v okolju ESP-IDF.

## Zahteve
Pred začetkom dela na projektu potrebujete naslednje:

- ESP32 razvojna plošča
- Računalnik z nameščenim operacijskim sistemom Windows, macOS ali Linux
- Visual Studio Code (VSCode)

## Namestitev Visual Studio Code in potrebnih extensionov

1. **Namestitev Visual Studio Code:**
   - Prenesite in namestite Visual Studio Code z uradne spletne strani: [Visual Studio Code](https://code.visualstudio.com/).

2. **Namestitev potrebnih razširitev:**
   - Odprite VSCode.
   - Pojdite na razdelek "Extensions" (ikona štirih kvadratov na levi strani).
     
     ![image](https://github.com/user-attachments/assets/5d1a8e08-d283-4420-b6bc-7cdba64b9b23)
   - V iskalno polje vpišite naslednje razširitve in jih namestite:
     - **C/C++** (Microsoft)
     - **PlatformIO IDE** (PlatformIO)
    
       ![image](https://github.com/user-attachments/assets/75da50e5-d9cd-47d1-8062-c306645d63bf)
       ![image](https://github.com/user-attachments/assets/a2ff5e1c-1d5e-4cce-9f92-349d626514b6)

## Kloniranje repozitorija

Klonirajte repozitorij na svoj računalnik z uporabo naslednje ukazne vrstice:

```bash
git clone https://github.com/andrejfox/esp32-car.git
```

Ali pa kliknete "Source Control" -> 3 pikice -> clone:

![image](https://github.com/user-attachments/assets/a7599677-bbb1-4bb8-8671-b170dca13321)

![image](https://github.com/user-attachments/assets/0cb5b1fc-8ea9-4b93-aa35-0c69256f7635)

![image](https://github.com/user-attachments/assets/c2300508-bdd3-4604-afc6-a0d25a1f2a98)

In nato na sredino zaslona v kvadratek vpišete download code link od repozitorja:

![image](https://github.com/user-attachments/assets/6fb5824e-cb70-4586-969a-4233810a72e0)

![image](https://github.com/user-attachments/assets/04a5cbf0-3c32-489a-ab78-2e6829bad836)

Zdaj pa samo kliknite enter in po izbiri lokacije nalaganja se vam bo repozitorij naložil!

![image](https://github.com/user-attachments/assets/8cb7d683-fabd-4672-944f-d316fa9818ee)

## Priprava in nalaganje

Preden kodo naložimo na esp32 moramo spremeniti spremenlivke:
```c++
// WiFi nastavitve
const char *ssid = "<ime WiFi-ja>";
const char *password = "<geslo WiFi-ja>";
```
Ter če imate svoj MQTT brocker:
```c++
// MQTT Broker nastavitve
const char *mqtt_broker = "<ime.com ali IP>";
const char *mqtt_topic = "<ime topica>";
const char *mqtt_username = "<broker ime>"; 
const char *mqtt_password = "<broker geslo>";
const int mqtt_port = <broker port (default: 1883)>;
```

Sedaj pa kodo samo nalozimo z klikom zgoraj desno na gumb "Upload":

![image](https://github.com/user-attachments/assets/2a5f3356-2d74-4031-a2e0-6ce35489a1d7)

in če je naš avtoček uspešno narejen in priklopljen bi morali videti nekaj takega:

![image](https://github.com/user-attachments/assets/119c013f-2c05-4cd1-8f84-c68de723d467)











