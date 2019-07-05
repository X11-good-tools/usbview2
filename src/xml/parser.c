/*************************************************************************
 * parser.c for USBView2
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

#include <glib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#include <xml/parser.h>

#if 0
#define PARSER_DEBUG
#endif

#define pr_err(fmt, args...)		do { g_print("[ERROR] parser: " fmt, ## args); } while(0)
#define pr_warn(fmt, args...)		do { g_print("[WARN] parser: " fmt, ## args); } while(0)
#define pr_info(fmt, args...)		do { g_print("[INFO] parser: " fmt, ## args); } while(0)

#if defined(PARSER_DEBUG)
#define pr_dbg(fmt, args...)		do { g_print("[DBG] parser: " fmt, ## args); } while(0)
#else
#define pr_dbg(fmt, args...)		do { } while (0)
#endif /* PARSER_DEBUG */

#if 0
#define PARSER_VENDOR_DEBUG
#endif
#if defined(PARSER_VENDOR_DEBUG)
#define pr_dbg_vendor(fmt, args...)	do { g_print("[DBG] parser: " fmt, ## args); } while(0)
#else
#define pr_dbg_vendor(fmt, args...)	do { } while (0)
#endif /* PARSER_VENDOR_DEBUG */

#if 0
#define PARSER_PRODUCT_DEBUG
#endif
#if defined(PARSER_PRODUCT_DEBUG)
#define pr_dbg_product(fmt, args...)	do { g_print("[DBG] parser: " fmt, ## args); } while(0)
#else
#define pr_dbg_product(fmt, args...)	do { } while (0)
#endif /* PARSER_PRODUCT_DEBUG */

static void xml_err(GMarkupParseContext *context, GError *error, gpointer _data)
{  
  g_critical ("%s", error->message);
}

static void xml_start(GMarkupParseContext *context, const gchar *element,
		      const gchar **attributes, const gchar **values,
		      gpointer _data, GError **error)
{
  const gchar *attr;
  struct hash_table *table;
  static struct vendor_t *last_vendor;
  struct product_t *product;
  int cnt, new_index = -1;
  
  table = (struct hash_table *)_data;
  
  /* I'll assume in this function that each element can have only one attribute. */
  if (!g_ascii_strcasecmp(element, "usb")) {
    unsigned long vendors;
    struct vendor_t *vendor;

    vendors = g_ascii_strtoull(values[0], NULL, 10);
    table->vendors = g_malloc0(sizeof(vendor) * vendors);
    table->number_vendors = vendors;
    
  } else if(!g_ascii_strcasecmp(element, "vendor")) {

    struct vendor_t *new_vendor;

    new_vendor = g_malloc0(sizeof(*new_vendor));
  
    for (cnt = 0; ; cnt++) {

      if (!(attr = attributes[cnt]))
	break;
      
      if (!g_ascii_strcasecmp(attr, "index")) {
	new_index = g_ascii_strtoull(values[cnt], NULL, 10);
	new_vendor->index = new_index;
      }

      if (!g_ascii_strcasecmp(attr, "products")) {
	unsigned long prods;
	
	prods = g_ascii_strtoull(values[cnt], NULL, 10);
	new_vendor->products = g_malloc0(sizeof(product) * (prods + 1));
	new_vendor->number_products = prods;
      }

      if (!g_ascii_strcasecmp(attr, "name"))
	g_utf8_strncpy(new_vendor->name, values[cnt], sizeof(new_vendor->name));

      if (!g_ascii_strcasecmp(attr, "id"))
	g_utf8_strncpy(new_vendor->id, values[cnt], sizeof(new_vendor->id));
    }

    /* Now check if we have a valid vendor and add it to the root hash table */
    if (!table->vendors[new_index]) {
      table->vendors[new_index] = new_vendor;
    } else {
      struct vendor_t *v;

      for (v = table->vendors[new_index]; v->next; v = v->next) {

      }
      v->next = new_vendor;
    }
    last_vendor = new_vendor;
    
    pr_dbg("Added NEW vendor index : %i [products %u]\n",
	     new_index, new_vendor->number_products);

  } else if(!g_ascii_strcasecmp(element, "product")) {

    const GSList *stack, *parent;
    struct product_t *new_product;
    
    /* Add this new product to the corresponding vendor */
    stack = g_markup_parse_context_get_element_stack (context);
    parent = stack->next;

    /* Add sanity checks for the parent */
    if (!last_vendor || !parent) {
      g_print("@FIXME: Something is wrong with the XML file\n");
      return;
    }

    new_product = g_malloc0(sizeof(*new_product));

    for (cnt = 0; ; cnt++) {

      if (!(attr = attributes[cnt]))
	break;

      if (!g_ascii_strcasecmp(attr, "index")) {
	new_index = g_ascii_strtoull(values[cnt], NULL, 10);
	new_product->index = new_index;
      }

      if (!g_ascii_strcasecmp(attr, "name"))
	g_utf8_strncpy(new_product->name, values[cnt], sizeof(new_product->name));

      if (!g_ascii_strcasecmp(attr, "id"))
	g_utf8_strncpy(new_product->id, values[cnt], sizeof(new_product->id));
      
    }
    
    pr_dbg("  - Adding NEW product %i to vendor %lu : %s\n",
	     new_index, last_vendor->index, new_product->name);
    if (!last_vendor->products[new_index]) {
      last_vendor->products[new_index] = new_product;
    } else {
      struct product_t *p;

      for (p = last_vendor->products[new_index]; p && p->next; p = p->next) {

      }
      p->next = new_product;
    }    
  }
}

int parse_ids_file(const gchar *file, struct hash_table *hash)
{
  static GMarkupParser parser;
  GMarkupParseContext *context;
  gchar *content;

  if (!hash || !file)
    return -EINVAL;

  
  if (!g_file_get_contents (file, &content, NULL, NULL))
    return -EAGAIN;

  /* First reset the parser struct */
  memset(&parser, 0x00, sizeof(parser));
  parser.start_element = xml_start;
  parser.error = xml_err;
  
  context = g_markup_parse_context_new (&parser, 0, hash, NULL);
  g_markup_parse_context_parse (context, content, -1, NULL);
   
  g_markup_parse_context_free (context);
  g_free (content);
  return 0;
}

struct product_t *parse_search_product(gchar *id, struct vendor_t *vendor)
{
  struct product_t *product, *ret;
  unsigned int hash;

  if (!vendor)
    return NULL;
  
  hash = hashnum(id, vendor->number_products);
  if (hash >= vendor->number_products)  {
    pr_err("Calculated hash %u for ID '%s' greater than %u\n",
	   hash, id, vendor->number_products);
    return NULL;
  }

  product = vendor->products[hash];
  if (!product) {
    pr_err("No found with id '%s' of vendor '%s' and hash '%u'\n",
	   id, vendor->name, hash);
    return NULL;
  }
  
  /* Search for the correct device ID */
  ret = NULL;
  for (; product; product = product->next) {
    if (!memcmp(product->id, id, PID_LEN)) {
      ret = product;
      break;
    }
  }

  if (ret)
    pr_dbg_product("Found product '%s' with ID '%s'\n", ret->name, id);

  return ret;
}

struct vendor_t *parse_search_vendor(gchar *id, struct hash_table *table)
{
  unsigned int hash;
  struct vendor_t *vendor, *ret;

  if (!id || !table)
    return NULL;
  
  hash = hashnum(id, table->number_vendors);

  if (hash >= table->number_vendors) {
    pr_err("Calculated hash %u for ID '%s' greater than %u\n",
	    hash, id, table->number_vendors);
    return NULL;
  }
  
  vendor = table->vendors[hash];
  if (!vendor) {
    pr_err("No vendor created for id %s with hash %u\n", id, hash);
    return NULL;
  }

  /* Search for the correct device ID */
  ret = NULL;
  for (; vendor; vendor = vendor->next) {
    if (!memcmp(vendor->id, id, VID_LEN)) {
      ret = vendor;
      break;
    }
  }

  if (ret)
    pr_dbg_vendor("Found vendor '%s' with ID '%s'\n", ret->name, id);
  
  return ret;
}
