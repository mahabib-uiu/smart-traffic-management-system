Smart Traffic Management System Considering Sensitive Areas

(Ex. Hospital, School/College)
Overview

This project aims to enhance traffic control in sensitive areas by detecting speed violations, capturing vehicle images, and controlling traffic signals and barriers through automation.
Features
✅ Current Features:

    Speed Detection – Uses IR sensors to measure vehicle speed.
    Traffic Signal Control – Manages traffic LED modules via Blynk Dashboard.
    Automated Lowered Gates – Controlled by an MG90 Servo Motor & Red LED via Blynk Dashboard (single switch).
    Violation Detection & Image Capture – Detects speed violations and captures images using ESP32-CAM.

🚀 Future Upgrades:

    Custom Web Dashboard – Replacing Blynk with a fully custom-built web dashboard using Python & FastAPI.
    Database Integration – Storing vehicle images and violation records in PostgreSQL.
    Number Plate Detection & Legal Action – Implementing OCR-based number plate recognition and an automated system for filing cases against rule violators.

Hardware Components

    ESP32 Microcontroller
    ESP32-CAM Module
    IR Sensors (for speed detection & violation detection)
    MG90 Servo Motor (for lowered gates)
    Traffic LED Modules
    Red LED Indicator

Software & Technologies

    Blynk Dashboard (for traffic control – to be replaced)
    Python, FastAPI (for future web dashboard)
    PostgreSQL (for storing vehicle & violation data)
    OpenCV (OCR) (for number plate recognition)

Installation & Setup
1️⃣ Hardware Setup:

    Connect IR sensors to ESP32 for speed detection.
    Integrate ESP32-CAM for image capture.
    Configure MG90 Servo for gate control.
    Set up traffic LED modules.

2️⃣ Software Setup:

    Install Blynk and configure dashboard controls.
    Deploy ESP32 firmware for data processing.
    (Future) Install FastAPI & PostgreSQL for the web dashboard.

How It Works

    Vehicle Speed Monitoring: IR sensors track vehicle speed.
    Traffic Control via Blynk: Dashboard manages LED signals and lowered gates.
    Violation Detection: If a vehicle exceeds speed limits, ESP32 triggers ESP32-CAM to capture an image.
    Future Enhancements: Data will be stored in PostgreSQL, and number plate detection will automate legal actions.

Contributors

    Md Ahsan Habib (Project Lead)
    Orchi Roy
    Sarikha Jahan Sisiliya
    G. M. Abir Hasan

Future Development

The project will evolve into a fully automated traffic management system with an AI-powered dashboard and law enforcement integration.
