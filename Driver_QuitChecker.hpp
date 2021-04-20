#pragma once
#ifndef WORLDSIM_QUIT_CHECKER_HPP
#define WORLDSIM_QUIT_CHECKER_HPP

/* This object exploits the c++ guarantee that the destructor is always called, in order to deal with unanticipated shutdowns, such as the player clicking the X. However, it seems the destructor guarantee does not apply in some cases, such as ending the process using the task manager, or using ctrl+c from the console.

It seems that threads are killed if the user uses CTRL+C on the cmd line however, which messes things up a bit.

*/
class QuitChecker
{
   private:
   Timer gameTime;

   public:
   QuitChecker()
   {
      gameTime.init();
      gameTime.start();

      // For now we will clear the savedata on startup because it can cause some instability.
      std::cout<<"Deleting temporary data folder ("<<globalSettings.SAVE_FOLDER_PATH<<")\n";
      if (globalSettings.SAVE_FOLDER_PATH.length() > 0 )
      {
         FileManager::deleteDirectory(globalSettings.SAVE_FOLDER_PATH,true);
      }
   }
   ~QuitChecker()
   {
      QUIT_FLAG=true;
      std::cout<<"Waiting to shutdown.\n";
#ifdef WILDCAT_THREADING
      std::unique_lock lock(MUTEX_SHUTDOWN); // wait until threads are not doing anything critical.
#endif
      std::cout<<"Shutting down.\n";
      gameTime.update();
      if (gameTime.seconds > 10 )
      {
         std::cout<<"Time played: "<<gameTime.seconds/60<<" minutes.\n";
      }

      if (CLEAN_SAVES_ON_EXIT)
      {
         std::cout<<"Deleting temporary data folder ("<<globalSettings.SAVE_FOLDER_PATH<<")\n";
         if (globalSettings.SAVE_FOLDER_PATH.length() > 0 )
         {
            FileManager::deleteDirectory(globalSettings.SAVE_FOLDER_PATH,true);
         }
      }
   }
};

#endif
