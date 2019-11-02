# Hyperspace and Jump arrival and departure modifiers

This is a proposal for a list of keywords for the feature requested in https://github.com/endless-sky/endless-sky/issues/4334 and for possible follow-up features if we can foresee them.

## System and Planet Arrival/departure

```arrival <distance> ["link"] ["jump"]```
System property and/or planet property. The <distance> indicates the minimum amount of distance (from respectively the system center or the planets center) at which ships arrive when entering the system.
If the property is given on multiple places, then the maximal distance is used.
If link keyword is given, then the arrival distance only applies to incomming hyperlinks.
If jump keyword is given, then the arrival distance only applies to incomming jumps.

```
departure <min> [<max>] ["link"] ["jump"]
    angle <nr>
```

System property and/or planet property. The <min> indicates the minimum distance that a ships needs to be away from respectively the system center or planet center before the ship can start a jump.
If a maxium is given, then a ship needs to be no more than distance away from the centers to be able to leave.
If an angle is given, then the ship needs to be at the proper distance, but also in the proper direction of leaving (within the range given by angle).
If multiple properties are given, then all departure constraints need to be fullfilled before a ship can make the jump.
Content creators need to be aware of blocking jumps if constraints are conflicting, they might also acutally want to use this, for example by having a system where jumps are only possible once every four days (due to planet alignments).


## Ship Arrival/departure modifiers

```
arrival <min> ["link"] ["jump"]
departure <min> ["link"] ["jump"]
```
Ships properties. <min> gives the minimum arrival and departure distances for ships. If a system or planet also has minimum arrival and departure distances then the maximum of all distances will be used for the actual distance.
I considered to also allow ship properties to override (or modify by a multiplier) the planet and system properties, but I expect that we will not want to do that in the end. If some content creator has a good reason not to allow jumps within range of a certain planet, then I expect the story immersion to be more imporant than being able to override the ranges.
(And if overriding by explicit number or modifier is very much desired, then it can just be added using this keyword anyway.)

## Combat jump behaviours

```
combat visibility
```
(System property)
Allow ships to arrive at the most appropriate distance for combat (based on their personality).
When this is false, then all ships will arrive as if the system has no combat.
When this is true, then fleets with "heroic" and other military personalities could arrive just beyond their maximum weapons range (allowing the long-range weapons to be used before short-range weapons are used).
When this is true, then unarmed carriers could arrive outside weapons ranges whenever possible.
When this is true, then fleets with merchant type of personalities could arrive at a large distance from the combat area (trying to avoid combat), based on their personalities.


```
combat detection <value> ["link"] ["jump"]
```
(Ship property)
The ability (strength) of a ship to detect combat and combatants (in general or for hyperlink or jump only). Related to the `combat visibility` system property.


```
arrive focusable
```
(Planet property)
Allow automatically aiming for this planet when arriving in the system.
