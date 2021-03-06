/*
  This file is part of STerm.

  STerm is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  STerm is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with STerm.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "functions.h"

void sterm_functions_command_pipe ( STermTerminal *sterm, gchar *pipe )
{
  gchar *text = vte_terminal_get_text ( sterm->terminal, NULL, NULL, NULL );

  FILE *output = popen ( pipe, "w" );

  if ( output ) {
    fprintf ( output, "%s", text );

    if ( ! ferror ( output ) ) {

      if ( pclose ( output ) != 0 )
        g_warning ( "ERROR: Failed to close the pipe '%s'.\n", pipe );

    } else
      g_warning ( "ERROR: Failed to write to the pipe '%s'.\n", pipe );

  } else
    g_warning ( "ERROR: Failed to open the pipe '%s'.\n", pipe );

  g_free ( text );
}

void sterm_functions_paste ( STermTerminal *sterm, gchar *selection )
{
  if ( g_strcmp0 ( selection, "primary" ) == 0 )
    vte_terminal_paste_primary ( sterm->terminal );
  else if ( g_strcmp0 ( selection, "clipboard" ) == 0 )
    vte_terminal_paste_clipboard ( sterm->terminal );
}

void sterm_functions_zoom ( STermTerminal *sterm, gchar *factor )
{
  gint size = g_ascii_strtoll ( factor, NULL, 0 );

  PangoFontDescription *font = (PangoFontDescription*) vte_terminal_get_font ( sterm->terminal );
  pango_font_description_set_size ( font, pango_font_description_get_size ( font ) + size );
  vte_terminal_set_font ( sterm->terminal, font );
}

void sterm_functions_insert ( STermTerminal *sterm, gchar *text )
{
  vte_terminal_feed_child ( sterm->terminal, text, -1 );
}

void sterm_functions_set_font ( STermTerminal *sterm, gchar *font )
{
  vte_terminal_set_font_from_string ( sterm->terminal, font );
}

void sterm_functions_init ( STermTerminal *sterm )
{
  sterm->functions = g_hash_table_new ( g_str_hash, g_str_equal );

  g_hash_table_insert ( sterm->functions, "command_pipe", sterm_functions_command_pipe );
  g_hash_table_insert ( sterm->functions, "insert", sterm_functions_insert );
  g_hash_table_insert ( sterm->functions, "paste", sterm_functions_paste );
  g_hash_table_insert ( sterm->functions, "set_font", sterm_functions_set_font );
  g_hash_table_insert ( sterm->functions, "zoom", sterm_functions_zoom );
}

