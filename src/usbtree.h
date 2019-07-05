/*************************************************************************
** usbtree.h for USBView - a USB device viewer
** Copyright (c) 1999, 2000 by Greg Kroah-Hartman, greg@kroah.com
**		2007 by Luis Galdos <felipito@gmx.net>
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** (See the included file COPYING)
*************************************************************************/

#ifndef __USB_TREE_H
#define __USB_TREE_H


enum get_device_enum {
  GET_NAME,
  GET_IDS,
  GET_DRIVERS,
  GET_CONFIGURATIONS,
  GET_INTERFACES,
  GET_ENDPOINTS,
  GET_ALL,
  GET_NOTHING
};


void initialize_stuff (const gchar *devfile);
int LoadUSBTree (int refresh, gchar *devfile);


void PopulateListBox (int deviceId);
void PopulateDevConfigs (int deviceId);
void PopulateDevInterfaces (int deviceId);
void PopulateDevEndpoints (int deviceId);

#endif	/* __USB_TREE_H */
