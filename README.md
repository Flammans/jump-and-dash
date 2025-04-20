# Dnipro Roller 🛼🇺🇦

**Dnipro Roller** is a 2D side-scrolling survival arcade game written in C++ using the [Raylib](https://www.raylib.com/) library.

You control **Rollfy**, a courageous roller racing through the war-torn streets of Dnipro during an air raid.

Your mission is clear:  
🏁 **Reach the shelter before it's too late.**

Along the way:
- Avoid deadly **anti-tank hedgehogs**.
- Jump or double-jump dash through danger.
- Listen for **air raid warnings**.
- Land safely — and stay alert.

One mistake, and it’s over.  
Each attempt is a lesson in urgency and survival.

> _"Don't ignore air raid warnings.  
> If you want to survive, you must reach the shelter  
> as fast as possible before another strike hits your city."_  

Will you make it in time?

🎶 Features immersive sounds, reactive controls, atmospheric scrolling visuals — and a message that matters.

---

## 🎮 Gameplay

- Move left and right with **A/D** or **←/→**.
- Press **SPACE** to jump.
- Press **SPACE twice** to perform a **double jump dash** (in the direction you're moving).
- Avoid anti-tank hedgehogs (obstacles).
- Reach the finish line to **win**.
- Colliding with a hedgehog means **game over**.

---

## 🕹 Controls

| Action         | Key         |
|----------------|-------------|
| Move Left      | A / ←       |
| Move Right     | D / →       |
| Jump           | Space       |
| Double Jump    | Space (twice) |
| Restart Game   | R           |

---

## 🔊 Sound & Music

- **Jump**: `jump.wav`
- **Landing**: `land.wav`
- **Hit/Crash**: `hit.wav`
- **Victory**: `win.wav`
- **Defeat**: `lose.wav`
- **Background Music**: `music.mp3`
- **Main Music**: `main.wav`

Make sure you have a `sounds/` folder with all required sound files.

---

## 📁 Assets

| Asset Name          | Location              |
|---------------------|-----------------------|
| Background          | `textures/dnipro-far-buildings.png` |
| Foreground          | `textures/dnipro-street.png`        |
| Character (run)     | `textures/rollfy.png`               |
| Character (jump)    | `textures/rollfy-jump.png`          |
| Obstacle (enemy)    | `textures/czech-hedgehog.png`       |
| Shellter            | `textures/shelter.png`              |
| Game Over Screen    | `textures/rollfy-game-over.png`     |
| Win Screen          | `textures/rollfy-vin.png`           |

---

## 🧱 Features

- Smooth parallax scrolling background & foreground
- Double jump with horizontal boost and rainbow trail (WIP)
- Sound effects and background music
- Game over and victory screens
- Resettable game loop
- Dynamic enemy 

---

## 🛠 Requirements

- [Raylib](https://www.raylib.com/) (C++)
- C++17 compiler (e.g., g++, clang++, MSVC)
- All textures and sounds in the proper folders (`textures/`, `sounds/`)

---

## ▶️ Build & Run

```bash
g++ -std=c++17 main.cpp -o DniproRoller -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./DniproRoller
