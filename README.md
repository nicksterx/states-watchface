# 50 States Watchface for Pebble Time

A watchface that teaches you US history one minute at a time.

## How it works

Each minute of the hour maps to a piece of American history:

| Minute | Content |
|--------|---------|
| :01    | Delaware — 1st state (ratified 1787) |
| :02    | Pennsylvania — 2nd state |
| ...    | ... |
| :50    | Hawaii — 50th state (1959) |
| :51    | Washington D.C. |
| :52    | Puerto Rico |
| :53    | Guam |
| :54    | US Virgin Islands |
| :55    | American Samoa |
| :56    | Northern Mariana Islands |
| :57    | Founding Facts |
| :58    | US Records & Superlatives |
| :59    | Weird America |
| :00    | America |

## Facts rotate daily

Each entry has 4–6 facts. The watchface picks one based on the day
of the year, so you see something different every day without repetition
within a single day.

## Layout

```
┌─────────────────┐
│     10:23       │  ← Large time (Bitham 42 Bold)
│ ─────────────── │
│    Illinois     │  ← State name (Gothic 24 Bold, white)
│ Springfield • 18│  ← Capital & year (Gothic 14, gray)
│ ─────────────── │
│  Chicago is the │  ← Daily rotating fact
│ birthplace of   │  ← (Gothic 18, yellow)
│ the skyscraper  │
└─────────────────┘
```

## Shake for another fact

Shake or flick your wrist to cycle through the other facts for the
state currently on screen.

## Settings

Open the watchface's settings in the Pebble phone app:

- **Text Size** — Normal, Large, or Extra Large:
  - **Normal** — the original layout
  - **Large** — bigger title, subtitle, and fact text (the default on
    Pebble Time 2, whose higher-resolution screen makes text render
    physically smaller)
  - **Extra Large** — like Large, with the fact text bumped up another
    step (Gothic 24 Bold); best on Pebble Time 2, where there's room
    for it — on smaller screens the longest facts may not fully fit
- **Show capital & year line** — hide it to free room for the fact
  text, so even the longest facts fit in Extra Large
- **Background Color** and **Fact Color** — applied on color screens;
  black & white watches pick the closest shade. With a light
  background, the time and title automatically switch to dark text.

All settings are saved on the watch, so they stick across restarts.

## Setup (CloudPebble)

1. Create a new **Pebble C SDK** project
2. Set project type to **Watchface**
3. Replace the default `main.c` with `src/c/main.c`
4. Create a new file `src/c/states_data.h` and paste in the data file
5. Add a JS file with the contents of `src/pkjs/index.js` (settings page)
6. Build and install

## Setup (Local SDK)

```bash
cd states-watchface
pebble build
pebble install --phone YOUR_PHONE_IP
```

## CloudPebble settings

- **Project type**: Pebble C SDK
- **App type**: Watchface
- **SDK**: 3
- **Capabilities**: Configurable (for the settings page)
- No packages needed
