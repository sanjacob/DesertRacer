/**
 * @file keybinds.h
 * Hold structs for user input keybinds and default ones
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_KEYBINDS_H
#define DESERT_RACER_KEYBINDS_H

#include <TL-Engine.h>


namespace desert
{
	// Camera control keybind
	struct SCameraKeybinding
	{
		const tle::EKeyCode kCamForward;
		const tle::EKeyCode kCamBackward;
		const tle::EKeyCode kCamRight;
		const tle::EKeyCode kCamLeft;
		const tle::EKeyCode kCamReset;
		const tle::EKeyCode kToggleRotation;
	};

	// Hover car control keybind
	struct SControlKeybinding
	{
		const tle::EKeyCode kForwardThrust;
		const tle::EKeyCode kBackwardsThrust;
		const tle::EKeyCode kClockwiseTurn;
		const tle::EKeyCode kAntiClockwiseTurn;
		const tle::EKeyCode kBoost;
	};

	// Some game control keys
	struct SMetaKeybinding
	{
		const tle::EKeyCode kPauseGame;
		const tle::EKeyCode kStartGame;
		const tle::EKeyCode kRestartGame;
		const tle::EKeyCode kToggleMouseCapture;
		const tle::EKeyCode kQuitGame;
	};

	// A struct that combines the three above
	struct SPlayerKeybinding
	{
		const SCameraKeybinding kCamKeybind;
		const SControlKeybinding kControlKeybind;
		const SMetaKeybinding kMetaKeybind;
	};

	// Default layout-agnostic keys
	const SCameraKeybinding kDefaultCameraBind = { tle::Key_Up, tle::Key_Down, tle::Key_Right, tle::Key_Left, tle::Key_1, tle::Key_Shift };
	const SMetaKeybinding kDefaultMetaBind = { tle::Key_P, tle::Key_Space, tle::Key_R, tle::Key_Tab, tle::Key_Escape };

	// QWERTY
	const SPlayerKeybinding kDefaultQwertyBind = { kDefaultCameraBind, { tle::Key_W, tle::Key_S, tle::Key_D, tle::Key_A, tle::Key_Space },  kDefaultMetaBind};

	// DVORAK
	const SPlayerKeybinding kDefaultDvorakBind = { kDefaultCameraBind, { tle::Key_Comma, tle::Key_O, tle::Key_E, tle::Key_A, tle::Key_Space }, kDefaultMetaBind };
}
#endif
