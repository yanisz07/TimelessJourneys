Week 1 Summary: 

As Git Leader, I made the research and implemented an efficient and safe git architecture for the project: (2 permanent branches main and develop), then for each new feature we create a temporary branch from develop or main, when the feature works (at least compiles), we can merge it into the develop branch then the main branch when we are sure it works and we want it in the game. We can then delete the temporary branch to avoid having too many.


Week 2 Summary: Installed SDL2 with the other members of the team and made sure everything worked. Started to watch tutorials to learn how to use it.

Week 3-4 Summary: Watching more tutorials on SDL2 / RPG development in C++. The goal is to be able to manage NPCs / enemy's behavior. Implemented a very basic Enemy Movement component to test enemy movement. 

Week 5 Summary: Implemented first collisions with the map for the enemy. Improved the EnemyMovement component, Created two mods, one where the enemy moves randomly around the map (Passive mod), and one when the player gets close, where the enemy starts chasing the player (Chase mod).

Week 6 Summary: Improved the enemy movement so that it seems more realistic and natural, changed the organization of the Enemy Component so that it is easier to manage each different type of enemy. Started to work with Robbi on improvements of the enemy collisions so that he manages to follow the player even when there are walls in between (using small bounces off the wall so that the enemy can adjust his chasing).

Week 7 Summary: Created a new type of enemy: The creeper (inspired by Minecraft). The first part of the movement is the same as the generic one we implemented before (passive and chase mod with the same principle of boundedness). The difference is that instead of attacking the player when the creeper is close enough, it will start arming its explosion which happens after some seconds (leaving just enough time for the player to escape. I implemented the arming and explosion part, with a knockback (the player gets pushed by the explosion) of the player in the right direction when the explosion happens and the player is in range.

