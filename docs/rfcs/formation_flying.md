# Formation flying

This is a proposal for specifying formations for ships (as discussed in https://github.com/endless-sky/endless-sky/issues/4438, https://github.com/endless-sky/endless-sky/issues/302 and others) by lines that are moved, rotated, extended and repeated.

The mechanism should be sufficiently powerful to handle the most common formations that players and writers would want to use.


## Intended audience and scope

Scenario writers to use for NPC fleets in missions (by editing datafiles directly).

Players to set formations on their ships use (in-game by using pre-defined formations).

Out of scope for this request is an in-game (or out-of-game) graphical formations editor (as discussed in https://github.com/endless-sky/endless-sky/issues/4606)


## Currently implemented functionality
(implemented in https://github.com/endless-sky/endless-sky/pull/4471)
- Formations on NPC fleets (and 1 example of a group of ships actually flying in formation)
- Ships going to formation positions when they are `idle`, and breaking formation in any other situation.
- Ships taking formation-positions based on the formation and leader set for them.
- Possibility for multiple formations around a single leader (possibly with different ships having different formations)
- Ships automatically filling up positions of killed ships (by moving to the earliest formation-position available to the ship).
- Ships in a formation will sync based on the movement vector of the reference/lead ship (not the facing vector of the reference/lead ship).


## Highly desired but currently not implemented functionaly
- UI elements to assign ships to formations.
- Ships going to formation positions when they are `gathering`.
- Automatic spacing between ships in formation based on ship sizes; currently not implemented, `double activeScalingFactor = 80` used as placeholder in https://github.com/endless-sky/endless-sky/pull/4471. The value of 80 works nicely for sparrows and other small ships.


## Nice to have, but not so critical not implemented functionality
- Formations not just around formation leader, but also around other ships/planet/stellar bodies.
- Add behaviours for when to break formations (separate RFC for specifying behaviours on escorts).


## Flying behaviour

### Position-keeping
Basic aggresive position keeping is in place in https://github.com/endless-sky/endless-sky/pull/4471 . More gracefull formation position keeping (similar to station keeping) would be nice for a follow-up PR.

### Formation turning
The positions in a formation as implemented in https://github.com/endless-sky/endless-sky/pull/4471 exclusively depend on the position and movement direction of the lead ship.
If the lead ships movement direction changes 180 degrees (because the lead ship is reversing course), then all ships in the formation will immediately fly to their new positions exactly on the other side of the lead ship, causing the formation to collapse towards the lead-ship and then expand from the lead-ship again.

This collapsing and re-expanding looks somewhat unprofessionally and can be undesired for protective formations that are around one or more targets to be protected. If the formation is used to screen for missiles, then a single missile just after collapsing the formation can do damage to a large number of formation members (including the protected ships in the middle of the formation).

Possible improvements to make the turn behavior of the formation nicer:
- Let the formation itself have some memory of its heading and turn the formation at the turn-speed of the lead-ship, some default minimum turn-speed value, or at a speed based on the maximum speed of the outer formation members. This will result in the formation staying more intact, but increases the time to align the formation to a new movement direction of the lead-ship.
- If the heading of the lead-ship doesn't match it's movement direction, then let the formation still use the movement direction as base heading, but apply 1/3rd of the movement-heading difference of the lead-ship (with a maximum of 30 degrees) as heading-delta for the formation to move to. This allows the formation to already apply some turning while the lead-ship is preparing a course change, but also keeps the formation mostly pointed in the movement direction.

Using symmetry (rotational or mirrored) of a formation can also allow for other nice turn behaviors.



## Data format and keywords

### Formation definitions

The basic structure of a definition of a formation in the data-files would look like:
```
formation <name>:
	symmetry ["transverse"] ["longitudinal"]
		rotational <nr>
	line <x> <y> <slots> <direction>
		spacing <nr>
		repeat <deltaX> <deltaY>
			increase <nr>
	line <x2> <y2> <slots2> <direction2>
		spacing <nr2>
	arc <refX> <refY> <radius> <angle> <segment> ["nofirst"] ["nolast"]
		spacing <nr>
		repeat <deltaRadius>
			ref <deltaX> <deltaY>
			resize <angle> <segment>
	arc <refX2> <refY2> <radius2> <angle2> <segment2>
		repeat <deltaRadius2>
```

Meaning of the keywords:
- `formation <name>`: Begins a new definition for a formation with name `<name>`.
- `line <x> <y> <slots> <direction>`: Begins a line at position x,y for a line with `<slots>` positions for ships. The very first ship in the line is at exactly x,y, the line stretches into the direction of `<direction>` with ships `<spacing>` distance apart. The last ship in the line is at `<slot> * <spacing>` distance from the first.
- `spacing <nr>`: The amount of space between ships on a line. This number is in "ship sizes", so 2 is twice the `max(width,lenght)` of a ship.
- `repeat <deltaX> <deltaY>`: The location where to repeat the line if the formation needs to grow (relative to x,y of previous repeated line).
- `increase <nr>`: The amount of extra slots to add to the line on each grow step.

(The following keywords are not implemented in https://github.com/endless-sky/endless-sky/pull/4471, but given as possible future extentions)
- `arc <refX> <refY> <radius> <angle> <segment>`: Specify partial or full circle segment. `<refX>` and `<refY>` specify the anchor/reference point for the arc.
    `<radius>` specifies the radius for the arc.
    `<angle>` specifies the direction to the start point of the arc angle (0 to 360)
    `<segment>` (-360 to 360) specifies how much of a circle is used; 0 is single formation position, 90 is a quarter circle, 360 is a full circle, an positive segment is clockwise, a negative segment is counter-clockwise.
    `"nofirst"`: Don't fill the first position (starting position) of the arc. Usefull if another line or arc fills that position.
    `"nolast"`: Don't fill the last position (end position) of the arc. Usefull if another line or arc fills that position.
- `spacing <nr>`: Specifies the minimum amount of spacing between ships on the arc. Ship positions are evenly spaced among the arc, with at least the minimum as given here.
- `repeat <deltaRadius>`: Specifies the repeat vector among which this arc is repeated.
- `ref <deltaX> <deltaY>`: Modifications for the anchor point when repeating
- `resize <angle> <segment>`: Modifications for the angle and segment sections when repeating

(The following keywords are not implemented in https://github.com/endless-sky/endless-sky/pull/4471, but given as possible future extentions as requested in https://github.com/endless-sky/endless-sky/issues/4603)
- `symmetry ["transverse"] ["longitudinal"]`: Indicates if a formation should be considered symmetric. Symmetric formations allow for nicer and more desired behaviour on turns.
- `rotational <nr>`: Indicates rotational symmetry (nr is in range 0 to 360). Rotational symetric formations don't need to turn fully to become aligned with the leadship/reference again.



### Ship specific keywords

The additional keyword on a ship is:
```
ship
	formation <name>
		ring <nr>
		reference center
		reference ship <name>
		reference ships <class>
		reference mission
		reference planet <name>
		reference planets <nr>
```

Actual formation positions are determined by the order in which ships are in the ships-list and only applies to ships in the system. If formation-flying ships from outside the system join, then they take their relative positions causing other ships already in formation to move to their new relative positions.

Meaning of the keywords
- `formation <name>`: Name of the formation to form (around the lead-ship)

(The following keywords are not implemented in https://github.com/endless-sky/endless-sky/pull/4471, but given as possible future extentions)
- `ring <nr>`: Area in the formation where the ship should appear (higher numbers is more outwards on most formations).
- `reference <...>`: Reference keyword to form the formation round something else than the ships leader. Multiple reference keywords can be given, if multiple are given, then the ships for a formation are split and spread over the different targets to form a formation around.
- `reference center`: Form the formation around the center of the system.
- `reference ship <name>`: Form the formation around the ship with name `<name>`.
- `reference ships <class>`: Form formation around the ships with class `<class>`. Can be given multiple times, for example for `Transport` and `Heavy Freighter` to form protective formations around transportships.
- `reference mission`: Form formation around mission (NPC) ships. Ships are evenly distributed around mission ships.
- `reference planet <name>`: Form formation around planet with name `<name>`. Not to be used by players, but can be usefull for defense fleets around a planet. Note that planets need to have a `direction` to make this option work nicely. Direction can be random (easy) or based on orbit around the star (a little bit more complex).
- `reference planets <nr1> [<nr2>]`: Form formation around the `<nr1>`th planet in the system. If two numbers are given, then form formations around all planets in the range `<nr1>-<nr2>`. If <nr2> is -1, then form formations around the <nr1>th planet up to the last planet in the system.
- `reference wormholes <nr1> [<nr2>]`: Form formation around the `<nr1>`th wormhole in the system, similar to planets (with ranges). This allows for NPC pirates to form an ambush around a wormhole.



## Examples
For examples of formation defintions see formations.txt in https://github.com/endless-sky/endless-sky/pull/4471 (and provide review-feedback on those directly there).

The screenshots and movie in that PR also show how the formations will look in-game.
