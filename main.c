/*
  This is a simple example in C of using the rich presence API asyncronously.
*/

#define _CRT_SECURE_NO_WARNINGS /* thanks Microsoft */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "include/discord-rpc.h"
#include "include/log.h"

#define RICHPRES_INPUT_PROMPT "rich_pres_bitch$>> "

static const char* APPLICATION_ID = "381973222847152139";
static int FrustrationLevel = 0;
static int64_t StartTime;

static char presence_state[256];

static int prompt(char* line, size_t size)
{
  int res;
  char* nl;
  printf("\nrich_pres_bitch> ");
  fflush(stdout);
  res = fgets(line, (int)size, stdin) ? 1 : 0;
  line[size - 1] = 0;
  nl = strchr(line, '\n');
  if (nl) {
    *nl = 0;
  }
  return res;
}

static void discord_update_presence()
{
  DiscordRichPresence discordPresence;
  memset(&discordPresence, 0, sizeof(discordPresence));

  discordPresence.details = presence_state;
  discordPresence.startTimestamp = StartTime;
  discordPresence.endTimestamp = time(0) + 5 * 60;

  discordPresence.largeImageKey = "ava1";
  discordPresence.smallImageKey = "kt1";

  discordPresence.state = "gay @ discord";
  discordPresence.partyId = "party1234";
  discordPresence.partySize = 1;
  discordPresence.partyMax = 1;

  discordPresence.matchSecret = "xyzzy";
  discordPresence.joinSecret = "join_the_meme";
  discordPresence.spectateSecret = "look_the_meme";

  discordPresence.instance = 0;

  LOG_INFO("d:rich_pres", "updating");
  Discord_UpdatePresence(&discordPresence);
}

static void handleDiscordReady()
{
  printf("\nDiscord: ready\n");
}

static void handleDiscordDisconnected(int errcode, const char* message)
{
  printf("\nDiscord: disconnected (%d: %s)\n", errcode, message);
}

static void handleDiscordError(int errcode, const char* message)
{
  printf("\nDiscord: error (%d: %s)\n", errcode, message);
}

static void handleDiscordJoin(const char* secret)
{
  printf("\nDiscord: join (%s)\n", secret);
}

static void handleDiscordSpectate(const char* secret)
{
  printf("\nDiscord: spectate (%s)\n", secret);
}

static void handleDiscordJoinRequest(const DiscordJoinRequest* request)
{
  int response = -1;
  char yn[4];
  printf("\nDiscord: join request from %s - %s - %s\n",
	 request->username,
	 request->avatar,
	 request->userId);
  do {
    printf("Accept? (y/n)");
    if (!prompt(yn, sizeof(yn))) {
      break;
    }

    if (!yn[0]) {
      continue;
    }

    if (yn[0] == 'y') {
      response = DISCORD_REPLY_YES;
      break;
    }

    if (yn[0] == 'n') {
      response = DISCORD_REPLY_NO;
      break;
    }
  } while (1);
  if (response != -1) {
    Discord_Respond(request->userId, response);
  }
}

static void discordInit()
{
  LOG_DEBUG("discord", "calling init");

  DiscordEventHandlers handlers;
  memset(&handlers, 0, sizeof(handlers));
  handlers.ready = handleDiscordReady;
  handlers.disconnected = handleDiscordDisconnected;
  handlers.errored = handleDiscordError;
  handlers.joinGame = handleDiscordJoin;
  handlers.spectateGame = handleDiscordSpectate;
  handlers.joinRequest = handleDiscordJoinRequest;
  Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

void str_offset(char in[], int start, int length, char dest[]){
  for(int i = 0; i < length; i++){
    // if(in[i + start] == '\n') continue;
    dest[i] = in[i + start];
  }
  dest[length] = '\0';
}

static void loop()
{
  char input[512];

  printf("how do??????\n");
  printf("u - update presence\n");
  printf("s - set state for next presence\n");
  printf("q - quit this shit\n");

  while(1){
    printf(RICHPRES_INPUT_PROMPT);
    fgets(input, sizeof(input), stdin);
    input[sizeof(input) - 1] = '\0';

    if (input[0] == 'u') {
      discord_update_presence();
    } else if (input[0] == 's') {
      str_offset(input, 2, 255, presence_state);
      printf("setting state to: '%s'\n", presence_state);
    } else if (input[0] == 'q') {
      break;
    }
  }
}

static void gameLoop()
{
  char line[512];
  char* space;

  StartTime = time(0);

  printf("ur a fucking bitch ass thot\n");
  while (prompt(line, sizeof(line))) {
    if (line[0]) {
      if (line[0] == 'q') {
	break;
      }
	
      if (line[0] == 't') {
	printf("Shutting off Discord.\n");
	Discord_Shutdown();
	continue;
      }
	
      if (line[0] == 'y') {
	printf("Reinit Discord.\n");
	discordInit();
	continue;
      }
	
      if (time(NULL) & 1) {
	printf("I don't understand that.\n");
      }
      else {
	space = strchr(line, ' ');
	if (space) {
	  *space = 0;
	}
	printf("I don't know the word \"%s\".\n", line);
      }
	
      ++FrustrationLevel;
	
      //updateDiscordPresence();
    }
      
#ifdef DISCORD_DISABLE_IO_THREAD
    Discord_UpdateConnection();
#endif
    Discord_RunCallbacks();
  }
}

int main(int argc, char* argv[])
{
  discordInit();

  //gameLoop();
  loop();

  Discord_Shutdown();
  return 0;
}
