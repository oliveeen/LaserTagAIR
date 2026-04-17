# DIY Low-Budget Arduino Laser Tag System
This project is an Arduino-based laser tag system designed for customizable, low-cost gameplay. It combines infrared (IR) communication, sensors, and visual feedback to simulate a real laser tag experience.  The system includes an IR transmitter and an IR receiver. The design supports multiple players and team configurations.

____________________________________________________________________

 COMPONENTS (for 1 rifle + helmet set)
Arduino Uno R3 / R4 or compatible clone with solderable headers (it's mandatory to have solderable headers if you don't want to use breadboards or solderable pcbs)
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
You can get the helmet printing anyone and installing IR receiver and LED

The repository includes STL files for:
Rifle body
Trigger mechanism
Battery pack housing (you can easily purchase it on aliexpress if you prefer)

Print everything and prepare the parts before assembly.

____________________________________________________________________

GAMEPLAY FEATURES
The system includes basic but customizable gameplay mechanics:
Multiple selectable classes
Each class has different:
Damage
HP (health points)
Fire rate
Damage
You can select the class through buttons.
Respawn system
When HP reaches zero, a countdown starts before the player respawns.
 Notes & Tips
Proper alignment of the IR emitter and collimating lens is important for range and accuracy.
The potentiometer can be used for display contrast or tuning (depending on implementation).
Using crimped connectors (dupont terminals) instead of loose jumper wires will make everything much more reliable.
You can expand the system easily (sound effects, score tracking, team modes, etc.).

____________________________________________________________________

WIRING 

-IR transmitter
DAT = Pin_3 
GND = GND

-IR receiver
OUT = Pin_11
GND = GND
VCC = 5v

-Buttons 
<img width="1204" height="1600" alt="buttonsWiring" src="https://github.com/user-attachments/assets/58c7b64a-f5d4-4668-96fd-a899b8fbe531" />
<img width="1536" height="2040" alt="FireButtonPosition" src="https://github.com/user-attachments/assets/455daa69-cc3e-48ad-9d4f-6644197a7199" />


____________________________________________________________________

ASSEMBLY

Once all parts are printed:

Assemble the rifle and helmet wiring (and soldering) as shown
<img width="1204" height="1600" alt="assembly2" src="https://github.com/user-attachments/assets/5732682b-21c3-44c0-893c-928f7e189814" />
<img width="1204" height="1600" alt="assembly2" src="https://github.com/user-attachments/assets/8a3bd42b-6f74-4d6f-b285-b53699f6bb59" />
<img width="1204" height="1600" alt="assembly1" src="https://github.com/user-attachments/assets/0fd5dd1e-7248-452d-921a-0476c5874516" />

Mount all electronic components inside the printed structures.
Wire everything according to the circuit design.
Solder connections where needed for stability.

____________________________________________________________________

TRIGGER MECHANISM

<img width="1536" height="2040" alt="triggerMechanism" src="https://github.com/user-attachments/assets/509867ee-269d-4d4b-8309-a6e14f789e6a" />






