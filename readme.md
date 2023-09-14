# GarbageGopher: Advanced Autonomous Garbage Robot

## Introduction

Welcome to the repository of `GarbageGopher`, a cutting-edge robotic solution that brings a paradigm shift in autonomous trash collection. Built upon C++, this project is an amalgamation of software design best practices, state-of-the-art algorithms, and intricate robotics.

## Purpose

The raison d'être of the `GarbageGopher` is not just to serve as an autonomous garbage collection robot, but to manifest a deep understanding of C++, algorithmic complexity, and robotics systems' architectural design.

## Overview: System Architecture & Workflow

At its core, `GarbageGopher` integrates various hardware components and software modules to function cohesively as an autonomous trash collection entity. It is an autonomous robot tailored to resemble a trash bin, outfitted with:
- **Motors**: Dual continuous servo motors for adaptive mobility.
- **Sensors**: An ensemble of sensors and a high-definition camera to perceive its surroundings.
- **Auto Lid**: A servo-driven lid mechanism for autonomous open/close operations.
- **Initialization**: Upon boot-up, the `GarbageGopher` undertakes a self-check routine, ensuring all components are functional.
- **Standby Mode**: Once initialized, it idles in its standby mode, conserving energy and awaiting user commands.
- **Path Planning and Mobility**: On user interaction, it calculates the most efficient route to the user's location while dynamically avoiding obstacles.
- **Trash Collection**: On reaching the destination, its lid actuator initiates the open sequence, and the robot waits for the trash to be deposited.
- **Return Sequence**: Post trash deposition, the robot's localization algorithms kick in, ensuring a seamless return to its base station.
- **Continuous Monitoring**: Throughout its operation, sensors and the camera provide real-time feedback, aiding in navigation and ensuring user safety.

This cyclical process is facilitated by the integration of various hardware components and software algorithms, working in unison to achieve a seamless trash collection experience.

## Technical Deep Dive

### **Actuators**:
- Employs advanced PID (Proportional-Integral-Derivative) controllers to achieve precise control over the servo motors, ensuring smooth and jerk-free movements.
- Utilizes the pyheader module to bridge the gap between C++ and Python, facilitating seamless control of the servo motors through the PCA9685 driver using I2C protocol.

### **Camera & Image Processing**:
- The 8MP camera module captures high-resolution images, providing a detailed visual perspective of the robot's surroundings.
- Initial image preprocessing techniques—like resizing, normalization, and image enhancement—are applied using the OpenCV library.
- Post object detection through ML models, bounding boxes, object labels, and potential paths are superimposed on the images, aiding in navigation decisions.

### **Sensors & Perception**:
- The ultrasonic depth sensor serves as the robot's eyes, measuring distances and ensuring a collision-free experience.
- Depth data, coupled with the visual input from the camera, provides a 170-degree perception environment, empowering the robot with informed decision-making capabilities.

### **Localization & Mapping**:
- Implements SLAM (Simultaneous Localization and Mapping) using the GTSAM library, continuously updating the robot's internal map and its position within that map.
- Such real-time localization ensures the robot remains aware of its environment, making path recalculations swift and accurate.

### **Path Planning & Navigation**:
- Algorithms like A* and RRT (Rapidly-exploring Random Trees) have been integrated for efficient path planning.
- A state-driven approach manages the robot's activities, such as "Moving to user", "Waiting for trash", or "Returning to base", ensuring logical and sequential operation flow.

### **ML Models & Intelligence**:
- Two core ML models guide the robot: Object Detection and Depth Perception. While object detection is focused on recognizing indoor entities, depth perception provides spatial depth insights.
- Models are optimized for the Jetson Nano's compute capacity, ensuring real-time processing without compromising on accuracy.

## Requirements & File Architecture

## Hardware and External Dependencies:

- **Jetson Nano**: The brain of the robot, responsible for on-board computations, ML inferences, and overall command and control.
- **SunFounder PCA9685**: This 16-channel, 12-bit PWM driver plays a pivotal role in orchestrating the precise movements of the servo motors.
- **Titri HC-SR04 Ultrasonic Sensor**: Initially integrated for depth perception, it provides a basic layer of obstacle detection, albeit with limitations.
- **LiDAR Sensor**: Replacing ultrasonic for primary obstacle detection and mapping due to its higher range and accuracy.
- **8MP IMX219-200 Camera**: This high-definition camera serves as the robot's primary vision system, feeding visual data for object detection and path planning.
- **20kg servo motor**: This robust actuator drives the lid's open/close mechanism, ensuring smooth operations.

## Software Dependencies:

- **OpenCV**: Integral for image processing tasks, from capturing images to preprocessing and post-processing.
- **GTSAM**: An advanced library that facilitates the robot's SLAM (Simultaneous Localization and Mapping) capabilities.
- **ONNX**: A runtime inference engine that allows the robot to utilize trained machine learning models for tasks like object detection and depth perception.
- **Python Bindings**: The `pyheader` module, enabling seamless integration between C++ and Python libraries, critical for various hardware interfaces.
- **PID Controller Library**: Integral for fine-tuning and achieving precise control over the servo motors, ensuring optimal movement accuracy.

**File Structure**: 
The entire project is modular, with clear separation between headers (`.hpp`) and source files (`.cpp`). A comprehensive directory structure ensures clarity, and dependencies like third-party libraries or ML models are neatly organized.
### File Structure
```
GarbageGopher/
|
|-- include/
|    |-- actuators.hpp
|    |-- camera.hpp
|    |-- pyheader.hpp
|    |-- sensors.hpp
|    |-- localization.hpp
|    |-- path_planning.hpp
|    |-- navigation.hpp
|    |-- ml_model.hpp
|    |-- image_processing.hpp
|
|-- src/
|    |-- actuators.cpp
|    |-- camera.cpp
|    |-- main.cpp
|    |-- pyheader.cpp
|    |-- sensors.cpp
|    |-- localization.cpp
|    |-- path_planning.cpp
|    |-- navigation.cpp
|    |-- ml_model.cpp
|    |-- image_processing.cpp
|
|-- models/
|    |-- object_detection.model
|    |-- depth_perception.model
|
|-- third_party/
|    |-- eigen/
|    |-- ONNX/
|    |-- gtsam/
|    |-- boost/
|    |-- pid/
|    |-- opencv/
|    |-- GPIO/
|    |-- Python/
|    |-- Torch/
|
|-- .gitignore
|-- CMakeLists.txt
|-- readme.md
```

## Conclusion

`GarbageGopher` is more than just a piece of software—it's a symphony of intricate design, meticulous engineering, and technological innovation. The robust, scalable, and modular architecture invites researchers, engineers, and enthusiasts to explore, contribute, and elevate it to new horizons.

## Limitations

The `GarbageGopher` is not without its constraints:

- **Ultrasonic Sensor**: The initial design integrated ultrasonic sensors for depth perception. Their limited range and accuracy proved inadequate for advanced operations. This motivated the shift to LiDAR, which provided superior range, accuracy, and compatibility with communication protocols like I2C and SPI.

- **Payload Capacity**: The current servo and structural design limit the weight of trash the robot can handle at one time. Overloading may lead to sub-optimal performance or hardware damage.

- **Operating Environment**: `GarbageGopher` is optimized for indoor environments. While it can function outdoors, its sensors and algorithms might not perform optimally in extreme weather conditions, varying lighting or on uneven terrains.

- **Battery Life**: Continuous operation, especially with the camera and LiDAR active, can drain the battery quickly. The current design needs periodic recharging to maintain optimum performance.

- **Navigation Speed**: While precision and safety are top priorities, the trade-off is speed. The robot is not designed to move quickly, which might be an issue if rapid response is needed.

- **Machine Learning Inferences**: The ML models, especially when dealing with high-resolution inputs from the 8MP camera, might occasionally produce false positives or miss certain objects. This is an inherent limitation of current ML models.

- **Network Dependency**: For certain functions or updates, the robot requires a stable internet connection. In areas of low network connectivity, these features might be compromised.

These limitations, while present, serve as areas of potential future improvement and optimization. Feedback, contributions, and iterative designs will undoubtedly help overcome these challenges, pushing the frontier of what `GarbageGopher` can achieve.

## Future Roadmap

- [ ] **Optimization**: Further refine pathfinding algorithms for enhanced efficiency.
- [ ] **Extended Sensing**: Integrate LIDAR for richer environmental mapping.
- [ ] **Connectivity**: IoT integrations for real-time monitoring and control via cloud.
- [ ] **User Interaction**: Voice command integration using NLP (Natural Language Processing).

Dive in, explore, and let's propel `GarbageGopher` to new zeniths together.

## License Information for GarbageGopher
<a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-nd/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/4.0/">Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License</a>.