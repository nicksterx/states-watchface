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

## Setup (CloudPebble)

1. Create a new **Pebble C SDK** project
2. Set project type to **Watchface**
3. Replace the default `main.c` with `src/c/main.c`
4. Create a new file `src/c/states_data.h` and paste in the data file
5. Build and install

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
- No packages or capabilities needed
