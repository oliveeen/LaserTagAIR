# LaserTagAIR
This project is an Arduino-based laser tag system designed for customizable, low-cost gameplay. It combines infrared (IR) communication, sensors, and sound/visual feedback to simulate a real laser tag experience.  The system includes a transmitter and a receiver. The design supports multiple players and team configurations.


 DIY Low-Budget Arduino Laser Tag System

This project is a simple, low-cost laser tag system built around Arduino. It includes a rifle and helmet setup, with 3D-printable parts and customizable gameplay mechanics.

The goal was to create something fun, modular, and easy to reproduce with widely available components.

 Components (for 1 rifle + helmet set)
Arduino Uno R3 / R4 (or compatible clone with solderable headers)
KY-005 IR emitter LED (5 mm)
KY-022 IR receiver module
Collimating lens (Ø 30 mm, focal length 50 mm)
LCD 1602 display
Battery pack (4x AA batteries)
On/off switch
Potentiometer
5.5 mm barrel jack adapter (to power the Arduino from the battery pack)
One or more standard LEDs (can be placed in parallel)
3 push buttons
Resistors (various values, ~220Ω to 10kΩ)
Jumper wires (a Dupont crimping kit is highly recommended)
Soldering equipment (tin, iron, etc.)
 3D Printed Parts

The repository includes STL files for:

Rifle body
Trigger mechanism
Battery pack housing

Print everything and prepare the parts before assembly.

 Assembly

Once all parts are printed:

Assemble the rifle and helmet following the reference images provided.
Mount all electronic components inside the printed structures.
Wire everything according to the circuit design.
Solder connections where needed for stability.
 Software

Upload the provided code to the Arduino using the Arduino IDE.

 Gameplay Features

The system includes basic but customizable gameplay mechanics:

Multiple selectable classes
Each class has different:
Damage
HP (health points)
Fire rate
Hit detection via IR communication
Respawn system
When HP reaches zero, a countdown starts before the player respawns.
 Notes & Tips
Proper alignment of the IR emitter and collimating lens is important for range and accuracy.
The potentiometer can be used for display contrast or tuning (depending on implementation).
Using crimped connectors instead of loose jumper wires will make everything much more reliable.
You can expand the system easily (sound effects, score tracking, team modes, etc.).

If you want, I can also:

turn this into a polished GitHub README with badges and sections
add wiring diagrams
or help you document the code structure 
