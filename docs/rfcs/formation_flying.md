# Formation flying

This is a specification for a feature to create formations and apply such formations to groups of ships (as discussed in [ES-#4438](https://github.com/endless-sky/endless-sky/issues/4438), [ES-#302](https://github.com/endless-sky/endless-sky/issues/302), [ES-#4471](https://github.com/endless-sky/endless-sky/pull/4471) and others). The formations themselves are named and specified using lines and arcs that are moved, rotated, extended and repeated. The formations are used on ships and fleets by referring to the formation-name from the ships or fleet.


## Intended users of the feature
Intended users:
- Content creators that want to create new formations.
- Scenario writers that use formations for NPC fleets in missions (by editing datafiles directly).
- Players that set formations on ships they own (in-game by using pre-defined formations).


## Scope limit
Out of scope for this specification are:
- Behaviours for when to break formations (through ship personalities or otherwise, this is a topic that deserves its own separate specification/RFC).
- The actual UI elements for players to assign ships to formations. In scope is that is should be possible to create such UI elements, but the actual UI is not in this base specification.
- An in-game (or out-of-game) graphical formations editor (as discussed in [ES-#4606](https://github.com/endless-sky/endless-sky/issues/4606)).


# Formation definitions
## Signs and axis units for formation coordinates
- Coordinates are relative to the center of the formation(pattern), with coordinate 0,0 being the center.
- Coordinate signs (plus/minus) in formation definitions follow the default Endless Sky convention;
   - a positive first coordinate (usually called X in Endless Sky) indicates the right side of a formation(pattern).
   - a negative first coordiante indicates the left side of a formation.
   - a positive second coordinate (usually called Y in Endless Sky) indicates the back of a formation(pattern).
   - a negative second coordinate indicates the front of the formation(pattern).
- Polar coordinates in formation definitions start with an angle and then have a distance.
   - Angle 0 is towards the front of the formation.
   - Angle 90 is towards the right of the formation.
- Coordinate axis lengths (both for carthesian as well as distances in polar coordinates) in formation definitions are measured in ship sizes, where 1 corresponds to the radius of the largest ship actively participating in a formation.
   - Using coordinates measured in ship sizes allows for using a single formation definition for multiple sizes of ships (fighters to city ships).


## Keywords and definitions
The basic structure of a definition of a formation in the data-files would look like:
```
formation <name>:
	symmetry [transverse] [longitudinal]
		rotational <nr#>
	line <direction#>
		start [polar] <x#> <y#> [<angle#>]
		end [polar] <x#> <y#>
		slots <nr#>
		spacing <nr#>
		repeat
			anchor [polar] <x#> <y#>
			slots <nr#>
	...
	arc <radius#>
		anchor [polar] <x#> <y#>
		start [polar] <x#> <y#> [<angle#>]
		end [polar] <x#> <y#>
		spacing <nr#>
		repeat
			anchor <x#> <y#>
			start [polar] <x#> <y#> [<angle#>]
			end [polar] <x#> <y#>
	...
```

Meaning of the keywords:
- `formation <name>`: Begins a new definition for a formation with name `<name>`.
- `symmetry [transverse] [longitudinal]`: Indicates if a formation should be considered symmetric. Symmetric formations allow for nicer and more desired behaviour on turns.
   - `rotational <nr#>`: Indicates rotational symmetry (nr is in range 0 to 360). Rotational symetric formations don't need to turn fully to become aligned with the leadship/reference again.
   - Example: A delta (triangluar) formation would have a 120 degrees rotational symmetry. It can turn 120 degrees and still have roughly the same shape as when turning 0 degrees.
   - Example: A delta (trianular) formation would also have longitudinal symmetry, it can be mirrored along the longitudinal axis and would still be roughly the same shape.
- `line`: Begins a line.
- `arc`: Begins a partial or full circle.
   - The first (starting) slot on the arc and the last slot on the arc are not filled unless the arc is a full arc of 360 degrees.
      - Formation designers can fill those first and last positions with line-segments of 1 slot if required.
   - Angle start and end positions are always interpreted clockwise from start to end (to avoid ambiguity).
- `start [polar] <x#> <y#> [<angle#>]` The location where to start a line or an arc within a formation. (Defaults are x=0, y=0 and angle=180.)
   - x and y give the coordinate in carthesian coordinates.
      - If the keyword `polar` is given, then this coordinate is given as polar coordinate with x being the angle (using the default Endless Sky conventions) and y being a distance (in ship-sizes as described above).
    - For lines this coordinate is relative to the center of the formation.
      - The very first ship in the line is at exactly x,y.
      - For repeat lines this coordinate is relative to the previous start coordinate.
   - For lines, if an angle is given then this gives the direction in which the line grows.
      - Ships are `<spacing#>` distance apart. The last ship in the line is at `<slot#> * <spacing#>` distance from the first.
      - For repeat lines, if an angle is given then this gives the direction change for repeat lines.
   - For arcs this is coordinate is relative to the anchor point for the arc.
      - For repeat arcs this coordinate contains the differences (in angle and distance) compared to the previous coordinate.
      - For repeat arcs the newly calculated coordinate is relative to the repeat anchor location.
   - For arcs, if an angle is given then this specifies the end-angle at which to stop.
      - For repeat arcs, if an angle is given then this gives the delta for the end-angle at which to stop.
- `end [polar] <x#> <y#>` The location where to end a line or an arc. (Default for an arc is to make a full circle, so start == end, default for a line is angle 180 and length based on nr of slots.)
   - For lines this is relative to the center of the formation.
      - For repeat lines this coordinate is relative to the previous end coordinate.
   - For arcs this is relative to the anchor point for the arc.
   - The keyword `polar` works the same as for the `start` keyword.
   - For arcs only the angle of the polar coordinate is used (the radius/distance was already determined based on the anchor and the start).
   - If a line has an end-coordinate and slots and spacing set, then the end coordinate is only used to determine the direction.
- `anchor [polar] <x#> <y#>` (arc only): The location of the anchor for the arc (the center of the circle if the arc were a full circle).
   - If given in a repeat section, then this gives the delta to apply to the anchor compared to the previous arc.
   - Defaults to 0,0 if not given, except for repeat sections where the default is applying the original anchor again.
- `slots <nr#> (line only)`: The amount of slots on a line. (Default is 1, meaning that the line is a single point.)
   - Or the amount of slots to increase/decrease on each growth step when given in a line repeat section.
- `spacing <nr#>`: The amount of space between slots/ships on a line or arc (measured in ship sizes as described above).
   - The default spacing is 2; so twice the radius of the largest ship in the formation.
     - If a line has an end-coordinate, slots and no spacing, then spacing is automatically calculated (instead of the default being used).
   - For lines this gives the exact spacing between slots/ships.
   - For arcs the ships/slots are evenly spaced among the arc, with at least the minimum as given here.
   - It is possible that an arc contains 0 ships if the arc is too small. This can be usefull for arcs where the repeat sections are larger than the original.
- `repeat`: Section for repeating a line or arc when the formation needs to grow. The contents of this section are applied each growth step, except for lines and arcs that reach 0 size.



# Formations usage (flying behavior)
## When to form
- Ships should go into formation when they have a formation set and when their personaly behaviour indicates to go into formation.
   - Default behaviours to go into formation are when idle or when ordered to gather.
   - Default behaviour to break the formation is when no longer idle and no longer ordered to gather.
- Ships should go into formation around the ship/object/planet/point they are assigned to (also in case of gather).
   - A single ship/object/planet/point can have multiple formations around it (possibly from different governments).
   - Ships form by default formations around their parent ship (if no specific ship/object/planet/point was given).
      - This is the players flagship for player-owner ships and the first ship in the fleet for NPC fleets.
- Formations for ships can be set for individual ships, for fleets and for governments.
   - If formations are set on multiple levels, then the most specific level applies, so a formation set on fleet level overrules a formation set on government level.


## Taking formation positions
- Positions in a formation are sequentially assigned.
   - Actual formation positions are determined by the order in which ships are in the ships-list
      - If new ships join a formation, then they take their relative positions causing other ships already in formation to move to their new later relative positions.
      - The use of the ships-list order allows players some control over which ships appear where in the formations.
   - The first ship that starts in formation starts on the first position.
   - Ships that are killed are no longer considered.
      - Ships with higher assigned positions automatically move to earlier positions of killed ships.
   - Formation members only consider other alive ships that are close to their formation positions when choosing their own position.
   - Positions are based on the formation definition and on the size of the largest ship actively participating in a formation.
      - If a larger ship enters the formation, then all positions are updated based on the larger size.
      - If the largest ship leaves a formation, then all positions are updated based on the second-largest ship.
   - Ships that are at their formation position align their facing direction with the ship/object/planet/point that leads the formation
      - If the lead has no facing direction, then the ships align with the front of the formation.


## Turning behaviour
- The front of an active formation is aligned with the movement vector of the ship/object/planet/point that the formation is formed around.
   - If the formation is around a lead ship that has a facing, then one third of a degree delta is added for every degree that the lead ship turns.
      - This is up to a maximum of 90 degrees for the lead-ship (and thus 30 degrees for the formation).
   - If the formation is formed around a ship/object that is not moving, then the front is aligned with the facing direction of the lead-ship/object.
      - Planets have an orbit specified, the facing direction could be calculated based on the orbit.
   - If the lead ship/object is not moving and not having a facing direction, then the front is in the direction of the system center.
   - If the lead ship/object is the system center, then the front is in a random direction.
- Formations have a maximum turning speed, if the front of the formation changes faster than that, then the turning of the formation lags behind.
   - The maximum turning speed is 110% of the speed that the slowest formation member would need to stay in position during a turn when the formation would not be moving.
      - This will result in some formation deformation during turns
   - Formations that have rotational symmetry should choose another suitable point in the formation as front if that results in a smaller turn.
      - An example is a delta-formation, it has 3 pointy corners, each one could function as front of the formation.
      - Another example is a hexagon-formation, it has 6 different flat fronts that could act as front.
   - Formations that are traverse or longitudinal should mirror according to their symmetry if that helps to perform the rotation faster

### Fast turning behaviour background
If the lead ships movement direction changes 180 degrees (because the lead ship is reversing course), then it would be undesired that all ships in the formation would immediately fly to their new positions, since each ship would then pass through the center of the formation (causing the formation to collapse and expand around the lead-ship).

This collapsing and re-expanding looks somewhat unprofessional and can be undesired for protective formations that are around one or more targets to be protected. If the formation is used to screen for missiles, then a single missile just after collapsing the formation can do damage to a large number of formation members (including the protected ships in the middle of the formation).


## Data format and keywords for flying

The additional keyword on a ship is:
```
ship
	formation <name>
		ring <nr#>
		reference <...> ...
```
This is used in savegames to store the formation used by the ship.


The additional keyword on a (NPC) fleet is:
```
fleet
	formation <name>
		class <shipclass>
		ring <nr#>
		reference <...> ...
	...
```
This is used when instantiating fleets. All fleet variants use this formation, unless the variant specifies another one.

The additional keyword on a (NPC) fleet-variant is:
```
fleet
	variant
		<type-name> <nr#>
			formation <name>
				ring <nr#>
				reference <...> ...
```
This is used when instantiating a specific fleet variant.

The additional keyword on a government is:
```
government
	formation <name>
		class <shipclass>
		ring <nr#>
		reference <...> ...
```
This indicates that all ships from this government should use the specified formation (with the specified starting ring) unless the NPC fleets specify otherwise.
Using reference for government would be a bit odd, unless we want to have governments where ships automatically form formations around planets.

Meaning of the keywords
- `formation <name>`: Name of the formation to form (around the lead-ship)
- `class <shipclass>`: Indicates for fleets/governments that the given formation should only be applied to the given class of ships. (TODO: attributes filter instead of class?)
- `ring <nr#>`: Area in the formation where the ship should appear (higher numbers is more outwards on most formations).
- `reference <...>`: Reference keyword to form the formation around something else than the ships leader. Multiple reference keywords can be given, if multiple are given, then the ships for a formation are split and spread over the different targets to form a formation around.
- `reference point [<x#> <y#>] [<direction#>]`: Form the formation around a specific fixed point in the system (direction indicates the formation direction). If x and y are not given, then use system center (point 0,0).
- `reference ship <name>`: Form the formation around the ship with name `<name>`.
- `reference ships <class>`: Form formation around the ships with class `<class>`. Can be given multiple times, for example for `Transport` and `Heavy Freighter` to form protective formations around transportships.
- `reference mission`: Form formation around mission (NPC) ships. Ships using this reference are evenly distributed around mission ships.
- `reference planet <name>`: Form formation around planet with name `<name>`. Not to be used by players, but can be usefull for defense fleets around a planet. Note that planets need to have a `direction` to make this option work nicely. Direction can be random (easy) or based on orbit around the star (a little bit more complex).
- `reference planets <nr1#> [<nr2#>]`: Form formation around the `<nr1>`th planet in the system. If two numbers are given, then form formations around all planets in the range `<nr1>-<nr2>`. If `<nr2>` is -1, then form formations around the `<nr1>`th planet up to the last planet in the system.
- `reference wormholes <nr1#> [<nr2#>]`: Form formation around the `<nr1>`th wormhole in the system, similar to planets (with ranges). This allows for NPC pirates to form an ambush around a wormhole.

# Examples
## External examples
For examples of formation defintions see formations.txt in [ES-#4471](https://github.com/endless-sky/endless-sky/pull/4471). The screenshots and movie in that PR also show how the formations will look in-game.
