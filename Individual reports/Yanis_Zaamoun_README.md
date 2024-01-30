Week 1 Summary: 

As Git Leader, I made the research and implemented an efficient and safe git architecture for the project: (2 permanent branches main and develop), then for each new feature we create a temporary branch from develop or main, when the feature works (at least compiles), we can merge it into the develop branch then the main branch when we are sure it works and we want it in the game. We can then delete the temporary branch to avoid having too many.


Week 2 Summary: 
Installed SDL2 with the other members of the team and made sure everything worked. Started to watch tutorials to learn how to use it.

Week 3 Summary: 

Watching more tutorials on SDL2 / RPG development in C++. The goal is to be able to manage NPCs / enemy's behavior. Implemented a very basic Enemy Movement component to test enemy movement.

Week 4 Summary:

I Implemented the first collisions with the map for the enemy, based mostly on what has been implemented for the player. 

Improved the EnemyMovement component, Created two mods, one where the enemy moves randomly around the map (Passive mod) and one when the player gets close, where the enemy starts chasing the player (Chase mod).
Passive mod: I implemented the general movement for the enemy around the map for when the player is not around. To make them look alive, the enemies move randomly around the map. I tried to make their movement look more realistic and natural. I thus defined a time interval (I found the best one by making tests) that is longer than the update interval of the game in which the enemy will change randomly its x and y direction the values taken by x and y can be -1 or 1 but also 0 since the enemies do not have to be constantly moving, just as the player is not always moving).
Chase mod: When the player is around an enemy, it starts chasing the player. The way it works is that at each update of the game, the enemy compares its position with the one of the players and adjust his direction accordingly (which can be vertical, horizontal or diagonal, thus the vectors have to be normalized to keep a constant speed) to get closer to the player until its close enough to do an action.


Week 5 Summary: 

Improved the enemy movement so that it seems more realistic and natural, changed the organization of the Enemy Component so that it is easier to manage each different type of enemy. Started to work with Robbi on improvements of the enemy collisions so that he manages to follow the player even when there are walls in between (using small bounces off the wall so that the enemy can adjust his chasing).

Week 6 Summary: 

Created a new type of enemy: The creeper (inspired by Minecraft). The first part of the movement is the same as the generic one we implemented before (passive and chase mod with the same principle of boundedness). The difference is that instead of attacking the player when the creeper is close enough, it will start arming its explosion which happens after some seconds (leaving just enough time for the player to escape. I implemented the arming and explosion part, with a knockback (the player gets pushed by the explosion) of the player in the right direction when the explosion happens and the player is in range.

Week 7 Summary: 
I implemented the swordsman attack. The swordsman is the classic enemy that will just attack the player with a melee attack to try to kill him. During the chase mod, when the swordsman is at a fighting distance from the player, he will make an attack every two seconds. The goal was that a skilled player would be able to “dodge” the enemies’ attacks by going back after attacking him before the enemy launched its second attack or even kill him in that time interval. If the player gets hit, he will take a small amount of damage and will get pushed back (gradually again) a little bit in the direction of the attack.


Week 8 Summary:

In addition to the enemies that spawn in the game when we press the start button, I implemented an automatic spawner which is a portal from which enemies arrive on the map. To implement this feature, I created the “Spawner Component” that will automatically spawn a random enemy after a defined time interval under certain conditions (ex: the player is close enough). To avoid having too many enemies at the same time, the main condition is that each spawner keeps track of the enemies it created and only creates more only if there are less than a certain defined number of them that are active/alive. 

I then helped in the creation of the demo version (putting everything together) for the presentation.
