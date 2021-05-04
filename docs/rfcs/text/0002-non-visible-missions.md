- Feature Name: Attributes to make missions non-visible in various ways.
- Affected audience: Game Developers, Plugin Authors, Content Creators
- RFC PR: [Update this after opening your PR](https://github.com/EndlessSkyCommunity/endless-sky/pull/0000)
- Relevant Issues/RFCs: {{insert links to associated documents & discussions here}}

# Summary
> In conversation regarding the use of invisible missions on Discord, Tehhowch mentioned that they believe that the `invisible` attribute for missions should be exclusively used for missions that are not visible to the player. They also mentioned that, ideally, dialogs and conversations in invisible missions should throw errors. That is to say, invisible missions should by definition not have dialogs or conversations. Currently, however, there are a number of missions in-game that use invisible simply to hide the mission from the mission list, make it unabortable, and prevent it from showing messages when it fails. These missions still have dialog and conversations, as the context of the mission is things that are happening in sequence and are missions of a sort, but for some reason would not be considered a mission, and should definitely not be something the player can abort.

Beyond this, however, are various edge cases. What if the mission writer wants a mission that does not show a "mission failed" activity message, but otherwise wants it to be visible? (such as using the complete/fail system to allow the player to go different directions in the story based on their actions.) Or what if a mission should be visible, but not abortable?

The solution to this is to add three new attributes to the mission definition that allow a mission to be specified as not visible on the mission list, not abortable, or not display the fail message while otherwise being visible, abortable, and normal fail message visibility.

# Motivation
> Adding these three attributes allows mission-writers greater flexibility in how they create missions, and in a variety of situations will reduce the need to use multiple missions to achieve a single effect. 

Likewise, these changes simplify the creation of some styles of missions, particularly ones that branch their story. By simplifying the process and reducing the need to use multiple simultaneous missions, it is hoped that the entry-bar for complex mission creation can be lowered. Conceptually, the idea that a mission goes direction A if the player does X, and goes direction B if the player does Y, is quite a simple concept. However, in the context of Endless Sky's current system, it is not a simple concept to implement.

# Detailed Design
> Quite simply, the creation of three new mission attributes:

1. `nolist` : this attribute, when present, would specify that a mission does not appear in the mission list. 
2. `noabort` : this attribute, when present, would specify that a mission cannot be aborted. Ideally, it would also mean that the "abort" button is greyed-out.
3. `nofailmessage` : this attribute, when present, would specify that a mission will not display the scroll text `mission failed` when the mission fails.
4. `nomarker` : this attribute would prevent the display of any markers on the map in relation to this mission.

# Drawbacks
> Missions created for a version of Endless Sky with these mechanics implemented would not be compatible with older versions of Endless Sky.

# Alternatives
> The current method of achieving these goals is using multiple simultaneous missions. These create complexity in how they interact, and often cause confusion because players looking at their save file see all these extra missions (some of which are failed) that they have no memory of. 

# Unresolved Questions
> Are there other attributes that could be useful for mission creation that we currently need to use multiple simultaneous missions to handle? Are they particular edge cases that we don't need to deal with? Or would we benefit from making it easier to do? For instance, the ability to query whether or not a mission NPC is active, disabled, destroyed, present in the current system, or hostile could be useful, but that isn't really a mission attribute thing.
