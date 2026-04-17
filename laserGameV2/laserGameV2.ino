#include <IRremote.hpp>
#include <LiquidCrystal.h>

// ====================== PIN ==========================
#define IR_SEND_PIN 3
#define IR_RECV_PIN 11
#define pinNext 8
#define pinSelect 9
#define pinShot 6
#define pinLight 10

// ====================== IR CODES ======================
#define BLUE_S 0x45
#define RED_S  0x46

#define NUM_CLASSES 3

// ====================== LCD ===========================
const int rs = 12, en = 2, d4 = 5, d5 = 4, d6 = 13, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// ====================== MENU SYSTEM ====================
enum MenuState { MENU_SQUAD, MENU_CLASS, GAME_RUNNING, RESPAWN_WAIT };
MenuState menuState = MENU_SQUAD;

String teamNames[2] = { "Rossa", "Blu" };
int currentTeamIndex = 0;
String squadraScelta = "";

String classNames[NUM_CLASSES] = { "ASSALTO", "TANK", "ASSASSINO" };
int currentClassIndex = 0;
int classID = 0;

// =================== CLASS DATA =====================
struct ClassStats {
  int vita;
  int danno;
  int ammoMax;
  float fireRate;
  int reloadMS;
};

ClassStats classData[NUM_CLASSES] = {
  {100, 20, 30, 4.0, 4000},           //ASSALTO
  {300, 12, 15, 5.0, 2500},           //TANK          {VITA - DANNO - CARICATORE - FIRERATE(COLPI/s\  \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ A) - RICARICA}
  {60, 240, 1, 1.0, 10000}            //ASSASSINO 
};

// ==================== GAME VARS =====================
int vita = 100;
int danno = 20;
int ammo = 30;
int ammoMax = 30;

bool reloading = false;
unsigned long reloadStartTime = 0;
unsigned long lastShot = 0;
unsigned long fireInterval = 250;

unsigned long lastValidHit = 0;
const unsigned long hitCooldown = 120;

// RESPAWN
unsigned long respawnStart = 0;
const unsigned long respawnTime = 30000;

// ========================================================
// ===================== DEBOUNCE =========================
// ========================================================
bool buttonPressed(int pin) {
  if (digitalRead(pin) == HIGH) {
    delay(180);
    while (digitalRead(pin));
    return true;
  }
  return false;
}

// ========================================================
// ===================== SETUP ============================
// ========================================================
void setup() {

  Serial.begin(9600);
  delay(100);

  lcd.begin(16, 2);

  pinMode(pinNext, INPUT);
  pinMode(pinSelect, INPUT);
  pinMode(pinShot, INPUT);
  pinMode(pinLight, OUTPUT);

  IrSender.begin(IR_SEND_PIN);
  IrReceiver.begin(IR_RECV_PIN);

  Serial.println("=== SISTEMA LASERTAG AVVIATO ===");

  showTeamMenu();
}

// ========================================================
// ======================= LOOP ===========================
// ========================================================
void loop() {

  switch (menuState) {

    case MENU_SQUAD:      handleTeamMenu();   break;
    case MENU_CLASS:      handleClassMenu();  break;
    case GAME_RUNNING:    gameLoop();         break;
    case RESPAWN_WAIT:    respawnLoop();      break;
  }
}

// ========================================================
// ====================== MENU TEAM =======================
// ========================================================
void showTeamMenu() {
  lcd.clear();
  lcd.print("Scegli squadra");
  lcd.setCursor(0, 1);
  lcd.print(teamNames[currentTeamIndex]);
}

void handleTeamMenu() {

  if (buttonPressed(pinNext)) {
    currentTeamIndex = (currentTeamIndex + 1) % 2;
    lcd.setCursor(0, 1);
    lcd.print(teamNames[currentTeamIndex] + "   ");
  }

  if (buttonPressed(pinSelect)) {
    squadraScelta = teamNames[currentTeamIndex];
    lcd.clear();
    delay(300);
    showClassMenu();
    menuState = MENU_CLASS;
  }
}

// ========================================================
// ====================== MENU CLASS ======================
// ========================================================
void showClassMenu() {
  lcd.clear();
  lcd.print("Scegli classe");
  lcd.setCursor(0, 1);
  lcd.print(classNames[currentClassIndex]);
}

void handleClassMenu() {

  if (buttonPressed(pinNext)) {
    currentClassIndex = (currentClassIndex + 1) % NUM_CLASSES;
    lcd.setCursor(0, 1);
    lcd.print(classNames[currentClassIndex] + "   ");
  }

  if (buttonPressed(pinSelect)) {

    classID = currentClassIndex;

    vita = classData[classID].vita;
    danno = classData[classID].danno;
    ammoMax = classData[classID].ammoMax;
    ammo = ammoMax;

    fireInterval = (1000.0 / classData[classID].fireRate);
    if (fireInterval < 50) fireInterval = 50;

    lcd.clear();
    delay(300);
    updateGameScreen();
    menuState = GAME_RUNNING;
  }
}

// ========================================================
// ===================== GAME LOOP ========================
// ========================================================
void gameLoop() {

  if (vita == 0) return;

  // ---------- RICARICA ----------
  if (reloading) {
    if (millis() - reloadStartTime >= classData[classID].reloadMS) {
      ammo = ammoMax;
      reloading = false;
      updateGameScreen();
    } else {
      lcd.setCursor(10, 1);
      lcd.print("RELD ");
      if ((millis() - reloadStartTime) % 400 < 200)
        digitalWrite(pinLight, HIGH);
      else
        digitalWrite(pinLight, LOW);
      return;
    }
  }

  // ---------- SPARO ----------
  if (digitalRead(pinShot) == HIGH &&
      millis() - lastShot >= fireInterval) {

    lastShot = millis();

    if (ammo > 0) {
      ammo--;
      updateGameScreen();

      uint8_t cmd;
      if (squadraScelta == "Rossa") cmd = RED_S + classID;
      else                         cmd = BLUE_S + classID;

      IrSender.sendNEC(0x00FF, cmd, 0);

    } else {
      reloading = true;
      reloadStartTime = millis();
      return;
    }
  }

  // ---------- RICEZIONE IR ----------
  leggiIR();
}

// ========================================================
// ===================== IR RECEIVER ======================
// ========================================================
void leggiIR() {

  // 🔥 NON REGISTRARE COLPI DURANTE RESPAWN
  if (menuState == RESPAWN_WAIT) {
    IrReceiver.resume();
    return;
  }

  if (!IrReceiver.decode())
    return;

  uint32_t raw = IrReceiver.decodedIRData.decodedRawData;
  uint8_t cmd = IrReceiver.decodedIRData.command;

  if (raw == 0xFFFFFFFF) {
    IrReceiver.resume();
    return;
  }

  if (millis() - lastValidHit < hitCooldown) {
    IrReceiver.resume();
    return;
  }

  bool nemico = false;

  if (squadraScelta == "Rossa") {
    if (cmd >= BLUE_S && cmd < BLUE_S + NUM_CLASSES) nemico = true;
  } else {
    if (cmd >= RED_S && cmd < RED_S + NUM_CLASSES) nemico = true;
  }

  if (!nemico) {
    IrReceiver.resume();
    return;
  }

  int enemyClass = (squadraScelta == "Rossa")
                     ? cmd - BLUE_S
                     : cmd - RED_S;

  if (enemyClass < 0 || enemyClass >= NUM_CLASSES) {
    IrReceiver.resume();
    return;
  }

  int dmg = classData[enemyClass].danno;
  vita -= dmg;
  if (vita < 0) vita = 0;

  updateGameScreen();

  digitalWrite(pinLight, HIGH);
  delay(120);
  digitalWrite(pinLight, LOW);

  lastValidHit = millis();

  // ---------- MORTE ----------
  if (vita == 0) {
    showRespawnScreen();
    respawnStart = millis();
    menuState = RESPAWN_WAIT;
  }

  IrReceiver.resume();
}

// ========================================================
// ===================== RESPAWN LOOP =====================
// ========================================================
void showRespawnScreen() {
  lcd.clear();
  lcd.print("SEI MORTO!");
  lcd.setCursor(0, 1);
  lcd.print("Respawn: 30s ");
}

void respawnLoop() {

  unsigned long elapsed = millis() - respawnStart;
  unsigned long remaining = (respawnTime - elapsed) / 1000;

  // 🔥 Riscrivo completamente la riga per evitare glitch LCD
  lcd.setCursor(0, 1);
  lcd.print("Respawn: ");
  if (remaining < 10) lcd.print(" "); // padding estetico
  lcd.print(remaining);
  lcd.print("s   ");

  // LED lampeggiante
  if ((elapsed % 600) < 300)
    digitalWrite(pinLight, HIGH);
  else
    digitalWrite(pinLight, LOW);

  if (elapsed >= respawnTime) {

    vita = classData[classID].vita;
    ammo = ammoMax;

    // 🔥 RESET anti-doppio-hit
    lastValidHit = millis();

    digitalWrite(pinLight, LOW);

    updateGameScreen();
    menuState = GAME_RUNNING;
  }
}

// ========================================================
// ===================== LCD UPDATE =======================
// ========================================================
void updateGameScreen() {

  lcd.setCursor(0, 0);
  lcd.print("Vita: ");
  lcd.print(vita);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("Ammo: ");
  lcd.print(ammo);
  lcd.print("/");
  lcd.print(ammoMax);
  lcd.print("  ");
}

