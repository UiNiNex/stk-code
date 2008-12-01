// $Id$
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006 SuperTuxKart-Team
//  Modelled after Supertux's configfile.h
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_USERCONFIG_HPP
#define HEADER_USERCONFIG_HPP

#define PLAYERS 4

/* The following config versions are currently used:
   0: the 0.2 release config file, without config-version number
      (so that defaults to 0)
   1: Removed singleWindowMenu, newKeyboardStyle, oldStatusDisplay,
      added config-version number
      Version 1 can read version 0 without any problems, so 
      SUPPORTED_CONFIG_VERSION is 0.
   2: Changed to SDL keyboard bindings
   3: Added username (userid was used for ALL players)
   4: Added username per player
   5: Enabled jumping, which might cause a problem with old
      config files (which have an unused entry for jump defined
      --> if a kart control for (say) player 2 uses the same key as
      jump for player 1, this problem is not noticed in 0.3, but will
      cause an undefined game action now
   6: Added stick configurations.
*/
#define CURRENT_CONFIG_VERSION   6

#include <string>
#include <vector>
#include "input.hpp"
#include "player.hpp"
#include "lisp/lisp.hpp"
#include "lisp/parser.hpp"
#include "lisp/writer.hpp"

class ActionMap;

/** Class for managing general STK user configuration data. */
class UserConfig
{
public:
    /** Stores information about joystick and gamepads. */
	class StickConfig
	{
	public:
		std::string  m_id;
		int          m_preferredIndex;
		int          m_deadzone;
		StickConfig(const std::string &id);
				
	};

private:
			
	std::vector <StickConfig *> m_stickconfigs;
			
	typedef struct 
	{
		int count;
		Input *inputs;
	} InputMapEntry;
		
    /** Filename of the user config file. */
    std::string m_filename;
	
	/** Stores the GameAction->Input mappings in a way that is suitable for
	  * quick modification of the mappings. Internally this allows multiple
	  * Input instances per GameAction but the public methods allow only one
	  * mapping.
	  * 
	  * It is named after what is put in as values.
	  */
	InputMapEntry inputMap[GA_COUNT];

    void        setFilename      ();
    int         CheckAndCreateDir();

    // Attributes which have setter/getter
    int         m_sfx;
    int         m_music;
    std::string m_warning;

	void readStickConfigs(const lisp::Lisp *);

	void writeStickConfigs(lisp::Writer *);
			
	void readPlayerInput(const lisp::Lisp *,
						 const char *,
						 KartAction ka,
						 int);

	void writePlayerInput(lisp::Writer *,
						  const char *,
						  KartAction,
						  int);


    void readInput(const lisp::Lisp *,
                   const char *,
				   GameAction);

    void writeInput(lisp::Writer *,
                    const char *,
					GameAction);
	
	
	/** Iterates through the input mapping and unsets all
	 * where the given input occurs.
	 * 
	 * This makes sure an input is not bound multiple times.
	 */
	void unsetDuplicates(GameAction, Input &);
	
	/** Creates an GameAction->Input mapping with one Input */
	void set(GameAction, Input);
	
	/** Creates an GameAction->Input mapping with two Inputs */
	void set(GameAction, Input, Input);
	
	/** Creates an GameAction->Input mapping with three Inputs */
	void set(GameAction, Input, Input, Input);
	
	/** Creates an GameAction->Input mapping with four Inputs */
	void set(GameAction, Input, Input, Input, Input);

	std::string getInputAsString(Input &);

	/** Creates an ActionMap for the GameAction values of the specified
	  * range.
	  */
	ActionMap *newActionMap(const int, const int);
	
	/** Sets the Input for the given GameAction. Includes a check for
	  * duplicates and automatic removing of the other candidate(s).
	  *
	  * For use when reading from file.
	  */
	void setInput(GameAction, Input &);

public:
    enum UC_Mode {UC_ENABLE, UC_DISABLE, UC_TEMPORARY_DISABLE};

    // Attributes that are accessed directly.
    bool        m_keyboard_debug;
    int         m_track_debug;
    bool        m_bullet_debug;
    bool        m_fullscreen;
    bool        m_no_start_screen;
    bool        m_graphical_effects;
    bool        m_display_fps;
    int         m_profile;         // Positive number: time in seconds, neg: # laps. (used to profile AI)
    bool        m_print_kart_sizes; // print all kart sizes
                                   // 0 if no profiling. Never saved in config file!
    float       m_sfx_volume;
    int         m_max_fps;
    std::string m_item_style;
    std::string m_username;
    std::string m_background_music;
    std::string m_kart_group;      //< Kart group used last
    std::string m_track_group;     //< Track group used last
    std::string m_last_track;      //< name of the last track used
    std::string m_server_address;
    int         m_server_port;
    bool        m_use_kph;
    int         m_width;
    int         m_height;
    int			m_prev_width;
    int			m_prev_height;
    bool		m_prev_windowed;
    bool		m_crashed;
    std::vector<std::string> m_blacklist_res;
    int         m_karts;
    Player      m_player[PLAYERS];
    bool        m_log_errors;

    UserConfig();
    UserConfig(const std::string& filename);
    ~UserConfig();
    void setDefaults();
    void setMusic(int m)     { m_music        =  m;        }
    void setSFX  (int m)     { m_sfx          =  m;        }
    bool doMusic() const     { return m_music == UC_ENABLE;}
    bool doSFX()   const     { return m_sfx   == UC_ENABLE;}
    void loadConfig();
    void loadConfig(const std::string& filename);
    void saveConfig();
    void saveConfig(const std::string& filename);
			
	void addStickConfig(UserConfig::StickConfig *);
			
	const std::vector<StickConfig *> *getStickConfigs() const;

			
	/** Retrieves a human readable string of the mapping for a GameAction */
    std::string getMappingAsString(GameAction);
	/** Retrieves a human readable string of the mapping for the given
	  * player and KartAction.
	  */
	std::string getMappingAsString(int, KartAction);
	
	/** Sets the Input for the given Player and KartAction. Includes a check
	  * for duplicates and automatic removing of the other candidate(s).
	  *
	  * For use when sensing input.
	  */
	void setInput(int, KartAction, Input &);
	
	/** Clears the mapping for a given Player and KartAction. */
	void clearInput(int, KartAction);
	
	bool isFixedInput(InputType, int, int, int);
    const std::string& getWarning() {return m_warning;}
    void  resetWarning() {m_warning="";}
	void  setWarning(std::string& warning) {m_warning=warning;}
    
	/** Creates ActionMap for use in menu mode. */
	ActionMap *newMenuActionMap();
	
	/** Creates ActionMap for use in ingame mode. */
	ActionMap *newIngameActionMap();
			
};


extern UserConfig *user_config;

#endif

/*EOF*/
