Week 1 Summary: 

As Git Leader, I made the research and implemented an efficient and safe git architecture for the project: (2 permanent branches main and develop), then for each new feature we create a temporary branch from develop or main, when the feature works (at least compiles), we can merge it into the develop branch then the main branch when we are sure it works and we want it in the game. We can then delete the temporary branch to avoid having too many.


Week 2 Summary: Installed SDL2 with the other members of the team and made sure everything worked. Started to watch tutorials to learn how to use it.

Week 3-4 Summary: Watching more tutorials on SDL2 / RPG development in C++. The goal is to be able to manage NPCs / enemy's behavior. Implemented a very basic Enemy Movement component to test enemy movement. 

Week 5 Summary: Implemented collisions with the map for the enemy. Improved the EnemyMovement component, Created two mods, one where the enemy moves randomly around the map (Passive mod), and one when the player gets close, where the enemy starts chasing the player (Chase mod). For now, there is only one enemy, but when the base is finished it will be easy to add many more with different behaviors.
