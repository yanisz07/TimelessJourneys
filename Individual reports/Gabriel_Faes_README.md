Gabriel Faes Timeless Journeys Contribution
13th November 2023
Initial week of the project. I helped set up the project as a whole, divided the project into 3 main categories (Game Design, Graphics, and Game Programming), had everyone decide which roles they were and wrote it in the Word document. Game programming was then initially split by me into 3 categories, but based on general discussion we decided to merge two of the categories into one, leaving us with “Character Programming” (which was originally Player programming and Enemy programming as two separate categories) and “Core Programming”. I explained (or tried to explain at least, the organization of the project as a whole still needs a lot of work) each category. 
•	Game Design would focus on how the game would work, would write game design documents and would explain to the team how the game should be behaving as a general rule. They would make sure the balance of the game is right (by changing or coding C++ files), help or do the level design (in C++) and would supplement the programming team with C++ coding, specifically after the initial weeks of the project (where in that time they would be working mostly on the game design).
•	The Graphics team would draw everything and come up with an art style. Obviously, pixel art was the most likely, but other choices could have been possible. They would then animate player characters and enemy characters as well as the different weapons and draw the world decorations and tiling and UI elements which were more complex graphically. They would write (or modify) C++ files to implement the sprites and animations into the game, and would help the programming team with C++ coding, specifically after the initial weeks of the project (where in that time they would be working mostly on the graphics).
•	Game Programming would do all the coding work, including the set up of the coding part of the project as a whole (dependencies, architecture of the classes, files and folders, etc.). After the initial set-up is done and a basic block could be moved on a map with the arrow keys, they would move on to reading the game design documents and coding the game as a whole. This second part would be supplemented by the Game Design and Graphics team where needed, especially since towards the end of project, time will be of the essence, and I foreshadow we will be very much behind what we originally planned.
  o	Character Programming would focus on player and enemy movements and attacks.
  o	Core Programming would focus on the rest of the game, including inventory and level system, doors and maps, and any UI elements.
We decided on the main themes of the game and how it was all going to work, including making sure we all were on the same page as to the overall game (top-down view, RPG real time hack and slash so no battle turns, etc.).
 
20th November 2023
Second week. I (and a few others) made sure everyone understands what their role is for the overall project. I devised tasks for the upcoming week and discussed them with the leaders of each group to make sure everything was right, and then told everyone what they should be doing for the upcoming week:
•	For all: Have everyone on GitHub, use Trello and understand SDL2. 
•	Game Programming: Figure out cpp file architecture.
•	Character Programming: Make a cube move on a map with potential attack prototypes (sword on a melee range and crossbow on a limited range).
•	Core Programming: Make a prototype of an inventory system with objects which have statistics that show up (unknown inventory mechanics yet).
•	Graphics: Figure out the graphical style and point of view of camera. Prototype of ground and decorative features as well as player and enemies.
•	Game Design: Figure out player mechanics and interactions with the world and enemies. Inventory and player statistics system. Figure out overarching story.
During this week I start writing the main design document, and discuss with other game designers what their own plans should be. Lila and Hiba would be working on the inventory system and the player levels (we would later add the fact they would also be working on the different items in-game). Hadrien would be working on the player movements and different weapons, as well as enemy generation and potential bosses (he came back later with great drawings of the different possibilities and some good ideas!). 
I came up with the unique game mechanic of the “time loop”. The player would have a limited time to complete the era they were in. The music for the era would match the amount of time the player has, going up in intensity as the time remaining gets shorter. I didn’t write much about it in my game design document yet, not knowing exactly how it worked at the time, but I discussed it with the other game designers and other people in the project on the next Monday meeting.
I also wrote a small two page “contract” if ever the development of the game were to continue after the project. It mentions how development would continue (who can continue it mainly) and, if ever it would get published, the revenue share of the people working on it now.
The story of the game was also loosely worked on. A big bad guy has put bosses in the eras to shatter the timeline (for some reason, still need to figure out why). The hero has a time machine (of some sorts) and travels to these eras to get rid of the bosses and recover the timeline. Some of the bosses add their own monsters in each era, others have the big bad guy add the monsters, and even others have monsters which are just part of the era they are normally in (human soldiers for example).
 
27th November 2023
Third week. On the Monday meeting, I discussed with the other game designers what we had respectively worked on. We settled to have the controls match those of other popular games with a keyboard only approach. The arrow keys would move the player while Z, X, and C (as well as the spacebar, ctrl and shift) would serve as the attack and general action buttons. 
Hadrien came up with an idea to have some kind of ability related to time. I hadn’t discussed my idea of the time loop yet, so I introduced it and we though it’d be great if the player could slow down time. The music would slow down/stop and all enemies would be frozen for a limited amount of time, allowing the player more time in general, and free damage against the enemies (or free exploration and running around). 
I also came up (while discussing with people) with the fact that the player had to return to the spawn location of the era before the timer elapsed, or they would die and lose everything they had on them. This would also apply if they simply lost all their HP (they would lose all their weapons, armors, items, they had on them). The player would now have the possibility of leaving some of their loot back in a chest at the time machine, which would be safe if ever they would die, but obviously at the drawback of being unable to use it during a run.
At that meeting, I also discussed with everyone the plans for the project now that we had a rough idea at the game designer’s table. We would be focusing on only one era (instead of 3 or more) for the project; more eras would simply bring more problems in the limited time we have.
I explained the rough idea of the story (it was quite vague anyway, so nobody objected) and then explained the mechanic of the time loop which would make everything click together. Interestingly enough, this had more backlash than I anticipated, especially when I explained that dying or running out of time would mean the player loses everything they had in their inventory. We had an interesting debate but we settled on the idea I had come up with in the end.
I reiterated the tasks everyone should be doing (mainly setting up SDL2 since that had still not been done) and tried to help a little with file architecture with little success (as it turns out a week later, only one of the folders I had requested had been made), although SDL2 had apparently been successfully installed on Mac, but not on Windows :,(.
During the next week I continued working on the game design document, implementing the new time loop mechanic mainly and explaining the bulk of the gameplay. Explaining how the tutorial would work (which would probably not be coded during project time unless we find we have some time left), and coming up with an AI which would help the player during their adventure. I also wrote down the fact that some places could be added as “safe zones” and would enable the player to come back to the safe zone instead of the spawn location when time runs out (they would not lose any gear, just like if they had come back to the spawn location).
 
4th December 2023
Fourth week. The week was mostly spent working on everyone getting the code, library and dependencies in order and working (including myself). Only a few people were left with SDL2 not working. Focusing mainly on making it work for everyone, I didn’t work nearly as much on game design. It was a good realization that time was ticking away faster than I was expecting, so the plans would need to be adjusted for next week.
So far, we were thinking on doing one “era” (initially it was 3!) with a couple of monsters and a boss as well as a whole level design. While I still think this is possible, we’ll be tight on time by the end of the project. Hopefully by next week we can begin coding the actual game so we’re not too far behind. The new “time loop” mechanic should hopefully not take too much time to code, I’m more concerned about the enemy programming, but we’ll only code those if we have time.

11th December 2023
Fifth week. Spent a little time making sure the last few people had the game working (some other people worked a lot more than me on that) on both PC and Mac (luckily no Linux). Someone merged directly to main so that was fun to fix as well. We had to remake the develop branch as well since the feature didn’t work on PC but did on Mac and someone pushed there. In the end it all worked out, a few more features were added to the (new) develop branch and we merged them into main.
We had a meeting about how everyone should be working more. People disagreed with how much we should be working and the implementation of the project. We settled on things working out during the vacation and keeping the project as is because it would take too much time to refactor the whole thing. Finally, I added some Trello cards for each team so that they knew approximately what they should be doing during the next week and vacation.
•	Graphics would be working on the collision box and sprite of the player, as it currently was a square, but the sprite was a long vertical rectangle. They would also be working on making the sprites for a Greek themed era, as well as starting the code to implement the map itself into the game.
•	Character programming would be working on attacks and enemies essentially. They would code attack patterns, some melee, some range, and then some enemies. The enemies for now would be pretty basic, but if we had some time, we would code them to be more complete.
•	Core programming would be working on an inventory system and UI, so we can display the health of the player in a nice way along with more elements which could reasonably fit and help the player during the game. I’d like to make the inventory system not pause the game, but we’ll see how that goes in terms of implementation.
•	Finally, I would be working on the player statistics (when the player levels up), the time loop mechanic, and when the player dies (either by the time loop, or by reaching 0 HP).

18th December 2023
Sixth week and vacation. The week and vacation were a little more hectic for the project, but we managed to make some good progress as well. I coded the player and enemy statistics during this time. I tried doing a character class where the player and enemy would inherit from, but the code was so static and the dependencies (include header) of each file was so confusing it kept giving me errors (I either didn’t understand, or it was simply impossible to make a subclass from any class of the component file directly), and after some fiddling around, I gave up and started again, this time directly in the statistics class.
I added a check to see if the element was a player or enemy, and if it was the player, it would allow for more functions and variables, otherwise essentially only damage and health would be available. I then cleaned up the code a little, added some comments and added “experience” that the player could get, which would make them level up after a certain amount of experience had been obtained.
Lastly, we “published” (me and Hadrien) the game design document to give a rough idea as to how the project should shape itself during the vacation. Hadrien worked on the player and enemy movements and attacks, while I worked more on the gameplay and how the world would work. I was aware that not everything would make it in the project, but as much as possible was the objective.

8th January 2024
2nd to last week. 