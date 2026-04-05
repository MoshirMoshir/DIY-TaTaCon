# DIY TaTaCon

Build your own taiko drum controller at a fraction of the cost. Better than Amazon knockoffs, cheaper than a TaikoForce.

**[View the full guide →](https://tatacon.moshir.dev)**

## About

This is a step-by-step guide to building a custom DIY TaTaCon using wood panels, piezo sensors, and an Arduino. Two build sizes documented:

- **Big TaTaCon** — 18" diameter, simulates the arcade/TaikoForce experience (~$121)
- **Small TaTaCon** — 12" diameter, portable and fits on a laptop stand (~$102)

## Tech Stack

Built with [Astro](https://astro.build), styled with [Tailwind CSS](https://tailwindcss.com) and [Catppuccin](https://catppuccin.com) themes.

## Development

```bash
npm install
npm run dev      # Start dev server at localhost:4321
npm run build    # Build for production
npm run preview  # Preview production build
```

## License

Firmware files include code from:
- [progmem's Switch-Fightstick](https://github.com/progmem/Switch-Fightstick) (Joystick/HID)
- [LUFA Library](https://github.com/abcminiuser/lufa) by Dean Camera (HID constants)
