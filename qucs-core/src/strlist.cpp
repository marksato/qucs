/*
 * strlist.cpp - string list class implementation
 *
 * Copyright (C) 2003, 2004 Stefan Jahn <stefan@lkcc.org>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this package; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.  
 *
 * $Id: strlist.cpp,v 1.3 2004-05-09 12:54:03 ela Exp $
 *
 */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strlist.h"

// Constructor creates an instance of the strlist class.
strlist::strlist () {
  root = NULL;
  txt = NULL;
}

/* This copy constructor creates a instance of the strlist class based
   on the given strlist. */
strlist::strlist (const strlist & o) {
  struct strlist_t * s;
  root = NULL;
  txt = NULL;
  for (s = o.root; s != NULL; s = s->next) append (s->str);
}

// Destructor deletes an instance of the strlist class.
strlist::~strlist () {
  struct strlist_t * next;
  while (root) {
    next = root->next;
    if (root->str) free (root->str);
    free (root);
    root = next;
  }
  if (txt) free (txt);
}

// This function adds a string to the list.
void strlist::add (char * str) {
  struct strlist_t * s;
  s = (struct strlist_t *) calloc (sizeof (struct strlist_t), 1);
  s->next = root;
  s->str = str ? strdup (str) : NULL;
  root = s;
}

// This function append a string to the list.
void strlist::append (char * str) {
  struct strlist_t * s;
  s = (struct strlist_t *) calloc (sizeof (struct strlist_t), 1);
  s->next = NULL;
  s->str = str ? strdup (str) : NULL;
  if (root) {
    struct strlist_t * e;
    for (e = root; e->next != NULL; e = e->next);
    e->next = s;
  }
  else {
    root = s;
  }
}

// This function counts the string in the list.
int strlist::length (void) {
  int res = 0;
  for (struct strlist_t * s = root; s != NULL; s = s->next) res++;
  return res;
}

// This function finds the specified string in the list.
int strlist::contains (char * str) {
  int res = 0;
  for (struct strlist_t * s = root; s != NULL; s = s->next) {
    if (s->str != NULL && str != NULL && !strcmp (s->str, str)) 
      res++;
  }
  return res;
}

/* This function returns the string positioned at the specified
   location in the string list. */
char * strlist::get (int pos) {
  struct strlist_t * s = root;
  for (int i = 0; i < pos && s != NULL; s = s->next, i++);
  return s ? s->str : NULL;
}

/* The function joins the given string lists to each other and returns
   the resulting list. */
strlist * strlist::join (strlist * pre, strlist * post) {
  strlist * res = pre ? new strlist (*pre) : new strlist ();
  for (int i = 0; post != NULL && i < post->length (); i++)
    res->append (post->get (i));
  return res;
}

/* The function returns a space seperated string representation of the
   string list instance. */
char * strlist::toString (void) {
  if (txt) { free (txt); txt = NULL; }
  int size = 0;
  for (struct strlist_t * s = root; s != NULL; s = s->next) {
    char * t = s->str ? s->str : (char *) "(null)";
    int len = strlen (t);
    size += len + 2;
    txt = (char *) (txt ? realloc (txt, size) : malloc (size));
    txt = (s == root) ? strcpy (txt, t) : strcat (txt, t);
    txt = strcat (txt, " ");
  }
  if (txt) txt[strlen (txt) - 1] = '\0';
  return txt ? txt : (char *) "";
}
