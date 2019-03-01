# Meine Veraenderungen am CeleX4 Repo
---

## Veraenderte Dateien sind:
* README.md
* /Sources/SDK/Makefile
* /Sources/SDK/include/celextypes.h
* /Sources/SDK/include/dvslib.h
* /Sources/SDK/include/celex4/celex4processeddata.h
* /Sources/SDK/include/celex4/celex4datamanager.h
* /Sources/SDK/include/celex4/celex4.h
* /Sources/SDK/frontpanel/okFrontPanelDLL.h
* /Sources/SDK/frontpanel/frontpanel.h
* /Sources/SDK/frontpanel/frontpanel.cpp
* /Sources/SDK/eventproc/fpgadatareader.h
* /Sources/SDK/eventproc/fpgadatareader.cpp
* /Sources/SDK/eventproc/eventprocessing.cpp
* /Sources/SDK/eventproc/datarecorder.h
* /Sources/SDK/eventproc/datarecorder.cpp
* /Sources/SDK/eventproc/datareaderthread.h
* /Sources/SDK/eventproc/datareaderthread.cpp
* /Sources/SDK/eventproc/dataprocessthread.h
* /Sources/SDK/eventproc/dataprocessthread.cpp
* /Sources/SDK/eventproc/celex4processeddata.cpp
* /Sources/SDK/configproc/hhxmlreader.h
* /Sources/SDK/configproc/hhxmlreader.cpp
* /Sources/SDK/configproc/hhwireincommand.h
* /Sources/SDK/configproc/hhwireincommand.cpp
* /Sources/SDK/configproc/hhdelaycommand.h
* /Sources/SDK/configproc/hhdelaycommand.cpp
* /Sources/SDK/configproc/hhcommand.h
* /Sources/SDK/configproc/hhcommand.cpp
* /Sources/SDK/base/xthread.h
* /Sources/SDK/base/xthread.cpp
* /Sources/SDK/base/xbase.h
* /Sources/SDK/base/xbase.cpp
* /Sources/SDK/base/dataqueue.h
* /Sources/SDK/base/dataqueue.cpp



## NICHT Veraenderte Dateien sind:
* ReleaseNotes.txt
* LICENSE

*MS Visual Studio Dateien:*
* /Sources/SDK/CeleX.vcxproj
* /Sources/SDK/CeleX.sln

*Bibliothek fuer das Frontpanel(?):*
* /Sources/SDK/lib/*

*TinyXML-Bibliothek:*
* /Sources/SDK/configproc/tinyxml/*

*Kompilierte Version von MS Visual Studio (hat nichts zu suchen im Repo):*
* /Sources/SDK/build/*

*Archiv mit Quelltext des FPGA(?):*
* /Sources/FPGA/*

*Eine Demo, deren Dateien nicht gleich der SDK sind! (sollte in /Samples/ sein):*
* /Sources/CeleXDemo/*

*Beispiel-Programme/-Projekte:*
* /Samples/*

*Betriebssystemabhaengige Treiber:*
* /Drivers/*

*Dokumentationen:*
* /Documentation/*

*Demonstrationsprogramm(?):*
* /DemoGUI/*


## NOCH NICHT Veraenderte Dateien sind:
* /Sources/SDK/eventproc/fpgadataprocessor.h
* /Sources/SDK/eventproc/fpgadataprocessor.cpp
* /Sources/SDK/eventproc/eventprocessing.cpp
* /Sources/SDK/eventproc/celex4.cpp
* /Sources/SDK/configproc/hhsequencemgr.h
* /Sources/SDK/configproc/hhsequencemgr.cpp
* /Sources/SDK/base/xthread.cpp


## Dateien, die von Kompatibilitaetsaenderungen OpenCV3 => 4 betroffen sind:
* kommt noch

---

## CeleX4
SDK for CeleX4 sensor.

![Structure](https://github.com/CelePixel/CeleX4-SDK/blob/master/Sources/CeleXDemo/images/SDK_Structure.png)

* CeleX<sup>TM</sup> is a family of smart image sensor, specially designed for machine vision. Each pixel in CeleX<sup>TM</sup>
sensor can individually monitor the relative change in light intensity and report an event if a threshold is
reached.

* The output of the sensor is not a frame, but a stream of asynchronous digital events. The speed of the sensor
is not limited by any traditional concept such as exposure time and frame rate. It can detect fast motion
which is traditionally captured by expensive, high speed cameras running at thousands of frames per second,
but with drastically reduced amount of data.

* Our technology allows post-capture change of frame-rate for video playback. One can view the video at
10,000 frames per second to see high speed events or at normal rate of 25 frames per second.

* This SDK provides an easy-to-use software interface to get data from the sensor and communicate with the
sensor, and it is consistent across the Windows (32-/64-bit) and Linux (32-/64-bit) development
environments. In addition, it provides both pure C++ interfaces without any third libraries and
OpenCV-based interfaces to obtain data from the sensor.

* This SDK provides three working modes of CeleX4 Sensor: Full-Picture data, Event data, and
Optical-Flow data. Full-Picture and Event data output alternately to create FullPic-Event data.

`The CeleX4 is structured as follows:`

* _DemoGUI_: CeleX4 Demo GUI execution (Windows and Linux).
* _Documentation_:
  * _CeleX_SDK_Reference_: The introduction of CeleX4 sensor and the references of all the classes and functions in the SDK.
  * _CeleX_SDK_Getting_Started_Guide_: The instructions to use the CeleX4 sensor demo kit, install OpalKelly driver, run the CeleXDemo GUI and compile the source code.
* _Drivers_: OpalKelly FPGA Board driver (Windows / Linux / ARM).
* _Sources_:
  * _SDK_: Source code of CeleX4 library.
  * _CeleXDemo_: Source code (developed by Qt) of CeleX4 demo.
  * _FPGA_: SE Project files, top level source code, Lower Layer FPGA module Declaration and Netlist file (ngc file).
* _Samples_: Several examples developed based on SDK and a sample user manual file.
* _ReleaseNotes.txt_: New features, fixed bugs and SDK development environment.
