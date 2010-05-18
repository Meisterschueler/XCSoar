/* Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009

	M Roberts (original release)
	Robin Birch <robinb@ruffnready.co.uk>
	Samuel Gisiger <samuel.gisiger@triadis.ch>
	Jeff Goodenough <jeff@enborne.f2s.com>
	Alastair Harrison <aharrison@magic.force9.co.uk>
	Scott Penrose <scottp@dd.com.au>
	John Wharington <jwharington@gmail.com>
	Lars H <lars_hn@hotmail.com>
	Rob Dunning <rob@raspberryridgesheepfarm.com>
	Russell King <rmk@arm.linux.org.uk>
	Paolo Ventafridda <coolwind@email.it>
	Tobias Lohner <tobias@lohner-net.de>
	Mirek Jezek <mjezek@ipplc.cz>
	Max Kellermann <max@duempel.org>
	Tobias Bieniek <tobias.bieniek@gmx.de>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
 */
#ifndef WAYPOINT_HPP
#define WAYPOINT_HPP

#include "Util/tstring.hpp"
#include "Navigation/GeoPoint.hpp"
#ifdef DO_PRINT
#include <iostream>
#endif

#include "Util/GenericVisitor.hpp"

class TaskProjection;

/** 
 * Bitfield structure for Waypoint capabilities
 * Several of these capabilities are not used by XCSoar, but are
 * present for compatibility
 */
struct WaypointFlags {
  bool Airport:1;      /**< If waypoint is an airport/airfield */
  bool TurnPoint:1;    /**< If waypoint can be used as a turnpoint */
  bool LandPoint:1;    /**< If waypoint can be landed at */
  bool Home:1;         /**< If waypoint is to be used as home */
  bool StartPoint:1;   /**< If waypoint is marked as a potential start point */
  bool FinishPoint:1;  /**< If waypoint is marked as a potential finish point */
  bool Restricted:1;   /**< If waypoint is marked for restricted access (unused?) */
  bool WaypointFlag:1; /**< ??? */

/** 
 * Set default flags (all off except turnpoint)
 * 
 * @param turnpoint Whether the waypoint is a turnpoint
 */
  void setDefaultFlags(bool turnpoint);
};

/**
 * Class for waypoints.  
 * This is small enough currently to be used with local copies (e.g. in a TaskPoint),
 * but this may change if we include airfield details inside.
 *
 * @todo
 * - consider having a static factory method provide the ID automatically
 *   so we know they will be unique.
 */
class Waypoint: 
  public BaseConstVisitable<>
{
public:
  friend class Serialiser;

  /**
   * Constructor for real waypoints
   *
   * @param is_turnpoint Whether newly created waypoint is a turnpoint
   * @return Uninitialised object
   */
  Waypoint(const bool is_turnpoint= false);

  unsigned id; /**< Unique id */
  GEOPOINT Location; /**< Geodetic location */
  fixed Altitude; /**< Height AMSL (m) of waypoint terrain */
  WaypointFlags Flags; /**< Flag types of this waypoint */
  int Zoom; /**< Minimum zoom level this waypoint is visible at */
  int FileNum; /**< File number to store waypoint in (0,1), -1 to delete/ignore */
  tstring Name; /**< Name of waypoint */
  tstring Comment; /**< Additional comment text for waypoint */
  tstring Details; /**< Airfield or additional (long) details */

  /** 
   * Determine if waypoint is marked as able to be landed at
   * 
   * @return True if waypoint is landable
   */
  bool
  is_landable() const
  {
    return Flags.LandPoint || Flags.Airport;
  }

  /**
   * Equality operator (by id)
   * 
   * @param wp Waypoint object to match against
   *
   * @return true if ids match
   */
  bool
  operator==(const Waypoint&wp) const
  {
    return id == wp.id;
  }

/** 
 * Determine if a waypoint is close to a given location within
 * a threshold
 * 
 * @param location Location to compare to
 * @param range Distance threshold (m)
 * 
 * @return True if close to reference location
 */
  bool
  is_close_to(const GEOPOINT &location, const fixed range) const;

public:
  DEFINE_CONSTVISITABLE()

#ifdef DO_PRINT
  friend std::ostream& operator<< (std::ostream& o, const Waypoint& wp);
#endif
};

#endif
