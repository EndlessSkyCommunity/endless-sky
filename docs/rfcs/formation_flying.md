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
## Signs and axis units for formation position coordinates.
- (x,y) coordinates are relative to the ship or other object around which a formation gets formed. Coordinate 0,0 would refer to the center of the ship or object around which the formation gets formed.
   - positive x is towards the right side of the ship or object around which the formation gets formed and positive y being towards the front of the ship or object around which the formation gets formed.
- (angle, distance) polar coordinates have angle 0 facing towards the front of the ship or object around which the formation gets formed and angle 90 would be towards the right side of the ship or object around which the formation gets formed.
- Coordinate (x,y and distance) axises are by default in pixels, but there are also keywords that allows the radius, the width and/or the height of the largest ship participating in the formation to be used as axis unit.
   - Using coordinates measured in ship sizes allows for using a single formation definition for multiple sizes of ships (fighters to city ships).


## Keywords and definitions
The basic structure of a definition of a formation in the data-files would look like:
```
formation <name>:
	flippable [x] [y]
	rotatable <angle#>
	line
		start [polar] [radius|width|height] <x#> <y#>
		end [polar] [radius|width|height] <x#> <y#>
		slots <nr#>
		skip [first] [last]
		centered
		repeat
			start [polar] [radius|width|height] <x#> <y#>
			end [polar] [radius|width|height] <x#> <y#>
			angle [<angle#>]
			slots <nr#>
				regulated
			alternating
			originating
			extending
		...
	...
	arc
		anchor [polar] [radius|width|height] <x#> <y#>
		start [polar] [radius|width|height] <x#> <y#>
		angle [<angle#>]
		slots <nr#>
		skip [first] [last]
		centered
		repeat
			anchor [polar] [radius|width|height] <x#> <y#>
			start [polar] [radius|width|height] <x#> <y#>
			angle [<angle#>]
			slots <nr#>
				regulated
			alternating
			originating
			extending
		...
	...
```

Meaning of the keywords:
- `formation <name>`: Begins a new definition for a formation with name `<name>`.
- `flippable [x] [y]`: Indicates if a formation can be mirrored/flipped along axises through the center and still be considered the same valid formation.
   - `x`: Indicates that a formation can be flipped/mirrored along the x(/horizontal/transverse)-axis and still be the same valid formation.
   - `y`: Indicates that a formation can be flipped/mirrored along the y(/vertical/longitudinal)-axis and still be the same valid formation.
   - Example: A delta (triangular) formation would have longitudinal symmetry, it can be flipped/mirrored along the longitudinal axis (`flippable y`) and would still be roughly the same shape.
- `rotatable <angle#>`: Indicates if a formation can be rotated and still be the same valid formation. (angle is in rage of 0 to 360 degrees)
   - Rotatable formations don't need to turn fully to become aligned with the leadship/reference again. This allows for nicer, more desired and more elegant behavior when the lead ship makes large changes in heading/velocity.  
   - Example: A delta (triangluar) formation around the lead-ship would have a 120 degrees rotational symmetry and be set to 120 degrees rotatable. It can turn 120 degrees and still have roughly the same shape as when turning 0 degrees.
   - Example: A delta-tailing (triangle behind the flagship) formation should however only be considered `transverse` symmetric, since most players would like the tailing formations to only be behind them.
   - Example: A formation that "writes text by forming letters and words" could be rotatable for 1 degree or less, basically causing the heading/velocity of the lead ship to be ignored and ships just staying in the "text positions".
- `line`: Begins a line.
   - `start [polar] [radius|width|height] <x#> <y#>` The location where to start a line within a formation. (The default value is x=0 and y=0.)
      - x and y give the coordinate in carthesian coordinates in pixels.
         - If the keyword `polar` is given, then this coordinate is given as polar coordinate with x being the angle (0 to 360 degrees) and y being a distance.
         - If the keyword `radius` is given, then this coordinate is not in pixels, but in radiusses of the largest ship in the formation.
         - If the keyword `width` is given, then this coordiate is not in pixels, but in widths of the largest ship in the formation.
         - If the keyword `height` is given, then this coordinate is not in pixels, butin heights of the largest ship in the formation.
      - For the first ring lines this coordinate is relative to the center of the formation.
      - For repeat ring lines this coordinate is relative to the start coordinate of the previous ring.
      - If the start keyword is given multiple times, then the values will be added.
         - This can be usefull if the coordinates should be calculated partially based on pixels and partially on ship sizes.
         - This can also be usefull if the x-coordinates should depend on ships-width and y-coordinates on ships-height.
   - `end [polar] [radius|width|height] <x#> <y#>` The location where to end a line. (Default is angle 180 and length based on nr of slots.)
      - If a line has an end-coordinate and slots and spacing set, then the end coordinate is only used to determine the direction.
      - For lines this is relative to the center of the formation.
         - For repeat lines this coordinate is relative to the previous end coordinate.
      - The keywords `polar`, `radius`, `width` and `height` work the same as for the `start` keyword.
   - `slots <nr#>`: The amount of slots on a line. (Default is 1, meaning that the line is just a single point on the start location.)
      - Or the amount of slots to increase/decrease on each growth step when given in a line repeat section.
      - Ships/slots are distributed evenly over the line between the start and the end coordiate.
      - The keyword `regulated` indicates that the slots increase will only happen when the resulting space between ships is at least the same amount as on the original line.
   - `skip [first] [last]` Indicates if the first and/or last slot in the line needs to be skipped.
   - `centered` Indicates that the line grows from the center instead of the start position.
   - `repeat`: Section for repeating a line when the formation needs to grow.
      - Repeat lines that reach a size of 0 will not repeat further.
      - The keyword 'alternating' indicates that every 1st, 3rd, and every other uneven repeating line will fill from end to start instead of from start to end.
         - This 'alternating' keyword can be combined with centered and will still be relevant for some regular formations when ships move to earlier positions.
      - A single line can have multiple repeat sections.
      - The keyword `originating` indicates that all positions added in the repeat section should be placed before the start position of the original line.
         - This effectively makes the line longer in front of the the start position.
         - This is mostly relevant for centered lines, where the repeat section moves the center position.
         - This can also help in giving a formation nicer behavior when ships move from later positions to earlier positions.
      - The keyword `extending` works the same as originating, but adds the repeat section after the last position of the original line (but before any later lines).

- `arc`: Begins a partial or full circle.
   - Angle start and end positions are always interpreted clockwise from start to end (to avoid ambiguity).
   - `start [polar] [radius|width|height] <x#> <y#>` The location where to start an arc within a formation.
      - This is coordinate is relative to the anchor point for the arc and provides the angle as well as the radius.
        - For repeat arcs this coordinate contains the differences (in angle and distance) compared to the previous coordinate.
        - For repeat arcs the newly calculated coordinate is relative to the repeat anchor location.
   - `angle [<angle#>]` Gives the partial (or full) angle at which to stop the arc. (range is -360 to 360)
      - Default is zero, meaning that the arc statement describes only a single point.
      - The arc is clockwise if the number is in the range between 0 and 360.
      - The arc is counter-clockwise if the number is in the range between -360 and 0.
      - For repeat arcs, if an angle is given then this gives the delta for the end-angle at which to stop.
   - `anchor [polar] [radius|width|height] <x#> <y#>`: The location of the anchor for the arc (the center of the circle if the arc were a full circle).
      - If given in a repeat section, then this gives the delta to apply to the anchor compared to the previous arc.
      - Defaults to 0,0 if not given, except for repeat sections where the default is applying the original anchor again.
   - `slots <nr#>`: The amount of slots on an arc. (Default is 1, meaning that the arc is a single point at the start location.)
      - Or the amount of slots to increase/decrease on each growth step when given in an arc repeat section.
      - Ships/slots are distributed evenly over the arc between the start and the end coordiate.
      - The keyword `regulated` indicates that the slots increase will only happen when the resulting space between ships is at least the same amount as on the original line.
   - `skip [first] [last]` Indicates if the first and/or last slot in the arc needs to be skipped.
   - `centered` Indicates that the arc grows from the center instead of the start position.
   - `repeat`: Section for repeating an arc when the formation needs to grow.
      - Repeat arcs that reach a size of 0 will not repeat further.
      - The keyword 'alternating' indicates that every uneven repeating arc will fill from end to start instead of from start to end, similar to this keyword on lines.
      - A single arc can have multiple repeat sections.
      - The keywords `originating` and `extending` work the same as for lines.



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
- Ships can have a formation-pattern and a formation-ring set to indicate their position in the formation.
   - If a ring is set, then the ship will not appear in the formation before that ring. (But it can appear after it if other ships fill the ring.)
- Positions in a formation are furthermore sequentially assigned.
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
   - If the formation is around a lead ship that has a facing/heading different from its velocity/movement-vector, then one third of a degree delta is added for every degree that the lead ships heading mismatches.
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

The additional keyword on a (NPC) fleet is:
```
fleet
	formation <name>
		filter
			<...>
		ring <nr#>
		instance <nr#>
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
				instance <nr#>
				reference <...> ...
```
This is used when instantiating a specific fleet variant.

The additional keyword on a government is:
```
government
	formation <name>
		filter
			<...>
		ring <nr#>
		instance <nr#>
		reference <...> ...
```
This indicates that all ships from this government should use the specified formation (with the specified starting ring) unless the NPC fleets specify otherwise.
Using reference for government would be a bit odd, unless we want to have governments where ships automatically form formations around planets.

Meaning of the keywords
- `formation <name>`: Name of the formation to form (around the lead-ship)
- `filter`: Gives filter criteria to select only ships that match the filter criteria (filtering on things like attributes, ship-class and specific outfits) according to locationFilter syntax.
- `ring <nr#>`: Area in the formation where the ship should appear (higher numbers is more outwards on most formations).
- `instance <nr#>`: It is possible that multiple instances of a single formation exists around a single leader. For example a delta formation for small ships and one for large ships.
     The instance number indicates which of the instances should be joined by the ships affected by the formation specification.
- `reference <...>`: Reference keyword to form the formation around something else than the ships leader. Multiple reference keywords can be given, if multiple are given, then the ships for a formation are split and spread over the different targets to form a formation around.
- `reference point [<x#> <y#>] [<direction#>]`: Form the formation around a specific fixed point in the system (direction indicates the formation direction). If x and y are not given, then use system center (point 0,0).
- `reference ship <name>`: Form the formation around the ship with name `<name>`.
- `reference ships <filter ...>`: Form formation around the ships which match filter criteria according to locationFilter syntax.
- `reference object [<system-name>] <name>`: Form formation around a planet, spacestation or wormhole with name '<name>'. System-name is optional and can be given if it is required to uniquely identify an object. Not to be used by players, but can be usefull for defense fleets around a planet or for NPC pirates forming an ambush around a wormhole. Note that objects need to have a `direction` to make this option work nicely. Direction can be random (easy) or based on orbit around the star (a little bit more complex).

# Examples
## External examples
For examples of formation defintions see formations.txt in [ES-#4471](https://github.com/endless-sky/endless-sky/pull/4471). The screenshots and movie in that PR also show how the formations will look in-game.
