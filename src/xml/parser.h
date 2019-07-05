/*************************************************************************
 * parser.h for USBView2
 * Copyright (c) 2010 Luis Galdos <felipito@gmx.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * (See the included file COPYING)
 *
 *************************************************************************/

#ifndef XMLPARSE_H_
#define XMLPARSE_H_

#define MAX_VENDOR_NAME			(64)
#define MAX_PRODUCT_NAME		(64)
#define VID_LEN				(4)
#define PID_LEN				(4)
#define VID_STR_LEN			(5)
#define PID_STR_LEN			(5)

struct vendor_t {
  unsigned long index;
  gchar name[MAX_VENDOR_NAME];
  gchar id[VID_LEN];
  unsigned int number_products;
  struct product_t **products;

  /*
   * In some cases we get the same HASH for two different Vendor IDs. For catching the
   * correct one uses this pointer so that we can search for the correct vendor ID!
   */
  struct vendor_t *next;
} __attribute__((packed));

struct product_t {
  unsigned long index;
  gchar name[MAX_PRODUCT_NAME];
  gchar id[PID_LEN];
  struct vendor_t *vendor;

  struct product_t *next;
} __attribute__((packed));

struct hash_table {
  int number_vendors;
  struct vendor_t **vendors;
};

#define M				(37)
static inline int hashnum(char *string, unsigned int max) {

  unsigned int hash_adresse;
  unsigned char *pointer;
 
  hash_adresse = 0;
  pointer = (unsigned char *) string;
  while (*pointer != '\0') {
    hash_adresse = M * hash_adresse + *pointer;
    pointer++;
  }
  
  return hash_adresse % max;
}


int parse_ids_file(const gchar *file, struct hash_table *hash);
struct vendor_t *parse_search_vendor(gchar *id, struct hash_table *table);
struct product_t *parse_search_product(gchar *id, struct vendor_t *vendor);

#endif /* XMLPARSE_H_ */
