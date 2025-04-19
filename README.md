# Dnipro Roller 🛹🇺🇦

**Dnipro Roller** is a 2D side-scrolling arcade game written in C++ using the [Raylib](https://www.raylib.com/) library.  
Control a brave rolling character as you dodge deadly obstacles and try to reach the finish line in the war-torn streets of Dnipro.

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
| Finish Line         | `textures/finish-line.png`          |
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
